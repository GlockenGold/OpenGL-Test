#version 330 core
layout (location = 0) in vec3 aPos; // The position variable has attribute positon 0

out vec4 vertexColor; // Specify a colour output to the fragment shader.

void main()
{
    gl_Position = vec4(aPos, 1.0); // We can give a vec3 directly to a vec4's constructor.
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // Set the output variable to a dark-red colour
}