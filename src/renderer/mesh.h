


struct Mesh {
    uint32 size;
    void *data;
    
    uint32 vertCount;
    vec3 *verts;
    vec3 *normals;

    uint32 indexCount;
    uint32 *indices;

    uint32 texcoordsCount;
    vec2 *texcoords;

    int32 vertBufferID;
    int32 indexBufferID;
};


struct vkMesh {
    uint32_t vertCount;
    uint32_t indexCount;
    VkDeviceSize size;
    VkDeviceSize vertexBufferSize;
    VkDeviceSize indexBufferSize;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    void* data; // Vertex data
    vec3* verts; // Pointer to vertex positions
    vec2* texcoords; // Pointer to texture coordinates
    uint32_t* indices; // Pointer to indices
};

