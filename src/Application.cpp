#include "Application.h"
#include "models/Config.h"
#include "utils/Logger.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

Application::Application() {
    double capacity   = 10.0;
    double refillRate = 1.0;
    _port             = 8080;

    std::ifstream configFile("config.json");
    if (configFile.is_open()) {
        json cfg  = json::parse(configFile);
        capacity   = cfg.value("bucketCapacity", 10.0);
        refillRate = cfg.value("refillRate",     1.0);
        _port      = cfg.value("port",           8080);
    }

    _app         = std::make_shared<crow::SimpleApp>();
    _rateLimiter = std::make_shared<RateLimiter>(Config(capacity, refillRate));
    _controller  = std::make_unique<RateLimiterController>(_rateLimiter, _app);
}

void Application::run() {
    Logger::info("Rate limiter starting on port " + std::to_string(_port) + "...");
    _app->port(_port).multithreaded().run();
}
