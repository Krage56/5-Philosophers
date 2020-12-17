#include <memory>
#include <vector>
#include <thread>
#include <iomanip>
#include <iostream>
#include <cstring>
#include "Philosopher.h"
using namespace std;


[[noreturn]]void printOrder(vector<Philosopher>& philosophers){
    const int frequency = 2;
    while(true) {
        cout << setfill('_') << setw(66);
        cout << "" << endl << flush;
        cout << setfill(' ');
        cout << "|" << setw(9) << "Name" << setw(6) << " |" << flush;
        cout << "  " << setw(9) << "Status" << setw(6) << "|" << flush;
        cout << "  " << "Duration of fasting (in sec)"
             << setw(3) << "|" << endl << flush;
        cout << setfill('_') << setw(67);
        cout << "\n" << flush;

        for (const auto &p: philosophers) {
            shared_ptr<Order> tmp = p.getOrder();
            cout << "|" << setfill(' ') <<
            tmp->name << setw(15-tmp->name.length()) << "|";
            int correction = 0;
            switch(tmp->status){
                case Status::starvation:{
                    cout << "Is starving";
                    correction = strlen("Is starving");
                    break;
                }
                case Status::is_thinking:{
                    cout << "Is thinking";
                    correction = strlen("Is thinking");
                    break;
                }
                case Status::is_eating:{
                    cout << "Is eating";
                    correction = strlen("Is eating");
                    break;
                }
                case Status::is_waiting:{
                    cout << "Is waiting";
                    correction = strlen("Is waiting");
                    break;
                }
                case Status::undefined:{
                    cout << "Isn't living";
                    correction = strlen("Isn't living");
                    break;
                }
            }
            cout << setw(17-correction) << "|" << flush;
            cout <<""<< setw((34)/2)
            <<  tmp->famine_sec.count() << setw((34)/2) << "|\n" << flush;
        }

        this_thread::sleep_for(chrono::seconds(frequency));
    }
}


int main(){
    vector<Philosopher> philosophers;
    vector<thread> threads;
    philosophers.emplace_back(Philosopher("Socrat", 0));
    philosophers.emplace_back(Philosopher("Platon", 1));
    philosophers.emplace_back(Philosopher("Decart", 2));
    philosophers.emplace_back(Philosopher("Aristotel", 3));
    philosophers.emplace_back(Philosopher("Fridman", 4));
    for(auto& philosopher: philosophers){
        threads.emplace_back(thread([&philosopher](){philosopher.livingProcess();}));
    }
    thread print_thread(printOrder, ref(philosophers));
    for(auto& t: threads){
        t.join();
    }
    print_thread.join();
    return 0;
}

