#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include "models/Request.h"
#include "models/Response.h"

using HandlerFunction = std::function<Response(const Request&)>;

// Maps endpoint paths (e.g. "/check", "/status", "/config") to their
// handler functions and dispatches incoming requests to the right one.
class Router {
public:
    Router();

    // Registers a handler for a given endpoint path.
    void registerRoute(const std::string& path, HandlerFunction handler);

    // Looks up the handler for request.endpoint and invokes it.
    // Throws InvalidRequestException if no route matches.
    Response route(const Request& request);

private:
    std::unordered_map<std::string, HandlerFunction> routes_;
};
