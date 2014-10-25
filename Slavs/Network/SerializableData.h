#pragma once

#include "include/NetworkAPI.h"

#include "ISerializable.h"

#include <Common/Utilities/TemplateFunctions.h>

namespace Network
  {
  struct Packet;

  typedef std::unique_ptr<unsigned char[]> DataPtr;

  struct DataHolder
    {
    int       m_type;
    size_t    m_size;
    DataPtr   mp_data;


    NETWORKEXPORT DataHolder(DataHolder&& i_other);
    NETWORKEXPORT DataHolder(size_t i_size, unsigned char* ip_data);
    NETWORKEXPORT DataHolder(int i_type, size_t i_size, DataPtr ip_data);
    NETWORKEXPORT DataHolder(size_t i_initial_buffer_size);
    NETWORKEXPORT ~DataHolder();

    NETWORKEXPORT void ResizeIfNeeded (size_t i_new_size);
    };

  class SerializableData : public ISerializable
    {
    private:
      std::vector<DataHolder> m_data_holders;
      size_t                  m_current_index;
      
    public:
      NETWORKEXPORT SerializableData(size_t i_initial_holders_number = 0, size_t i_initial_buffer_size = sizeof(float));
      NETWORKEXPORT ~SerializableData();

      template <typename DataType>
      void    AddDataDefaultConverter(int i_type, DataType* ip_data, size_t i_size);

      template <typename DataType, typename Converter>
      void    AddData(int i_type, DataType* ip_data, size_t i_size);

      NETWORKEXPORT virtual void    Serialize(Packet& i_packet) const override;
      NETWORKEXPORT virtual void    Deserialize(const Packet& i_packet) override;

      template <typename DataType>
      DataType GetDataWithType(int i_type) const;
    };

  //////////////////////////////////////////////////////////////////////////

  template <typename DataType>
  struct DefaultConverter
    {
    void operator () (unsigned char* ip_buffer, DataType* ip_data)
      {
      ToChar<DataType>(*ip_data, ip_buffer, sizeof(DataType));
      }
    };

  template <typename DataType>
  void SerializableData::AddDataDefaultConverter(int i_type, DataType* ip_data, size_t i_size)
    {
    AddData<DataType, DefaultConverter<DataType>>(i_type, ip_data, i_size);
    }

  template <typename DataType, typename Converter>
  void SerializableData::AddData(int i_type, DataType* ip_data, size_t i_size)
    {
    if (m_current_index == m_data_holders.size())
      {
      throw std::exception("[TODO] Resize vector");
      //m_data_holders.resize(m_current_index+m_current_index);
      }


    DataHolder& data_holder = m_data_holders.at(m_current_index);
    ++m_current_index;

    data_holder.ResizeIfNeeded(i_size);
    data_holder.m_type = i_type;
    Converter converter;
    converter(&data_holder.mp_data[0], ip_data);

    m_needed_size += i_size + sizeof(int) + sizeof(size_t);
    }

  template <typename DataType>
  DataType SerializableData::GetDataWithType(int i_type) const
    {
    for (const DataHolder& holder : m_data_holders)
      if (holder.m_type == i_type)
        {
        DataType data;
        FromChar<DataType>(holder.mp_data.get(), data);
        return data;
        }
    return DataType();
    }

  } // Network