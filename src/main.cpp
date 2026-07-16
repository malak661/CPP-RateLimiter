#include "api/HttpServer.h"
#include "api/RequestHandler.h"
#include "core/RateLimiter.h"
#include "models/Config.h"
#include "constants/Constants.h"
#include "utils/Logger.h"

int main() {
    Config defaultConfig(DEFAULT_BUCKET_CAPACITY, DEFAULT_REFILL_RATE);

    RateLimiter rateLimiter(defaultConfig);
    RequestHandler handler(rateLimiter);
    HttpServer server(handler, 8080);

    Logger::info("Rate limiter starting...");
    server.run();

    return 0;
}
