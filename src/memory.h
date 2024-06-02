
#define Bytes(n) (n)
#define Kilobytes(n) (1024 * Bytes(n))
#define Megabytes(n) (1024 * Kilobytes(n))
#define Gigabytes(n) (1024 * Megabytes(n))


struct MAllocator;

typedef void *AllocateFunc(MAllocator *allocator, uint64 size);
typedef void DeallocateFunc(MAllocator *allocator, void *data);

struct MAllocator
{
    AllocateFunc *allocate;
    DeallocateFunc *deallocate;
};


struct MemoryArena : MAllocator
{
    uint32 capacity;
    uint32 size; // in bytes
    void *ptr;
};


void *PushSizeMemoryArena(MemoryArena *arena, uint64 size);
