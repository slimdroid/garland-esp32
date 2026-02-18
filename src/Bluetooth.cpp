#include "Bluetooth.h"

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "Logging.h"

static const char *TAG = "BLUETOOTH";

// BLE device name
#define BLE_NAME "I AM ESP32"

// Service and characteristic UUIDs
#define SERVICE_REGISTRATION_UUID           "459aa3b5-52c3-4d75-a64b-9cd76f65cfbb"
#define SERVICE_WORK_TIME_UUID              "cafa0333-8a16-4a59-b706-2f0e3fd38f58"

#define CHARACTERISTIC_REGISTRATION_CREDENTIALS_UUID "b9e70f80-d55e-4cd7-bec6-14be34590efc"
#define CHARACTERISTIC_REGISTRATION_RESPONSE_UUID    "7048479a-23f2-4f5b-8113-e60e59294b5a"
#define CHARACTERISTIC_WORK_TIME_UUID                "2c1529cd-f45d-4739-9738-2886fe46f7f1"

// Local (module-level) states
static BLECharacteristic *characteristicRegistrationCredentials = nullptr;
static BLECharacteristic *characteristicRegistrationResponse = nullptr;
static BLECharacteristic *characteristicWorkTime = nullptr;

static bool bleConnected = false;
static bool bleStarted = false;
static bool isInitialized = false;
static BLEServer *bleServer = nullptr;

static BluetoothCredentialsReceivedCallback g_credentialsCallback = nullptr;
static BluetoothConnectionStateCallback g_stateCallback = nullptr;

// ---- Helper handlers ----
class BluetoothServerEventCallback : public BLEServerCallbacks {
    void onConnect(BLEServer * /*server*/) override {
        bleConnected = true;
        if (g_stateCallback) g_stateCallback(BT_CONNECTED);
        BLEDevice::stopAdvertising();
        LOG(TAG, "Device connected");
    }

    void onDisconnect(BLEServer *server) override {
        if (bleStarted) {
            bleConnected = false;
            if (g_stateCallback) g_stateCallback(BT_DISCONNECTED);
            LOG(TAG, "Device disconnected");
            server->startAdvertising();
            LOG(TAG, "Advertising restarted");
        } else {
            LOG(TAG, "BLE stopped, not restarting advertising");
        }
    }
};

class BLECharacteristicRegistrationResponseCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) override {
        std::string value = pCharacteristic->getValue();
        if (!value.empty() && g_credentialsCallback) {
            g_credentialsCallback(String(value.c_str()));
            LOG(TAG, "Received registration response: %s", value.c_str());
        }
    }
};

static BluetoothServerEventCallback serverCallbacks;
static BLECharacteristicRegistrationResponseCallbacks characteristicCallbacks;

namespace Bluetooth {
    void init(
        BluetoothCredentialsReceivedCallback credentialsCallback,
        BluetoothConnectionStateCallback stateCallback
    ) {
        g_credentialsCallback = credentialsCallback;
        g_stateCallback = stateCallback;
    }

    void enable() {
        if (bleStarted) {
            LOG(TAG, "BLE already started");
            return;
        }

        LOG(TAG, "Starting BLE work!");
        if (!isInitialized) {
            BLEDevice::init(BLE_NAME);
            isInitialized = true;
        }

        if (bleServer == nullptr) {
            bleServer = BLEDevice::createServer();
            bleServer->setCallbacks(&serverCallbacks);
        }

        // Registration service
        BLEService *serviceRegistration = bleServer->createService(SERVICE_REGISTRATION_UUID);
        characteristicRegistrationCredentials = serviceRegistration->createCharacteristic(
            CHARACTERISTIC_REGISTRATION_CREDENTIALS_UUID,
            BLECharacteristic::PROPERTY_WRITE);
        characteristicRegistrationCredentials->setCallbacks(&characteristicCallbacks);

        characteristicRegistrationResponse = serviceRegistration->createCharacteristic(
            CHARACTERISTIC_REGISTRATION_RESPONSE_UUID,
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
        characteristicRegistrationResponse->addDescriptor(new BLE2902());

        // Work time service
        BLEService *serviceWorkTime = bleServer->createService(SERVICE_WORK_TIME_UUID);
        characteristicWorkTime = serviceWorkTime->createCharacteristic(
            CHARACTERISTIC_WORK_TIME_UUID,
            BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
        characteristicWorkTime->addDescriptor(new BLE2902());

        serviceRegistration->start();
        serviceWorkTime->start();

        BLEAdvertising *advertising = BLEDevice::getAdvertising();
        advertising->addServiceUUID(SERVICE_REGISTRATION_UUID);
        advertising->addServiceUUID(SERVICE_WORK_TIME_UUID);
        advertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
        advertising->setMaxPreferred(0x12);
        advertising->setScanResponse(true); // Add for reliability
        advertising->start();

        bleStarted = true;
        if (g_stateCallback) {
            g_stateCallback(BT_ENABLED);
        }
        LOG(TAG, "Characteristic defined! Now you can read it in your phone!");
    }

    void disable() {
        if (!bleStarted) {
            LOG(TAG, "BLE already stopped");
            return;
        }
        LOG(TAG, "Stopping BLE work!");

        // 1. Reset state flags BEFORE deinitialization
        bleStarted = false;
        bleConnected = false;

        // 2. Stop advertising and disconnect connections
        BLEDevice::getAdvertising()->stop();
        if (bleServer != nullptr) {
            uint32_t connCount = bleServer->getConnectedCount();
            if (connCount > 0) {
                LOG(TAG, "Disconnecting %d clients", connCount);
                // There is no simple way to disconnect all at once in the standard library,
                // but we can get the map of connected devices from the server.
                auto peerDevices = bleServer->getPeerDevices(false);
                for (auto const &[conn_id, status]: peerDevices) {
                    LOG(TAG, "Disconnecting conn_id: %d", conn_id);
                    bleServer->disconnect(conn_id);
                }
            }
            // To prevent connection even knowing the address, we could remove services
            // But this may complicate re-enabling.
            // For now, we'll limit ourselves to forced disconnect.
        }

        // 3. We do NOT call BLEDevice::deinit(true), as it causes hanging
        // when calling BLEDevice::init() again on some ESP32 versions.
        // Instead, we just stop the operation.
        // If you need to free memory, you can use deinit(false),
        // but this still won't fix the hanging problem and in most cases
        // it's enough to just stop broadcasting.

        // 4. Nullify characteristic pointers to avoid Dangling Pointer
        characteristicRegistrationCredentials = nullptr;
        characteristicRegistrationResponse = nullptr;
        characteristicWorkTime = nullptr;

        if (g_stateCallback) {
            g_stateCallback(BT_DISABLED);
        }
        LOG(TAG, "BLE has been stopped");
    }

    void sendWiFiConnectInfo(bool success, const String &value) {
        if (bleConnected && characteristicRegistrationResponse) {
            characteristicRegistrationResponse->setValue(value.c_str());
            characteristicRegistrationResponse->notify();
        }
        if (success == true) disable();
    }

    void sendWorkedTime(int seconds) {
        if (bleConnected && characteristicWorkTime) {
            characteristicWorkTime->setValue(seconds);
            characteristicWorkTime->notify();
        }
    }

    bool isConnected() {
        return bleConnected;
    }
}
