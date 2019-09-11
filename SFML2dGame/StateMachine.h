#pragma once
#include <memory>
#include <stack>
#include "State.h"

typedef std::unique_ptr<State> StateRef;
class StateMachine
{
public:

	void AddState(StateRef newState, bool isReplacing);
	void RemoveState();

	void ProcessStateChanges();
	StateRef& GetCurrentState();

private:
	std::stack<StateRef> _states;
	StateRef _newState;

	bool _isRemoving;
	bool _isAdding;
	bool _isReplacing;
};

