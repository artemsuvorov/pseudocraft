#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
out vec2 vUv;
uniform mat4 u_ViewProjection;

void main()
{
   vUv = uv;
   gl_Position = u_ViewProjection * vec4(position, 1.0);
}

#shader fragment
#version 330 core

in vec2 vUv;
out vec4 color;
uniform sampler2D u_Texture;

void main()
{
   color = texture(u_Texture, vUv);
}
