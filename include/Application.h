#pragma once

#include <memory>
#include <crow.h>
#include "core/RateLimiter.h"
#include "controller/RateLimiterController.h"

class Application {
public:
    Application();
    void run();

private:
    int _port;
    std::shared_ptr<crow::SimpleApp> _app;
    std::shared_ptr<RateLimiter> _rateLimiter;
    std::unique_ptr<RateLimiterController> _controller;
};
