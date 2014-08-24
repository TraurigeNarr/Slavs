#pragma once

#include <Common/Patterns/State.h>

namespace Network
  {
  class PacketProvicer;
  struct Packet;
  }

namespace ClientStates
  {
  class LoadingState;

  namespace LoadingStages
    {

    enum class LoadingResult
      {
      LR_SUCCEEDED,
      LR_FAILED
      };

    class ConnectionState : public State<LoadingState, long>
      {
      private:
        Network::PacketProvicer& m_packet_provider;

      public:
        ConnectionState (Network::PacketProvicer& i_packet_provider);

        virtual void Enter(LoadingState* ip_owner) override;
        virtual void Execute(LoadingState* ip_owner, long i_parameter) override;
        virtual void Exit(LoadingState* ip_owner) override;
      };

    class ResultsState : public State<LoadingState, long>
      {
      private:
        const LoadingResult m_result;
      public:
        ResultsState (LoadingResult i_result);

        virtual void Enter(LoadingState* ip_owner) override;
        virtual void Execute(LoadingState* ip_owner, long i_parameter) override;
        virtual void Exit(LoadingState* ip_owner) override;

        LoadingResult GetResult() const;
      };

    class GettingData : public State<LoadingState, long>
      {
      public:
        enum class CurrentData
          {
          CD_DEFINITIONS,
          CD_OBJECTS,
          CD_ALL_LOADED,
          CD_NOTHING
          };

      private:
        Network::PacketProvicer&  m_packet_provider;
        CurrentData               m_current_data_type;

        size_t                    m_content_to_load_number;
        size_t                    m_current_content_number;

      private:
        void _HandleDefinition(const Network::Packet& i_packet);
        void _HandleObject(const Network::Packet& i_packet);
        void _SendReadyPacket() const;

      public:
        GettingData (Network::PacketProvicer& i_packet_provider);

        virtual void Enter(LoadingState* ip_owner) override;
        virtual void Execute(LoadingState* ip_owner, long i_parameter) override;
        virtual void Exit(LoadingState* ip_owner) override;

        size_t       GetLoadedContentNumber() const;
        size_t       GetTotalContentNumber() const;
        CurrentData  GetStage() const;
      };

    } // LoadingStages

  } // ClientStates