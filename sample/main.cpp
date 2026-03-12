#include <iostream>
#include <thread-pool/threadpool.hpp>

void thread_test_first()
{
    for (int i = 0; i < 10; i++)
        std::cout << "hello 1" << std::endl;
}

void thread_test_second()
{
    for (int i = 0; i < 10; i++)
        std::cout << "hello 2" << std::endl;
}

void thread_test_third()
{
    for (int i = 0; i < 10; i++)
        std::cout << "hello 3" << std::endl;
}


int main()
{
    ThreadPool pool(2);
    pool.addTask(thread_test_first);
    pool.addTask(thread_test_second);
    pool.addTask(thread_test_third);
    return 0;
}
