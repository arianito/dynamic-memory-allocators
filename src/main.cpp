#include <cstddef>

#include "benchmark.h"
#include "arena_allocator.h"
#include "stack_allocator.h"
#include "pool_allocator.h"

int main(int argc, char const *argv[])
{
    struct Temp
    {
        short value;
    };
    std::size_t n = 1000000;
    benchmark("malloc", [n]()
              {
			for (int i = 0; i < n; i++)
			{
				Temp* a = new Temp{ (short)(i + 2) };
                delete a;
			} });
    benchmark("arena", [n]()
              {
                  BaseAllocator *alloc = new ArenaAllocator{(n + 1) * 8};
                  for (int i = 0; i < n; i++)
                  {
                      Temp *a = new (alloc->allocate(sizeof(Temp))) Temp{(short)(i + 2)};
                      alloc->deallocate(a);
                  } delete alloc; });
    benchmark("stack", [n]()
              {
			BaseAllocator* alloc = new StackAllocator{ 64 };
			for (int i = 0; i < n; i++)
			{
				Temp* a = new (alloc->allocate(sizeof(Temp))) Temp{ (short)(i + 2) };
				alloc->deallocate(a);
			} delete alloc; });
    benchmark("pool", [n]()
              {
			BaseAllocator* alloc = new PoolAllocator{ 1024, 64 };
			for (int i = 0; i < n; i++)
			{
				Temp* a = new (alloc->allocate(64)) Temp{ (short)(i + 2) };
                alloc->deallocate(a);
			} delete alloc; });

    return 0;
}
