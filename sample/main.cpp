#include <cmath>
#include <cstring>
#include <iostream>
#include <windows.h> // 引入 Windows 头文件以使用 Sleep 函数
#include <logger/logger.h>

#include <memory-pool/objectpool.hpp>
#include <memory-pool/memorypoolconstant.hpp>
#include <thread-pool/threadpool.hpp>
#include <endian-test/endiantest.hpp>

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


// int main()
// {
// 	LOG_DEBUG("==================== 线程池 ===================");
// 	ThreadPool pool(2);
// 	pool.addTask(thread_test_first);
// 	pool.addTask(thread_test_second);
// 	pool.addTask(thread_test_third);
//
// 	LOG_DEBUG("==================== 内存池 - 固定大小 ===================");
// 	MemoryPoolConstant memoryPool(16, 10);
// 	auto p = memoryPool.alloc();
// 	int* p_int = (int*)p;
// 	*p_int = 1;
// 	LOG_DEBUG("p_int: ", *(int*)p);
//
// 	struct ObjectTest
// 	{
// 		int a = 1;
// 		int b = 2;
// 	};
//
// 	LOG_DEBUG("==================== 对象池 - 不自动扩容 ===================");
// 	ObjectPool<ObjectTest> objectPool(10);
// 	auto p_object = objectPool.create();
// 	LOG_DEBUG("p_object: ", p_object->a, " ", p_object->b);
//
//
// 	LOG_DEBUG("==================== 杂七杂八的测试 ===================");
// 	LOG_DEBUG(sizeof(int));
//
// 	EndianTest::isLittleEndian();
//
//
// 	return 0;
// }

int main() {
	float A = 0, B = 0;
	float i, j;
	int k;
	float z[1760];
	char b[1760];
	printf("\x1b[2J");
	for(;;) {
		memset(b,32,1760);
		memset(z,0,7040);
		for(j=0; j < 6.28; j += 0.07) {
			for(i=0; i < 6.28; i += 0.02) {
				float c = sin(i);
				float d = cos(j);
				float e = sin(A);
				float f = sin(j);
				float g = cos(A);
				float h = d + 2;
				float D = 1 / (c * h * e + f * g + 5);
				float l = cos(i);
				float m = cos(B);
				float n = sin(B);
				float t = c * h * g - f * e;
				int x = 40 + 30 * D * (l * h * m - t * n);
				int y= 12 + 15 * D * (l * h * n + t * m);
				int o = x + 80 * y;
				int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
				if(22 > y && y > 0 && x > 0 && 80 > x && D > z[o]) {
					z[o] = D;
					b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
				}
			}
		}
		printf("\x1b[H");
		for(k = 0; k < 1761; k++) {
			putchar(k % 80 ? b[k] : 10);
			A += 0.00004;
			B += 0.00002;
		}
		Sleep(10);
	}
	return 0;
}