#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>
using namespace std;
int n = 0;
std::mutex mu;
void increase_number(){
    for (int i = 0; i < 1000000; i++){
        mu.lock();
        n++;    
        mu.unlock();
    }
}

int main(){
    vector<thread> threads;

    //创建10个线程
    for (int i = 0; i < 10; i++){
        threads.push_back(thread(increase_number,n));
    }

    // sleep(5);
    std::cout << "n = " << n << std::endl; 
}

