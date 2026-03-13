/*************************************************
  * 描述：固定大小的简易线程池
  *
  * File：memorypoolconstant.hpp
  * Date：2026/3/13
  * Update：
  * ************************************************/
#ifndef IMPLEMENT_MY_OWN_CXX_MEMORYPOOLCONSTANT_HPP
#define IMPLEMENT_MY_OWN_CXX_MEMORYPOOLCONSTANT_HPP
#include <cstdlib>

class MemoryPoolConstant
{
public:
	MemoryPoolConstant(size_t blockSize, size_t blockNum)
	{
		m_pool = malloc(blockNum * blockSize);
		m_freeList = nullptr;

		char* p = (char*)m_pool;
		for (int i = 0; i < blockNum; i++)
		{
			// 逆序构成链表，从前往后连，但是链表头在末尾，next是上一个
			FreeNode* node = (FreeNode*)(p + i * blockSize);
			node->next = m_freeList;
			m_freeList = node;
		}
	}

	// TODO: 这样写，如果指针越界怎么办，例如传入了一个不属于pool管辖范围的指针
	void* alloc()
	{
		if (!m_freeList)
		{
			return nullptr;
		}
		FreeNode* node = m_freeList;
		m_freeList = m_freeList->next;
		return node;
	}

	// TODO: 这样写的话，如果传入的p节点，后面还有未被使用的空间，但是这样会不会导致freelist永远也找不到这些未被使用的空间了，应为freeList经过这些操作之后，链表成环了
	void free(void* p)
	{
		FreeNode* node = (FreeNode*)p;
		node->next = m_freeList;
		m_freeList = node;

	}

private:
	struct FreeNode
	{
		FreeNode* next;
	};

	void* m_pool;
	FreeNode* m_freeList;// 用来表示未被外界占用的内存池node
};

#endif //IMPLEMENT_MY_OWN_CXX_MEMORYPOOLCONSTANT_HPP