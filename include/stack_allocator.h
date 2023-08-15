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
    StackAllocator(const std::size_t &totalSize, const std::size_t &alignment = 8) : BaseAllocator(totalSize, alignment)
    {
        reset();
    };

    virtual void *allocate(const std::size_t &size) override
    {
        const std::size_t header_size = sizeof(Header);
        std::size_t address = (std::size_t)m_pointer + m_cursor;
        std::size_t modulo = fast_modulo(address);
        std::size_t padding = 0;

        if (modulo != 0)
            padding += m_alignment - modulo;

        if (padding < header_size)
            padding += m_alignment;

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