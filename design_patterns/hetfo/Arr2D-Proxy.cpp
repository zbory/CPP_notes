#include <utility>

template<typename T, std::size_t N, std::size_t M>
class Arr2D {

    private:

		T data[N][M];
		struct proxy {
		    private:
			    T *arr;
		    public:
			    explicit proxy(T *arr) : arr{ arr } {}
				[[nodiscard("mert")]] T& operator[](std::size_t idx) {
					return arr[idx];
				}
		};

    public:
		proxy operator[](std::size_t idx) {
			return proxy{ data[idx] };
		}

};


int main() {

	Arr2D<int, 4, 3> arr;

	arr[12][5] = 15;

}