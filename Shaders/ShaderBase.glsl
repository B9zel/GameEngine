
#type vertex
#version 330 core
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
    gl_Position = Projection * View * Model * vec4(position, 1.0f);
}



#type fragment

#version 330 core 

in vec3 NormalFr;
in vec2 TexCoord;
in vec3 FragPos;

out vec4 color;

uniform sampler2D ourTexture1;
uniform vec3 PosLight;
uniform vec3 ViewPos;


void main()
{
    vec3 LightColor = vec3(1,1,1);
    vec3 ObjColor = vec3(1,1, 0);
    vec3 ambient = 0.1 * LightColor;

    // Diffuse 
    vec3 norm = normalize(NormalFr);
    vec3 lightDir = normalize(PosLight - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * LightColor;

    // Specular
    float strangeSpecular = 0.5;
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = strangeSpecular * spec * LightColor;

    vec3 result = (specular + ambient + diffuse) * ObjColor;
   
    color = vec4(result, 1.0f);  //* texture(ourTexture1, TexCoord); 
   
}