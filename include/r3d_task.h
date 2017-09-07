//-------------------------------------------------------------------------------------------------
// File : r3d_task.h
// Desc : Task System.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------
#pragma once

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_queue.h>
#include <thread>
#include <vector>
#include <atomic>


///////////////////////////////////////////////////////////////////////////////////////////////////
// task_system class
///////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, typename U>
class task_system
{
public:
    using task_func = std::function<void(T*, U*)>;

    task_system(uint32_t count, task_func func)
    : m_count(count)
    , m_func (func)
    , m_finish(false)
    {
        m_data.resize(count);
    }

    U& thread_data(uint32_t index)
    { return m_data[index]; }

    void enqueue(const T& data)
    { m_queue.push(data); }

    void run()
    {
        wait();

        m_finish = false;

        for(uint32_t i=0; i<m_count; ++i)
        {
            m_worker.push_back(std::thread(std::bind(&worker::run, worker(i, *this))));
        }
    }

    void wait()
    {
        for(size_t i=0; i<m_worker.size(); ++i)
        {
            if (m_worker[i].joinable())
            { m_worker[i].join(); }
        }

        m_worker.clear();
        m_worker.shrink_to_fit();
    }

    void request_exit()
    {
        m_finish = true;
    }

private:
    class worker
    {
    public:
        worker(uint32_t id, task_system& owner)
        : m_id(id)
        , m_owner(owner)
        { /* DO_NOTHING */ }

        void run()
        {
            T data;
            for(;;)
            {
                if (m_owner.m_finish)
                { return; }

                if (!m_owner.m_queue.pop(data))
                { continue; }

                m_owner.m_func(&data, &m_owner.m_data[m_id]);
            }
        }

    private:
        uint32_t        m_id;
        task_system&    m_owner; 
    };

    task_func                   m_func;
    uint32_t                    m_count;
    std::vector<std::thread>    m_worker;
    std::vector<U>              m_data;
    lockfree_queue<T>           m_queue;
    std::atomic<bool>           m_finish;
};
