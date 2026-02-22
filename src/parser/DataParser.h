#pragma once

#include "../effects/Effects.h"

/**
 * @brief Utility for parsing incoming data strings
 */
namespace DataParser {
    /**
     * @brief Sets the application state context used by the parser
     * @param mode Pointer to the current effects mode variable
     * @param isSystemOff Pointer to the system power state variable
     */
    void setContext(Effects::Mode *mode, bool *isSystemOff);

    /**
     * @brief Parses a data string and executes corresponding commands
     * @param data String to parse
     * @return true if the string was successfully parsed
     */
    bool parse(const String &data);
}
