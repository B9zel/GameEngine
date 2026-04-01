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
			DRAW_INDEX,
			UNIFORM_1I,
			UNIFORM_MATRIX_4X4,
			UNIFORM_VECTOR3
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

		using PredParams = void (*)(const RHI::BufferHandle&);
		// Call after Execute
		FunctionPtr<void(const RHI::BufferHandle&)> Predicate;

	public:

		CmdCreateVertexBuffer(const DArray<float>& VertexData, PredParams Pred) : Data(VertexData), Predicate(Pred)
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

		using PredParams = void (*)(const RHI::BufferHandle&);
		// Call after Execute
		FunctionPtr<void(const RHI::BufferHandle&)> Predicate;

	public:

		CmdCreateIndexBuffer(const DArray<uint32>& IndexData, PredParams Pred) : Data(IndexData), Predicate(Pred)
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

		using PredParams = void (*)(const RHI::HandleVAO&);
		// Call after Execute
		FunctionPtr<void(const RHI::HandleVAO&)> Predicate;

	public:

		CmdCreateVertexArrayObject(const RHI::BufferHandle& VBO, const RHI::BufferHandle& IBO,
								   const DArray<Turple<uint32, uint32, uint32, ETypeData, uint32>>& Attribs, PredParams Pred)
			: VBO(VBO), IBO(IBO), Attributes(Attribs), Predicate(Pred)
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
		// Out
		RHI::TextureHandle OutHandle;
		//

		using PredParams = void (*)(const RHI::TextureHandle&);
		// Call after Execute
		FunctionPtr<void(const RHI::TextureHandle&)> Predicate;

	public:

		CmdCreateTexture2D(const uint32 width, const uint32 height, const ETypeChannel channel, const DArray<uint32>& Indexes, PredParams Pred)
			: Width(width), Height(height), Channel(channel), Data(Indexes), Predicate(Pred)
		{
		}

		CmdCreateTexture2D(const StringView& Path, PredParams Pred) : Path(Path), Predicate(Pred)
		{
		}

		virtual void Execute(RenderDevice* Devise) override
		{

			/*if (!Path.empty())
			{
				OutHandle = Devise->CreateTexture2D(Path.data());
				return;
			}
			OutHandle = Devise->CreateTexture2D(Width, Height, Channel, Data.data());*/
		}
	};

	struct CmdCreateShaderProgram : public RenderCommand
	{
		String VertexShader, FragmentShader;
		RHI::ShaderHandle OutHandle;

		using PredParams = void (*)(const RHI::ShaderHandle&);
		// Call after Execute
		FunctionPtr<void(const RHI::ShaderHandle&)> Predicate;

	public:

		CmdCreateShaderProgram(const StringView VertexShad, const StringView FragmendShad, PredParams Pred)
			: VertexShader(VertexShad), FragmentShader(FragmendShad), Predicate(Pred)
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

		using PredParams = void (*)();
		// Call after Execute
		FunctionPtr<void()> Predicate;

	public:

		CmdBindShaderProgram(const RHI::ShaderHandle& ShadHandle, PredParams Pred) : Handle(ShadHandle), Predicate(Pred)
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
		// Key: layout, Value: Id in render device and name of texture in shader
		HashTableMap<uint32, Pair<RHI::TextureHandle, String>> Textures;
		RHI::ShaderHandle ShaderHandle;
	};

	struct CmdActivationTexture : public RenderCommand
	{
		UniquePtr<ActivateTextureSet> ActivationSet;

		using PredParams = void (*)();
		// Call after Execute
		FunctionPtr<void()> Predicate;

	public:

		CmdActivationTexture(UniquePtr<ActivateTextureSet> Activations, PredParams Pred) : ActivationSet(std::move(Activations)), Predicate(Pred)
		{
		}

		virtual void Execute(RenderDevice* Devise) override
		{
			int32 i = 0;
			for (auto& DataActivate : ActivationSet->Textures)
			{
				Devise->BindTexture(DataActivate.second.first, DataActivate.first);
				Devise->SetUniform1i(ActivationSet->ShaderHandle, DataActivate.second.second, i++);
			}
		}

		virtual ETypeCommand GetType() const override
		{
			return ETypeCommand::ACTIVATION_TEXTURE;
		}
	};

} // namespace CoreEngine::Render
