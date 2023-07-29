#include <iostream>
#include <thread>
#include <atomic>
#include <cstdlib>
#include <ctime>

#include "WeatherStation.h"
#include "Clientr.h"


int main() {

	WeatherStation weatherStation;

	auto one = std::make_shared<Client>(1);
	auto two = std::make_shared<Client>(2);
	auto three = std::make_shared<Client>(3);
	auto four = std::make_shared<Client>(4);

	weatherStation.registerObserver(one);
	weatherStation.registerObserver(two);
	weatherStation.registerObserver(three);

	std::atomic_bool ready{ false };

	std::thread th{ [&ready, &weatherStation]() {
		std::srand(std::time(0));

		while (!ready.load()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(
				10 + std::rand() % 90
			));

			weatherStation.setState(std::rand() % 40 - 10,
									std::rand() % 100,
									std::rand() % 20);
		}
	} };


	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	weatherStation.removeObserver(three);
	weatherStation.registerObserver(four);
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	ready.store(true);
	th.join();
}
