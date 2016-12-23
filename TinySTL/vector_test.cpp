#include"Vector.h"
#include"Test.h"

void TinySTL::VectorTest::test1()
{
	Vector<int> vec(10, 5);

	for (int i = 0; i < 10; ++i)
		std::cout << vec[i] << " ";
	std::cout << std::endl;
}