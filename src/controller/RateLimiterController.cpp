#include "controller/RateLimiterController.h"
#include "exceptions/InvalidRequestException.h"
#include "constants/Constants.h"
#include "models/Config.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

RateLimiterController::RateLimiterController(
    std::shared_ptr<RateLimiter> rateLimiter,
    std::shared_ptr<crow::SimpleApp> app)
    : _rateLimiter(std::move(rateLimiter)), _app(std::move(app))
{
    _registerRoutes();
}

void RateLimiterController::_registerRoutes() {
    CROW_ROUTE((*_app), "/api/check").methods(crow::HTTPMethod::POST)
    ([this](const crow::request& req) {
        return _check(req);
    });

    CROW_ROUTE((*_app), "/api/status/<string>").methods(crow::HTTPMethod::GET)
    ([this](const std::string& clientKey) {
        return _status(clientKey);
    });

    CROW_ROUTE((*_app), "/api/config").methods(crow::HTTPMethod::PUT)
    ([this](const crow::request& req) {
        return _updateConfig(req);
    });
}

crow::response RateLimiterController::_check(const crow::request& req) {
    try {
        json body = json::parse(req.body);

        if (!body.contains("clientKey") || body["clientKey"].get<std::string>().empty()) {
            throw InvalidRequestException("Missing clientKey");
        }

        std::string clientKey = body["clientKey"];
        Response res = _rateLimiter->check(clientKey);

        ordered_json resJson;
        resJson["allowed"]           = res.allowed;
        resJson["remainingTokens"]   = res.remainingTokens;
        resJson["retryAfterSeconds"] = res.retryAfterSeconds;
        resJson["message"]           = res.message;

        crow::response crowRes(res.statusCode, resJson.dump());
        crowRes.set_header("Content-Type", "application/json");
        return crowRes;
    }
    catch (const InvalidRequestException& e) {
        ordered_json resJson = {{"status", HTTP_BAD_REQUEST}, {"message", e.what()}};
        crow::response res(HTTP_BAD_REQUEST, resJson.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (const std::exception& e) {
        ordered_json resJson = {{"status", HTTP_INTERNAL_ERROR}, {"message", e.what()}};
        crow::response res(HTTP_INTERNAL_ERROR, resJson.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
}

crow::response RateLimiterController::_status(const std::string& clientKey) {
    try {
        if (clientKey.empty()) {
            throw InvalidRequestException("Missing clientKey");
        }

        Response res = _rateLimiter->status(clientKey);

        ordered_json resJson;
        resJson["clientKey"]       = clientKey;
        resJson["remainingTokens"] = res.remainingTokens;
        resJson["message"]         = res.message;

        crow::response crowRes(HTTP_OK, resJson.dump());
        crowRes.set_header("Content-Type", "application/json");
        return crowRes;
    }
    catch (const InvalidRequestException& e) {
        ordered_json resJson = {{"status", HTTP_BAD_REQUEST}, {"message", e.what()}};
        crow::response res(HTTP_BAD_REQUEST, resJson.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (const std::exception& e) {
        ordered_json resJson = {{"status", HTTP_INTERNAL_ERROR}, {"message", e.what()}};
        crow::response res(HTTP_INTERNAL_ERROR, resJson.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
}

crow::response RateLimiterController::_updateConfig(const crow::request& req) {
    try {
        json body = json::parse(req.body);

        if (!body.contains("capacity") || !body.contains("refillRate")) {
            throw InvalidRequestException("Body must contain capacity and refillRate");
        }

        double capacity   = body["capacity"].get<double>();
        double refillRate = body["refillRate"].get<double>();

        if (capacity <= 0.0 || refillRate < 0.0) {
            throw InvalidRequestException("capacity must be > 0 and refillRate must be >= 0");
        }

        _rateLimiter->updateConfig(Config(capacity, refillRate));

        ordered_json resJson;
        resJson["message"]    = "Config updated.";
        resJson["capacity"]   = capacity;
        resJson["refillRate"] = refillRate;

        crow::response crowRes(HTTP_OK, resJson.dump());
        crowRes.set_header("Content-Type", "application/json");
        return crowRes;
    }
    catch (const InvalidRequestException& e) {
        ordered_json resJson = {{"status", HTTP_BAD_REQUEST}, {"message", e.what()}};
        crow::response res(HTTP_BAD_REQUEST, resJson.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (const std::exception& e) {
        ordered_json resJson = {{"status", HTTP_INTERNAL_ERROR}, {"message", e.what()}};
        crow::response res(HTTP_INTERNAL_ERROR, resJson.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
}
