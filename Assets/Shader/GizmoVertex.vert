#version 450

struct CameraProjection
{
    mat4 view;
    mat4 proj;
};

layout(set = 0, binding = 0) uniform CameraProjectionBufferObject
{
    CameraProjection CameraProjection;
} CPBO;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main()
{
	gl_Position = CPBO.CameraProjection.proj * CPBO.CameraProjection.view * vec4(inPosition, 1.0);
    fragColor = inColor;
}