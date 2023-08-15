#pragma once

#include <cstddef>

template <class T>
class BasicAllocator
{

public:
    using value_type = T;

    BasicAllocator() noexcept {};
    
    template <class U>
    inline BasicAllocator(const BasicAllocator<U> &) noexcept {};

    inline T *allocate(std::size_t n) { return static_cast<T *>(operator new(n * sizeof(T))); }

    inline void deallocate(T *p, std::size_t n) { operator delete(p); }

    inline void construct(T *p, T const &val) { new ((void *)p) T(val); }

    inline void destroy(T *p) { p->~T(); }
};