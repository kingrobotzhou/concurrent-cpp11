
// 原子
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <unistd.h> 
#include <vector>

std::atomic<int> k={0};
/*
这种写法会报错，error: use of deleted function \
‘std::atomic<bool>::atomic(const std::atomic<bool>&)’
*/
//std::atomic<int> k=0;

void foo(int n)
{
for(int i=0;i<n;i++)
{k++;}
}
 
void foo_2(int n)
{
for(int i=0;i<n;i++)
{k++;}
}
 
int main()
{
    std::thread Tr1(foo,100);
    std::thread Tr2(foo_2,100);
 
    Tr1.join();
    Tr2.join();
 
    std::cout<<k<<std::endl;
}
