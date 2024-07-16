

enum CameraType: uint8 
{
    CameraType_Perspective,
    CameraType_Orthographic,
};

struct Camera
{

    CameraType type;

    mat4 projection = Identity4();
    mat4 view = Identity4();

    mat4 viewProjection;

    vec3 pos;

    int32 frameCount;

    real32 vFOV;
    real32 aspect;
    real32 nearPlane;
    real32 farPlane;

    // ortho
    real32 left;
    real32 right;
    real32 top;
    real32 bottom;
    real32 near;
    real32 far;

};