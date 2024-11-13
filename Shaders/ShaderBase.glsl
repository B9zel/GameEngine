
#type vertex
#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(position,1, 1.0f);
   // ourColor = color;
    TexCoord = texCoord;
}



#type fragment

#version 330 core 
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D ourTexture3;

void main()
{
    color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord),0.5);//mix(, texture(ourTexture3, TexCoord), 0.2);
}