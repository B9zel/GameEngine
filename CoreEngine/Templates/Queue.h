#pragma once
#include <Core/includes/Dispatcher.h>
#include <Core/includes/Base.h>
#include <Core/includes/Log.h>



template<class T>
class Queue
{
public:

	Queue() = default;

	Queue(const Queue& Other);
	Queue(Queue&& Other);

	T& operator[](uint64 index);

public:

	const T& Front() const;
	const T& Back() const;

	bool IsEmpty() const;

	void PushBack(T& value);
	void PushBack(T&& value);

	void PushFront(T& value);
	void PushFront(T&& value);

	void PopBack();
	void PopFront();

	void Clear();

	uint64 Size() const;

public:

	CoreEngine::Dispatcher<T*> RemoveElement;
	CoreEngine::Dispatcher<T*> AddElement;

private:

	Deque<T> m_queue;
};


template<class T>
inline Queue<T>::Queue(const Queue& Other)
{
	m_queue = Other.m_queue;
}

template<class T>
inline Queue<T>::Queue(Queue&& Other)
{
	m_queue = std::move(Other);
}

template<class T>
inline T& Queue<T>::operator[](uint64 index)
{
#ifdef DEVELOPMENT_DEBUG

	DECLARE_LOG_CATEGORY_EXTERN(QUEUE_DATA_STRUCT)

	if (index >= m_queue.size())
	{
		EG_LOG(QUEUE_DATA_STRUCT, ELevelLog::WARNING, "Out of range");
	}

#endif // DEVELOPMENT_DEBUG

	return m_queue[index];
}

template<class T>
inline const T& Queue<T>::Front() const
{
	return m_queue.front();
}

template<class T>
inline const T& Queue<T>::Back() const
{
	return m_queue.back();
}

template<class T>
inline bool Queue<T>::IsEmpty() const
{
	return m_queue.empty();
}

template<class T>
inline void Queue<T>::PushBack(T& value)
{
	AddElement.Call(&value);
	m_queue.push_back(value);
}

template<class T>
inline void Queue<T>::PushBack(T&& value)
{
	AddElement.Call(&value);
	m_queue.push_back(std::move(value));
}

template<class T>
inline void Queue<T>::PushFront(T& value)
{
	AddElement.Call(value);
	m_queue.push_front(value);
}

template<class T>
inline void Queue<T>::PushFront(T&& value)
{
	AddElement.Call(value);
	m_queue.push_front(std::move(value));
}


template<class T>
inline void Queue<T>::PopBack()
{
	RemoveElement.Call(&m_queue.back());
	m_queue.pop_back();
}

template<class T>
inline void Queue<T>::PopFront()
{
	RemoveElement.Call(&m_queue.front());
	m_queue.pop_front();
}

template<class T>
inline void Queue<T>::Clear()
{
	m_queue.clear();
}

template<class T>
inline uint64 Queue<T>::Size() const
{
	return m_queue.size();
}

