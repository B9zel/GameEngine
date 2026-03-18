#pragma once
#include <Core/includes/Platform.h>
#include <Core/includes/Log.h>

namespace CoreEngine::Render::RHI
{
	DECLARE_LOG_CATEGORY_EXTERN(RenderHandleLog)

	struct Handle
	{
	public:

		using DataTypeID = uint64;

	public:

		bool IsValid() const { return IdHandle != 0; }
		DataTypeID GetId() const { return IdHandle; }
		void SetId(const DataTypeID NewId)
		{
			if (IsValid())
			{
				EG_LOG(RenderHandleLog, ELevelLog::WARNING, "Handle is valid, before call Invalide");
				return;
			}
			IdHandle = NewId;
		}

		void Invalide() { IdHandle = 0; }

	public:

		DataTypeID IdHandle = 0;
	};


	struct BufferHandle : public Handle
	{
	
	};

	struct TextureHandle : public Handle
	{
	};

	struct ShaderHandle : public Handle
	{
	};

	struct HandleVAO : public Handle
	{
	};

}