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
      std::unique_ptr<ObjectStateMap>     mp_objects;

      loading_(net::Connection* ip_connection, ServerMain* ip_owner, const GameContext& i_game_context)
        : mp_connection(ip_connection)
        , mp_owner(ip_owner)
        , mp_objects(new ObjectStateMap)
        {
        i_game_context.GetAllGameObjectsState(*mp_objects);
        }

      ~loading_()
        {
        ClearSTLMap(*mp_objects);
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
        ObjectStateMap*                     mp_objects;

        template <typename Event, typename FSM>
        void on_entry(const Event& i_evt, FSM& i_loading_fsm)
          {
          mp_owner            = i_loading_fsm.mp_owner;
          mp_connection       = i_loading_fsm.mp_connection;
          mp_objects          = i_loading_fsm.mp_objects.get();
          }

        struct send_definitions_ : public msm::front::state<> 
          {
          net::Connection*                     mp_connection;

          template <typename Event, typename FSM>
          void on_entry(const Event& i_evt, FSM& i_send_definitions_fsm)
            {
            mp_connection  = i_send_definitions_fsm.mp_connection;
            const MetaFactory::TDefinitionsMap& definitions = i_send_definitions_fsm.mp_owner->GetMetaFactory().GetDefinitions();
            // send
            std::unique_ptr<char[]> h_buffer(new char[256]);
            // send definitions
            std::for_each(definitions.begin(), definitions.end(), [&h_buffer, this](std::pair<std::string, int> i_definition)
              {
              size_t needed_size = sizeof(PacketType) + i_definition.first.size() + sizeof(int) + 1;
              ToChar(PT_Definitions, &h_buffer[0], sizeof(PacketType));
              i_definition.first.copy(&h_buffer[sizeof(PacketType)], i_definition.first.size());
              h_buffer[sizeof(PacketType)+i_definition.first.size()] = ';';
              ToChar(i_definition.second, &h_buffer[sizeof(PacketType)+i_definition.first.size()+1], sizeof(int));
              mp_connection->SendPacket(&h_buffer[0], needed_size);
              });
            }

          };

        struct send_objects_ : public msm::front::state<> 
          {
          net::Connection*                    mp_connection;
          ObjectStateMap*                     mp_objects;

          template <typename Event, typename FSM>
          void on_entry(const Event& i_evt, FSM& i_send_definitions_fsm)
            {
            mp_connection  = i_send_definitions_fsm.mp_connection;
            mp_objects     = i_send_definitions_fsm.mp_objects;
            // send
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
        void on_entry(const Event& i_evt, FSM& i_send_definitions_fsm)
          {
          printf ("asd");
          }
        };

      // the initial state of Loading FSM
      typedef connecting_ initial_state;

      // transition map
      struct transition_table : mpl::vector<
        //      Start                     Event                   Next                    Action               Guard
        //    +--------------------------+-----------------------+----------------------+---------------------+----------------------+
        _row < connecting_,               ConfirmAchievement,     SendingData>,
        _row < SendingData,               ConfirmAchievement,     wait_>
      >{};
      };

    // back-end
    typedef msm::back::state_machine<loading_> LoadingFSM;

    } // LoadingStages
  } // Slavs