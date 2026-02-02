#include <fmt/core.h>
#include <nlohmann/json.hpp>

int main() {
    fmt::print("Environment verified.\n");
    nlohmann::json test = {{"status", "WSL connected"}};
    fmt::print("{}\n", test.dump(4));
    return 0;
}