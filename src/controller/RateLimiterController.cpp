#include "controller/RateLimiterController.h"
#include "exceptions/InvalidRequestException.h"
#include "models/Config.h"
#include "utils/Logger.h"
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

        if (res.allowed) {
            Logger::info("[CHECK] " + clientKey + " allowed — remaining: " + std::to_string(res.remainingTokens));
        } else {
            Logger::warn("[CHECK] " + clientKey + " blocked — retry after: " + std::to_string(res.retryAfterSeconds) + "s");
        }

        ordered_json resJson;
        resJson["allowed"]           = res.allowed;
        resJson["limit"]             = res.limit;
        resJson["remainingTokens"]   = res.remainingTokens;
        resJson["retryAfterSeconds"] = res.retryAfterSeconds;
        resJson["message"]           = res.message;

        crow::response crowRes(res.statusCode, resJson.dump());
        crowRes.set_header("Content-Type", "application/json");
        return crowRes;
    }
    catch (const nlohmann::json::parse_error&) {
        ordered_json resJson = {{"status", crow::status::BAD_REQUEST}, {"message", "Invalid JSON body"}};
        crow::response res(crow::status::BAD_REQUEST, resJson.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (const InvalidRequestException& e) {
        ordered_json resJson = {{"status", crow::status::BAD_REQUEST}, {"message", e.what()}};
        crow::response res(crow::status::BAD_REQUEST, resJson.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (const std::exception& e) {
        Logger::error(std::string("[CHECK] Internal error: ") + e.what());
        ordered_json resJson = {{"status", crow::status::INTERNAL_SERVER_ERROR}, {"message", e.what()}};
        crow::response res(crow::status::INTERNAL_SERVER_ERROR, resJson.dump());
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

        Logger::info("[STATUS] " + clientKey + " — remaining: " + std::to_string(res.remainingTokens));

        ordered_json resJson;
        resJson["clientKey"]       = clientKey;
        resJson["limit"]           = res.limit;
        resJson["remainingTokens"] = res.remainingTokens;
        resJson["message"]         = res.message;

        crow::response crowRes(crow::status::OK, resJson.dump());
        crowRes.set_header("Content-Type", "application/json");
        return crowRes;
    }
    catch (const InvalidRequestException& e) {
        ordered_json resJson = {{"status", crow::status::BAD_REQUEST}, {"message", e.what()}};
        crow::response res(crow::status::BAD_REQUEST, resJson.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (const std::exception& e) {
        Logger::error(std::string("[STATUS] Internal error: ") + e.what());
        ordered_json resJson = {{"status", crow::status::INTERNAL_SERVER_ERROR}, {"message", e.what()}};
        crow::response res(crow::status::INTERNAL_SERVER_ERROR, resJson.dump());
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

        Logger::info("[CONFIG] Updated — capacity: " + std::to_string(capacity) + ", refillRate: " + std::to_string(refillRate));

        ordered_json resJson;
        resJson["message"]    = "Config updated.";
        resJson["capacity"]   = capacity;
        resJson["refillRate"] = refillRate;

        crow::response crowRes(crow::status::OK, resJson.dump());
        crowRes.set_header("Content-Type", "application/json");
        return crowRes;
    }
    catch (const nlohmann::json::parse_error&) {
        ordered_json resJson = {{"status", crow::status::BAD_REQUEST}, {"message", "Invalid JSON body"}};
        crow::response res(crow::status::BAD_REQUEST, resJson.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (const InvalidRequestException& e) {
        ordered_json resJson = {{"status", crow::status::BAD_REQUEST}, {"message", e.what()}};
        crow::response res(crow::status::BAD_REQUEST, resJson.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (const std::exception& e) {
        Logger::error(std::string("[CONFIG] Internal error: ") + e.what());
        ordered_json resJson = {{"status", crow::status::INTERNAL_SERVER_ERROR}, {"message", e.what()}};
        crow::response res(crow::status::INTERNAL_SERVER_ERROR, resJson.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
}
