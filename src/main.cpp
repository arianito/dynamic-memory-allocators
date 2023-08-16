#include <cstddef>

#include "benchmark.h"
#include "arena_allocator.h"
#include "stack_allocator.h"
#include "pool_allocator.h"
#include "freelist_allocator.h"

constexpr std::size_t n = 1000000;

struct Temp
{
    std::size_t value;
};

void test_malloc()
{
    for (std::size_t i = 0; i < n; i++)
    {
        Temp *a = new Temp{i + 2};
        delete a;
    }
}

void test_arena()
{
    BaseAllocator *alloc = new ArenaAllocator{n * 8};
    for (std::size_t i = 0; i < n; i++)
    {
        Temp *a = new (alloc->allocate(sizeof(Temp))) Temp{i + 2};
        alloc->deallocate(a);
    }
    delete alloc;
}

void test_stack()
{
    BaseAllocator *alloc = new StackAllocator{64};
    for (std::size_t i = 0; i < n; i++)
    {
        Temp *a = new (alloc->allocate(sizeof(Temp))) Temp{i + 2};
        alloc->deallocate(a);
    }
    delete alloc;
}

void test_pool()
{
    BaseAllocator *alloc = new PoolAllocator{128, 8};
    for (std::size_t i = 0; i < n; i++)
    {
        Temp *a = new (alloc->allocate(8)) Temp{(i + 2)};
        alloc->deallocate(a);
    }
    delete alloc;
}

void test_freelist()
{
    BaseAllocator *alloc = new FreelistAllocator{1024};

    for (std::size_t i = 0; i < n; i++)
    {
        Temp *a = new (alloc->allocate(sizeof(Temp))) Temp{(i + 2)};
        alloc->deallocate(a);
    }

    delete alloc;
}

int main(int argc, char const *argv[])
{
    benchmark("malloc", test_malloc);
    benchmark("arena", test_arena);
    benchmark("stack", test_stack);
    benchmark("pool", test_pool);
    benchmark("freelist", test_freelist);
    return 0;
}
