#pragma once
#include <Core/includes/LayerStack.h>


namespace CoreEngine
{
	class Layer
	{
	protected:

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void NativeUpdate(float deltaTime) = 0;

	private:

		friend class LayerStack;
	};
}