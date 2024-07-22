#include <Runtime/CoreObject/Include/Object.h>


namespace CoreEngine
{
	namespace Runtime
	{
		void Object::NativeUpdate(float deltaTime)
		{
			Tick(deltaTime);
		}

		void Object::OnAttach()
		{
			BeginPlay();
		}

		void Object::OnDetach()
		{
			EndPlay();
		}
	}
}