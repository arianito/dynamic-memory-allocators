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

    void push(std::size_t address)
    {
        const std::size_t header_size = sizeof(Node);
        Node *newNode = (Node *)(address - header_size);
        newNode->m_next = m_head;
        newNode->m_address = address;
        m_head = newNode;
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
    std::size_t m_chunk_size;
    Node *m_head{nullptr};

public:
    PoolAllocator(const std::size_t &total_size, const std::size_t &chunk_size, const std::size_t &alignment = 8) : BaseAllocator(total_size, alignment)
    {
        m_chunk_size = chunk_size;
        reset();
    };

    virtual void *allocate(const std::size_t &size) override
    {
        assert(size == m_chunk_size);

        Node *address = pop();

        if (address == nullptr)
            return nullptr;

        m_used += m_chunk_size;

        return (void *)address->m_address;
    }

    virtual void deallocate(void *p) override
    {
        m_used -= m_chunk_size;
        push((std::size_t)p);
    }

    virtual void reset() override
    {
        const std::size_t header_size = sizeof(Node);
        const std::size_t n = m_total / m_chunk_size;
        std::size_t padding, address, cursor = 0;

        m_used = 0;
        for (std::size_t i = 0; i < n; i++)
        {
            address = (std::size_t)m_pointer + cursor;
            padding = calculate_padding(address, header_size);

            if (cursor + m_chunk_size + padding > m_total)
                break;

            address += padding;
            m_used += padding;
            cursor += m_chunk_size + padding;
            push(address);
        }
    }
};