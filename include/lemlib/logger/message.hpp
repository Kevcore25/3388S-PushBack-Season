<<<<<<< HEAD
#pragma once

#include <string>
#include <cstdint>

namespace lemlib {
/**
 * @brief Level of the message
 *
 */
enum class Level { INFO, DEBUG, WARN, ERROR, FATAL };

/**
 * @brief A loggable message
 *
 */
struct Message {
        /* The message */
        std::string message;

        /** The level of the message */
        Level level;

        /** The time the message was logged, in milliseconds */
        uint32_t time;
};

/**
 * @brief Format a level
 *
 * @param level
 * @return std::string
 */
std::string format_as(Level level);
} // namespace lemlib
=======
#pragma once

#include <string>
#include <cstdint>

namespace lemlib {
/**
 * @brief Level of the message
 *
 */
enum class Level { INFO, DEBUG, WARN, ERROR, FATAL };

/**
 * @brief A loggable message
 *
 */
struct Message {
        /* The message */
        std::string message;

        /** The level of the message */
        Level level;

        /** The time the message was logged, in milliseconds */
        uint32_t time;
};

/**
 * @brief Format a level
 *
 * @param level
 * @return std::string
 */
std::string format_as(Level level);
} // namespace lemlib
>>>>>>> 1d9f8e89a47b9fdcfcd5046e5cc23ef5a95901bd
