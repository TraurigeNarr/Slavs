#include "CommandData.h"
#include "../Utilities/TemplateFunctions.h"

#include "../Math/Vector2D.h"

CommandData::CommandData(CommandID command /* = Cmd_None */, CommandDataParameters cdParams /* = 0 */)
	: m_CmdID(command), m_Parameters(cdParams), m_ExtraData(NULL), m_iNeededSize(0)
{}

CommandData::~CommandData()
{
	if(NULL != m_ExtraData)
		delete m_ExtraData;
	m_ExtraData = NULL;
}

void CommandData::AddParameter(int parameter)
{
	m_Parameters |= parameter;
}

void CommandData::SetExtraData(void* extraData)
{
	m_ExtraData = extraData;
}

void* CommandData::GetExtraData() const
{
	return m_ExtraData;
}

CommandDataParameters CommandData::GetParameters() const
{
	return m_Parameters;
}

CommandID CommandData::GetCommand() const
{
	return m_CmdID;
}

int CommandData::Serialize(char* buf_end, int size) const
{
	if(0 == m_iNeededSize)
		m_iNeededSize = NeededSize();
	//check if there is enough buffer size
	if(m_iNeededSize > size || NULL == buf_end)
		return 0;
	//serialize command
	ToChar(m_CmdID, buf_end, sizeof(CommandID));
	buf_end += sizeof(CommandID);
	ToChar(m_Parameters, buf_end, sizeof(CommandDataParameters));
	buf_end += sizeof(CommandDataParameters);
	if(HasParameter(CDParams_NeedPosition))
	{
		static_cast<Vector2D*>(m_ExtraData)->Serialize(buf_end);
	}
	if(HasParameter(CDParams_NeedTarget))
	{

	}
	return m_iNeededSize;
}

int CommandData::Deserialize(char* buf_end)
{
	int deserializeCount = 0;
	
	if(NULL == buf_end)
		return 0;

	//deserialize command id
	m_CmdID = (CommandID)FromChar<int>(buf_end);
	deserializeCount += sizeof(CommandID);
	//deserialize parameters
	m_Parameters = FromChar<CommandDataParameters>(buf_end + deserializeCount);
	deserializeCount += sizeof(CommandDataParameters);
	//deserialize extra data
	if(HasParameter(CDParams_NeedPosition))
	{
		Vector2D *position = new Vector2D();
		position->Deserialize(buf_end + deserializeCount);
		m_ExtraData = position;
		deserializeCount += Vector2D::NeededSize();
	}
	return deserializeCount;
}

int CommandData::NeededSize() const
{
	m_iNeededSize = sizeof(CommandID) + sizeof(CommandDataParameters);
	if(HasParameter(CDParams_NeedPosition))
		m_iNeededSize += Vector2D::NeededSize();
	return m_iNeededSize;
}

bool CommandData::HasParameter(int parameter) const
{
	return 0 != (m_Parameters & parameter);
}

void CommandData::ClearExtraData()
{
	if(NULL != m_ExtraData)
	{
		delete m_ExtraData;
		m_ExtraData = NULL;
	}
}