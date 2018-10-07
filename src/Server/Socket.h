#pragma once

#include <winsock2.h>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

class Socket
{
	struct client 
	{
		client()
		{
			ZeroMemory(&addressClient, sizeof(sockaddr_in));
			time = 0;
			VolumeData = 0;
		}

		void CountVolumeData(char buf[])
		{
			VolumeData += sizeof(buf);
		}

		int GetVolumeData()
		{
			return VolumeData;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
		}

		struct sockaddr_in addressClient;
		int VolumeData;
		int time;
	};

public:
	Socket();
	void Bind();
	void Listen();
	void Processing();
	int GetCountClient();
	void AddNewClient(client client);
	void ProcessingRequest(SOCKET &new_socket);
	int SetNonblocking();
	~Socket();

private:
	SOCKET listen_socket;
	SOCKADDR_IN address;
	std::vector <client> clients;
};

