#include "memory-pool/objectpool.hpp"

#include <iostream>
#include <memory-pool/memorypoolconstant.hpp>
#include <logger/logger.h>
#include <thread-pool/threadpool.hpp>

void thread_test_first()
{
	for (int i = 0; i < 10; i++) std::cout << "hello 1" << std::endl;
}

void thread_test_second()
{
	for (int i = 0; i < 10; i++) std::cout << "hello 2" << std::endl;
}

void thread_test_third()
{
	for (int i = 0; i < 10; i++) std::cout << "hello 3" << std::endl;
}


int main()
{
	LOG_DEBUG("==================== 线程池 ===================");
	ThreadPool pool(2);
	pool.addTask(thread_test_first);
	pool.addTask(thread_test_second);
	pool.addTask(thread_test_third);

	LOG_DEBUG("==================== 内存池 - 固定大小 ===================");
	MemoryPoolConstant memoryPool(16, 10);
	auto p = memoryPool.alloc();
	int* p_int = (int*)p;
	*p_int = 1;
	LOG_DEBUG("p_int: ", *(int*)p);

	struct ObjectTest
	{
		int a = 1;
		int b = 2;
	};

	LOG_DEBUG("==================== 对象池 - 不自动扩容·、、 ===================");
	ObjectPool<ObjectTest> objectPool(10);
	auto p_object = objectPool.create();
	LOG_DEBUG("p_object: ", p_object->a, " ", p_object->b);

	return 0;
}