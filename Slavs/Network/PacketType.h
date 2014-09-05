#pragma once

namespace Network
  {

  enum class PacketType
    {
    PT_NO_PACKETS       = 0xff,
    PT_HasConnection	  = 0x00,	//two-way, server sends random number, client responds with the same number
    PT_Connects			    = 0x01,	//server -> client: current map
    PT_Maps				      = 0x02,	//server -> client: list of maps
    PT_StartGame		    = 0x03,	//client -> server: selected map
    PT_ClientReady		  = 0x04, //client -> server: at loading time notify server about the finishing of loading resources
    PT_ServerReady		  = 0x05, //server -> client: at loading time notify client about finishing preparing level
    PT_Position			    = 0x05,	//two-way
    PT_GOState			    = 0x06,	//server -> client
    PT_ControllerState	= 0x07,	//server -> client
    PT_SpawnEntity		  = 0x08,	//server -> client
    PT_CameraMove		    = 0x09,	//two-way
    PT_EndGame			    = 0x0A, //two-way
    PT_Command			    = 0x0B, //client -> server: sends commands from GUI (first 4 bytes are PacketType, 4 bytes - Command id, rest bytes - command data)
    PT_Selection		    = 0x0C, //two-way: (?client -> server)
    PT_Definitions      = 0x0D,
    PT_Achived          = 0x0E,
    PT_ContentNumber    = 0x0f,
    PT_None				      = 0x64,
    };

  } // Network