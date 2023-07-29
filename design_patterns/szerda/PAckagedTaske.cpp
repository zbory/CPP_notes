#include <future>
#include <thread>
#include <functional>
#include <iostream>

template<typename>class Task;

template<typename R, typename ...Args>
class Task<R(Args...)> {

    private:

        std::function<R(Args...)> fun;
        std::promise<R>           prom;

    public:

        Task(std::function<R(Args...)> fun)
            : fun{ std::move(fun) }
        {}


        auto get_future() {
            return prom.get_future();
        }

        template<typename ...Ts>
        void operator()(Ts&& ...ts) noexcept {
            try {
                prom.set_value(std::invoke(fun, std::forward<Ts>(ts)...));
            }
            catch (...) {
                prom.set_exception(std::current_exception());
            }
        }
};

int main() {


    Task<int(int, int)> task{
       [](int a, int b) { return a + b; }
    };

    auto fut = task.get_future();

    std::thread th{ std::move(task), 1, 45 };

    std::cout << fut.get() << '\n';

    th.join();
}