#include <utility>
#include <future>
#include <deque>
#include <vector>
#include <algorithm>
#include <iostream>

struct IsPrime {
	std::pair<bool, int> operator()(int i) const {
		for (int j = 2; j * j <= i; j++)
			if (i % j == 0) return { false, i };
		return { true, i };
	}
};

class ActiveObject {

    private:

		std::mutex mux;
		std::deque<
			std::pair<
			    std::packaged_task<std::pair<bool, int>(int i)>,
			    int
			>
		> activationList;

    public:

		std::future<std::pair<bool, int>> enqueueTask(int i) {
			std::packaged_task<
				std::pair<bool, int>(int i)
			> job{ IsPrime{} };
			auto fut = job.get_future();

			{
				std::lock_guard<std::mutex> _{ mux };
				activationList.emplace_back(std::move(job), i);
			}

			return fut;
		}

		void run() {
			std::vector<std::jthread> servants;
			for (int i = 0; i < 3; i++) {
				servants.emplace_back(
					[this] {
						while (runNextTask())
							;
					}
				);
			}
		}

    private:

		bool runNextTask() {
			std::lock_guard<std::mutex> lck{ mux };
			if (!activationList.empty()) {
				auto task = std::move(activationList.front());
				activationList.pop_front();
				task.first(task.second);
				return true;
			}
			return false;
		}

};

std::future<std::vector<std::future<std::pair<bool, int>>>>
addTasks(ActiveObject& ao, int num) {
	return std::async(
		[&ao, num] {
			std::vector<std::future<std::pair<bool, int>>> futs;
			for (int i = 0; i < num; i++) {
				futs.push_back(ao.enqueueTask(i));
			}
			return futs;
		}
	);
}


int main() {


	ActiveObject ao;

	auto client1 = addTasks(ao, 1998);
	auto client2 = addTasks(ao, 2003);
	auto client3 = addTasks(ao, 2011);
	auto client4 = addTasks(ao, 2014);
	auto client5 = addTasks(ao, 2017);
	auto client6 = addTasks(ao, 2020);
	auto client7 = addTasks(ao, 2023);

	auto futures = client1.get();
	auto f2 = client2.get();
	auto f3 = client3.get();
	auto f4 = client4.get();
	auto f5 = client5.get();
	auto f6 = client6.get();
	auto f7 = client7.get();

	futures.insert(
		futures.cend(),
		std::make_move_iterator(f2.begin()),
		std::make_move_iterator(f2.end())
	);
	futures.insert(
		futures.cend(),
		std::make_move_iterator(f3.begin()),
		std::make_move_iterator(f3.end())
	);
	futures.insert(
		futures.cend(),
		std::make_move_iterator(f4.begin()),
		std::make_move_iterator(f4.end())
	);
	futures.insert(
		futures.cend(),
		std::make_move_iterator(f5.begin()),
		std::make_move_iterator(f5.end())
	);
	futures.insert(
		futures.cend(),
		std::make_move_iterator(f6.begin()),
		std::make_move_iterator(f6.end())
	);
	futures.insert(
		futures.cend(),
		std::make_move_iterator(f7.begin()),
		std::make_move_iterator(f7.end())
	);


	ao.run();

	std::vector<std::pair<bool, int>> futRes;
	futRes.reserve(futures.size());
	for (auto& f : futures)
		futRes.push_back(f.get());

	std::sort(futRes.begin(), futRes.end());
	auto prIt = std::find_if(futRes.cbegin(), futRes.cend(),
	          	             [](std::pair<bool, int> p) { return p.first == true; });

	std::cout << "Number of Primes: "
	       	  << std::distance(prIt, futRes.cend()) << '\n';

	std::cout << "Primes: ";
	std::for_each(prIt, futRes.cend(), [](const auto& p) {
		std::cout << p.second << ' ';
	});

	std::cout << "\n\n";

	std::cout << "Number of Not-Primes: "
		      << std::distance(futRes.cbegin(), prIt) << '\n';

	std::cout << "Not-Primes: ";
	std::for_each(futRes.cbegin(), prIt, [](const auto& p) {
		std::cout << p.second << ' ';
	});
}