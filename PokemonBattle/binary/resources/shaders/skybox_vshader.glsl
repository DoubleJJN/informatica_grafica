#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 uP;
uniform mat4 uV;

void main()
{
    TexCoords = aPos;  
    gl_Position =  uP * uV * vec4(aPos, 1.0);
}
