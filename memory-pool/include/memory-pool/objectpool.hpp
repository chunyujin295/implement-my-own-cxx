/*************************************************
  * 描述：简易对象池
  *		  基于简易内存池，可以封装出来一个对象池
  *
  * File：objectpool.hpp
  * Date：2026/3/13
  * Update：
  * ************************************************/
#ifndef IMPLEMENT_MY_OWN_CXX_OBJECTPOOL_HPP
#define IMPLEMENT_MY_OWN_CXX_OBJECTPOOL_HPP

#include "memorypoolconstant.hpp"

#include <utility>

template<typename T>
class ObjectPool
{
public:
	ObjectPool(size_t count) :
		m_memoryPool(sizeof(T), count)
	{
	}

	template<typename... Args>
	T* create(Args... args)
	{
		void* mem = m_memoryPool.alloc();
		return new(mem) T(std::forward<Args>(args)...);
	}

	void destroy(T* p)
	{
		p->~T();
		m_memoryPool.free(p);
	}

private:
	MemoryPoolConstant m_memoryPool;
};

#endif //IMPLEMENT_MY_OWN_CXX_OBJECTPOOL_HPP