#pragma once
#include <Core/includes/Base.h>



namespace CoreEngine
{

	class Timer;
	class TimerManager;
	struct TimerHandle;


	struct TimerHandle
	{
	public:

		friend TimerManager;

		bool IsValid() { return m_Hadle != 0; }

	private:

		unsigned long long m_Hadle = 0;

	};

	class Timer
	{
	public:

		float GetFullTime() { return m_fullTime; }
		float GetTime() { return m_currentTime; }

	private:

		bool m_isPause = false;
		bool m_isActive = false;
		bool m_isLoop = false;

		float m_fullTime = 0;
		float m_currentTime = 0;

		Function<void()> m_callback;

		friend TimerManager;
	};

	class TimerManager
	{
	public:

		template<class TClass>
		using MethodPointer = void(TClass::*)();
		//using FunctionType = MethodPtr<TClass, void()>;

	public:

		void Update(float deltaTime);

		template<class TObject>
		TimerHandle& SetTimer(TimerHandle& handler, TObject* object, MethodPointer<TObject> method, float InRate, bool isLoop);

		void RemoveTimer(TimerHandle& handler);
		void PouseTimer(TimerHandle& handler);
		void PlayTimer(TimerHandle& handler);

	private:

		long long GetIDTimer() { return ++timerID; }

	private:

		HashTableMap<long long, Timer> m_timers;
		HashTableMap<long long, Timer> m_activeTimers;

		static unsigned long long timerID;
	};

	template<class TObject>
	TimerHandle& TimerManager::SetTimer(TimerHandle& handler, TObject* object, MethodPointer<TObject> method, float InRate, bool isLoop)
	{
		if (InRate > 0.f)
		{
			Timer timer;
			timer.m_callback.Assign(method, object);
			timer.m_fullTime = InRate;
			timer.m_isLoop = isLoop;
			timer.m_isActive = true;

			long long id = GetIDTimer();
			handler.m_Hadle = id;

			m_activeTimers[id] = std::move(timer);
		}

		return handler;
	}

}