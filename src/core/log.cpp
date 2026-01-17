#include "core/log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Core {

std::shared_ptr<spdlog::logger> Log::s_logger;

void Log::init() {
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_logger = spdlog::stdout_color_mt("AutoFrac");
    s_logger->set_level(spdlog::level::info);
}

} // Core