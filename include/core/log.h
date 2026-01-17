#ifndef AUTOFRAC_LOG_H
#define AUTOFRAC_LOG_H

#include <memory>
#include <spdlog/spdlog.h>

namespace Core {

class Log {
public:
    static void init();

    inline static std::shared_ptr<spdlog::logger>& getLogger() { return s_logger; }

private:
    static std::shared_ptr<spdlog::logger> s_logger;
};

#define LOG_ERROR(...) Log::getLogger()->error(__VA_ARGS__)
#define LOG_WARN(...) Log::getLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...) Log::getLogger()->info(__VA_ARGS__)
#define LOG_DEBUG(...) Log::getLogger()->debug(__VA_ARGS__)
} // Core

#endif //AUTOFRAC_LOG_H
