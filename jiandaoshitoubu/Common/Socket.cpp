#include "Socket.h"

using namespace JSB;

#define JSB_SOCKET_ERROR(x) {setState(x); return;}
#define JSB_SOCKET_ERROR_RET(x, y) {setState(x); return (y);}

Socket::Socket(Port port)
{
	mState = SS_OK;
	if ((mID = ::socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		JSB_SOCKET_ERROR(SS_SOCKET_FAILED);

	sockaddr_in ski;
	ski.sin_family = AF_INET;
	ski.sin_port = htons(port);
	ski.sin_addr.s_addr = INADDR_ANY;//local
	memset(&(ski.sin_zero), 0, sizeof(ski.sin_zero));

	if (::bind(mID, (sockaddr *)&ski, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		JSB_SOCKET_ERROR(SS_BIND_FAILED);
	}
	if (::listen(mID, SOMAXCONN) == SOCKET_ERROR)
		JSB_SOCKET_ERROR(SS_LISTEN_FAILED);

	makeNoBlocking();

	
}

Socket::Socket(SOCKET id) :mID(id)
{
	mState = SS_OK;
	makeNoBlocking();
}

unsigned long Socket::inet_addrW(const String& string)
{
	Stringstream ss;
	ss << string;
	char addr[8];

	ss >> (int&)addr[0];
	ss.ignore(1);
	ss >> (int&)addr[1];
	ss.ignore(1);
	ss >> (int&)addr[2];
	ss.ignore(1);
	ss >> (int&)addr[3];

	return *(unsigned long*)addr;
}

Socket::Socket(const String& addr, Port port)
{
	mState = SS_OK;
	if ((mID = ::socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		JSB_SOCKET_ERROR(SS_SOCKET_FAILED);

	sockaddr_in ski;
	ski.sin_family = AF_INET;
	ski.sin_port = htons(port);
	ski.sin_addr.s_addr = inet_addrW(addr);//inet_addr cannt support wchar_t
	memset(&(ski.sin_zero), 0, sizeof(ski.sin_zero));

	if (::connect(mID, (sockaddr *)& ski, sizeof(sockaddr)) == SOCKET_ERROR)
		JSB_SOCKET_ERROR(SS_CONNECT_FAILED);
	makeNoBlocking();

	
}

Socket::~Socket()
{
	::closesocket(mID);
}


void Socket::makeNoBlocking()
{
	u_long nonblock = 1;
	if (ioctlsocket(mID, FIONBIO, &nonblock) == SOCKET_ERROR)
		JSB_SOCKET_ERROR(SS_CONNECT_FAILED);

}


SocketState Socket::getState()const
{
	return mState;
}

SOCKET Socket::getID()const
{
	return mID;
}

void Socket::restoreRecvData()
{
	const int maxSize = Socket::DEFAULT_BUFFER_SIZE;

	RingBuffer::AllocBuffer mem;
	mem.size = maxSize;

	while (mem.size != 0)
	{
		mem = mRecvBuffer.alloc(maxSize);
		//Assert(mem.size != 0);//buffer´óÐ¡²»×ã
		if (mem.size == 0)
			break;
		mem.size = ::recv(mID, (char*)mem.buffer, mem.size, 0);
		checklink();
		if (mem.size == SOCKET_ERROR)
		{
			mem.size = 0;
		}
		else if (mem.size == 0)
		{
		}


		mRecvBuffer.adjust(mem.size);

	}

}


bool Socket::recv(void* buffer, size_t size)
{
	restoreRecvData();

	if (mRecvBuffer.size() < size)
		return false;

	mRecvBuffer.read(buffer, size);
	return true;

}

void Socket::restoreSendData(const char* buffer, size_t size)
{
	auto mem = mSendBuffer.alloc(size);
	Assert(mem.size != 0);

	if (mem.size < size)
	{
		restoreSendData(buffer, mem.size);
		restoreSendData(buffer + mem.size, size - mem.size);
	}
	else
	{
		memcpy(mem.buffer, buffer, size);
		mSendBuffer.adjust(size);
	}
}

bool Socket::sendNow()
{
	while (mSendBuffer.size() != 0)
	{
		auto ab = mSendBuffer.data();

		while (ab.size != 0)
		{
			int ret = ::send(mID, (const char*)ab.buffer, ab.size, 0);
			checklink();
			if (ret != SOCKET_ERROR)
			{
				ab.buffer = (char*)ab.buffer + ret;
				ab.size -= ret;
				mSendBuffer.free(ret);
			}
			else
			{
				//HRESULT r = WSAGetLastError();
				//std::stringstream ss;
				//ss << STR("failed to call socket send. error code: ") << r << std::endl;
				//std::cout << ss.str();
				JSB_SOCKET_ERROR_RET(SS_SEND_ERROR, false);
			}
		}
	}
	return true;
}

bool Socket::send(const void* buffer, size_t size)
{
	restoreSendData((const char*)buffer, size);
	return sendNow();
}

void Socket::update()
{
	sendNow();

}


SOCKET Socket::accept()
{
	sockaddr inaddr;
	int inlen = sizeof(inaddr);
	SOCKET ret = ::accept(mID, &inaddr, &inlen);
	return ret;
}

void Socket::checklink()
{
	HRESULT r = WSAGetLastError();
	if (r == WSAECONNRESET)
		JSB_SOCKET_ERROR(SS_CLOSE)
}

void Socket::setState(SocketState state)
{
	if (mState == SS_CLOSE)
		return;
	mState = state;
}