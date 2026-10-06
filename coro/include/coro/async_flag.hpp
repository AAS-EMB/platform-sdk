#pragma once
#include <coroutine>
#include <cassert>

namespace driver::async {

struct async_flag {
    async_flag() noexcept = default;
    async_flag(const async_flag&) noexcept = delete;
    async_flag& operator=(const async_flag&) noexcept = delete;
    ~async_flag() noexcept {
        handle_ = {};
    }

    async_flag& operator=(bool value) noexcept {
        value_ = value;

        if (value_ and handle_) {
            auto h  = handle_;
            handle_ = {};
            h.resume();
        }

        return *this;
    }

    operator bool() const {
        return value_;
    }

    bool await_ready() const noexcept {
        return value_;
    }

    void await_suspend(std::coroutine_handle<> h) noexcept {
        assert(handle_ == nullptr && "Only one waiter");
        if (value_) {
            h.resume();
        } else {
            handle_ = h;
        }
    }

    bool await_resume() noexcept {
        handle_ = {};
        return value_;
    }

private:
    bool value_{false};
    std::coroutine_handle<> handle_{};
};

}
