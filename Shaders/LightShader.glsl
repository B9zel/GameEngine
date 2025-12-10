
#type vertex
#version 330 core
layout (location = 0) in vec3 position;
layout(location = 1) in vec2 TexCoord;

out vec2 CoordTex;

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

void main()
{
    gl_Position = Projection * View * Model * vec4(position, 1.0f);
    CoordTex = TexCoord;

}


#type fragment

#version 330 core 

out vec4 color;


in vec2 CoordTex;
uniform sampler2DArray ourTexture1;


void main()
{  
    float Col = texture(ourTexture1, vec3(CoordTex, 1)).r;
    color = vec4(vec3(1.5 - Col), 1.0);
   // color = vec4(1,1,1,1);
}