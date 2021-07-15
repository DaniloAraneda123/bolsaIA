#ifndef _Threads_W_
#define _Threads_W_

#include<thread>
#include<mutex>
#include <vector>
#include <queue>
#include <functional>
#include <atomic>
#include "Regla.h"

using namespace std;

class ThreadPool
{
private:
	std::mutex 	m_queue_mutex;
	std::mutex 	m_results_mutex;
	std::vector<std::thread> m_threads;
	unsigned int m_thread_count;
public:
	ThreadPool(unsigned int pool_size = thread::hardware_concurrency()) :
		m_queue_mutex(),
		m_thread_count(pool_size),
		m_threads()
	{};

	template <class F, class R>
	std::vector<R> map(const F& f, vector<vector<Regla>> args, vector<vector<double>>* datos, int nroAtributos, vector<double>* resultados)
	{
		queue <function<R()>> tasks;
		vector<R> results;
		int orden = 0;
		double* casilla;
		for (auto& arg : args)
		{
			casilla = &((*resultados)[orden]);
			auto task = [&f, &arg, datos, nroAtributos, casilla]()
			{
				return f(arg, datos, nroAtributos, casilla);
			};
			tasks.push(move(task));
			orden++;
		}
		for (unsigned int i = 0; i < m_thread_count; ++i)
		{
			std::thread t
			(
				[this, &tasks, &results]()
				{
					while (true)
					{
						if (m_queue_mutex.try_lock())
						{
							if (!tasks.empty()) {
								auto task = tasks.front();
								tasks.pop();
								m_queue_mutex.unlock();
								auto res = task();
								lock_guard<mutex> results_lock(m_results_mutex);
								results.push_back(res);
							}
							else
							{
								m_queue_mutex.unlock();
								break;
							}
						}
					}
				}
			);
			m_threads.push_back(move(t));
		}
		for (auto& t : m_threads) t.join();
		return results;
	}
};
#endif //worker