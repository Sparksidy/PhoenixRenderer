#version 450
#extension GL_ARB_separate_shader_objects: enable

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 0) out vec3 fragColor;

layout(binding = 0) uniform UniformBufferObject{
	 mat4 model;
	 mat4 view;
	 mat4 projection;
}ubo;

void main()
{
	gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);
	fragColor   = inColor;
}