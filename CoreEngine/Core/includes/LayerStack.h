#pragma once
#include <Core/includes/Base.h>
#include <Core/includes/Layer.h>


namespace CoreEngine
{

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		bool RemoveLayer(Layer* layer);
		void RemoveAll();

		DArray<Layer*>::iterator begin() { return m_layerStack.begin(); }
		DArray<Layer*>::iterator end() { return m_layerStack.end(); }

	private:
		DArray<Layer*> m_layerStack;
	};
}