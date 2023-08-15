# dynamic Memory allocation
implementation of arena/stack/pool memory allocators

benchmark: number of allocations: <b>1,000,000
```yaml
malloc: 224.225ms
arena: 15.162ms
stack: 17.29ms
pool: 23.375ms
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
