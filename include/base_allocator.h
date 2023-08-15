#pragma once

#include <cstddef>
#include <malloc.h>

class BaseAllocator
{
protected:
    void *m_pointer = nullptr;
    std::size_t m_total;
    std::size_t m_used;

public:
    BaseAllocator(const BaseAllocator &ref) = delete;
    BaseAllocator(const std::size_t &totalSize)
    {
        m_total = totalSize;
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

    virtual void *allocate(const std::size_t &size, const std::size_t &alignment = 8) = 0;
    virtual void deallocate(void *p) = 0;
    virtual void reset() = 0;
};