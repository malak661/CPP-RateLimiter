#include "Application.h"
#include "constants/Constants.h"
#include "utils/Logger.h"

Application::Application() {
    _app         = std::make_shared<crow::SimpleApp>();
    _rateLimiter = std::make_shared<RateLimiter>(
        Config(DEFAULT_BUCKET_CAPACITY, DEFAULT_REFILL_RATE)
    );
    _controller  = std::make_unique<RateLimiterController>(_rateLimiter, _app);
}

void Application::run() {
    Logger::info("Rate limiter starting on port 8080...");
    _app->port(8080).multithreaded().run();
}
