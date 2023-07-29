#ifndef OBSERVER_H_
#define OBSERVER_H_

struct Observer {

    virtual void update(float temp, float humidity, float pressure) = 0;
    virtual ~Observer() = default;

};

#endif  /* OBSERVER_H_ */