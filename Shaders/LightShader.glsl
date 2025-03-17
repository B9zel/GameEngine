
#type vertex
#version 330 core
layout (location = 0) in vec3 position;


uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

void main()
{
    gl_Position = Projection * View * Model * vec4(position, 1.0f);

}


#type fragment

#version 330 core 

out vec4 color;

uniform sampler2D ourTexture1;


void main()
{  
    color = vec4(1,1,1,1);
}