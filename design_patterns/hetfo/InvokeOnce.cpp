#include <iostream>
#include <string>
#include <functional>
#include <tuple>

template<typename ...Args>
class InvokeInScopeOnce {
    private:
        std::function<void(Args...)> callback;
        std::tuple<Args...> defaultArgs;
        bool called = false;

    public:

        template<typename F, typename ...T>
        InvokeInScopeOnce(F fun, T&& ...t)
            : callback{ std::move(fun) }
            , defaultArgs{ std::forward<T>(t)... }
        {}

        InvokeInScopeOnce(const InvokeInScopeOnce&) = delete;
        InvokeInScopeOnce& operator=(const InvokeInScopeOnce&) = delete;

        InvokeInScopeOnce(InvokeInScopeOnce&&) = delete;
        InvokeInScopeOnce& operator=(InvokeInScopeOnce&&) = delete;

        template<typename ...T>
        void operator()(T&& ...t) const {
            called = true;
            std::invoke(callback, std::forward<T>(t)...);
        }

        ~InvokeInScopeOnce() {
            if (!called)
                std::apply(callback, defaultArgs);
        }
};

// make_...
template<typename F, typename ...Args>
auto invokeInScopeOnce(F f, Args&& ...args) {
    return InvokeInScopeOnce<std::remove_reference_t<Args>...>{
        std::move(f),
        std::forward<Args>(args)... };
}

// C++17, deduction guide
template<typename F, typename ...Args>
InvokeInScopeOnce(F, Args&& ...args)
    ->InvokeInScopeOnce<std::remove_reference_t<Args>...>;


int main() {

    {
        auto i = 7;

        auto iisc = invokeInScopeOnce([](int i, int j) {
            std::cout << (i + j) << '\n'; }, 5, std::cref(i));

        i = 100;
    }

    {
        auto iisc = InvokeInScopeOnce{ [](int i, int j) {
            std::cout << (i + j) << '\n';
        }, 5, 12 };

    }
}
