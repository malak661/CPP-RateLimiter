#include "api/Router.h"
#include "exceptions/InvalidRequestException.h"

namespace ratelimiter {
namespace api {

Router::Router() {}

void Router::registerRoute(const std::string& path, HandlerFunction handler) {
    routes_[path] = handler;
}

models::Response Router::route(const models::Request& request) {
    auto it = routes_.find(request.endpoint);

    if (it == routes_.end()) {
        throw exceptions::InvalidRequestException(
            "No route registered for endpoint: " + request.endpoint
        );
    }

    return it->second(request);
}

} // namespace api
} // namespace ratelimiter
