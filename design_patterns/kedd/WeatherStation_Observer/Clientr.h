#ifndef CLIENT_H_
#define CLIENT_H_

#include <iostream>

#include "Observer.h"

class Client : public Observer {

private:

    int id;

public:

    Client(int id) : id{ id } {}

    void update(float temp, float humidity, float pressure) final {
        std::cout << "---Client(" << id << ") Data---"
            << "\nTemperature:\t" << temp
            << "\nHumidity:\t" << humidity
            << "\nPressure:\t" << pressure
            << '\n';
    }
};

#endif  /* CLIENT_H_ */