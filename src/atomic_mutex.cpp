#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>

const int threads = 4;
const int iterations_per_thread = 10000000;
std::mutex mtx;
int mutex_counter = 0;
std::atomic<int> atomic_counter;

void at_incr() {
    for (int i = 0;i < iterations_per_thread;i++) {
        atomic_counter++;
    }
}

void mut_incr() {
    for (int i = 0;i < iterations_per_thread;i++) {
        mtx.lock();
        mutex_counter++;
        mtx.unlock();
    }
}

int main() {
    auto starting_mut = std::chrono::high_resolution_clock::now();
    std::vector <std::thread> mut_threads;
    for (int i = 0;i < threads;i++) {
        std::thread thing(mut_incr);
        mut_threads.push_back(std::move(thing));
    }
    for (auto& thread : mut_threads) {
        thread.join();
    }
    auto ending_mut = std::chrono::high_resolution_clock::now();
    std::chrono::duration <double> timing_mut = ending_mut - starting_mut;
    std::cout << "mutex time:" << timing_mut.count() << "\n";
    std::cout << "mutex counter:" << mutex_counter << "\n";

    auto starting_at = std::chrono::high_resolution_clock::now();
    std::vector <std::thread> at_threads;
    for (int i = 0;i < threads;i++) {
        std::thread some_thing(at_incr);
        at_threads.push_back(std::move(some_thing));
    }
    for (auto& thread : at_threads) {
        thread.join();
    }
    auto ending_at = std::chrono::high_resolution_clock::now();
    std::chrono::duration <double> timing_at = ending_at - starting_at;
    std::cout << "atomic time:" << timing_at.count() << "\n";
    std::cout << "atomic counter:" << atomic_counter << "\n";

}