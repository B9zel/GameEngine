#pragma once
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#include <Render/includes/VertexArrayObject.h>
#include <Render/includes/ElementBufferObject.h>
#include <Core/includes/Base.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			class OpenGLVertexArrayObject : public VertexArrayObject
			{
			public:

				OpenGLVertexArrayObject();
				~OpenGLVertexArrayObject();

				OpenGLVertexArrayObject(const OpenGLVertexArrayObject&) = delete;
				OpenGLVertexArrayObject& operator=(const OpenGLVertexArrayObject&) = delete;

				OpenGLVertexArrayObject(OpenGLVertexArrayObject&& other) noexcept;
				OpenGLVertexArrayObject& operator=(OpenGLVertexArrayObject&& other) noexcept;

				virtual void CreateVertexArray() override;

				virtual void SetupIntorprit(RenderDevice* Device, uint32 location, uint32 sizeArgument, uint32 step, const ETypeData& typeData,
											const VertexBufferObject& bufferObject, const uint32 beginStep = 0) override;
				virtual void SetupIntorprit(RenderDevice* Device, uint32 location, uint32 sizeArgument, uint32 step, const ETypeData& typeData,
											const VertexBufferObject& bufferObject, const ElementBufferObject& elementObject,
											const uint32 beginStep = 0) override;
				virtual void SetupIntorprit(RenderDevice* Device, uint32 location, uint32 sizeArgument, uint32 step, const ETypeData& typeData,
											const uint32 beginStep = 0) override;

				virtual void DeleteVertexObject(RenderDevice* Device) override;

				virtual void Bind(RenderDevice* Device) const override;
				virtual void UnBind() const override;

				virtual RHI::HandleVAO GetHandle() const override;

				void ResetLinkElementBuffer();

			private:

				uint32 m_VAO;
				bool m_isCreate;

				RHI::HandleVAO Handle;

				const ElementBufferObject* m_elementBuffer;
			};
		} // namespace OpenGL
	} // namespace Render
} // namespace CoreEngine
