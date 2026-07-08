#pragma once
#include <Core/includes/Platform.h>
#include <Core/includes/Log.h>

namespace CoreEngine::Render
{
	DECLARE_LOG_CATEGORY_EXTERN(RenderHandleLog)

	

	struct RenderHandle
	{
	public:

		using DataTypeID = uint64;

	public:

		bool IsValid() const
		{
			return IdHandle != 0;
		}
		DataTypeID GetId() const
		{
			return IdHandle;
		}
		void SetId(const DataTypeID NewId)
		{
			if (IsValid())
			{
				EG_LOG(RenderHandleLog, ELevelLog::WARNING, "Handle is valid, before call Invalide");
				return;
			}
			IdHandle = NewId;
		}

		void Invalide()
		{
			IdHandle = 0;
		}

	public:

		bool operator==(const RenderHandle& Other) const noexcept
		{
			return IdHandle == Other.IdHandle;
		}

	public:

		DataTypeID IdHandle = 0;

		static const uint64 StartId = 1;
	};

	struct RenderHandleHasher
	{
		size_t operator()(const RenderHandle& Handle) const;
	};

} // namespace CoreEngine::Render