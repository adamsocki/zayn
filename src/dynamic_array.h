

struct ArrayChunk
{
    ArrayChunk *nextChunk;
    uint8 data[0];
};


struct DynamicArray_Untyped
{
    MAllocator *allocator;

    uint32 count;
    uint32 elementsPerChunk;

    uint32 chunkCount;

    ArrayChunk *headChunk;
    ArrayChunk *tailChunk;
};




template <typename T>
struct DynamicArray
{
    MAllocator *allocator;

    uint32 elementsPerChunk;

    uint32 count;
    uint32 chunkCount;

    ArrayChunk *headChunk;
    ArrayChunk *tailChunk;
    inline T& operator[](const int index) const
    {
           // (index / elementsPerChunk) to get the chunk
           // then index % chunkCount to get the index

           s32 chunkIndex = index / elementsPerChunk;
           ArrayChunk *chunk = headChunk;
           for (int32 i = 0; i < chunkIndex; i++)
           {
               chunk = chunk->nextChunk;
           }

           return *(T *)(chunk->data + (sizeof(T) * (index % elementsPerChunk)));
    }
};

template <typename T>
void DynamicArrayAllocateChunk(DynamicArray<T> *array)
{
    ArrayChunk *newChunk = (ArrayChunk *)AllocateMem(array->allocator, sizeof(ArrayChunk) + (array->elementsPerChunk * sizeof(T)));
    newChunk->nextChunk = NULL;

    if (array->headChunk == NULL)
    {
        array->headChunk = newChunk;
        array->tailChunk = newChunk;
    }
    else
    {
        ArrayChunk *chunk = array->tailChunk;
        chunk->nextChunk = newChunk;
        array->tailChunk = chunk->nextChunk;
    }

    array->chunkCount++;
}


template <typename T>
void DynamicArrayEnsureCapacity(DynamicArray<T> *array, uint32 capacity)
{
    ASSERT(array->allocator != NULL);
    ASSERT(array->elementsPerChunk > 0);

    if (array->chunkCount * array->elementsPerChunk < capacity)
    {
        uint32 chunksToAdd = ((capacity / array->elementsPerChunk) - array->chunkCount) + 1;

        for (int i = 0; i < chunksToAdd; i++)
        {
            DynamicArrayAllocateChunk(array);
        }
    }
}

template <typename T>
inline DynamicArray<T> MakeDynamicArray(MAllocator *allocator, uint32 elementsPerChunk, uint32 chunkCount = 1)
{
    DynamicArray<T> array = {};
    array.allocator = allocator;
    array.elementsPerChunk = elementsPerChunk;

    DynamicArrayEnsureCapacity(&array, chunkCount * elementsPerChunk);

    return array;
};

template <typename T>
inline uint32 PushBack(DynamicArray<T> *array, T elem)
{
    DynamicArrayEnsureCapacity(array, array->count + 1);
    uint32 index = array->count;
    array->count++;
    (*array)[index] = elem;
    return index;
}


// template <typename T>
// inline T *PushBackPtr(DynamicArray<T> *array)
// {
//     DynamicArrayEnsureCapacity(array, array->count + 1);
//     uint32 index = array->count;
//     array->count++;
//     T *result = &(*array)[index];
//     memset(result, 0, sizeof(T));
//     return result;
// }



template <typename T>
inline void DynamicArrayClear(DynamicArray<T> *array) {
    array->count = 0;
}
