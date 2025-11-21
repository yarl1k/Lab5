#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <syncstream>
#include <iomanip>

using namespace std;

void task(const string& name, int duration) {
    this_thread::sleep_for(chrono::seconds(duration));
    osyncstream(cout) << name << endl;
}

void slow(const string& name) {
    task(name, 7);
}

void quick(const string& name) {
    task(name, 1);
}

void work() {

    future<void> f_A1 = async(launch::async, slow, "A1");
    future<void> f_A2 = async(launch::async, slow, "A2");

    slow("D");

    f_A1.wait();
    f_A2.wait();

    future<void> f_B = async(launch::async, quick, "B");

    quick("F");

    f_B.wait();

    future<void> f_C1 = async(launch::async, quick, "C1");

    quick("C2");

    f_C1.wait();

    osyncstream(cout) << "Work is done!" << endl;
}

int main() {
    auto start_time = chrono::steady_clock::now();
    work();

    auto end_time = chrono::steady_clock::now();
    chrono::duration<double> work_time = end_time - start_time;

    osyncstream(cout) << "\nWork time: " << fixed << setprecision(4) << work_time.count() << " seconds" << endl;
    return 0;
}