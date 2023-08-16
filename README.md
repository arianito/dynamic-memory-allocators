# dynamic Memory allocation
implementation of arena/stack/pool/freelist memory allocators

benchmark: number of allocations: <b>1,000,000
```yaml
malloc: 226.262ms
arena: 14.723ms
stack: 22.965ms
pool: 15.224ms
freelist: 57.83ms
```

### usage:
```c++
struct Temp
{
    short value;
};


BaseAllocator* alloc = new PoolAllocator{ 1024, 64 };

std::size_t address = alloc->allocate(64);
Temp* tempVar = new (address) Temp{ 86 };
alloc->deallocate(tempVar);


delete alloc;
```


### build:
```sh
mkdir build/ && cd build/
cmake ..
make
```
