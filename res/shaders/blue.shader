#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
uniform float u_Aspect;

void main()
{
   gl_Position = vec4(position.x / u_Aspect, position.y, 0.0, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
   color = vec4(0.2, 0.2, 0.8, 1.0);
}