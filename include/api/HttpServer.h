#pragma once

#include "api/RequestHandler.h"

// Wraps the Crow HTTP server. Registers routes and delegates to RequestHandler.
class HttpServer {
public:
    // port: TCP port to listen on (default 8080)
    HttpServer(RequestHandler& handler, int port = 8080);

    // Starts the server. Blocks until the server is stopped.
    void run();

private:
    RequestHandler& handler_;
    int port_;
};
