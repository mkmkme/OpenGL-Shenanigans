#version 330 core

out gl_PerVertex { vec4 gl_Position; };

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
