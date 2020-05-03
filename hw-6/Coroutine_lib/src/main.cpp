#include <iostream>
#include "Coroutine.hpp"

void routine1(size_t init, size_t max)
{
	auto id = Coroutine::current();
	for (size_t i = init; i <= max; i += 2)
	{
		std::cout << id << ": " << i << std::endl;
		Coroutine::yield();
	}
}

void routine2(size_t init, size_t max)
{
	auto id = Coroutine::current();
	for (size_t i = init; i < max; ++i)
	{
		std::cout << id << ": " << i << std::endl;
		Coroutine::yield();
	}
}

void routine3()
{
	auto id = Coroutine::current();
	throw std::runtime_error{"runtime_error from coroutine " + std::to_string(id)};
}

int main()
{
	auto c_id = Coroutine::current();

	std::cout << "routine1" << std::endl;

	auto id = Coroutine::create(1, routine1, 1, 10);

	for (size_t i = 0; i <= 10; i += 2)
	{
		std::cout << c_id << ": " << i << std::endl;
		Coroutine::resume(id);
	}

	std::cout << "routine2" << std::endl;

	id = Coroutine::create(2, routine2, 1, 6);

	for (size_t i = 0; i <= 10; i += 2)
	{
		std::cout << c_id << ": " << i << std::endl;
		Coroutine::resume(id);
	}

	std::cout << "routine3" << std::endl;

	try
	{
		Coroutine::create_and_run(3, routine3);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
