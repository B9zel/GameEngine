#include <Platform/Renderer/OpenGL/include/Debug/OpenGLDebugRender.h>
#include <Render/includes/RenderDevice.h>
#include <glad/glad.h>
#include <cstddef>

namespace
{
	struct DebugVertex
	{
		float Position[3];
		float Color[3];
	};

	constexpr const char* VertexSource = R"(
#version 450 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;

uniform mat4 View;
uniform mat4 Projection;

out vec3 LineColor;

void main()
{
    LineColor = Color;
    gl_Position = Projection * View * vec4(Position, 1.0);
}
)";

	constexpr const char* FragmentSource = R"(
#version 450 core
in vec3 LineColor;
layout(location = 0) out vec4 OutColor;

void main()
{
    OutColor = vec4(LineColor, 1.0);
}
)";

	struct ScopedDebugRenderState
	{
		GLint Program, VAO, ArrayBuffer, DepthFunction;
		GLboolean DepthTest, DepthWrite, ObjectIDColorMask[4];
		GLfloat LineWidth;

		ScopedDebugRenderState()
		{
			glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &VAO);
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &ArrayBuffer);
			glGetIntegerv(GL_DEPTH_FUNC, &DepthFunction);
			DepthTest = glIsEnabled(GL_DEPTH_TEST);
			glGetBooleanv(GL_DEPTH_WRITEMASK, &DepthWrite);
			glGetBooleani_v(GL_COLOR_WRITEMASK, 1, ObjectIDColorMask);
			glGetFloatv(GL_LINE_WIDTH, &LineWidth);
		}

		~ScopedDebugRenderState()
		{
			glUseProgram(Program);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, ArrayBuffer);
			if (DepthTest) glEnable(GL_DEPTH_TEST);
			else glDisable(GL_DEPTH_TEST);
			glDepthFunc(DepthFunction);
			glDepthMask(DepthWrite);
			glColorMaski(1, ObjectIDColorMask[0], ObjectIDColorMask[1], ObjectIDColorMask[2], ObjectIDColorMask[3]);
			glLineWidth(LineWidth);
		}
	};
}

namespace CoreEngine::Render::OpenGL
{
	OpenGLDebugRender::OpenGLDebugRender(RenderDevice& Device) : m_Device(Device)
	{
	}

	OpenGLDebugRender::~OpenGLDebugRender()
	{
		if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
		if (m_VBO.IsValid()) m_Device.DeleteVBO(m_VBO);
		if (m_Shader.IsValid()) m_Device.DeleteShader(m_Shader);
	}

	void OpenGLDebugRender::Flush(const FMatrix4x4& view, const FMatrix4x4& projection)
	{
		if (DebugLines.empty()) return;

		DArray<DebugVertex> Vertices;
		Vertices.reserve(DebugLines.size() * 2);
		for (const DebugLine& Line : DebugLines)
		{
			const float R = Line.Color.GetX(), G = Line.Color.GetY(), B = Line.Color.GetZ();
			Vertices.push_back({{Line.Start.GetX(), Line.Start.GetY(), Line.Start.GetZ()}, {R, G, B}});
			Vertices.push_back({{Line.End.GetX(), Line.End.GetY(), Line.End.GetZ()}, {R, G, B}});
		}

		ScopedDebugRenderState PreviousState;
		if (!m_Shader.IsValid())
		{
			m_Shader = m_Device.CreateShader(VertexSource, FragmentSource);
			if (!m_Shader.IsValid())
			{
				ClearLines();
				return;
			}
		}

		if (!m_VBO.IsValid())
		{
			m_VBO = m_Device.CreateBuffer(EBufferTargetType::VERTEX_BUFFER, nullptr, 0, ETypeData::FLOAT, ETypeStorageDraw::STREAM);
		}

		if (!m_VAO)
		{
			glGenVertexArrays(1, &m_VAO);
			glBindVertexArray(m_VAO);
			m_Device.BindVBO(m_VBO);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), reinterpret_cast<void*>(offsetof(DebugVertex, Position)));
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), reinterpret_cast<void*>(offsetof(DebugVertex, Color)));
			glEnableVertexAttribArray(1);
		}

		glBindVertexArray(m_VAO);
		m_Device.BindVBO(m_VBO);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(Vertices.size() * sizeof(DebugVertex)), Vertices.data(), GL_STREAM_DRAW);

		m_Device.BindShader(m_Shader);
		m_Device.SetUniformMatrix4x4(m_Shader, "View", view);
		m_Device.SetUniformMatrix4x4(m_Shader, "Projection", projection);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_FALSE);
		glLineWidth(1.0f);
		// Debug geometry must not overwrite the editor's object-picking IDs.
		glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(Vertices.size()));

		ClearLines();
	}
} // namespace CoreEngine::Render::OpenGL
