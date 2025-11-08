
#type vertex
#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in int ID;


out vec2 TexCoord;
out vec3 NormalFr;
out vec3 FragPos;
layout(location = 4) out flat int OutID;

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

void main()
{
    OutID = ID;
    TexCoord = texCoord;
    NormalFr = normalize(mat3(transpose(inverse(Model))) * Normal);
    FragPos = vec3(Model * vec4(position, 1.0f));
   
    gl_Position = Projection * View * Model * vec4(position, 1.0f);
    
}



#type fragment

#version 450 core 

struct Light
{
    int Type;
    vec3 Location;
    vec3 Direction;
    vec3 Color;
    float Intencity;
    float CutOff;
};

struct DirectionLight
{
    vec3 Color;
	float Intencity;
    vec3 Direction;
};

struct PointLight
{
    vec3 Color;
	float Intencity;
    vec3  Location;
    float Constant;
	float Linear;
	float Quadratic;
};

struct SpotLight
{
    vec3 Color; 
    vec3 Direction; 
    vec3 Location;
	float Intencity;
    float CutOff; 
    float OuterCutOff;
    float Constant;
	float Linear;
	float Quadratic;
};

layout(std430, binding = 0) buffer DirectionLightLayout
{
    DirectionLight DirectionLights[];
};

layout(std430, binding = 1) buffer PointLightLayout
{
    PointLight PointLights[];
};

layout(std430, binding = 2) buffer SpotLightLayout
{
    SpotLight SpotLights[];
};


in vec3 NormalFr;
in vec2 TexCoord;
in vec3 FragPos;
layout(location = 4) in flat int OutID;

layout(location = 0) out vec4 color;
layout(location = 1) out int ObjectID;
//layout(location = 1) out vec4 ObjectID;

uniform sampler2D ourTexture1;
uniform vec3 PosLight;
uniform vec3 ViewPos;
uniform int CountPointLight;
uniform int CountDirectionLight;
uniform int CountSpotLight;

vec3 CalculateDirectionLight()
{
    vec3 resColor = {0.0f, 0.0f, 0.0f};

    for (int i = 0; i < CountDirectionLight; i++)
    {
        // Diffuse 
        vec3 norm = normalize(NormalFr);
        vec3 lightDir = normalize(-DirectionLights[i].Direction);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * DirectionLights[i].Color;

        // Specular
        float strangeSpecular = 0.5;
        vec3 viewDir = normalize(ViewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = strangeSpecular * spec * DirectionLights[i].Color;

        resColor += (diffuse + specular) * DirectionLights[i].Intencity;
    }

    return resColor;
}


vec3 CalculatePointLight()
{
    vec3 resColor = {0.0f, 0.0f, 0.0f};

    for (int i = 0; i < CountPointLight; i++)
    {
        
        // Diffuse 
        vec3 norm = normalize(NormalFr);
        vec3 lightDir = normalize(PointLights[i].Location - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * PointLights[i].Color;

        // Specular
        float strangeSpecular = 0.5;
        vec3 viewDir = normalize(ViewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = strangeSpecular * spec * PointLights[i].Color;

        float Distance = length(PointLights[i].Location - FragPos);
        float Attenuation = 1.0f / (PointLights[i].Constant + PointLights[i].Linear * Distance + PointLights[i].Quadratic * (Distance * Distance));

        Attenuation *= PointLights[i].Intencity;

        diffuse *= Attenuation;
        specular *= Attenuation;

        resColor += (diffuse + specular);
    }
    return resColor;
   
}

vec3 CalculateSpotLight()
{
    vec3 resColor = {0.0f, 0.0f, 0.0f};


    for (int i = 0; i < CountSpotLight; i++)
    {
        vec3 lightDir = normalize(SpotLights[i].Location - FragPos);
        float Theta = dot(lightDir, normalize(-SpotLights[i].Direction));
        
        if (Theta > SpotLights[i].OuterCutOff)
        {
             // Diffuse 
            vec3 norm = normalize(NormalFr);
      
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * SpotLights[i].Color;

            // Specular
            float strangeSpecular = 0.5;
            vec3 viewDir = normalize(ViewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = strangeSpecular * spec * SpotLights[i].Color;

            float Epsilon = SpotLights[i].CutOff - SpotLights[i].OuterCutOff;
            float Intensity = clamp((Theta - SpotLights[i].OuterCutOff) / Epsilon, 0.0, 1.0);
            diffuse *= Intensity;
            specular *= Intensity;

            float Distance = length(SpotLights[i].Location - FragPos);
            float Attenuation = 1.0f / (SpotLights[i].Constant + SpotLights[i].Linear * Distance + SpotLights[i].Quadratic * (Distance * Distance));


            diffuse *= Attenuation;
            specular *= Attenuation;

            diffuse *= SpotLights[i].Intencity;
            specular *= SpotLights[i].Intencity;
            resColor += (diffuse + specular);
        }
    }

    return resColor;
}


void main()
{
    
    vec3 ObjColor = vec3( 1, 1, 1);
    vec3 ambient = vec3(0.01) * ObjColor;

   /* vec3 ambient = 0.01 * LightColor;

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
    vec3 specular = strangeSpecular * spec * LightColor; */


    vec3 result;
   
    /* if (NormalFr.x == 1 )
    {
        result.x = 1.0f;
    }
    else if (NormalFr.x == -1)
    {
        result.x = 0.5f;
    }
    else if (NormalFr.y == 1)
    {
         result.y = 1.0f;
    }
    else if (NormalFr.y == -1)
    {
        result.y = 0.5f;
    }
    else if (NormalFr.z == 1)
    {
         result.z = 1.0f;
    }
    else if (NormalFr.z == -1)
    {   
        result.z = 0.5;
    }
    else
    {
        result = vec3(0.05);
    } */
    //
    result += (CalculateDirectionLight() + CalculatePointLight() + CalculateSpotLight()) * ObjColor;
    result += ambient;
    ObjectID = OutID;
    color = vec4(result, 1.0f);  //* texture(ourTexture1, TexCoord); 
   
    /*switch (ObjectID.r)
    {
        case 0:
            color = vec4(1.0, 0.0, 0.0, 1.0);
            break;
        case 1:
            color = vec4(0.0, 1.0, 0.0, 1.0);
            break;
        case 2:
            color = vec4(0.0, 0.0, 1.0, 1.0);
            break;
        case 3:
            color = vec4(1.0, 1.0, 0.0, 1.0);
            break;
        case 4:
            color = vec4(1.0, 0.0, 1.0, 1.0);
            break;
        case 5:
            color = vec4(0.0, 1.0, 1.0, 1.0);
            break;
        case 6:
            color = vec4(1.0, 0.0, 0.0, 1.0);
            break;
        case 7:
            color = vec4(0.0, 1.0, 0.0, 1.0);
            break;
        case 8:
            color = vec4(0.0, 0.0, 1.0, 1.0);
            break;
        case 9:
            color = vec4(1.0, 1.0, 0.0, 1.0);
            break;
        case 10:
            color = vec4(1.0, 0.0, 1.0, 1.0);
            break;
        case 11:
            color = vec4(0.0, 1.0, 1.0, 1.0);
            break;
        default:
            break;
    }*/

}