#pragma once

// Default configuration used when no client-specific config exists yet.
constexpr double DEFAULT_BUCKET_CAPACITY = 10.0;   // max tokens
constexpr double DEFAULT_REFILL_RATE = 1.0;        // tokens per second

// HTTP status codes used across the API layer.
constexpr int HTTP_OK = 200;
constexpr int HTTP_BAD_REQUEST = 400;
constexpr int HTTP_TOO_MANY_REQUESTS = 429;
constexpr int HTTP_INTERNAL_ERROR = 500;

// API endpoint paths.
constexpr const char* ENDPOINT_CHECK = "/check";
constexpr const char* ENDPOINT_STATUS = "/status";
constexpr const char* ENDPOINT_CONFIG = "/config";
