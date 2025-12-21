#pragma once
#include <exception>

class Error : public std::exception {
public:
    explicit Error(const char* msg) : msg_(msg) {}
    const char* what() const noexcept override { return msg_; }
private:
    const char* msg_;
};

class LogicError : public Error { public: explicit LogicError(const char* m) : Error(m) {} };
class InvalidMoveError : public Error { public: explicit InvalidMoveError(const char* m) : Error(m) {} };
class PresenterError : public Error { public: explicit PresenterError(const char* m) : Error(m) {} };
