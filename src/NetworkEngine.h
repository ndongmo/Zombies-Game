/*!
* \file NetworkEngine.h
* \brief Simple Network engine.
* \author Ndongmo Silatsa Fabrice
* \date 01-12-2016
* \version 1.0
*/

#pragma once

#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

#include "Logger.h"

namespace NS2 
{

#define DEFAULT_SERVER_PORT 1234
#define DEFAULT_CLIENT_PORT 1235
#define BUFFER_SIZE 1400
#define MAX_PLAYER 4

	/*!
	* \briefData structure for a single network player.
	*/
	struct NetworkData
	{
		NetworkData(TCPsocket socket, unsigned int timeout, unsigned int id)
		{
			this->socket = socket;
			this->timeout = timeout;
			this->id = id;
		}

		TCPsocket socket;
		unsigned int timeout;
		unsigned int id;
	};

	enum class NetworkError {NONE, SERVER_FULL};

	class NetworkServer
	{
	public:
		void init();
		void destroy();
		NetworkError accept();

	private:
		SDLNet_SocketSet m_sockets = nullptr;
		TCPsocket m_server = nullptr;
		IPaddress m_ipAddress;						/* IP Address */
		std::vector<NetworkData> m_data;			/* Players data */
		char m_tmpBuffer[BUFFER_SIZE];				/* Temporary buffer */
		unsigned int m_port = DEFAULT_SERVER_PORT;  /* Network port */
		unsigned int m_currentID = 0;				/* Current ID */
		unsigned int m_playerNum = 0;				/* Number of player connect to the server */
	};

	class NetworkClient
	{
	public:
		void init();
		void destroy();
	private:
		unsigned int m_playerID = 0;				/* Player ID */
		unsigned int m_playerNum = 0;				/* Number of player connect to the server */
		unsigned int m_port = DEFAULT_CLIENT_PORT;	/* Network port */
		IPaddress m_ipAddress;						/* IP Address */
		std::vector<NetworkData> m_data;			/* Players data */
		char m_tmpBuffer[BUFFER_SIZE];				/* Temporary buffer */
	};

	class NetworkEngine
	{
	public:
		bool init();
		void destroy();
	private:
		
	};

}