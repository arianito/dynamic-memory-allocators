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

    std::size_t fast_modulo(std::size_t p)
    {
        return p & (m_alignment - 1);
    }

public:
    BaseAllocator(const BaseAllocator &ref) = delete;
    BaseAllocator(const std::size_t &totalSize, const std::size_t &alignment = 8)
    {
        assert((alignment & (alignment - 1)) == 0);
        m_total = totalSize;
        m_alignment = alignment;
        m_used = 0;
        m_pointer = malloc(totalSize);
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