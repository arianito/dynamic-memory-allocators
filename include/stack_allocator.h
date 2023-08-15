#pragma once

#include "base_allocator.h"

class StackAllocator : public BaseAllocator
{
private:
    std::size_t m_cursor;
    struct Header
    {
        char padding;
    };

public:
    StackAllocator(const std::size_t &totalSize) : BaseAllocator(totalSize)
    {
        reset();
    };

    virtual void *allocate(const std::size_t &size, const std::size_t &alignment = 8) override
    {
        std::size_t address = (std::size_t)m_pointer + m_cursor;
        std::size_t padding = 0;
        const std::size_t header_size = sizeof(Header);
        if (alignment != 0)
        {
            padding = (((address / alignment) + 1) * alignment) - address;
            if (padding < header_size)
                padding += ((header_size / alignment) + (header_size % alignment > 0 ? 1 : 0)) * alignment;
        }
        if (m_cursor + size + padding > m_total)
            return nullptr;

        address += padding;
        m_cursor += size + padding;
        m_used = m_cursor;

        Header *header = new ((void *)(address - header_size)) Header();
        header->padding = (char)padding;

        return (void *)(address);
    }

    virtual void deallocate(void *p) override
    {
        const std::size_t header_size = sizeof(Header);
        const Header *header = (Header *)((std::size_t)p - header_size);
        m_cursor = (std::size_t)p - ((std::size_t)m_pointer + header->padding);
        m_used = m_cursor;
    }

    virtual void reset() override
    {
        m_cursor = 0;
        m_used = 0;
    }
};