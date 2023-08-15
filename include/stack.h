#pragma once

#include <cstddef>

class Stack
{
private:
    struct Node
    {
        Node *m_prev;
        Node *m_next;
        std::size_t m_size;

        Node(Node *prev, Node *next, std::size_t size) : m_prev(prev), m_next(next), m_size(size) {}
    };

private:
    Node *m_head{nullptr};
    Node *m_tail{nullptr};

public:
    void push(std::size_t size)
    {
        Node *newNode = new Node(nullptr, m_head, size);
        if (m_head == nullptr)
        {
            m_tail = m_head = newNode;
            return;
        }
        m_head->m_prev = newNode;
        m_head = newNode;
    }

    std::size_t pop()
    {
        if (m_head == nullptr)
            return 0;
        std::size_t v = m_head->m_size;
        m_head = m_head->m_next;
        if (m_head != nullptr)
            m_head->m_prev = nullptr;
        return v;
    }
    bool empty()
    {
        return m_head == nullptr;
    }

    void push_back(std::size_t size)
    {
        Node *newNode = new Node(m_tail, nullptr, size);
        if (m_head == nullptr)
        {
            m_head = m_tail = newNode;
            return;
        }
        m_tail->m_next = newNode;
        m_tail = newNode;
    }
    std::size_t pop_back()
    {
        if (m_tail == nullptr)
            return 0;

        std::size_t v = m_tail->m_size;
        m_tail = m_tail->m_prev;
        if (m_tail != nullptr)
            m_tail->m_next = nullptr;
        return v;
    }
};