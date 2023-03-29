#include <iostream>
#include "Allocator.h"

struct Testing
{
	Testing(float a, float r, float g, float b) : a(a), r(r), g(g), b(b) {};
	~Testing() {};
	float a;
	float r;
	float g;
	float b;
};

int main()
{
	Allocator<Testing>alloc(120000 * sizeof(Testing));

	for (size_t i = 0; i <= 120000; i++)
	{
		alloc.CreateObject();
		
		Testing* obj = alloc.GetLastObjectAdded();
		obj->a = (float)i;
		std::cout << obj->a << std::endl;
	}

	std::cout << "size of alloc " << sizeof(alloc) << std::endl;

	alloc.DeleteAllObjects();

	std::cin.get();

	return 0;
}