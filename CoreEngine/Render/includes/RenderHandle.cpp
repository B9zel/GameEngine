#include "RenderHandle.h"

size_t CoreEngine::Render::RenderHandleHasher::operator()(const RenderHandle& Handle) const
{
	return std::hash<uint64>()(Handle.IdHandle);
}
