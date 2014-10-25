#pragma once

class Application;

template<typename T, typename Parameter>
class StateMachine;

template<typename T, typename Parameter>
class State;

//////////////////////////////////////////////////////////////////////////

typedef StateMachine<Application, long> ApplicationStateMachine;
typedef State<Application, long>        ApplicationState;