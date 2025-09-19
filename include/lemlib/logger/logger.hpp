<<<<<<< HEAD
#pragma once

#include <memory>
#include <array>

#define FMT_HEADER_ONLY
#include "fmt/core.h"

#include "lemlib/logger/baseSink.hpp"
#include "lemlib/logger/infoSink.hpp"
#include "lemlib/logger/telemetrySink.hpp"

namespace lemlib {

/**
 * @brief Get the info sink.
 * @return std::shared_ptr<InfoSink>
 */
std::shared_ptr<InfoSink> infoSink();

/**
 * @brief Get the telemetry sink.
 * @return std::shared_ptr<TelemetrySink>
 */
std::shared_ptr<TelemetrySink> telemetrySink();
} // namespace lemlib
=======
#pragma once

#include <memory>
#include <array>

#define FMT_HEADER_ONLY
#include "fmt/core.h"

#include "lemlib/logger/baseSink.hpp"
#include "lemlib/logger/infoSink.hpp"
#include "lemlib/logger/telemetrySink.hpp"

namespace lemlib {

/**
 * @brief Get the info sink.
 * @return std::shared_ptr<InfoSink>
 */
std::shared_ptr<InfoSink> infoSink();

/**
 * @brief Get the telemetry sink.
 * @return std::shared_ptr<TelemetrySink>
 */
std::shared_ptr<TelemetrySink> telemetrySink();
} // namespace lemlib
>>>>>>> 1d9f8e89a47b9fdcfcd5046e5cc23ef5a95901bd
