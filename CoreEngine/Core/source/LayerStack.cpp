#include <Core/includes/LayerStack.h>


namespace CoreEngine
{
	LayerStack::~LayerStack()
	{
		RemoveAll();
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_layerStack.emplace_back(layer);
	}

	bool LayerStack::RemoveLayer(Layer* layer)
	{
		auto it = std::find(m_layerStack.begin(), m_layerStack.end(), layer);
		if (it != m_layerStack.end())
		{
			layer->OnDetach();
			m_layerStack.erase(it);
			return true;
		}
		return false;
	}

	void LayerStack::RemoveAll()
	{
		for (auto* el : m_layerStack)
		{
			el->OnDetach();
			delete el;
		}
	}

}