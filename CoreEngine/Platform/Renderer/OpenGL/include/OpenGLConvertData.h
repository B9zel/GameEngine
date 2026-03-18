#pragma once

#include <Render/includes/Enums/TypeData.h>
#include <Render/includes/Enums/TypeDraw.h>
#include <Core/includes/Log.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace CoreEngine
{
	static int32 GetAPITypeFromEnum(const ETypeData type)
	{
		switch (type)
		{
		case ETypeData::FLOAT: return GL_FLOAT;
		case ETypeData::INT: return GL_INT;
		case ETypeData::UNSIGNED_INT: return GL_UNSIGNED_INT;
		}
		EG_LOG(CORE, ELevelLog::ERROR, "Don't implementation type data");
		return 0;
	}

	static short GetSizeOfFromEnum(const ETypeData type)
	{
		switch (type)
		{
		case ETypeData::FLOAT: return sizeof(float);
		case ETypeData::INT: return sizeof(int);
		case ETypeData::UNSIGNED_INT: return sizeof(unsigned int);
		default:
			EG_LOG(CORE, ELevelLog::ERROR, "Don't implementation type: {0}", static_cast<short>(type));
			return 0;
		}
	}
	namespace Render
	{
		static int32 GetDrawTypeAPIFromEnum(const ETypeStorageDraw draw)
		{
			switch (draw)
			{
			case ETypeStorageDraw::STATIC:
				return GL_STATIC_DRAW;
			case ETypeStorageDraw::DYNAMIC:
				return GL_DYNAMIC_DRAW;
			case ETypeStorageDraw::STREAM:
				return GL_STREAM_DRAW;
			}
			EG_LOG(CORE, ELevelLog::ERROR, "Not type draw");
			return 0;
		}
	}
}