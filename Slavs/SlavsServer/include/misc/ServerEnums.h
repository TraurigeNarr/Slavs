#ifndef ServerEnums_h
#define ServerEnums_h

const long MS_FOR_TICK = 10;

enum message_type_server
{
	Msg_HiredWorker=100,
	Msg_NewRequestsAvailable,
	Msg_HumanHungry,
	Msg_ObtainProfession,
	Msg_HumanDied,
	Msg_HumanFired,
	Msg_HouseDeleted,
	Msg_WorkerPassResources,
	Msg_PeriodBegins,
	Msg_PeriodEnds,
	Msg_TickPerformed,
	Msg_Attack,
	Msg_UnitReady,
	Msg_PlaceBuilding,
	Msg_AllObjectsUpdated,
};

#endif