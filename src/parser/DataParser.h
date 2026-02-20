#pragma once

/**
 * @brief Utility for parsing incoming data strings
 */
namespace DataParser {
    /**
     * @brief Parses a data string and executes corresponding commands
     * @param data String to parse
     * @return true if the string was successfully parsed
     */
    bool parse(const String & data);
}
