#pragma once
#include <Runtime/includes/ActorComponent.h>
#include <Core/includes/Dispatcher.h>
#include <Core/includes/Base.h>
#include <Events/include/Event.h>
#include <InputComponent.generated.h>

class PlayerController;
class Actor;

struct BindInputActionInfo
{
	struct
	{
		void (Actor::*Method)();
		Actor* ClassOfMethod;

		void Execute()
		{
			(ClassOfMethod->*Method)();
		}

	} Callback;
	CoreEngine::EActionType Action = CoreEngine::EActionType::RELEASED;
};

struct BindInputAxisInfo
{
	struct
	{
		void (Actor::*Method)(float);
		Actor* ClassOfMethod;

		void Execute(float Value)
		{
			(ClassOfMethod->*Method)(Value);
		}

	} Callback;
	float ValueInput;
	int32 Key;
	bool isHold = false;
	CoreEngine::EActionType Action = CoreEngine::EActionType::RELEASED;
};

struct BindInputMouseMoveInfo
{
	struct
	{
		void (Actor::*Method)(double, double);
		Actor* ClassOfMethod;

		void Execute(double ValueX, double ValueY)
		{
			(ClassOfMethod->*Method)(ValueX, ValueY);
		}

	} Callback;
};
RCLASS()
class InputComponent : public ActorComponent
{

	GENERATED_BODY()

public:

	InputComponent(const CoreEngine::InitializeObject& Object);

	template <class TClass> void BindAction(const uint32 Key, const CoreEngine::EActionType& Action, void (TClass::*Method)(), TClass* ClassOfMethod);
	template <class TClass> void BindAxis(const uint32 Key, const float InputValue, void (TClass::*Method)(float), TClass* ClassOfMethod);
	template <class TClass> void BindMouseMotionAxis(void (TClass::*Method)(double, double), TClass* ClassOfMethod);

	// Event that send Controller
	void ExecuteEvent(CoreEngine::Event& event);
	void ExecuteAxis();

	const HashTableMap<int, DArray<BindInputActionInfo>>& GetActionBinds() const;
	const HashTableMap<int, BindInputAxisInfo>& GetAxisBinds() const;

	virtual bool ProcessUpdateAxis(CoreEngine::Event* event);

protected:

	virtual void UpdateComponent(float deltaTime) override
	{
	}

	virtual bool ProcessUpdateAction(CoreEngine::Event* event);

private:

	bool ExecuteKeyCallback(const int32 Key, const CoreEngine::EActionType& Action);
	bool ChangeAxisAction(const int32 Key, const CoreEngine::EActionType& NewAction);

public:

	Dispatcher<CoreEngine::Event&> TakeProcessedEvent;
	Dispatcher<CoreEngine::Event&> TakeEvent;

private:

	HashTableMap<int, DArray<BindInputActionInfo>> m_CallbackInputAction;
	HashTableMap<int, BindInputAxisInfo> m_CallbackInputAxis;
	DArray<BindInputMouseMoveInfo> m_CallbackInputMotion;

	int32 PressedKey;

	friend PlayerController;
};

template <class TClass>
inline void InputComponent::BindAction(const uint32 Key, const CoreEngine::EActionType& Action, void (TClass::*Method)(), TClass* ClassOfMethod)
{
	BindInputActionInfo ActionBind;
	ActionBind.Action = Action;
	ActionBind.Callback.ClassOfMethod = ClassOfMethod;
	ActionBind.Callback.Method = static_cast<void (Actor::*)()>(Method);

	m_CallbackInputAction[Key].push_back(ActionBind);
}

template <class TClass> inline void InputComponent::BindAxis(const uint32 Key, const float InputValue, void (TClass::*Method)(float), TClass* ClassOfMethod)
{
	BindInputAxisInfo ActionBind;
	ActionBind.ValueInput = InputValue;
	ActionBind.Key = Key;
	ActionBind.Callback.ClassOfMethod = ClassOfMethod;
	ActionBind.Callback.Method = static_cast<void (Actor::*)(float)>(Method);

	m_CallbackInputAxis[Key] = ActionBind;
}

template <class TClass> inline void InputComponent::BindMouseMotionAxis(void (TClass::*Method)(double, double), TClass* ClassOfMethod)
{
	BindInputMouseMoveInfo NewMotion;
	NewMotion.Callback.ClassOfMethod = ClassOfMethod;
	NewMotion.Callback.Method = static_cast<void (Actor::*)(double, double)>(Method);

	m_CallbackInputMotion.push_back(NewMotion);
}
