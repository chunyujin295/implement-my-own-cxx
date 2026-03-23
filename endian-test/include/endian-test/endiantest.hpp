/*************************************************
* 描述：计算机大小端序测试
  *
  * File：endiantest.hpp
  * Date：2026/3/23
  * Update：
  * ************************************************/

#ifndef IMPLEMENT_MY_OWN_CXX_ENDIANTEST_HPP
#define IMPLEMENT_MY_OWN_CXX_ENDIANTEST_HPP
#include <iostream>

class EndianTest
{
public:
	static void isLittleEndian()
	{
		int p = 1;
		char* pChar = reinterpret_cast<char*>(&p);

		if (*pChar = 1)
		{
			std::cout << "Little Endian" << std::endl;
		}
		else
		{
			std::cout << "Big Endian" << std::endl;
		}
	}
};


#endif //IMPLEMENT_MY_OWN_CXX_ENDIANTEST_HPP