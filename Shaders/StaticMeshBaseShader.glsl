
#type vertex
#version 450 core
#extension GL_ARB_gpu_shader_int64 : enable
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in int ID;


out vec2 TexCoord;
out vec3 NormalFr;
out vec3 FragPos;
out vec4 FragPosLightSpace;
layout(location = 4) out flat int OutID;

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;
//uniform mat4 DirectionLightSpace;

uniform int CountDirectionLight;
uniform int CountSpotLight;

layout(location = 5) out vec4 FragPosLightSpaces[8];


layout(std430, binding = 3) buffer LightSpaceLayout
{
    mat4 LightSpace[];
};

struct DirectionLight
{
    vec3 Color;
	float Intencity;
    vec3 Direction;
    uint64_t LayerShadow;
};

struct SpotLight
{
    vec3 Color; 
    vec3 Direction; 
    vec3 Location;
    uint64_t LayerShadow;
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
layout(std430, binding = 2) buffer SpotLightLayout
{
    SpotLight SpotLights[];
};



void main()
{
    OutID = ID;
    TexCoord = texCoord;
    NormalFr = normalize(mat3(transpose(inverse(Model))) * Normal);
    FragPos = vec3(Model * vec4(position, 1.0f));
    vec3 Offset = vec3(0, 0, 0);
    for (int i = 0; i < CountDirectionLight; i++)
    {
        const int Index = int(DirectionLights[i].LayerShadow);
        FragPosLightSpaces[Index] = LightSpace[Index] * vec4(FragPos, 1.0);
      
    }
    for (int i = 0; i < CountSpotLight; i++)
    {
        const int Index = int(SpotLights[i].LayerShadow);
        FragPosLightSpaces[Index] = LightSpace[Index] * vec4(FragPos, 1.0);
       
    }
    //const int a = int(DirectionLights[0].LayerShadow);
   
    gl_Position = Projection * View * Model * vec4(position + Offset, 1.0f);
    
    
}



#type fragment

#version 450 core 
#extension GL_ARB_gpu_shader_int64 : enable

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
    uint64_t LayerShadow;
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
    uint64_t LayerShadow;
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
in vec4 FragPosLightSpace;
layout(location = 4) in flat int OutID;
layout(location = 5) in vec4 FragPosLightSpaces[8];

layout(location = 0) out vec4 color;
layout(location = 1) out int ObjectID;

uniform sampler2DArray DirectionShadowMap;
//uniform sampler2DArray SpotlightShadowMap;

uniform sampler2D ShadowMap;
uniform vec3 PosLight;
uniform vec3 ViewPos;
uniform int CountPointLight;
uniform int CountDirectionLight;
uniform int CountSpotLight;


float ShadowDirectionCalculation(vec4 FragPosLightSpace, vec3 normal, vec3 LightDir, uint64_t LayerIndex)
{
    vec3 ProjCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    ProjCoords = ProjCoords * 0.5 + 0.5;

   // if (ProjCoords.z > 1.0) return 0.0;
    float Shadow = 0.0f;
    float CurrentDepth = ProjCoords.z;

    float Bias = max(0.000699 * (1.0 - dot(normal, -LightDir)) , 0.000699) ;

    vec2 TexelSize = 1.0 / textureSize(ShadowMap, 0);
    int CountPoints = 0;
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float ClosestDepth = texture(DirectionShadowMap, vec3(ProjCoords.xy + vec2(x, y) * TexelSize, LayerIndex)).r;
            Shadow += CurrentDepth - Bias > ClosestDepth ? 1.0 : 0.0f;
            CountPoints++;
        }

    }

    return Shadow / CountPoints;
}

float ShadowSpotlightCalculation(vec4 FragPosLightSpace, vec3 normal, vec3 LightDir, vec3 Position, uint64_t LayerIndex)
{
    vec3 ProjCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    ProjCoords = ProjCoords * 0.5 + 0.5;

   // if (ProjCoords.z > 1.0) return 0.0;
    float Shadow = 0.0f;
    float CurrentDepth = ProjCoords.z;

    float Bias = max(0.005 * (1.0 - dot(normal, -LightDir) - length(FragPosLightSpace.xyz - Position)) , 0.000699) ;

    vec2 TexelSize = 1.0 / textureSize(ShadowMap, 0);
    int CountPoints = 0;
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float ClosestDepth = texture(DirectionShadowMap, vec3(ProjCoords.xy + vec2(x, y) * TexelSize, LayerIndex)).r;
            Shadow += CurrentDepth - Bias > ClosestDepth ? 1.0 : 0.0f;
            CountPoints++;
        }

    }

    return Shadow / CountPoints;
}



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

        float Shadow = ShadowDirectionCalculation(FragPosLightSpaces[int(DirectionLights[i].LayerShadow)], norm, lightDir, DirectionLights[i].LayerShadow);
        if (Shadow == 20)
        {
            return vec3(0,0,1);
        }
       /* if (Shadow < 0)
        {
            resColor = vec3(1,0,0);
            return resColor;
        }*/
        if (Shadow > 1.0f)
        {
            return vec3(0, 1, 0);
        }
        //resColor = vec3(FragPosLightSpaces[2]);
        resColor += (1.0 - Shadow) * (diffuse + specular) * DirectionLights[i].Intencity;
      
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
            float Shadow = ShadowSpotlightCalculation(FragPosLightSpaces[int(SpotLights[i].LayerShadow)], norm, -lightDir, SpotLights[i].Location, SpotLights[i].LayerShadow);

            diffuse *= Attenuation;
            specular *= Attenuation;

            diffuse *= SpotLights[i].Intencity;
            specular *= SpotLights[i].Intencity;
            resColor += (1.0 - Shadow) * (diffuse + specular);
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
    result = (CalculateDirectionLight() + CalculatePointLight() + CalculateSpotLight()) * ObjColor;
    result += ambient;
    ObjectID = OutID;
    //result = vec3(1,1,1);
    color = vec4(result, 1.0f);  //* texture(ourTexture1, TexCoord); 
   

    //float depthValue = texture(ShadowMap, TexCoord).r;
    //color = vec4(vec3(depthValue,depthValue,depthValue), 1.0);
    

}