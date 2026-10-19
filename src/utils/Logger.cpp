#include "geocad/utils/Logger.h"

#include <iostream>

namespace geocad::utils {

void Logger::log(LogLevel level, const std::string& message)
{
    switch (level) {
    case LogLevel::Info:
        std::cerr << "[INFO] " << message << '\n';
        break;
    case LogLevel::Warning:
        std::cerr << "[WARNING] " << message << '\n';
        break;
    case LogLevel::Error:
        std::cerr << "[ERROR] " << message << '\n';
        break;
    }
}

} // namespace geocad::utils
