#include "SerializableData.h"

#include "Packet.h"

namespace Network
  {

  //////////////////////////////////////////////////////////////////////////
  // DataHolder

  DataHolder::DataHolder(DataHolder&& i_other)
    : m_type(i_other.m_type)
    , m_size(i_other.m_size)
    , mp_data(std::move(i_other.mp_data))
    {    }

  DataHolder::DataHolder(size_t i_size, unsigned char* ip_data)
    : m_type(-1)
    , m_size(i_size)
    , mp_data(ip_data)
    {    }

  DataHolder::DataHolder(int i_type, size_t i_size, DataPtr ip_data)
    : m_type(i_type)
    , m_size(i_size)
    , mp_data(std::move(ip_data))
    {    }

  DataHolder::DataHolder(size_t i_initial_buffer_size)
    : m_type(-1)
    , m_size(i_initial_buffer_size)
    , mp_data(new unsigned char[i_initial_buffer_size])
    {    }

  DataHolder::~DataHolder(){}

  void DataHolder::ResizeIfNeeded(size_t i_new_size)
    {
    if (i_new_size > m_size)
      mp_data.reset(new unsigned char[i_new_size]);
    m_size = i_new_size;
    }

  //////////////////////////////////////////////////////////////////////////
  // Serializable

  SerializableData::SerializableData (size_t i_initial_holders_number, size_t i_initial_buffer_size)
    : m_current_index(0)
    {
    m_needed_size = sizeof(size_t) + sizeof(size_t);
    if (i_initial_holders_number != 0 && i_initial_buffer_size != 0)
      {
      //DataPtr p_buffer(new unsigned char[i_initial_buffer_size*i_initial_holders_number]);

      for (size_t i = 0; i < i_initial_holders_number; ++i)
        {
        //const size_t offset = i*i_initial_buffer_size;
        DataHolder holder(i_initial_buffer_size, new unsigned char[i_initial_buffer_size]);
        m_data_holders.push_back(std::move(holder));
        }
      }
    }

  SerializableData::~SerializableData()
    {    }

  void SerializableData::Serialize(Packet& i_packet) const
    {
    char* p_data = static_cast<char*>(i_packet.GetPointerToCurrent());

    size_t offset = 0;
    offset += ToChar(&m_current_index, static_cast<void*>(p_data), sizeof(size_t));
    offset += ToChar(&m_needed_size, static_cast<void*>(p_data + offset), sizeof(size_t));

    for (const DataHolder& holder : m_data_holders)
      {
      if (holder.m_size == -1)
        continue;
      offset += ToChar(&holder.m_type, static_cast<void*>(p_data + offset), sizeof(int));
      offset += ToChar(&holder.m_size, static_cast<void*>(p_data + offset), sizeof(size_t));
      offset += ToChar(&holder.mp_data[0], static_cast<void*>(p_data + offset), holder.m_size);
      }
    i_packet.IncreaseOffset(offset);
    }

  void SerializableData::Deserialize(const Packet& i_packet)
    {
    const char* p_data = static_cast<const char*>(i_packet.GetPointerToCurrentConst());

    size_t offset = 0;
    offset += FromChar<size_t>(p_data, m_current_index);
    offset += FromChar<size_t>(p_data, m_needed_size);
    
    //DataPtr p_buffer(new unsigned char[m_needed_size]);
    //size_t offset_in_buffer = 0;
    for (size_t i = 0; i < m_current_index; ++i)
      {
      int type = -1;
      size_t size = 0;
      offset += FromChar<int>(p_data + offset, type);
      offset += FromChar<size_t>(p_data + offset, size);
      DataPtr p_buffer(new unsigned char[size]);
      offset += FromChar<unsigned char>(p_data + offset, *p_buffer.get());
      
      m_data_holders.push_back( std::move(DataHolder(type, size, std::move(p_buffer))) );
      //offset_in_buffer += size;
      }

    i_packet.IncreaseOffset(offset);
    }

  } // Network