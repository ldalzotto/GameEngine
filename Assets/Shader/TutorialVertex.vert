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

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main()
{
	gl_Position = CPBO.CameraProjection.proj * CPBO.CameraProjection.view * MPBO.ModelProjection.model * vec4(inPosition, 0.0, 1.0);
	fragColor = inColor;
}