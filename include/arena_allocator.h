#pragma once

#include "base_allocator.h"

class ArenaAllocator : public BaseAllocator
{
private:
    std::size_t m_cursor;

public:
    ArenaAllocator(const std::size_t &totalSize, const std::size_t &alignment = 8) : BaseAllocator(totalSize, alignment)
    {
        reset();
    };

    virtual void *allocate(const std::size_t &size) override
    {
        std::size_t address = (std::size_t)m_pointer + m_cursor;
        std::size_t padding = calculate_padding(address);

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