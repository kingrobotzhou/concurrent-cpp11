// 手动锁
#include <iostream>
#include <thread>
#include <mutex>
 
std::mutex mtx;
int k=0;
 
void foo(int n)
{
mtx.lock();
for(int i=0;i<n;i++)
{k++;}
mtx.unlock();
}
 
void foo_2(int n)
{
mtx.lock();
for(int i=0;i<n;i++)
{k++;}
mtx.unlock();
}
 
int main()
{
    std::thread Tr1(foo,100);
    std::thread Tr2(foo_2,100);
 
    Tr1.join();
    Tr2.join();
 
    std::cout<<k<<std::endl;
}