#ifndef API_ROUTER_H
#define API_ROUTER_H

#include <string>
#include <functional>
#include <unordered_map>
#include "models/Request.h"
#include "models/Response.h"

namespace ratelimiter {
namespace api {

using HandlerFunction = std::function<models::Response(const models::Request&)>;

// Maps endpoint paths (e.g. "/check", "/status", "/config") to their
// handler functions and dispatches incoming requests to the right one.
class Router {
public:
    Router();

    // Registers a handler for a given endpoint path.
    void registerRoute(const std::string& path, HandlerFunction handler);

    // Looks up the handler for request.endpoint and invokes it.
    // Throws exceptions::InvalidRequestException if no route matches.
    models::Response route(const models::Request& request);

private:
    std::unordered_map<std::string, HandlerFunction> routes_;
};

} // namespace api
} // namespace ratelimiter

#endif // API_ROUTER_H
