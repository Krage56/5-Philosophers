//
// Created by krage56 on 17.12.2020.
//

#include "Philosopher.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <ctime>
mutex cout_mutex;
static mutex forks[5];
void print_msg(const string& msg)
{
    cout_mutex.lock();
    cout << msg << flush;
    cout_mutex.unlock();
}
Philosopher::Philosopher(const string &input_name, size_t fork) {
    _name = input_name;
    _right_fork = fork;
    _left_fork = (fork + 4) % 5;
    _time_of_birth = chrono::system_clock::now();
    _famine_sec = 0s;
    _starvation = false;
    _left = false;
    _right = false;
}

shared_ptr<Order> Philosopher::getOrder() const {
    shared_ptr<Order> order(new Order);
    order->famine_sec = _famine_sec;
    order->time_of_live = chrono::system_clock::now() - _time_of_birth;
    return order;
}

void Philosopher::livingProcess() {
    srand(time(NULL));
    while(true){
        if(!_starvation){
            const int thinking = 8;
            ostringstream ostr;
            ostr << _name << " is thinking about..." << endl;
            print_msg(ostr.str());
            _famine_sec += chrono::seconds(thinking);
            this_thread::sleep_for(chrono::seconds(thinking));
        }
        {
            int waiting = rand() % 5;
            _left = _left ? _left : forks[_left_fork].try_lock();
            _right = _right ? _right : forks[_right_fork].try_lock();
            if (_left && _right) {
                _starvation = false;
                _famine_sec = 0s;
                ostringstream ostr;
                ostr << _name << " is eating now!" << endl;
                print_msg(ostr.str());
            }
            else {
                ostringstream ostr;
                ostr << _name << " can't eating now!" << endl;
                print_msg(ostr.str());
                if (_famine_sec > 12s) {
                    _starvation = true;
                }
                _famine_sec += chrono::seconds(waiting);
            }
            if (!_starvation) {
                if (_left) {
                    _left = false;
                    forks[_left_fork].unlock();
                }
                if (_right) {
                    _right = false;
                    forks[_right_fork].unlock();
                }
            }
            if (_starvation) {
                ostringstream ostr;
                ostr << _name << " is starving" << endl;
            }
            if(_starvation && _famine_sec > 30s){
                ostringstream ostr;
                ostr << _name << " has died" << endl;
                break;
            }
            this_thread::sleep_for(chrono::seconds(waiting));
        }
    }
}
