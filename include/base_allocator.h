#pragma once

#include <cstddef>
#include <cassert>
#include <malloc.h>

class BaseAllocator
{
protected:
    void *m_pointer = nullptr;
    std::size_t m_total;
    std::size_t m_alignment;
    std::size_t m_used;

    std::size_t calculate_padding(std::size_t address, std::size_t needed_space = 0)
    {
        std::size_t modulo = address & (m_alignment - 1);
        std::size_t padding = 0;

        if (modulo != 0)
            padding = m_alignment - modulo;

        if (padding >= needed_space)
            return padding;

        int n = (needed_space & (m_alignment - 1)) > 0 ? 1 : 0;
        return padding + m_alignment * ((needed_space / m_alignment) + n);
    }

public:
    BaseAllocator(const BaseAllocator &ref) = delete;
    BaseAllocator(const std::size_t &total_size, const std::size_t &alignment = 8)
    {
        assert((alignment & (alignment - 1)) == 0);
        m_total = total_size;
        m_alignment = alignment;
        m_used = 0;
        m_pointer = malloc(total_size);
    };
    virtual ~BaseAllocator()
    {
        m_total = 0;
        m_used = 0;
        free(m_pointer);
        m_pointer = nullptr;
    };

    std::size_t usage() { return m_used; }
    std::size_t total() { return m_total; }

    virtual void *allocate(const std::size_t &size) = 0;
    virtual void deallocate(void *p) = 0;
    virtual void reset() = 0;
};