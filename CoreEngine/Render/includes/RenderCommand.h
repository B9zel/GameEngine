#pragma once
#include "Render/includes/RenderDevice.h"
#include "Core/includes/Base.h"

DECLARE_LOG_CATEGORY_EXTERN(RenderCommandLog);

namespace CoreEngine::Render
{
	class Shader;
	class VertexArrayObject;

	struct RenderCommand
	{
	public:

		enum class ETypeCommand : uint32
		{
			CREATE_VERTEX_BUFFER = 0,
			CREATE_INDEX_BUFFER,
			CREATE_VERTEX_ARRAY_OBJECT,
			CREATE_TEXTURE_2D,
			CREATE_SHADER_PROGRAM,
			BIND_SHADER_PROGRAM,
			ACTIVATION_TEXTURE,
			DRAW_INDEX
		};

	public:

		virtual ~RenderCommand() = default;
		virtual ETypeCommand GetType() const = 0;
		virtual void Execute(RenderDevice* Devise) = 0;
	};

	struct CmdCreateVertexBuffer : public RenderCommand
	{
		DArray<float> Data;
		RHI::BufferHandle OutHandle;

		CmdCreateVertexBuffer(const DArray<float>& VertexData) : Data(VertexData)
		{
		}

		virtual void Execute(RenderDevice* Devise) override
		{
			OutHandle = Devise->CreateBuffer(EBufferTargetType::VERTEX_BUFFER, Data.data(), Data.size(), ETypeData::FLOAT);
		}

		virtual ETypeCommand GetType() const override
		{
			return ETypeCommand::CREATE_VERTEX_BUFFER;
		}
	};

	struct CmdCreateIndexBuffer : public RenderCommand
	{
		DArray<uint32> Data;
		RHI::BufferHandle OutHandle;

		CmdCreateIndexBuffer(const DArray<uint32>& IndexData) : Data(IndexData)
		{
		}

		virtual void Execute(RenderDevice* Devise) override
		{
			OutHandle = Devise->CreateBuffer(EBufferTargetType::VERTEX_BUFFER, Data.data(), Data.size(), ETypeData::UNSIGNED_INT);
		}

		virtual ETypeCommand GetType() const override
		{
			return ETypeCommand::CREATE_INDEX_BUFFER;
		}
	};

	struct CmdCreateVertexArrayObject : public RenderCommand
	{
		RHI::HandleVAO OutVAO;
		RHI::BufferHandle VBO;
		RHI::BufferHandle IBO;
		DArray<Turple<uint32, uint32, uint32, ETypeData, uint32>> Attributes;

	public:

		CmdCreateVertexArrayObject(const RHI::BufferHandle& VBO, const RHI::BufferHandle& IBO,
								   const DArray<Turple<uint32, uint32, uint32, ETypeData, uint32>>& Attribs)
			: VBO(VBO), IBO(IBO), Attributes(Attribs)
		{
		}

		virtual void Execute(RenderDevice* Devise) override
		{
			OutVAO = Devise->CreateVAO(VBO, IBO, Attributes);
		}

		virtual ETypeCommand GetType() const override
		{
			return ETypeCommand::CREATE_VERTEX_ARRAY_OBJECT;
		}
	};

	struct CmdCreateTexture2D : public RenderCommand
	{
		// Input
		uint32 Width, Height;
		ETypeChannel Channel;
		DArray<uint32> Data;

		StringView Path;
		//
		RHI::TextureHandle OutHandle;

	public:

		CmdCreateTexture2D(const uint32 width, const uint32 height, const ETypeChannel channel, const DArray<uint32>& Indexes)
			: Width(width), Height(height), Channel(channel), Data(Indexes)
		{
		}

		CmdCreateTexture2D(const StringView& Path) : Path(Path)
		{
		}

		virtual void Execute(RenderDevice* Devise) override
		{
			if (!Path.empty())
			{
				OutHandle = Devise->CreateTexture2D(Path.data());
				return;
			}
			OutHandle = Devise->CreateTexture2D(Width, Height, Channel, Data.data());
		}
	};

	struct CmdCreateShaderProgram : public RenderCommand
	{
		String VertexShader, FragmentShader;
		RHI::ShaderHandle OutHandle;

	public:

		CmdCreateShaderProgram(const StringView VertexShad, const StringView FragmendShad) : VertexShader(VertexShad), FragmentShader(FragmendShad)
		{
		}

		virtual void Execute(RenderDevice* Devise) override
		{
			OutHandle = Devise->CreateShader(VertexShader, FragmentShader);
		}

		virtual ETypeCommand GetType() const override
		{
			return ETypeCommand::CREATE_SHADER_PROGRAM;
		}
	};

	struct CmdBindShaderProgram : public RenderCommand
	{
		RHI::ShaderHandle Handle;

	public:

		CmdBindShaderProgram(const RHI::ShaderHandle& ShadHandle) : Handle(ShadHandle)
		{
		}

		virtual void Execute(RenderDevice* Devise) override;

		virtual ETypeCommand GetType() const override
		{
			return ETypeCommand::BIND_SHADER_PROGRAM;
		}
	};

	struct ActivateTextureSet
	{
		HashTableMap<uint32, RHI::TextureHandle> Textures;
	};

	struct CmdActivationTexture : public RenderCommand
	{
		UniquePtr<ActivateTextureSet> ActivationSet;

	public:

		CmdActivationTexture(UniquePtr<ActivateTextureSet> Activations) : ActivationSet(std::move(Activations))
		{
		}

		virtual void Execute(RenderDevice* Devise) override
		{
			for (auto& DataActivate : ActivationSet->Textures)
			{
				const Render::Texture2D* Texture = Devise->GetTextureID(DataActivate.second);
				Texture->Bind(DataActivate.first);
			}
		}

		virtual ETypeCommand GetType() const override
		{
			return ETypeCommand::ACTIVATION_TEXTURE;
		}
	};

} // namespace CoreEngine::Render
