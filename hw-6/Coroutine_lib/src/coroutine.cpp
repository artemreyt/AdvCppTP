#include "Coroutine.hpp"
#include <queue>
#include <memory>
#include <ucontext.h>
#include <map>


//#include <iostream>
#include <stdexcept>
//#include <thread>

namespace Coroutine
{

struct Routine;

namespace
{

thread_local struct Ordinator
{
	static constexpr size_t STACK_SIZE = 1 << 18;

	std::map<routine_t, Routine> routines;
	std::queue<decltype(routines)::node_type> finished;
	routine_t current = 0;
	ucontext_t ctx{};
} ordinator;

void entry();

}

struct Routine
{
	RoutineFunction func;
	std::unique_ptr<uint8_t[]> stack;
	bool finished = false;
	ucontext_t ctx;
	std::exception_ptr exception = {};

	void reset(const RoutineFunction& f)
	{
		func = f;
		finished = false;
		exception = {};
		makecontext(&ctx, entry, 0);
	}

	Routine(const RoutineFunction& f)
			: func(f),
			  stack(std::make_unique<uint8_t[]>(Ordinator::STACK_SIZE))
	{
        getcontext(&ctx);
		ctx.uc_stack.ss_sp = stack.get();
		ctx.uc_stack.ss_size = Ordinator::STACK_SIZE;
		ctx.uc_link = &ordinator.ctx;
		makecontext(&ctx, entry, 0);
	}

	Routine(const Routine&) = delete;
	Routine(Routine&&) = default;
};

routine_t create(routine_t id, const RoutineFunction& function)
{
	auto& o = ordinator;
	if (o.finished.empty())
	{
		o.routines.emplace(id, function);
		return id;
	}
	else
	{
		auto node(std::move(o.finished.front()));
		o.finished.pop();
		node.key() = id;
		node.mapped().reset(function);
		o.routines.insert(std::move(node));
		return id;
	}
}

bool resume(routine_t id) {
    auto &o = ordinator;
    if (id == 0)
        return false;

    const auto &routine = o.routines.at(id);
    if (routine.finished)
        return false;

    o.current = id;
    if (swapcontext(&o.ctx, &routine.ctx) < 0) {
        o.current = 0;
        return false;
    }

    if (routine.finished) {
        auto exception = routine.exception;
        o.finished.emplace(o.routines.extract(id));

        if (exception) {
            std::rethrow_exception(exception);
        }

    }

	return true;
}

void yield()
{
	auto& o = ordinator;
	routine_t id = o.current;
	auto& routine = o.routines.at(id);

	o.current = 0;

    if (routine.exception)
        std::rethrow_exception(routine.exception);

	swapcontext(&routine.ctx, &o.ctx);

    if (routine.exception)
        std::rethrow_exception(routine.exception);
}

routine_t current()
{
	return ordinator.current;
}

void kill(routine_t id, const std::exception_ptr &ex) {
    if (id == 0) {
        throw std::invalid_argument("You can't kill main routine");
    } else if (!ex) {
        throw std::invalid_argument("Empty exception_ptr");
    }

    auto &o = ordinator;
    auto &routine = o.routines.at(id);

    routine.exception = ex;
    resume(id); // must throw ex
    throw std::logic_error("Resume had to throw exception, but didn't");
}


namespace
{

void entry()
{
	auto& o = ordinator;
	routine_t id = o.current;
	auto &routine = o.routines.at(id);

	if (routine.func) try
	{
		routine.func();
	}
	catch (...)
	{
		routine.exception = std::current_exception();
	}

    routine.finished = true;
    o.current = 0;
}

}

}
