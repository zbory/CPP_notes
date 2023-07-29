#include <string>
#include <iostream>
#include <mutex>
#include <future>
#include <vector>


template<typename T>
class monitor {
    private:

        T t;
        mutable std::mutex mux;

    public:

        monitor(T t = T{})
            : t{ t } 
        {}

        template<typename F>
        auto operator()(F &&f) {
            std::lock_guard<std::mutex> _{ mux };
            return std::invoke(std::forward<F>(f), t);
        }

        template<typename F>
        auto operator()(F&& f) const {
            std::lock_guard<std::mutex> _{ mux };
            return std::invoke(std::forward<F>(f), t);
        }
};

int main() {
    monitor<std::string> s;

    std::vector<std::future<void>> v;

    for (int i = 0; i < 30; i++) {
        v.push_back(
            std::async([&, i] {
                s(
                    [=](std::string &s) {
                        s += std::to_string(i);                       
                    }
                );
            })
        );
    }

    for (auto& f : v)
        f.wait();

    std::as_const(s)([](const std::string& s) { std::cout << s << '\n'; });
}