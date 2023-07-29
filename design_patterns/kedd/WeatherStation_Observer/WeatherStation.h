#ifndef SUBJECT_H_
#define SUBJECT_H_

#include <vector>
#include <algorithm>
#include <mutex>
#include <memory>

#include "Observer.h"

class Subject {

private:

    std::mutex mux;
    std::vector<std::shared_ptr<Observer>> observers;

public:

    virtual ~Subject() = default;

    void registerObserver(const std::shared_ptr<Observer>& obs) {
        std::lock_guard<std::mutex> _{ mux };
        if (std::find(std::cbegin(observers), std::cend(observers), obs) == std::cend(observers)) {
            observers.push_back(obs);
        }
    }

    void removeObserver(const std::shared_ptr<Observer>& obs) {
        std::lock_guard<std::mutex> _{ mux };
        const auto it = std::find(std::cbegin(observers), std::cend(observers), obs);
        if (it != std::cend(observers))
            observers.erase(it);
    }

    void notifyObservers(float temp, float humidity, float pressure) {
        std::lock_guard<std::mutex> _{ mux };
        for (const auto& obs : observers)
            obs->update(temp, humidity, pressure);
    }

};


class WeatherStation : public Subject {

private:

    float temp = 30;
    float humidity = 50;
    float pressure = 72;

public:

    void setState(float temp, float humidity, float pressure) {
        this->temp = temp;
        this->humidity = humidity;
        this->pressure = pressure;

        notifyObservers(temp, humidity, pressure);
    }
};




#endif  /* SUBJECT_H_ */
