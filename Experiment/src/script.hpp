#pragma once
#include "common.hpp"
#include "fiber_pool.hpp"
#include "thread_pool.hpp"

namespace big
{
	class script
	{
	public:
		using func_t = void(*)();
	public:
		explicit script(func_t func, std::optional<std::size_t> stack_size = std::nullopt);
		~script();

		void tick();
		void yield(std::optional<std::chrono::high_resolution_clock::duration> time = std::nullopt);
		static script* get_current();
		static void script_exception_handler(PEXCEPTION_POINTERS exp);
	private:
		void fiber_func();
	private:
		void* m_script_fiber;
		void* m_main_fiber;
		func_t m_func;
		std::optional<std::chrono::high_resolution_clock::time_point> m_wake_time;
	};


#define TRY_CLAUSE  __try
#define EXCEPT_CLAUSE  __except (script::script_exception_handler(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) { }
#define QUEUE_JOB_BEGIN_CLAUSE(...) g_fiber_pool->queue_job([__VA_ARGS__] { __try
#define QUEUE_JOB_END_CLAUSE __except (script::script_exception_handler(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {} });
#define QUEUE_JOB_BEGIN(...) g_fiber_pool->queue_job([__VA_ARGS__] {
#define QUEUE_JOB_END  });
#define THREAD_JOB_BEGIN_CLAUSE(...) g_thread_pool->push([__VA_ARGS__] {
#define THREAD_JOB_END_CLAUSE  });
#define THREAD_PUSH_BEGIN_CLAUSE(...) g_thread_pool->push([__VA_ARGS__] { __try
#define THREAD_PUSH_END_CLAUSE  __except (script::script_exception_handler(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {} });
}
