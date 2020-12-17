//
// Created by krage56 on 17.12.2020.
//

#include "Philosopher.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <ctime>

mutex forks[5];
Philosopher::Philosopher(const string &input_name, size_t fork) {
    _name = input_name;
    _rightFork.first = fork;
    _leftFork.first = (fork + 4) % 5;
    _famineSec = 0s;
    _status = Status::undefined;
    _rightFork.second = false;
    _leftFork.second = false;
}

shared_ptr<Order> Philosopher::getOrder() const {
    shared_ptr<Order> order(new Order);
    order->famine_sec = _famineSec;
    order->status = _status;
    order->name = _name;
    return order;
}

void Philosopher::livingProcess() {
    while(true){
        thinkingProcess();
        eatingProcess();
    }
}

void Philosopher::thinkingProcess() {
    const int thinking = 8;
    _status = Status::is_thinking;
    _famineSec += chrono::seconds(thinking);
    this_thread::sleep_for(chrono::seconds(thinking));
}

void Philosopher::eatingProcess() {
    srand(time(nullptr));
    const int aggression_limit = 15;
    _status = Status::is_waiting;
    while(!_leftFork.second || !_rightFork.second){
        if(_famineSec > chrono::seconds(aggression_limit)){
            _status = Status::starvation;
        }
        int waiting = rand() % 5;
        _leftFork.second = _leftFork.second ? _leftFork.second :
                           forks[_leftFork.first].try_lock();
        _rightFork.second = _rightFork.second ? _rightFork.second :
                            forks[_rightFork.first].try_lock();
        if (_leftFork.second && _rightFork.second) {
            _status = Status::is_eating;
            _famineSec = 0s;
            ostringstream ostr;
            waiting = 4;
        }
        else{
            _famineSec += chrono::seconds(waiting);
        }
        if((_status != Status::starvation) &&
        (_leftFork.second ^ _rightFork.second)){
            if(_leftFork.second){
                _leftFork.second = false;
                forks[_leftFork.first].unlock();
            }
            if(_rightFork.second){
                _rightFork.second = false;
                forks[_rightFork.first].unlock();
            }
        }
        this_thread::sleep_for(chrono::seconds(waiting));
    }
    _leftFork.second = false;
    forks[_leftFork.first].unlock();
    _rightFork.second = false;
    forks[_rightFork.first].unlock();
}
