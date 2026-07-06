# Obsidium SDK API Reference

Short GitHub-friendly reference for the public C++ SDK API.

## Setup

```cpp
#include "obsidium.hpp"
```

Basic flow:

```cpp
auto init = obsidium::init("APP_ID", "1.0.0");
// init.success
// init.message

if (!init.success) {
    return false;
}

auto login = obsidium::login("username", "password");
// login.success
// login.message
// login.expiry
// login.subscriptions

if (!login.success) {
    return false;
}
```

## Result Structs

### `init_result`

Returned by `init`.

| Field | Type | Meaning |
| --- | --- | --- |
| `success` | `bool` | `true` when SDK initialization succeeded. |
| `message` | `std::string` | Status or error message. |

### `register_result`

Returned by `register_user`.

| Field | Type | Meaning |
| --- | --- | --- |
| `success` | `bool` | `true` when registration succeeded. |
| `message` | `std::string` | Status or error message. |

### `license_result`

Returned by `license` and `activate_license`.

| Field | Type | Meaning |
| --- | --- | --- |
| `success` | `bool` | `true` when the license action succeeded. |
| `message` | `std::string` | Status or error message. |
| `license_required` | `bool` | `true` when another license step is still required. |
| `expiry` | `std::string` | License expiry when supplied by the server. |

### `login_result`

Returned by `login`.

| Field | Type | Meaning |
| --- | --- | --- |
| `success` | `bool` | `true` when login succeeded with an active accepted entitlement. |
| `message` | `std::string` | Status or error message. |
| `expiry` | `std::string` | Account/license expiry when supplied. |
| `subscriptions` | `std::vector<subscription>` | Active subscriptions. |

### `subscription`

Used inside `login_result`.

| Field | Type | Meaning |
| --- | --- | --- |
| `id` | `std::string` | Subscription ID. |
| `name` | `std::string` | Display name. |
| `slug` | `std::string` | Stable plan slug. |
| `expires_at` | `std::string` | Expiry timestamp. |
| `expiry` | `std::string` | Compatibility expiry field. |

### `ban_result`

Returned by `ban`.

| Field | Type | Meaning |
| --- | --- | --- |
| `success` | `bool` | `true` when the ban request succeeded. |
| `message` | `std::string` | Status or error message. |

### `variable_result`

Returned by `variable`.

| Field | Type | Meaning |
| --- | --- | --- |
| `success` | `bool` | `true` when the variable was retrieved. |
| `message` | `std::string` | Status or error message. |
| `value` | `std::string` | Variable value. |

### `user_variable_result`

Returned by `user_variable` and `update_user_variable`.

| Field | Type | Meaning |
| --- | --- | --- |
| `success` | `bool` | `true` when the user variable action succeeded. |
| `message` | `std::string` | Status or error message. |
| `key` | `std::string` | User variable key. |
| `value` | `std::string` | User variable value. |

### `file_result`

Returned by `file`.

| Field | Type | Meaning |
| --- | --- | --- |
| `success` | `bool` | `true` when the file was retrieved. |
| `message` | `std::string` | Status or error message. |
| `bytes` | `std::vector<std::uint8_t>` | File bytes. |
| `size_bytes` | `std::uint64_t` | File size in bytes. |

### `log_level`

Used by `log`.

```cpp
obsidium::log_level::info
obsidium::log_level::warning
obsidium::log_level::critical
```

## Functions

### `last_error`

Returns the last SDK error string for functions that return `bool`.

```cpp
std::string error = obsidium::last_error();
// error
```

Result:

```cpp
"Invalid Response"
```

### `init`

Initializes the SDK session. Call this before auth or resource requests.

```cpp
auto result = obsidium::init("APP_ID", "1.0.0");
// result.success
// result.message
```

Result:

```cpp
obsidium::init_result {
    .success = true,
    .message = "Initialized"
}
```

### `register_user`

Creates a username/password account with a license key.

```cpp
auto result = obsidium::register_user("username", "password", "XXXX-XXXX-XXXX-XXXX");
// result.success
// result.message
```

Result:

