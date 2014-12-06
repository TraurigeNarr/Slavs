#pragma once

#include "LoadingParameters.h"
#include "IO/GameSerializer.h"

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
class IController;
class SPlayerController;
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
    struct LoadingGameCompleted { };

    //////////////////////////////////////////////////////////////////////////

    struct loading_ : public msm::front::state_machine_def<loading_> 
      {
      net::Connection*                              mp_connection;
      ServerMain*                                   mp_owner;
      std::unique_ptr<ObjectStateMap>               mp_objects_states;
      GameContext&                                  m_game_context;
      LoadingParameters                             m_loading_parameters;
			IController*																	mp_controller_to_send;

      loading_(GameContext& io_game_context, 
                const LoadingParameters& i_loading_parameters,
                net::Connection* ip_connection, 
                ServerMain* ip_owner)
        : mp_connection(ip_connection)
        , mp_owner(ip_owner)
        , mp_objects_states(new ObjectStateMap)
        , m_game_context(io_game_context)
        , m_loading_parameters(i_loading_parameters)
				, mp_controller_to_send(nullptr)
        {
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

      struct loading_data_stage_ : public msm::front::state_machine_def<loading_data_stage_>
        {
        template <typename Event, typename FSM>
        void on_entry(const Event& i_evt, FSM& i_loading_fsm)
          {
          // register controller for player
          i_loading_fsm.m_game_context.RegisterController( std::unique_ptr<IController>
            (
            new SPlayerController(i_loading_fsm.mp_connection->GetAddress().GetAddress(), i_loading_fsm.m_game_context)
            ));
					i_loading_fsm.mp_controller_to_send = i_loading_fsm.m_game_context.GetControllers().begin()->mp_controller.get();
          // load game
          GameSerializer serializer(i_loading_fsm.m_game_context);
          serializer.LoadConfigurations(i_loading_fsm.m_loading_parameters.m_configurations_path);
          serializer.LoadGame(i_loading_fsm.m_loading_parameters.m_map_path);

          // initialize objects state
          i_loading_fsm.m_game_context.GetAllGameObjectsState(*i_loading_fsm.mp_objects_states);

          // process event
          i_loading_fsm.process_event(LoadingGameCompleted());
          }
        };

      struct sending_data_ : public msm::front::state_machine_def<sending_data_>
        {
        net::Connection*                    mp_connection;
        ServerMain*                         mp_owner;
        ObjectStateMap*                     mp_objects_states;
				IController*												mp_controller_to_send;

        template <typename Event, typename FSM>
        void on_entry(const Event& i_evt, FSM& i_loading_fsm)
          {
          mp_owner            = i_loading_fsm.mp_owner;
          mp_connection       = i_loading_fsm.mp_connection;
          mp_objects_states   = i_loading_fsm.mp_objects_states.get();
					mp_controller_to_send = i_loading_fsm.mp_controller_to_send;
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

            ToChar(Network::PacketType::PT_ContentNumber, &p_buffer[0], sizeof(Network::PacketType));
            ToChar(definitions.size(), &p_buffer[sizeof(Network::PacketType)], sizeof(size_t));
            mp_connection->SendPacket( &p_buffer[0], sizeof(Network::PacketType) + sizeof(size_t) );
            // send definitions
            std::for_each(definitions.begin(), definitions.end(), [&p_buffer, this](std::pair<std::string, int> i_definition)
              {
              size_t needed_size = sizeof(Network::PacketType) + i_definition.first.size() + sizeof(int) + 1;
              ToChar(Network::PacketType::PT_Definitions, &p_buffer[0], sizeof(Network::PacketType));
              i_definition.first.copy(&p_buffer[sizeof(Network::PacketType)], i_definition.first.size());
              p_buffer[sizeof(Network::PacketType)+i_definition.first.size()] = ';';
              ToChar(i_definition.second, &p_buffer[sizeof(Network::PacketType)+i_definition.first.size()+1], sizeof(int));
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
            ToChar(Network::PacketType::PT_ContentNumber, &p_buffer[0], sizeof(Network::PacketType));
            ToChar(mp_objects_states->size(), &p_buffer[sizeof(Network::PacketType)], sizeof(size_t));
            mp_connection->SendPacket( &p_buffer[0], sizeof(Network::PacketType) + sizeof(size_t) );

            // send objects states
            std::for_each(mp_objects_states->begin(), mp_objects_states->end(), [&p_buffer, this](std::pair<long, GameObjectState*> p)
              {
              int packet_size = p.second->NeededSize() + sizeof(Network::PacketType);
              ToChar(Network::PacketType::PT_GOState, &p_buffer[0], sizeof(Network::PacketType));
              p.second->Serialize(&p_buffer[sizeof(Network::PacketType)], packet_size);
              mp_connection->SendPacket(&p_buffer[0], packet_size);
              });
            }

          };

				struct send_categories_ : public msm::front::state<>
					{
					net::Connection*                    mp_connection;

					template <typename Event, typename FSM>
					void on_entry(const Event& i_evt, FSM& i_send_definitions_fsm)
						{
						mp_connection = i_send_definitions_fsm.mp_connection;
						std::vector<std::string> categories = i_send_definitions_fsm.mp_owner->GetMetaFactory().GetCommandManager().GetCommandCategories();

						std::unique_ptr<char[]> p_buffer(new char[256]);
						// send number of objects
						ToChar(Network::PacketType::PT_ContentNumber, &p_buffer[0], sizeof(Network::PacketType));
						ToChar(categories.size(), &p_buffer[sizeof(Network::PacketType)], sizeof(size_t));
						mp_connection->SendPacket(&p_buffer[0], sizeof(Network::PacketType) + sizeof(size_t));

						for (auto& category : categories)
							{
							const size_t needed_size = sizeof(Network::PacketType) + category.size() + 1;
							ToChar(Network::PacketType::PT_CategoryDefinition, &p_buffer[0], sizeof(Network::PacketType));
							category.copy(&p_buffer[sizeof(Network::PacketType)], category.size());
							p_buffer[sizeof(Network::PacketType) + category.size()] = '\0';
							mp_connection->SendPacket(&p_buffer[0], needed_size);
							}
						}

					};

				struct send_commands_ : public msm::front::state<>
					{
					net::Connection*                    mp_connection;

					template <typename Event, typename FSM>
					void on_entry(const Event& i_evt, FSM& i_send_definitions_fsm)
						{
						mp_connection = i_send_definitions_fsm.mp_connection;
						IController* p_controller = i_send_definitions_fsm.mp_controller_to_send;
						std::vector<std::string> commands = i_send_definitions_fsm.mp_owner->GetMetaFactory().GetCommandManager().GetCommandStrings(p_controller);

						std::unique_ptr<char[]> p_buffer(new char[256]);
						// send number of objects
						ToChar(Network::PacketType::PT_ContentNumber, &p_buffer[0], sizeof(Network::PacketType));
						ToChar(commands.size(), &p_buffer[sizeof(Network::PacketType)], sizeof(size_t));
						mp_connection->SendPacket(&p_buffer[0], sizeof(Network::PacketType) + sizeof(size_t));

						for (auto& command : commands)
							{
							const size_t needed_size = sizeof(Network::PacketType) + command.size() + 1;
							ToChar(Network::PacketType::PT_CommandDefinition, &p_buffer[0], sizeof(Network::PacketType));
							command.copy(&p_buffer[sizeof(Network::PacketType)], command.size());
							p_buffer[sizeof(Network::PacketType) + command.size()] = '\0';
							mp_connection->SendPacket(&p_buffer[0], needed_size);
							}
						}

					};


        // the initial state of sending_data FSM
        typedef send_definitions_ initial_state;

        // transition map
        struct transition_table : mpl::vector<
          //      Start                     Event                   Next                    Action               Guard
          //    +--------------------------+-----------------------+----------------------+---------------------+----------------------+
          _row < send_definitions_,         ConfirmAchievement,     send_objects_>,
					_row < send_objects_,							ConfirmAchievement,			send_categories_>,
					_row < send_categories_,					ConfirmAchievement,			send_commands_>
        >{};
        };
      // back-end
      typedef msm::back::state_machine<sending_data_> SendingData;

      struct wait_ : public msm::front::state<>
        {
        template <typename Event, typename FSM>
        void on_entry(const Event& i_evt, FSM& i_loading_fsm)
          {
					net::Connection* p_connection = i_loading_fsm.mp_connection;
					char buffer[sizeof(Network::PacketType)];
					ToChar(Network::PacketType::PT_ServerReady, buffer, sizeof(Network::PacketType));
					p_connection->SendPacket(&buffer, sizeof(Network::PacketType));
          }
        };

      // the initial state of Loading FSM
      typedef connecting_ initial_state;

      // transition map
      struct transition_table : mpl::vector<
        //      Start                     Event                   Next                    Action               Guard
        //    +--------------------------+-----------------------+----------------------+---------------------+----------------------+
        _row < connecting_,               ConfirmAchievement,     loading_data_stage_>,
        _row < loading_data_stage_,       LoadingGameCompleted,   SendingData>,
        _row < SendingData,               ConfirmAchievement,     wait_>,
        _row < wait_,                     ConfirmAchievement,     wait_>
      >{};
      };

    // back-end
    typedef msm::back::state_machine<loading_> LoadingFSM;

    const size_t LAST_STATE_INDEX = 3;

    } // LoadingStages
  } // Slavs