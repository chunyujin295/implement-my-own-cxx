/*************************************************
  * 描述：
  *
  * File：threadpool.hpp
  * Author：chenyujin@3029795434@qq.com
  * Date：2026/3/11
  * Update：
  * ************************************************/
#ifndef MY_OWN_THREAD_POOL_THREADPOOL_HPP
#define MY_OWN_THREAD_POOL_THREADPOOL_HPP
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <condition_variable>

class ThreadPool
{
    public:
        ThreadPool(size_t thread_num)
        {
            m_stop = false;
            m_threads.clear();
            for (int i = 0; i < thread_num; i++)
            {
                m_threads.emplace_back([this]() {
                    while (1)
                    {
                        std::function<void()> task; {
                            std::unique_lock<std::mutex> lock(m_mutex);

                            m_cv.wait(lock, [this] {
                                return !m_tasks.empty() || m_stop;
                            });

                            if (m_stop && m_tasks.empty())
                            {
                                return;
                            }

                            task = std::move(m_tasks.front());
                            m_tasks.pop();
                        }
                        task();
                    }
                });
            }
        }

        ~ThreadPool()
        { {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_stop = true;
            }
            m_cv.notify_all();

            for (auto& thread: m_threads) // 修改了stop之后，等待线程将所有任务都完成，然后自己return
            // 此处阻塞等待线程执行完上面的步骤
            {
                thread.join();
            }
        }

        void addTask(std::function<void()> task)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_tasks.push(std::move(task));
            m_cv.notify_one();
        }

    private:
        std::vector<std::thread> m_threads; // 线程池容器
        std::queue<std::function<void()> > m_tasks; // 任务队列
        bool m_stop; // 线程池是否需要停止运行
        std::mutex m_mutex;
        std::condition_variable m_cv;
};

#endif //MY_OWN_THREAD_POOL_THREADPOOL_HPP
