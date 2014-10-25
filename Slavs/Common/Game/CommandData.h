#ifndef CommandData_h
#define CommandData_h

#define ClrBit(Bit16,BitNumber) (Bit16 = (Bit16 | (1<<BitNumber)) - (1<<BitNumber))

#include "Enumerations.h"

/*
-------------------------------------------------------------------
Serializable class. The order of serialization:
0. Command id
1. Command parameters
2. Extra Data
-------------------------------------------------------------------
*/

class CommandData
{
public:
	CommandData(CommandID command = Cmd_None, CommandDataParameters cdParams = 0);
	~CommandData();

	void					AddParameter(int parameter);
	void					SetExtraData(void* extraData);

	void*					GetExtraData() const;
	CommandDataParameters	GetParameters() const;
	CommandID				GetCommand() const;

	int						Serialize(char* buf_end, int size) const;
	int						Deserialize(char* buf_end);

	int						NeededSize() const;
	inline bool				HasParameter(int parameter) const;
	void					ClearExtraData();
protected:
	CommandID				m_CmdID;
	CommandDataParameters	m_Parameters;
	void*					m_ExtraData;
	//has mutable because it`s meaning changed in NeededSize
	mutable int				m_iNeededSize;
};

#endif