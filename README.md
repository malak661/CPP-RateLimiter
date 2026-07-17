# C++ Token Bucket Rate Limiter (`CPP-RateLimiter`)

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/compiler_support/20)
[![Framework](https://img.shields.io/badge/Crow-v1.2.0-orange.svg)](https://crowcpp.org/)
[![JSON](https://img.shields.io/badge/nlohmann%2Fjson-v3.11.3-green.svg)](https://github.com/nlohmann/json)
[![Build](https://img.shields.io/badge/Build-CMake%203.16%2B-064F8C.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-MIT-purple.svg)](#license)

A high-performance, thread-safe, in-memory **Rate Limiter API service** built in modern **C++20** using the **Token Bucket Algorithm**. The application exposes a clean REST API powered by the [Crow](https://crowcpp.org/) C++ microframework and [`nlohmann/json`](https://github.com/nlohmann/json), making it easy to integrate with web gateways, microservices, and client applications.

---

## ✨ Features

- **Token Bucket Algorithm**: Smooth, continuous token refilling based on high-precision monotonic clocks (`std::chrono::steady_clock`).
- **Thread-Safe Architecture**: Guarded operations using `std::mutex`, fully safe for multithreaded concurrent API handling (`crow::SimpleApp::multithreaded()`).
- **In-Memory Storage**: Fast client state retrieval and updates using `std::unordered_map` with independent buckets per `clientKey`.
- **Dynamic Configuration Updates**: Modify capacity and refill rate at runtime via `PATCH /api/config` without restarting the server; instantly propagates changes to all existing client buckets (`BucketStore::updateAll`).
- **Zero-Friction Build**: Uses CMake's `FetchContent` to automatically download and link all required external libraries (`Asio`, `Crow`, and `nlohmann/json`) during compilation.

---

## 🏗️ Architecture & Project Structure

The project follows clean architectural principles with strict separation of concerns across layers:

```text
CPP-RateLimiter/
├── CMakeLists.txt              # CMake build configuration & FetchContent rules
├── config.json                 # Startup configuration file
├── include/
│   ├── controller/             # HTTP route handlers (RateLimiterController)
│   ├── core/                   # Core domain logic (RateLimiter, TokenBucket, DecisionEngine)
│   ├── exceptions/             # Custom domain exceptions (InvalidRequestException)
│   ├── models/                 # Value objects & DTOs (Bucket, Config, Response)
│   ├── storage/                # State storage layer (BucketStore)
│   └── utils/                  # Helper utilities (Logger, TimeUtils)
├── src/
│   ├── controller/             # Controller implementation
│   ├── core/                   # Engine & algorithm implementations
│   ├── storage/                # BucketStore implementation
│   ├── utils/                  # Logger implementation
│   ├── Application.cpp         # Application initialization and lifecycle
│   └── main.cpp                # Application entry point
└── tests/
    ├── core/                   # Unit tests for TokenBucket & RateLimiter
    └── storage/                # Unit tests for BucketStore
```

---

## 🛠️ Prerequisites

To build and run `CPP-RateLimiter`, ensure your system meets the following requirements:

- **C++ Compiler**: Supporting **C++20** (e.g., GCC 10+, Clang 11+, or MSVC 2019/2022).
- **CMake**: Version **3.16** or higher.
- **Git**: Required by CMake's `FetchContent` mechanism to pull dependencies.
- **Operating System**: Windows, Linux, or macOS.

---

## 🚀 Installation & Setup

### 1. Clone the Repository

```bash
git clone https://github.com/malak661/CPP-RateLimiter.git
cd CPP-RateLimiter
```

### 2. Configure & Build with CMake

Use modern CMake commands (`-S . -B build`) to configure and compile the project directly from the root directory:

```bash
# Configure the build directory and fetch dependencies (Asio, Crow, nlohmann_json)
cmake -S . -B build

# Build the application in Release mode
cmake --build build --config Release
```

### 3. Run the Server

Because the server reads `config.json` from the current working directory, run the executable directly from the project root:

#### On Linux / macOS:
```bash
./build/CPP-RateLimiter
```

#### On Windows (PowerShell / CMD):
```powershell
.\build\Release\CPP-RateLimiter.exe
# OR if built without --config Release on single-config generators:
.\build\CPP-RateLimiter.exe
```

When started, you should see logs similar to:
```text
[INFO] Rate limiter starting on port 8080...
```

---

## ⚙️ Configuration (`config.json`)

The application loads its startup settings from `config.json` located in the current working directory:

```json
{
    "bucketCapacity": 3.0,
    "refillRate": 0.16666666666666666,
    "port": 8080
}
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| `bucketCapacity` | `double` | Maximum number of tokens a client's bucket can hold (burst limit). |
| `refillRate` | `double` | Number of tokens refilled **per second** (e.g., `0.1667` ≈ 10 tokens/minute). |
| `port` | `integer` | TCP port on which the Crow HTTP server listens (`8080` by default). |

---

## 📡 API Endpoints & Usage

### 1. Check & Consume Rate Limit (`POST /api/check/<clientKey>`)

Checks whether the specified `clientKey` has available tokens. If allowed, **1.0 token** is consumed from the client's bucket.

#### Example Request (`curl`):
```bash
curl -X POST http://localhost:8080/api/check/client_123
```

#### ✅ Response when Allowed (`200 OK`):
```json
{
  "allowed": true,
  "limit": 3.0,
  "message": "Request allowed",
  "remainingTokens": 2.0,
  "retryAfterSeconds": 0.0
}
```

#### 🚫 Response when Rate Limited (`429 Too Many Requests`):
```json
{
  "allowed": false,
  "limit": 3.0,
  "message": "Rate limit exceeded",
  "remainingTokens": 0.0,
  "retryAfterSeconds": 4.5
}
```

---

### 2. Inspect Bucket Status (`GET /api/status/<clientKey>`)

Returns the current token count and limit for the given `clientKey` **without consuming** any tokens. If the client has not made previous requests, a fresh full bucket is initialized.

#### Example Request (`curl`):
```bash
curl http://localhost:8080/api/status/client_123
```

#### Response (`200 OK`):
```json
{
  "clientKey": "client_123",
  "limit": 3.0,
  "message": "Bucket status retrieved",
  "remainingTokens": 2.5
}
```

---

### 3. Update Configuration at Runtime (`PATCH /api/config`)

Dynamically updates the rate limiter capacity and refill rate while the server is running. This immediately updates the global default configuration for new clients and **retroactively updates all existing client buckets in memory**.

#### Example Request (`curl`):
```bash
curl -X PATCH http://localhost:8080/api/config \
  -H "Content-Type: application/json" \
  -d '{"capacity": 10.0, "refillRate": 1.0}'
```

#### Response (`200 OK`):
```json
{
  "capacity": 10.0,
  "message": "Config updated.",
  "refillRate": 1.0
}
```

---

## 🧪 Running Tests

The project includes standalone unit test suites covering the core token bucket math, bucket store behavior, and rate limiter orchestration.

You can compile and run each test directly using your C++ compiler. For example, using `g++` on Linux/macOS or Git Bash:

```bash
# Test TokenBucket core mechanics
g++ -std=c++20 -Iinclude tests/core/TokenBucketTest.cpp src/core/TokenBucket.cpp -o test_token_bucket
./test_token_bucket

# Test BucketStore storage and update logic
g++ -std=c++20 -Iinclude tests/storage/BucketStoreTest.cpp src/storage/BucketStore.cpp -o test_bucket_store
./test_bucket_store

# Test RateLimiter integration & exhaustion
g++ -std=c++20 -Iinclude tests/core/RateLimiterTest.cpp src/core/RateLimiter.cpp src/core/TokenBucket.cpp src/storage/BucketStore.cpp -o test_rate_limiter
./test_rate_limiter
```

Or with MSVC (`cl.exe`) on Windows Developer Command Prompt:
```cmd
cl /std:c++20 /EHsc /Iinclude tests\core\RateLimiterTest.cpp src\core\RateLimiter.cpp src\core\TokenBucket.cpp src\storage\BucketStore.cpp
RateLimiterTest.exe
```

---

## 🧮 How the Token Bucket Algorithm Works

1. **Bucket Initialization**: When a client (`clientKey`) requests the API for the first time, a bucket is created with `availableTokens = capacity` and `lastUpdate = now()`.
2. **Time-Based Refill**: On every subsequent request (`check` or `status`), the system calculates elapsed time $\Delta t = \text{now}() - \text{lastUpdate}$.
3. **Token Accrual**: The bucket is refilled using the formula:
   $$\text{availableTokens} = \min(\text{capacity}, \text{availableTokens} + \Delta t \times \text{refillRate})$$
4. **Consumption & Decision**: If $\text{availableTokens} \ge 1.0$, one token is consumed and the request is allowed (`allowed = true`). Otherwise, the request is rejected with `429 Too Many Requests` and calculates the exact `retryAfterSeconds` needed to accrue 1 token.

---

## 📝 License

This project is open-source and available under the **MIT License**.
