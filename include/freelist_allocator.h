#pragma once

#include "base_allocator.h"
#include <iostream>

class FreelistAllocator : public BaseAllocator
{
private:
    struct Node
    {
        Node *m_next;
        std::size_t m_size;
        char m_padding;
    };

    void insert(Node *node, Node *newNode)
    {
        if (node == nullptr)
        {
            newNode->m_next = m_head;
            m_head = newNode;
            return;
        }

        newNode->m_next = node->m_next;
        node->m_next = newNode;
    }

    void remove(Node *prevNode, Node *node)
    {
        if (prevNode == nullptr)
        {
            m_head = m_head->m_next;
            return;
        }

        prevNode->m_next = node->m_next;
    }

    void find_first(std::size_t size, Node *&out_prev_node, Node *&out_node)
    {
        const std::size_t header_size = sizeof(Node);
        std::size_t needed_space;
        Node *node = m_head,
             *prev = nullptr;

        while (node != nullptr)
        {
            needed_space = size + node->m_padding;
            if (node->m_size >= needed_space)
                break;
            prev = node;
            node = node->m_next;
        }

        out_node = node;
        out_prev_node = prev;
    }

    void find_best(std::size_t size, Node *&out_prev_node, Node *&out_node)
    {
        const std::size_t header_size = sizeof(Node);
        std::size_t needed_space, diff, min = ~((std::size_t)0);

        Node *node = m_head,
             *prev = nullptr,
             *best = nullptr;

        while (node != nullptr)
        {
            needed_space = size + node->m_padding;
            diff = node->m_size - needed_space;
            if (diff >= 0 && diff < min)
            {
                best = node;
                min = diff;
            }
            prev = node;
            node = node->m_next;
        }

        out_node = best;
        out_prev_node = prev;
    }

private:
    Node *m_head{nullptr};

public:
    FreelistAllocator(const std::size_t &total_size, const std::size_t &alignment = 8) : BaseAllocator(total_size, alignment)
    {
        reset();
    };

    virtual void *allocate(const std::size_t &size) override
    {
        Node *node;
        Node *prevNode;
        find_first(size, prevNode, node);
        assert(node != nullptr && "not enough memory");

        const std::size_t header_size = sizeof(Node);
        std::size_t address = (std::size_t)node + header_size;

        std::size_t required_space = size + node->m_padding;
        std::size_t remaining_space = node->m_size - required_space;
        if (remaining_space > 0)
        {
            std::size_t new_address = address + required_space;
            std::size_t padding = calculate_padding(new_address, header_size);
            std::size_t alignment_padding = padding - header_size;
            Node *newNode = (Node *)(new_address + alignment_padding);
            newNode->m_size = remaining_space;
            newNode->m_padding = (char)padding;
            insert(node, newNode);
        }
        remove(prevNode, node);

        m_used += required_space;
        node->m_size = required_space;

        return (void *)(address);
    }

    virtual void deallocate(void *p) override
    {
        const std::size_t header_size = sizeof(Node);
        std::size_t address = (std::size_t)p - header_size;

        Node *freeedNode = (Node *)address;
        freeedNode->m_next = nullptr;

        Node *node = m_head,
             *prevNode = nullptr;

        while (node != nullptr)
        {
            if (p < node)
                break;
            prevNode = node;
            node = node->m_next;
        }

        insert(prevNode, freeedNode);
        m_used -= freeedNode->m_size;

        join_next(freeedNode);
        join_next(prevNode);
    }

    void join_next(Node *node)
    {
        if (node == nullptr)
            return;
            
        const std::size_t header_size = sizeof(Node);

        Node *next = node->m_next;
        if (next == nullptr)
            return;

        std::size_t start = (std::size_t)node + header_size - node->m_padding;
        std::size_t end = (std::size_t)next - next->m_padding;

        if (start + node->m_size != end)
            return;

        node->m_size += next->m_size;
        remove(node, next);
    }

    virtual void reset() override
    {
        const std::size_t header_size = sizeof(Node);
        std::size_t address = (std::size_t)m_pointer;
        std::size_t padding = calculate_padding(address, header_size);
        std::size_t alignment_padding = padding - header_size;

        m_used = 0;
        m_head = (Node *)(address + alignment_padding);
        m_head->m_size = m_total;
        m_head->m_padding = (char)padding;
        m_head->m_next = nullptr;
    }
};