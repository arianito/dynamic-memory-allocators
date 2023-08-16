# dynamic Memory allocation
implementation of arena/stack/pool/freelist memory allocators

benchmark: number of allocations: <b>1,000,000
```yaml
malloc: 227.338ms
arena: 14.746ms
stack: 22.773ms
pool: 15.238ms
freelist: 39.125ms
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
