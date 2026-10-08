#pragma once

#include <Render/includes/Enums/TypeData.h>
#include <Render/includes/Enums/TypeDraw.h>
#include <Core/includes/Platform.h>

namespace CoreEngine
{
	int32 GetAPITypeFromEnum(const ETypeData type);

	short GetSizeOfFromEnum(const ETypeData type);

	namespace Render
	{
		int32 GetDrawTypeAPIFromEnum(const ETypeStorageDraw draw);
	}
} // namespace CoreEngine
