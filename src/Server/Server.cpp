#include "stdafx.h"
#include "Socket.h"

int main()
{
	Socket socketServer;
	socketServer.Bind();
	socketServer.Listen();
	socketServer.Processing();
}
