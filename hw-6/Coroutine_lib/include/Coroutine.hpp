#ifndef COROUTINE_H
#define COROUTINE_H

#include <functional>
#include <memory>

namespace Coroutine
{

using routine_t = uint64_t ;
using stack_t = std::unique_ptr<uint8_t[]>;
using RoutineFunction = std::function<void()>;

routine_t create(routine_t id, const RoutineFunction& function);
bool resume(routine_t id);
bool finish(routine_t id);
void yield();
routine_t current();

template <class F, class ...Args, class = std::enable_if_t<!std::is_invocable_v<F>>>
routine_t create(routine_t id, F&& f, Args&&... args)
{
	return create(id, std::bind(std::forward<F>(f), std::forward<Args>(args)...));
}

template <class F, class ...Args>
bool create_and_run(routine_t id, F&& f, Args&&... args)
{
	return resume(create(id, std::forward<F>(f), std::forward<Args>(args)...));
}

}

#endif //COROUTINE_H
