#pragma once
#include <coroutine>
#include <exception>
#include <functional>
#include <memory_resource>

namespace driver::async {

struct coro_task {
    static inline std::pmr::memory_resource*& get_resource() noexcept {
        static std::pmr::memory_resource* res = std::pmr::new_delete_resource();
        return res;
    }

    static inline void set_resource(std::pmr::memory_resource& r) noexcept {
        get_resource() = &r;
    }

    struct promise_type {
        coro_task get_return_object() {
            return coro_task{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }

        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }

        static void* operator new(std::size_t size) {
            return coro_task::get_resource()->allocate(size, alignof(promise_type));
        }

        static void operator delete(void* ptr, std::size_t size) noexcept {
            #if defined(PLATFORM_HOST)
                coro_task::get_resource()->deallocate(ptr, size, alignof(promise_type));
            #else
                (void)ptr;
                (void)size;
                std::terminate();
            #endif
        }
    };

    std::coroutine_handle<promise_type> h;

    coro_task(const coro_task&) noexcept = delete;
    coro_task& operator=(const coro_task&) noexcept = delete;
    explicit coro_task(std::coroutine_handle<promise_type> h) noexcept : h(h) {}
    coro_task(coro_task&& other) noexcept : h(other.h) { other.h = {}; }

    void resume() noexcept {
        if (h && !h.done())
            h.resume();
    }

    bool done() const noexcept {
        return !h || h.done();
    }
};

template<typename F, typename... Ts>
static inline auto launch_task(F && f, Ts &&... ts) noexcept
{ return std::invoke(static_cast<F&&>(f), static_cast<Ts&&>(ts)...); }

}
