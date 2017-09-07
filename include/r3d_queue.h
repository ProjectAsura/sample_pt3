//-------------------------------------------------------------------------------------------------
// File : r3d_queue.h
// Desc : Lock-Free Queue.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------
#pragma once

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_cas.h>
#include <new>


///////////////////////////////////////////////////////////////////////////////////////////////////
// lockfree_queue class
///////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class lockfree_queue
{
public:
    struct Node
    {
        T       value;
        Node*   next  = nullptr;
    };

    lockfree_queue()
    {
        m_head = new (std::nothrow) Node();
        m_tail = m_head;
        m_head->next  = nullptr;
    }

    ~lockfree_queue()
    { /* DO_NOTHING */ }

    bool push(const T& value)
    {
        auto node = new (std::nothrow) Node();
        if (node == nullptr)
        { return false; }

        node->value = value;
        node->next  = nullptr;

        Node* n;

        for(;;)
        {
            n = m_tail;
            if (cas(
                reinterpret_cast<void* volatile*>(&n->next),
                nullptr,
                reinterpret_cast<void*>(node)))
            { break; }
            else
            { cas(
                reinterpret_cast<void* volatile*>(&m_tail),
                reinterpret_cast<void*>(n),
                reinterpret_cast<void*>(n->next)); }
        }

        cas(
            reinterpret_cast<void* volatile*>(&m_tail),
            reinterpret_cast<void*>(n),
            reinterpret_cast<void*>(node));
        return true;
    }

    bool pop(T& value)
    {
        Node* n;
        for(;;)
        {
            n = m_head;
            if (n->next == nullptr)
            { return false; }

            if (cas(
                reinterpret_cast<void* volatile*>(&m_head),
                reinterpret_cast<void*>(n),
                reinterpret_cast<void*>(n->next)))
            { break; }
        }

        value = n->next->value;
        delete n;
        return true;
    }

private:
    Node* m_head = nullptr;
    Node* m_tail = nullptr;
};


