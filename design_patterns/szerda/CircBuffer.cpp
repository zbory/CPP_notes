#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>


class CBuffer {

    private:
        std::size_t capacity;
        int *data;
        std::size_t count = 0, front = 0, rear = 0;

        std::mutex mux;
        std::condition_variable not_full;
        std::condition_variable not_empty;

    public:

        CBuffer(std::size_t capacity)
            : capacity{ capacity }
            , data{ new int[capacity] }
        {}

        ~CBuffer() { delete data; }

        CBuffer(const CBuffer&) = delete;
        CBuffer& operator=(const CBuffer&) = delete;
        CBuffer(CBuffer&&) = delete;
        CBuffer& operator=(CBuffer&&) = delete;

        void push(int i) {
            std::unique_lock<std::mutex> lck{ mux };                      
            not_full.wait(lck, [this](){ return capacity != count; });

            data[rear] = i;
            rear = (rear + 1) % capacity;
            count++; 

            not_empty.notify_one();
        }

        int pop() {
            std::unique_lock<std::mutex> lck{ mux };
            not_empty.wait(lck, [this]() { return count != 0; });

            const auto i = data[front];
            front = (front + 1) % capacity;
            count--;

            not_full.notify_one();

            return i;
        }

};


void producer(CBuffer& b, int i) {
    for (int i = 0; i < 4; i++) {
        b.push(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void consumer(CBuffer& b) {
    for (int i = 0; i < 6; i++) {
        std::cout << std::this_thread::get_id() << ": " << b.pop() << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


int main() {

    CBuffer buffer{ 3u };

    std::thread t1{ producer, std::ref(buffer), 1 };
    std::thread t2{ producer, std::ref(buffer), 2 };
    std::thread t3{ producer, std::ref(buffer), 3 };

    std::thread t4{ consumer, std::ref(buffer) };
    std::thread t5{ consumer, std::ref(buffer) };

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
}
