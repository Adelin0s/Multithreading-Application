// =====================================================================
//   @ Author: Cucorianu Eusebiu Adelin                                                                                      
//   @ Create Time: 22-10-2022 8:03 PM                                                                                                                                                
//   @ Contact: cucorianu.adelin@protonmail.com                                                                                                                          
//   @ Modified time: 23-10-2022 5:48 PM                                                                                                                                    
//   @ Description: Worker that executes async multiple tasks                                                                                                                                                                         
// =====================================================================


#ifndef FILE_WORKER_H
#define FILE_WORKER_H

#include "Platform/platform.hpp"

#include <vector>
#include <queue>

#include <condition_variable>

#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace Eset
{
    using Task = std::function<void()>;

	class Worker
	{
	public:
		Worker() noexcept = delete;

		Worker(uint32 numberOfWorkers);

		// cant copy
		Worker(const Worker& fileSearchWorker) = delete;
		Worker& operator=(const Worker& fileSearchWorker) = delete;

		// cant move
		Worker(Worker&& fileSearchWorker) = delete;
		Worker& operator=(Worker&& fileSearchWorker) = delete;

		 ///\brief Push task in queue
		 ///\param task that is pushed in queue
		void Push(Task &&task);

		 ///\brief Tells the worker to stop the execution
		void Done();

		~Worker() noexcept = default;

	private:
		///\brief The number of threads that the worker has
		uint32 m_numberOfWorkers;

		///\brief The task queue
		std::queue<Task> m_tasks;

		///\brief The lock that synchronizes the execution of tasks 
		std::mutex m_lock;

		///\brief Notifies when a task is pushed
		std::condition_variable m_condition;

		///\brief Tells the worker when to stop the execution
		std::atomic_bool m_done{ false };

		///\brief Vector of jthreads
		std::vector<std::jthread> m_threadPool;

	private:
		///\brief The function loop for each thread
        void WorkerRoutine();
	};
} // namespace Eset
#endif