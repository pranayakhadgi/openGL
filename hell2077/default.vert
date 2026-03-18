#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 outColor;

//using a horizontal offset to shift the triangle to the right making sure that the triangle is fully visible in the viewport

uniform float xOffset;

void main()
{
   gl_Position = vec4(aPos.x + xOffset, -aPos.y, aPos.z, 1.0);
   outColor = aColor;
}