#pragma once


namespace CoreEngine
{
	class Layer
	{
	public:
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void Update(float deltaTime) = 0;
	};
}