#type vertex
#version 450 core

layout(location = 0) in vec3 Position;

uniform mat4 ModelMat;
uniform mat4 LightSpaceMat;

void main()
{
	gl_Position = LightSpaceMat * ModelMat * vec4(Position, 1.0);
}





#type fragment
#version 450 core 


void main()
{
}