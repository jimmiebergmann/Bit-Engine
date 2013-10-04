// ///////////////////////////////////////////////////////////////////////////
// Copyright (C) 2013 Jimmie Bergmann - jimmiebergmann@gmail.com
// 
// This software is provided 'as-is', without any express or
// implied warranty. In no event will the authors be held
// liable for any damages arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but
//    is not required.
// 
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
// 
// 3. This notice may not be removed or altered from any
//    source distribution.
// ///////////////////////////////////////////////////////////////////////////


#ifndef __BIT_NETWORK_TCP_HPP__
#define __BIT_NETWORK_TCP_HPP__

#include <Bit/DataTypes.hpp>
#include <Bit/Network.hpp>
#include <Bit/Network/Address.hpp>

namespace Bit
{

	namespace Net
	{

		//namespace TCP
		//{

			/*class Server
			{
			
			public:

				Server( );
				BIT_UINT32 Start( );
				void Stop( );
				BIT_UINT32 Host( BIT_UINT16 p_Port, BIT_SINT32 p_MaxConnections );
				BIT_UINT32 Accept( Socket & p_Client );
				BIT_BOOL DisconnectClient( const Socket p_Client );
				BIT_SINT32 Send( const Socket & p_Client, const char * p_Buffer, const BIT_UINT16 p_Size );
				char * Receive( Socket & p_Client, BIT_UINT16 & p_Size );
				BIT_BOOL PollDisconnections( Socket & p_Client );
				BIT_BOOL SetBlocking( BIT_BOOL p_Status );
				BIT_INLINE BIT_BOOL IsHosted( ) const { return m_Hosted; }

			private:

				void NonBlockSocket( Socket & p_Socket );
				static void * StaticThreadFunction( void * p_Param );
				

				struct ReceiveStruct
				{
					char * pBuffer;
					BIT_UINT16 BufferSize;
					Socket Client;
				};

				Thread m_Thread;
				Mutex m_ShutDownMutex;
				Mutex m_SetMutex;
				Mutex m_ReceiveQueueMutex;
				Mutex m_DisconnectQueueMutex;
				BIT_BOOL m_ShuttingDown;
				BIT_BOOL m_Started;
				BIT_BOOL m_Hosted;
				BIT_UINT16 m_Port;
				BIT_BOOL m_Blocking;
				Socket m_ListenSocket;
				//Socket m_ConnectedSocket;
				fd_set m_FDSet;
				//BIT_UINT32 m_ClientCount;
				std::list< ReceiveStruct > m_ReceiveQueue;
				std::list< Socket > m_DisconnectQueue;
				
			};

			*/
/*
			class Client
			{
			
			public:

				Client( );
				BIT_UINT32 Connect( Address & p_Address );
				BIT_UINT32 Disconnect( );
				BIT_SINT32 Send( const char * p_Buffer, const BIT_UINT16 p_Size );
				BIT_SINT32 Receive( char * p_Buffer, const BIT_UINT16 p_Size );
				BIT_BOOL SetBlocking( BIT_BOOL p_Status );
				BIT_INLINE BIT_BOOL IsConnected( ) const { return m_Connected; }

			private:

				Socket m_Socket;
				Address m_Address;
				BIT_BOOL m_Connected;

			};

		}*/


		enum eType
		{
			None = 0,
			Client = 1,
			Server = 2
		};


		class TCP
		{

		public:

			// Constructors/destructors
			TCP( );
			~TCP( );
			
			// Public general functions
			BIT_UINT32 Connect( const Address & p_Address );
			BIT_UINT32 Disconnect( );
			BIT_UINT32 Host( const BIT_UINT16 p_Port );
			BIT_BOOL IsEstablished( ) const;
			BIT_SINT32 Receive( BIT_BYTE * p_pBuffer, const BIT_UINT32 p_Size );
			BIT_SINT32 Send( const BIT_BYTE * p_Buffer, const BIT_UINT32 p_Size );

		private:

			// Private members
			eType m_Type;
			BIT_BOOL m_Established;
			Socket m_Socket;
			Address m_Address;

		};

	}
}

#endif