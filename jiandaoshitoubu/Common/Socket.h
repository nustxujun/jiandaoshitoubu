#ifndef _Socket_H_
#define _Socket_H_

#include "Common.h"
#include <winsock.h>
#include <Windows.h>
#include "DataStream.h"
#include "RingBuffer.h"

namespace JSB
{

	typedef u_short Port;

	enum SocketState
	{
		SS_OK,

		SS_SOCKET_FAILED,
		SS_BIND_FAILED,
		SS_LISTEN_FAILED,
		SS_CONNECT_FAILED,
		SS_RECV_FAILED,
		SS_NONBLOCK_FAILED,

		SS_CONNECT_ERROR,

		SS_SEND_ERROR,

		SS_CLOSE
	};
	
	class Socket
	{
	public:
		static const Port DEFAULT_PORT = 10001;
		static const size_t DEFAULT_BUFFER_SIZE = 0xff;

	public:
		Socket(Port port);//listen
		Socket(SOCKET id);
		Socket(const String& addr, Port port = DEFAULT_PORT);
		Socket(const Socket&) = delete;
		void operator=(const Socket&) = delete;

		~Socket();

		SOCKET accept();

		bool recv(void* buffer, size_t size);
		bool send(const void* buffer, size_t size);
		void update();

		SocketState getState()const;
		SOCKET getID()const;

	private:
		void restoreSendData(const char* buffer, size_t size);
		void restoreRecvData();

		bool sendNow();

		void makeNoBlocking();
		unsigned long inet_addrW(const String& string);

		void checklink();
		void setState(SocketState state);
	private:
		SOCKET mID;
		SocketState mState = SS_OK;
		RingBuffer mRecvBuffer = { DEFAULT_BUFFER_SIZE };
		RingBuffer mSendBuffer = { DEFAULT_BUFFER_SIZE };
	};

}

#endif
