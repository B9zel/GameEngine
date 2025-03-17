#include <Core/includes/PrimitiveProxy.h>

namespace CoreEngine
{
    const HashTableMap<Render::Shader*, Render::VertexArrayObject*>& PrimitiveProxy::GetShaders() const
    {
        return m_Shaders;
    }

    void PrimitiveProxy::AddShaderWithArrayObject(Render::Shader* shaderKey, Render::VertexArrayObject* arrayValue)
    {
        if (shaderKey && arrayValue)
        {
            m_Shaders.insert(Pair<Render::Shader*, Render::VertexArrayObject*>(shaderKey, arrayValue));
        }
    }

    void PrimitiveProxy::AddTexture(Render::Texture* NewTexture)
    {
        m_Textures.push_back(NewTexture);
    }

    const DArray<Render::Texture*>& PrimitiveProxy::GetTextures() const
    {
        return m_Textures;
    }

    const Transform& PrimitiveProxy::GetTransform() const
    {
        return m_Transform;
    }

    void PrimitiveProxy::SetTransform(const Transform& transform)
    {
        m_Transform = transform;
    }

}
