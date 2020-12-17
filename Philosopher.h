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
struct Order{
    bool starvation;
    chrono::seconds famine_sec;
    std::chrono::duration<double> time_of_live;
}typedef Order;

class Philosopher {
public:
    Philosopher(const string& input_name, size_t fork);
    [[noreturn]] void livingProcess();
    [[nodiscard]] shared_ptr<Order> getOrder()const;
private:
    bool _starvation;
    bool _left;
    bool _right;
    chrono::seconds _famine_sec;
    chrono::time_point<std::chrono::system_clock>
    _time_of_birth;
    string _name;
    size_t _right_fork;
    size_t _left_fork;

};


#endif //PHILOSOPHERS_PHILOSOPHER_H
