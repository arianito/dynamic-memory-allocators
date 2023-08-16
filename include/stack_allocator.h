#pragma once

#include "base_allocator.h"

class StackAllocator : public BaseAllocator
{
private:
    std::size_t m_cursor;
    struct Header
    {
        std::size_t padding;
    };

public:
    StackAllocator(const std::size_t &total_size, const std::size_t &alignment = 8) : BaseAllocator(total_size, alignment)
    {
        reset();
    };

    virtual void *allocate(const std::size_t &size) override
    {
        const std::size_t header_size = sizeof(Header);
        std::size_t address = (std::size_t)m_pointer + m_cursor;
        std::size_t padding = calculate_padding(address, header_size);

        if (m_cursor + size + padding > m_total)
            return nullptr;

        address += padding;
        m_cursor += size + padding;
        m_used = m_cursor;

        Header *header = (Header *)(address - header_size);
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