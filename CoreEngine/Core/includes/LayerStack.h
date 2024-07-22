#pragma once
#include <Core/includes/Base.h>
#include <Core/includes/Layer.h>


namespace CoreEngine
{
	class Layer;

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		bool RemoveLayer(Layer* layer);
		void RemoveAll();

		void NativeUpdateAll(float deltaTime);
		void OnAttachAll();
		void OnDetachAll();


		DArray<Layer*>::iterator begin() { return m_layerStack.begin(); }
		DArray<Layer*>::iterator end() { return m_layerStack.end(); }


	private:

		DArray<Layer*> m_layerStack;

	};
}