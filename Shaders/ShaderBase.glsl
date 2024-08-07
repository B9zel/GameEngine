
#type vertex
#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 TexCoord;

out vec2 outTexCoord;

uniform mat4 scale;
uniform mat4 offset;
uniform mat4 rotate;
void main()
{
	gl_Position = scale * rotate * offset * vec4(position.x,position.y, 1,1);
	outTexCoord = TexCoord;
};


#type fragment
#version 330 core 
out vec4 fragColor; 
in vec2 outTexCoord;

uniform sampler2D outTexture;

// main
void main() {
   
	vec4 color = texture(outTexture, outTexCoord);
	if (color.rgb == 0)
	{
		color.rgba = vec4(1,1,1, 1);
	}
    // post
	fragColor = color;
}