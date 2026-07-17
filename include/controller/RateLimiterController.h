#pragma once

#include <memory>
#include <crow.h>
#include "core/RateLimiter.h"

class RateLimiterController {
public:
    RateLimiterController(std::shared_ptr<RateLimiter> rateLimiter,
                          std::shared_ptr<crow::SimpleApp> app);

private:
    void _registerRoutes();

    crow::response _check(const std::string& clientKey);
    crow::response _status(const std::string& clientKey);
    crow::response _updateConfig(const crow::request& req);

    std::shared_ptr<RateLimiter> _rateLimiter;
    std::shared_ptr<crow::SimpleApp> _app;
};