```cpp
obsidium::register_result {
    .success = true,
    .message = "Registration successful"
}
```

### `login`

Logs in with username/password.

```cpp
auto result = obsidium::login("username", "password");
// result.success
// result.message
// result.expiry
// result.subscriptions
```

Result:

```cpp
obsidium::login_result {
    .success = true,
    .message = "Login successful",
    .expiry = "2026-12-31T23:59:59.000Z",
    .subscriptions = {
        {
            .id = "sub_123",
            .name = "Pro",
            .slug = "pro",
            .expires_at = "2026-12-31T23:59:59.000Z",
            .expiry = "2026-12-31T23:59:59.000Z"
        }
    }
}
```

Expired users, users without an active entitlement, and malformed responses return `success = false`.

Reading subscriptions:

```cpp
for (const auto& subscription : result.subscriptions) {
    // subscription.id
    // subscription.name
    // subscription.slug
    // subscription.expires_at
    // subscription.expiry
}
```

### `license`

Logs in with a license key.

```cpp
auto result = obsidium::license("XXXX-XXXX-XXXX-XXXX");
// result.success
// result.message
// result.license_required
// result.expiry
```

Result:

```cpp
obsidium::license_result {
    .success = true,
    .message = "License accepted",
    .license_required = false,
    .expiry = "2026-12-31T23:59:59.000Z"
}
```

### `activate_license`

Activates a license for a supplied username, or for the current authenticated session when the username is omitted.

```cpp
auto result = obsidium::activate_license("XXXX-XXXX-XXXX-XXXX", "enduser");
// result.success
// result.message
// result.license_required
// result.expiry
```

Result:

```cpp
obsidium::license_result {
    .success = true,
    .message = "License activated",
    .license_required = false,
    .expiry = "2026-12-31T23:59:59.000Z"
}
```

### `ban`

Bans the current session/user.

```cpp
auto result = obsidium::ban();
// result.success
// result.message
```

Result:

```cpp
obsidium::ban_result {
    .success = true,
    .message = "Successfully banned"
}
```

### `log`

Sends a log message to the API.

```cpp
bool ok = obsidium::log("Application started", obsidium::log_level::info);
// ok
// obsidium::last_error() if ok is false
```

Result:

```cpp
true  // log accepted
false // log failed, call obsidium::last_error()
```

### `variable`

Gets an application variable.

```cpp
auto result = obsidium::variable("news_message");
// result.success
// result.message
// result.value
```

Result:

```cpp
obsidium::variable_result {
    .success = true,
    .message = "Successfully retrieved variable",
    .value = "Welcome to Obsidium."
}
```

### `user_variable`

Gets a variable for the authenticated user.

```cpp
auto result = obsidium::user_variable("theme");
// result.success
// result.message
// result.key
// result.value
```

Result:

```cpp
obsidium::user_variable_result {
    .success = true,
    .message = "Successfully retrieved user data",
    .key = "theme",
    .value = "dark"
}
```

### `update_user_variable`

Sets a variable for the authenticated user.

```cpp
auto result = obsidium::update_user_variable("theme", "dark");
// result.success
// result.message
// result.key
// result.value
```

Result:

```cpp
obsidium::user_variable_result {
    .success = true,
    .message = "Successfully saved user data",
    .key = "theme",
    .value = "dark"
}
```

### `file`

Gets a file from the API.

```cpp
auto result = obsidium::file("config.bin");
// result.success
// result.message
// result.bytes
// result.size_bytes
```

Result:

```cpp
obsidium::file_result {
    .success = true,
    .message = "Successfully retrieved file",
    .bytes = { 0x48, 0x65, 0x6c, 0x6c, 0x6f },
    .size_bytes = 5
}
```

## Failure Handling

Always check `success` before using returned data.

```cpp
auto result = obsidium::variable("example");
// result.success
// result.message
// result.value

if (!result.success) {
    return;
}

std::string value = result.value;
```

Common failure shape:

```cpp
obsidium::variable_result {
    .success = false,
    .message = "Invalid Response",
    .value = ""
}
```
