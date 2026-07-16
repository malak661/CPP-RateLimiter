#include "api/Router.h"
#include "exceptions/InvalidRequestException.h"

Router::Router() {}

void Router::registerRoute(const std::string& path, HandlerFunction handler) {
    routes_[path] = handler;
}

Response Router::route(const Request& request) {
    auto it = routes_.find(request.endpoint);

    if (it == routes_.end()) {
        throw InvalidRequestException(
            "No route registered for endpoint: " + request.endpoint
        );
    }

    return it->second(request);
}
