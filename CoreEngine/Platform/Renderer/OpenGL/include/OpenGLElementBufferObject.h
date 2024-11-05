#pragma once
#include <Render/includes/ElementBufferObject.h> 
#include <glad/glad.h>


namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			class OpenGLElementBufferObject final : public ElementBufferObject
			{
			public:

				OpenGLElementBufferObject();
				~OpenGLElementBufferObject();

			public:

				virtual void CreateBuffer(const void* dataArr, uint32 sizeArr, const ETypeData& typeData, const ETypeDraw& typeDraw, const VertexBufferObject& buffObj) override;
				virtual void DeleteBuffer() override;

				virtual bool SetData(const int32 beginFillData, const uint32 sizeData, const void* data, const bool isBind = true) override;

				virtual void Bind() const override;
				virtual void UnBind() const override;

				bool IsCreate() const { return m_isCreate; }

			private:

				uint32 m_EBO;
				ETypeData m_typeStorageData;
				bool m_isCreate;
			};
			
		}
	}
}
