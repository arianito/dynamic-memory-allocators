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
    PoolAllocator(const std::size_t &totalSize, const std::size_t &chunkSize, const std::size_t &alignment = 8) : BaseAllocator(totalSize, alignment)
    {
        m_chunkSize = chunkSize;
        reset();
    };

    virtual void *allocate(const std::size_t &size) override
    {
        if (size != m_chunkSize)
            return nullptr;

        Node *address = pop();

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
        const std::size_t n = m_total / m_chunkSize + 1;
        std::size_t padding, address, modulo;
        std::size_t cursor = 0;

        for (std::size_t i = 0; i < n; i++)
        {
            address = (std::size_t)m_pointer + cursor;
            modulo = fast_modulo(address);
            padding = 0;

            if (modulo != 0)
                padding += m_alignment - modulo;

            if (cursor + m_chunkSize + padding > m_total)
                return;

            address += padding;
            cursor += m_chunkSize + padding;
            push(address);
        }
        m_used = 0;
    }
};