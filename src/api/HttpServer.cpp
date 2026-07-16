#include "api/HttpServer.h"
#include "exceptions/InvalidRequestException.h"
#include "constants/Constants.h"
#include "utils/Logger.h"
#include <crow.h>

HttpServer::HttpServer(RequestHandler& handler, int port)
    : handler_(handler), port_(port) {}

void HttpServer::run() {
    crow::SimpleApp app;

    // POST /check  — rate-limit check for a client
    CROW_ROUTE(app, "/check").methods(crow::HTTPMethod::POST)
    ([this](const crow::request& req) {
        std::string clientKey = req.get_header_value("X-Client-Key");
        try {
            Response res = handler_.handle(ENDPOINT_CHECK, clientKey, "");
            crow::response crowRes(res.statusCode, res.message);
            crowRes.add_header("Content-Type", "application/json");
            // Simple JSON serialisation without pulling in nlohmann just for this.
            crowRes.body = "{\"allowed\":" + std::string(res.allowed ? "true" : "false") +
                           ",\"remainingTokens\":" + std::to_string(res.remainingTokens) +
                           ",\"retryAfterSeconds\":" + std::to_string(res.retryAfterSeconds) +
                           ",\"message\":\"" + res.message + "\"}";
            return crowRes;
        } catch (const InvalidRequestException& e) {
            Logger::warn(std::string("Bad request on /check: ") + e.what());
            return crow::response(HTTP_BAD_REQUEST, e.what());
        }
    });

    // GET /status  — returns current token count for a client
    CROW_ROUTE(app, "/status").methods(crow::HTTPMethod::GET)
    ([this](const crow::request& req) {
        std::string clientKey = req.get_header_value("X-Client-Key");
        try {
            Response res = handler_.handle(ENDPOINT_STATUS, clientKey, "");
            crow::response crowRes(res.statusCode);
            crowRes.add_header("Content-Type", "application/json");
            crowRes.body = "{\"remainingTokens\":" + std::to_string(res.remainingTokens) +
                           ",\"message\":\"" + res.message + "\"}";
            return crowRes;
        } catch (const InvalidRequestException& e) {
            Logger::warn(std::string("Bad request on /status: ") + e.what());
            return crow::response(HTTP_BAD_REQUEST, e.what());
        }
    });

    // POST /config  — update global rate-limit config
    CROW_ROUTE(app, "/config").methods(crow::HTTPMethod::POST)
    ([this](const crow::request& req) {
        std::string clientKey = req.get_header_value("X-Client-Key");
        try {
            Response res = handler_.handle(ENDPOINT_CONFIG, clientKey, req.body);
            crow::response crowRes(res.statusCode);
            crowRes.add_header("Content-Type", "application/json");
            crowRes.body = "{\"message\":\"" + res.message + "\"}";
            return crowRes;
        } catch (const InvalidRequestException& e) {
            Logger::warn(std::string("Bad request on /config: ") + e.what());
            return crow::response(HTTP_BAD_REQUEST, e.what());
        }
    });

    Logger::info("HttpServer listening on port " + std::to_string(port_));
    app.port(port_).multithreaded().run();
}
