#version 450

struct CameraProjection
{
    mat4 view;
    mat4 proj;
};

struct ModelProjection
{
    mat4 model;
};

layout(set = 0, binding = 0) uniform CameraProjectionBufferObject
{
    CameraProjection CameraProjection;
} CPBO;

layout(set = 1, binding = 0) uniform ModelProjectionBufferObject
{
    ModelProjection ModelProjection;
} MPBO;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

void main()
{
	gl_Position = CPBO.CameraProjection.proj * CPBO.CameraProjection.view * MPBO.ModelProjection.model * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
}