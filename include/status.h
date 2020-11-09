#pragma once

#include <algorithm>
#include <string>

class Status {
public: 
    Status() {}
    Status(const std::string& msg) : _msg(msg) {}
    Status(const std::string&& msg) : _msg(std::move(msg)) {}

    Status(const Status& rhs) : _msg(rhs._msg) {}
    Status& operator=(const Status& rhs);

    Status(std::string&& msg) : _msg(std::move(msg)) {}
    Status& operator=(Status&& rhs) noexcept;

    static Status OK() { return Status(); }
    static Status Error(const std::string& msg) { return Status(msg); }
    static Status Error(std::string&& msg) { return Status(std::move(msg)); }

    inline bool ok() const { return _msg.empty(); }
    inline const std::string& msg() { return _msg; }

private:
    std::string _msg;
};

inline Status& Status::operator=(const Status& rhs) {
    if (this != &rhs) {
        _msg = rhs._msg;
    }

    return *this;
}

inline Status& Status::operator=(Status&& rhs) noexcept {
    if (this != &rhs) {
        _msg = std::move(rhs._msg);
    }

    return *this;
}