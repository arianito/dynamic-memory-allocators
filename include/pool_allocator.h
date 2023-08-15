#pragma once

#include "base_allocator.h"

class PoolAllocator : public BaseAllocator
{
private:
    struct Node
    {
        Node *m_next;
        std::size_t m_address;
    };

    void push(std::size_t size)
    {
        Node newNode{m_head, size};
        m_head = &newNode;
    }

    Node *pop()
    {
        if (m_head == nullptr)
            return nullptr;
        Node *head = m_head;
        m_head = m_head->m_next;
        return head;
    }

private:
    std::size_t m_chunkSize;
    Node *m_head{nullptr};

public:
    PoolAllocator(const std::size_t &totalSize, const std::size_t &chunkSize) : BaseAllocator(totalSize)
    {
        m_chunkSize = chunkSize;
        reset();
    };

    virtual void *allocate(const std::size_t &size, const std::size_t &alignment = 8) override
    {
        if (size != m_chunkSize)
            return nullptr;

        Node* address = pop();

        if (address == nullptr)
            return nullptr;

        m_used += m_chunkSize;

        return (void *)address->m_address;
    }

    virtual void deallocate(void *p) override
    {
        m_used -= m_chunkSize;
        push((std::size_t)p);
    }

    virtual void reset() override
    {
        m_used = 0;
        const std::size_t n = m_total / m_chunkSize;
        for (std::size_t i = 0; i < n; i++)
            push((std::size_t)m_pointer + (i * m_chunkSize));
    }
};