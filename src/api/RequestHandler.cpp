#include "api/RequestHandler.h"
#include "exceptions/InvalidRequestException.h"
#include "constants/Constants.h"
#include <sstream>

namespace ratelimiter {
namespace api {

RequestHandler::RequestHandler(core::RateLimiter& rateLimiter)
    : rateLimiter_(rateLimiter) {}

std::string RequestHandler::extractClientKey(const std::string& clientKeyHeader) const {
    if (clientKeyHeader.empty()) {
        throw exceptions::InvalidRequestException("Missing client key");
    }
    return clientKeyHeader;
}

models::Request RequestHandler::parseConfigBody(const std::string& clientKey,
                                                 const std::string& body) const {
    models::Request request(clientKey, constants::ENDPOINT_CONFIG);

    if (body.empty()) {
        throw exceptions::InvalidRequestException("Empty config body");
    }

    // Expected format: "capacity=10&refillRate=2"
    double capacity = -1.0;
    double refillRate = -1.0;

    std::stringstream ss(body);
    std::string pair;

    while (std::getline(ss, pair, '&')) {
        auto eqPos = pair.find('=');
        if (eqPos == std::string::npos) {
            continue;
        }

        std::string key = pair.substr(0, eqPos);
        std::string value = pair.substr(eqPos + 1);

        try {
            if (key == "capacity") {
                capacity = std::stod(value);
            } else if (key == "refillRate") {
                refillRate = std::stod(value);
            }
        } catch (const std::exception&) {
            throw exceptions::InvalidRequestException("Invalid numeric value for " + key);
        }
    }

    if (capacity <= 0.0 || refillRate < 0.0) {
        throw exceptions::InvalidRequestException(
            "capacity must be > 0 and refillRate must be >= 0"
        );
    }

    request.hasConfigUpdate = true;
    request.newCapacity = capacity;
    request.newRefillRate = refillRate;

    return request;
}

models::Response RequestHandler::handle(const std::string& path,
                                         const std::string& clientKeyHeader,
                                         const std::string& body) {
    std::string clientKey = extractClientKey(clientKeyHeader);

    if (path == constants::ENDPOINT_CHECK) {
        models::Request request(clientKey, constants::ENDPOINT_CHECK);
        return rateLimiter_.checkRequest(request);
    }

    if (path == constants::ENDPOINT_STATUS) {
        models::Request request(clientKey, constants::ENDPOINT_STATUS);
        return rateLimiter_.getStatus(request);
    }

    if (path == constants::ENDPOINT_CONFIG) {
        models::Request request = parseConfigBody(clientKey, body);
        return rateLimiter_.updateConfig(request);
    }

    throw exceptions::InvalidRequestException("Unknown endpoint: " + path);
}

} // namespace api
} // namespace ratelimiter
