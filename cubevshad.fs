#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 FragPos;
out mat4 Rotation;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 rot;

void main()
{
    //gl_Position = projection * view * round(model * vec4(aPos, 1.0) * 10) / 10;   //stable wobbly
    //gl_Position = round(projection * view * model * vec4(aPos, 1.0) * 3) / 3;   //unstable wobbly
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    Normal = aNormal;
    Rotation = rot;
    TexCoords = aTexCoords;
    FragPos = vec3(model * vec4(aPos, 1.0));
}

