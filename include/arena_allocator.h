#pragma once

#include "base_allocator.h"

class ArenaAllocator : public BaseAllocator
{
private:
    std::size_t m_cursor;

public:
    ArenaAllocator(const std::size_t &totalSize) : BaseAllocator(totalSize)
    {
        reset();
    };

    virtual void *allocate(const std::size_t &size, const std::size_t &alignment = 8) override
    {
        std::size_t address = (std::size_t)m_pointer + m_cursor;
        std::size_t padding = 0;

        if (alignment != 0 && m_cursor % alignment != 0)
            padding = ((address / alignment) + 1) * alignment - address;

        if (m_cursor + size + padding > m_total)
            return nullptr;

        address += padding;
        m_cursor += size + padding;
        m_used = m_cursor;
        return (void *)(address);
    }

    virtual void deallocate(void *p) override
    {
        // nothing!
    }

    virtual void reset() override
    {
        m_cursor = 0;
        m_used = 0;
    }
};