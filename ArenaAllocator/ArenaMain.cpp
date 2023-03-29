#include <iostream>
#include <sstream>
#include "Allocator.h"

struct Testing
{
	Testing(double a, double r, double g, double b) : a(a), r(r), g(g), b(b) {};
	~Testing() {};
	friend std::ostream& operator<<(std::ostream& oss, const Testing& test)
	{
		oss << "a: " << test.a << std::endl;
		oss << "r: " << test.r << std::endl;
		oss << "g: " << test.g << std::endl;
		oss << "v: " << test.b << std::endl;

		return oss;
	}
	double a;
	double r;
	double g;
	double b;
};

Testing* CreateTesting(double a, double r, double g, double b)
{
	return new Testing(a, r, g, b);
}

int main()
{
	Allocator<Testing>alloc(40000 * sizeof(Testing));

	for (size_t i = 0; i <= 40000; i++)
	{
		alloc.CreateObject<double, double, double, double>(&CreateTesting, 1.0, 45.0 * i / 1.32, 23.0 * i / 0.36, 21.0 * i / 0.223);

		Testing* obj = alloc.GetLastObjectAdded();
		std::cout << *obj << std::endl;
	}

	std::cout << "size of alloc " << sizeof(alloc) << std::endl;

	alloc.DeleteAllObjectsAndBuffer();
	alloc.DeleteAllObjectsAndBuffer();

	//bool successful = alloc.CreateBufferWithSize(40000 * sizeof(Testing));

	//if (!successful)
	//{
	//	std::cout << "Error creating new buffer\n";
	//	return 0;
	//}

	//for (size_t i = 0; i <= 40000; i++)
	//{
	//	//alloc.CreateObject();

	//	Testing* obj = alloc.GetLastObjectAdded();
	//	obj->a = (float)i;
	//	std::cout << obj->a << std::endl;
	//}

	//alloc.~Allocator();


	std::cin.get();

	return 0;
}