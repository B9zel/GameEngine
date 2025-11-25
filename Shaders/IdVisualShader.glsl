#type vertex
#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 texCoord;


out vec2 TexCoord;
out vec3 NormalFr;
out vec3 FragPos;

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

void main()
{
    TexCoord = texCoord;
    NormalFr = normalize(mat3(transpose(inverse(Model))) * Normal);
    FragPos = vec3(Model * vec4(position, 1.0f));
    gl_Position = Projection * View * Model * vec4(position + vec3(Normal * 0.0001), 1.0f);
}

#type fragment

#version 450 core


uniform usampler2D texture;
in vec2 TexCoord;
layout(location = 0) out vec4 outColor; 

void main()
{
   // ivec2 coord = ivec2(floor(gl_FragCoord.xy));
    uint IdValue = texture(texture, TexCoord, 0).r;
    if (IdValue == 8)
    {
        outColor = vec4(IdValue,0, 0, 1.0);
    }
    else
    {
       outColor = vec4(1.0f, 1.0f,1.0f, 0.0);
    }
   // gl_FragColor = vec4(1.0f, 1.0f,1.0f, 1.0);
}