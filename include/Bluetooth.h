#pragma once

#include <Arduino.h>

enum BT_ConnectionState {
    BT_ENABLED,
    BT_CONNECTED,
    BT_DISCONNECTED,
    BT_DISABLED
};

/**
 * @brief Тип коллбека для получения данных по BLE
 * @param value Строка с полученными данными
 */
typedef void (*BluetoothCredentialsReceivedCallback)(String value);

typedef void (*BluetoothConnectionStateCallback)(BT_ConnectionState value);

namespace Bluetooth {
    /**
     * @brief Инициализация BLE-модуля и системных обработчиков (Wi-Fi events и т.п.)
     * @param callback Функция, которая будет вызвана при записи в характеристику регистрации
     */
    void init(
        BluetoothCredentialsReceivedCallback credentialsCallback,
        BluetoothConnectionStateCallback stateCallback
    );

    /**
     * @brief Запуск BLE-режима (создание серверов/сервисов/характеристик и старт рекламы)
     */
    void enable();

    void disable();

    /**
     * @brief Отправка рабочего времени по BLE
     * @param seconds Время в секундах
     */
    void sendWorkedTime(int seconds);

    /**
     * @brief Признак активного BLE-подключения
     * @return true, если устройство подключено по BLE
     */
    bool isConnected();

    void sendWiFiConnectInfo(bool success, const String &value);
}
