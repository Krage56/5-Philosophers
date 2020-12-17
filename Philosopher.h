//
// Created by krage56 on 17.12.2020.
//

#ifndef PHILOSOPHERS_PHILOSOPHER_H
#define PHILOSOPHERS_PHILOSOPHER_H
using namespace std;
#include <string>
#include <cstdlib>
#include <chrono>
#include <memory>
#include <mutex>

enum class Status{
    starvation,
    is_thinking,
    is_eating,
    is_waiting,
    undefined
};
struct Order{
    string name;
    Status status;
    chrono::seconds famine_sec;
}typedef Order;

class Philosopher {
public:
    Philosopher(const string& input_name, size_t fork);
    [[noreturn]] void livingProcess();
    [[nodiscard]] shared_ptr<Order> getOrder()const;
protected:
    void eatingProcess();
    void thinkingProcess();
private:
    Status _status;
    chrono::seconds _famineSec;
    string _name;
    pair<size_t, bool>_rightFork;
    pair<size_t, bool>_leftFork;
};


#endif //PHILOSOPHERS_PHILOSOPHER_H
