#pragma once

// back-end
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>
// functors
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/euml/common.hpp>
// for And_ operator
#include <boost/msm/front/euml/operator.hpp>
// for func_state and func_state_machine
#include <boost/msm/front/euml/state_grammar.hpp>

#include <Utilities/TemplateFunctions.h>

#include <Game/GameContext.h>

// forward declarations
namespace net
  {
  class Connection;
  }

class ServerMain;

//////////////////////////////////////////////////////////////////////////

namespace Slavs
  {  
  namespace LoadingStages
    {
    namespace msm = boost::msm;
    namespace mpl = boost::mpl;

    //////////////////////////////////////////////////////////////////////////
    // events

    struct ConfirmAchievement { };

    //////////////////////////////////////////////////////////////////////////

    struct loading_ : public msm::front::state_machine_def<loading_> 
      {
      net::Connection*                    mp_connection;
      ServerMain*                         mp_owner;
      std::unique_ptr<ObjectStateMap>     mp_objects_states;

      loading_(net::Connection* ip_connection, ServerMain* ip_owner, const GameContext& i_game_context)
        : mp_connection(ip_connection)
        , mp_owner(ip_owner)
        , mp_objects_states(new ObjectStateMap)
        {
        i_game_context.GetAllGameObjectsState(*mp_objects_states);
        }

      ~loading_()
        {
        ClearSTLMap(*mp_objects_states);
        }

      struct connecting_ : public msm::front::state<>
        {
        net::Connection*                    mp_connection;
        ServerMain*                         mp_owner;

        template <typename Event, typename FSM>
        void on_entry(const Event& i_evt, FSM& i_loading_fsm)
          {
          mp_connection  = i_loading_fsm.mp_connection;
          mp_owner       = i_loading_fsm.mp_owner;
          }
        };

      struct sending_data_ : public msm::front::state_machine_def<sending_data_>
        {
        net::Connection*                    mp_connection;
        ServerMain*                         mp_owner;
        ObjectStateMap*                     mp_objects_states;

        template <typename Event, typename FSM>
        void on_entry(const Event& i_evt, FSM& i_loading_fsm)
          {
          mp_owner            = i_loading_fsm.mp_owner;
          mp_connection       = i_loading_fsm.mp_connection;
          mp_objects_states   = i_loading_fsm.mp_objects_states.get();
          }

        struct send_definitions_ : public msm::front::state<> 
          {
          net::Connection*                     mp_connection;

          template <typename Event, typename FSM>
          void on_entry(const Event& i_evt, FSM& i_send_definitions_fsm)
            {
            mp_connection  = i_send_definitions_fsm.mp_connection;
            const MetaFactory::TDefinitionsMap& definitions = i_send_definitions_fsm.mp_owner->GetMetaFactory().GetDefinitions();

            std::unique_ptr<char[]> p_buffer(new char[256]);

            ToChar(PT_ContentNumber, &p_buffer[0], sizeof(PacketType));
            ToChar(definitions.size(), &p_buffer[sizeof(PacketType)], sizeof(size_t));
            mp_connection->SendPacket( &p_buffer[0], sizeof(PacketType) + sizeof(size_t) );
            int size = FromChar<int>(&p_buffer[4]);
            
            // send definitions
            std::for_each(definitions.begin(), definitions.end(), [&p_buffer, this](std::pair<std::string, int> i_definition)
              {
              size_t needed_size = sizeof(PacketType) + i_definition.first.size() + sizeof(int) + 1;
              ToChar(PT_Definitions, &p_buffer[0], sizeof(PacketType));
              i_definition.first.copy(&p_buffer[sizeof(PacketType)], i_definition.first.size());
              p_buffer[sizeof(PacketType)+i_definition.first.size()] = ';';
              ToChar(i_definition.second, &p_buffer[sizeof(PacketType)+i_definition.first.size()+1], sizeof(int));
              mp_connection->SendPacket(&p_buffer[0], needed_size);
              });
            }

          };

        struct send_objects_ : public msm::front::state<> 
          {
          net::Connection*                    mp_connection;
          ObjectStateMap*                     mp_objects_states;

          template <typename Event, typename FSM>
          void on_entry(const Event& i_evt, FSM& i_send_definitions_fsm)
            {
            mp_connection         = i_send_definitions_fsm.mp_connection;
            mp_objects_states     = i_send_definitions_fsm.mp_objects_states;

            std::unique_ptr<char[]> p_buffer(new char[256]);

            // send number of objects
            ToChar(PT_ContentNumber, &p_buffer[0], sizeof(PacketType));
            ToChar(mp_objects_states->size(), &p_buffer[sizeof(PacketType)], sizeof(size_t));
            mp_connection->SendPacket( &p_buffer[0], sizeof(PacketType) + sizeof(size_t) );

            // send objects states
            std::for_each(mp_objects_states->begin(), mp_objects_states->end(), [&p_buffer, this](std::pair<long, GameObjectState*> p)
              {
              int packet_size = p.second->NeededSize() + sizeof(PacketType);
              ToChar(PT_GOState, &p_buffer[0], sizeof(PacketType));
              p.second->Serialize(&p_buffer[sizeof(PacketType)], packet_size);
              mp_connection->SendPacket(&p_buffer[0], packet_size);
              });
            
            //after sending all states send msg about server ready state
            ToChar(PT_ServerReady, &p_buffer[0], sizeof(PacketType));
            mp_connection->SendPacket( &p_buffer[0], sizeof(PacketType) );
            }

          };

        // the initial state of sending_data FSM
        typedef send_definitions_ initial_state;

        // transition map
        struct transition_table : mpl::vector<
          //      Start                     Event                   Next                    Action               Guard
          //    +--------------------------+-----------------------+----------------------+---------------------+----------------------+
          _row < send_definitions_,         ConfirmAchievement,     send_objects_>
        >{};
        };
      // back-end
      typedef msm::back::state_machine<sending_data_> SendingData;

      struct wait_ : public msm::front::state<>
        {
        template <typename Event, typename FSM>
        void on_entry(const Event& i_evt, FSM& i_loading_fsm)
          {
          
          }
        };

      // the initial state of Loading FSM
      typedef connecting_ initial_state;

      // transition map
      struct transition_table : mpl::vector<
        //      Start                     Event                   Next                    Action               Guard
        //    +--------------------------+-----------------------+----------------------+---------------------+----------------------+
        _row < connecting_,               ConfirmAchievement,     SendingData>,
        _row < SendingData,               ConfirmAchievement,     wait_>,
        _row < wait_,                     ConfirmAchievement,     wait_>
      >{};
      };

    // back-end
    typedef msm::back::state_machine<loading_> LoadingFSM;

    } // LoadingStages
  } // Slavs