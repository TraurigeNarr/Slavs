#pragma once

namespace Network
  {
  struct  Packet;
  class   SerializableData;

  class ISerializable
    {
    protected:
      size_t m_needed_size;

    public:
      ISerializable()
        : m_needed_size(0)
        {        }
      virtual ~ISerializable(){}

    size_t GetSize() const
      {
      return m_needed_size;
      }

    virtual void Serialize(Packet& i_packet) const = 0;
    virtual void Deserialize(const Packet& i_packet) = 0;
    };
  } // Network