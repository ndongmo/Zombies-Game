#include "NetworkEngine.h"

namespace NS2
{
	bool NetworkEngine::init()
	{
		if (SDLNet_Init())
			return true;
		else {
			logE << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
			return false;
		}
	}

	void NetworkEngine::destroy()
	{
		SDLNet_Quit();
	}

	/* NETWORK SERVER */

	void NetworkServer::init()
	{
		SDLNet_ResolveHost(&m_ipAddress, nullptr, m_port);

		m_server = SDLNet_TCP_Open(&m_ipAddress);

		m_sockets = SDLNet_AllocSocketSet(MAX_PLAYER);
	}
	void NetworkServer::destroy()
	{
		SDLNet_FreeSocketSet(m_sockets);
		SDLNet_TCP_Close(m_server);
	}

	NetworkError NetworkServer::accept()
	{
		TCPsocket socket = SDLNet_TCP_Accept(m_server);
		if (socket)
		{
			if (m_playerNum < MAX_PLAYER)
			{
				SDLNet_TCP_AddSocket(m_sockets, socket);
				m_data.push_back(NetworkData(socket, SDL_GetTicks(), m_currentID));
				m_playerNum++;
				m_currentID++;

				return NetworkError::NONE;
			}
			else
			{
				return NetworkError::SERVER_FULL;
			}
		}
	}
}