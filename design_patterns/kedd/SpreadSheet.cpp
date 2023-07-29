#include <iostream>

#include <functional>
#include <future>
#include <optional>
#include <initializer_list>

template<typename T, std::size_t N, std::size_t M>
class Arr2D {
    private:
        T data[N][M];
    public:
        
        struct proxy {
            private:
                T *arr;
            public:
                explicit proxy(T *arr) : arr{ arr } {}                
                T& operator[](std::size_t idx) {
                    return arr[idx];
                }
        };

        struct const_proxy {
        private:
            const T *arr;
        public:
            explicit const_proxy(const T *arr) : arr{ arr } {}
            const T& operator[](std::size_t idx) const {
                return arr[idx];
            }            
        };

        const_proxy operator[](std::size_t idx) const {
            return const_proxy{ data[idx] };
        }

        proxy operator[](std::size_t idx) {
            return proxy{ data[idx] };
        }
};

template<typename T>
class Lazy {
    private:

        mutable std::future<T>   fut;
        mutable std::optional<T> val;

    public:

        Lazy() : val{ T{} } {}

        template<typename F, typename ...Args>
        Lazy(F f, Args&&... args)
            : fut{ std::async(std::launch::deferred, std::move(f), std::forward<Args>(args)...) }
        {}

        const auto& get() const {
            if (!val)
                val = fut.get();
            return *val;
        }
};

template<typename T, std::size_t N, std::size_t M>
class SpreadSheet;

template<typename T, std::size_t N, std::size_t M>
struct LineOfFun {
    std::vector<std::function<T(const SpreadSheet<T, N, M>&)>> funs;
    LineOfFun(std::initializer_list<
                  std::function<T(const SpreadSheet<T, N, M>&)>
              > lst) 
        : funs{ lst }
    {}
};

template<typename T, std::size_t N, std::size_t M>
class SpreadSheet {

    private:

        Arr2D<Lazy<T>, N, M> lazy_res;

    public:

        SpreadSheet(std::initializer_list<LineOfFun<T, N, M>> fs) {
            std::size_t i = 0;
            for (const auto &line : fs) {
                std::size_t j = 0;
                for (const auto &f : line.funs) {
                    lazy_res[i][j] = Lazy<T>{ f, std::cref(*this) };
                    j++;
                }
                i++;
            }
        }
                
        struct const_proxy {
            private:
                const Arr2D<Lazy<T>, N, M> &lazy_res;
                std::size_t i;
            public:

                const_proxy(const Arr2D<Lazy<T>, N, M> &lazy_res,
                      std::size_t i)
                    : lazy_res{ lazy_res }, i{ i } {}

                const T& operator[](std::size_t j) const {
                    return lazy_res[i][j].get();
                }
        };

        const_proxy operator[](std::size_t i) const {
            return const_proxy{ lazy_res, i };
        }

        auto operator()() {
            Arr2D<T, N, M> res;
            for (std::size_t i = 0; i < N; i++) {
                for (std::size_t j = 0; j < M; j++) {
                    res[i][j] = lazy_res[i][j].get();
                }
            }
            return res;
        }

};


int main() {

    auto tbl = SpreadSheet<int, 2, 2>{
        {
            [](const auto &sp) { return sp[0][1] + 1; },
            [](const auto &sp) { return sp[1][1] + 1; },
        },
        {
            [](const auto &sp) { return sp[0][0] + 10; },
            [](const auto &sp) { return sp[1][0] + 10; },
        }
    }();

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++)
            std::cout << tbl[i][j] << ' ';
        std::cout << '\n';
    }

}
