#pragma once
#include <Core/includes/Base.h>
#include <Render/includes/Shader.h>
#include <Render/includes/VertexArrayObject.h>
#include <Render/includes/Texture.h>

namespace CoreEngine
{
	class PrimitiveProxy
	{
	public:

		PrimitiveProxy() = default;

	public:

		virtual const HashTableMap<Render::Shader*, Render::VertexArrayObject*>& GetShaders() const;
		virtual void AddShaderWithArrayObject(Render::Shader* shaderKey, Render::VertexArrayObject* arrayValue);
		virtual void AddTexture(Render::Texture* NewTexture);
		virtual const DArray<Render::Texture*>& GetTextures() const;
		

	private:

		HashTableMap<Render::Shader*, Render::VertexArrayObject*> m_Shaders;
		DArray<Render::Texture*> m_Textures;

	public:
		// Test
		unsigned int CountVertex = 0;
	};
}