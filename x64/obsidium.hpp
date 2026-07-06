#pragma once
#pragma comment(lib, "obsidium-lib.lib")
#pragma comment(lib, "libcurl.lib")
#pragma comment(lib, "zs.lib")
#pragma comment(lib, "libsodium.lib")

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "normaliz.lib")
#pragma comment(lib, "secur32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "bcrypt.lib")


#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace obsidium {

struct init_result {
    bool success = false;
    std::string message;
};

struct license_result {
    bool success = false;
    std::string message;
    bool license_required = false;
    std::string expiry;
};

struct register_result {
    bool success = false;
    std::string message;
};

struct ban_result {
    bool success = false;
    std::string message;
};

enum class log_level {
    info,
    warning,
    critical
};

struct variable_result {
    bool success = false;
    std::string message;
    std::string value;
};

struct user_variable_result {
    bool success = false;
    std::string message;
    std::string key;
    std::string value;
};

struct file_result {
    bool success = false;
    std::string message;
    std::vector<std::uint8_t> bytes;
    std::uint64_t size_bytes = 0;
};

struct subscription {
    std::string id;
    std::string name;
    std::string slug;
    std::string expires_at;
    std::string expiry;
};

struct login_result {
    bool success = false;
    std::string message;
    std::string expiry;
    std::vector<subscription> subscriptions;
};

bool configure(const sdk_config& config);
sdk_config current_config();
std::string last_error();
init_result init(std::string_view application_id, std::string_view application_version);
register_result register_user(std::string_view username, std::string_view password, std::string_view license_key);
login_result login(std::string_view username, std::string_view password);
license_result license(std::string_view license_key);
license_result activate_license(std::string_view license_key, std::string_view username = {});
ban_result ban();
bool log(std::string_view message, log_level level);
variable_result variable(std::string_view name);
user_variable_result user_variable(std::string_view key);
user_variable_result update_user_variable(std::string_view key, std::string_view value);
file_result file(std::string_view name);

}  // namespace obsidium
