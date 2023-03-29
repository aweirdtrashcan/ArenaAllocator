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
	Allocator<Testing>alloc(40000 * sizeof(Testing));

	for (size_t i = 0; i <= 40000; i++)
	{
		alloc.CreateObject();
		
		Testing* obj = alloc.GetLastObjectAdded();
		obj->a = (float)i;
		std::cout << obj->a << std::endl;
	}

	std::cout << "size of alloc " << sizeof(alloc) << std::endl;

	alloc.DeleteAllObjectsAndBuffer();

	bool successful = alloc.CreateBufferWithSize(40000 * sizeof(Testing));

	if (!successful)
	{
		std::cout << "Error creating new buffer\n";
		return 0;
	}

	for (size_t i = 0; i <= 40000; i++)
	{
		alloc.CreateObject();

		Testing* obj = alloc.GetLastObjectAdded();
		obj->a = (float)i;
		std::cout << obj->a << std::endl;
	}

	alloc.~Allocator();

	std::cin.get();

	return 0;
}