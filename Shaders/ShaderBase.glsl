
#type vertex
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(position, 1.0f);
    ourColor = color;
    TexCoord = texCoord;
}



#type fragment
#version 330 core 
in vec3 ourColor;
in vec2 TexCoord;

uniform vec4 inputColor;
out vec4 color;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture1;
uniform float a;

void main()
{
    color = vec4(1,1,1,1);//mix(texture(ourTexture, TexCoord), texture(ourTexture1, TexCoord), a) * inputColor;
}
