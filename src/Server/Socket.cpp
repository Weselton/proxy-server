#include "stdafx.h"
#include "Socket.h"
#include <vector>
#include <sstream>
#include <iostream>
#include <winsock2.h>
#include "curl/curl.h"
#pragma comment(lib, "Ws2_32.lib")

#define CURL_STATICLIB
#define MAX_SIZE_MESSAGE 1024
#define PORT 80

const int max_client_buffer_size = 10240000;
char buf[max_client_buffer_size];
char buf_test[max_client_buffer_size];


static size_t write_data(char *ptr, size_t size, size_t nmemb, std::string* data)
{
	if (data)
	{
		data->append(ptr, size *nmemb);
		return size * nmemb;
	}
	else return 0;
}

Socket::Socket()
{
	WSADATA WsaData;
	int err = WSAStartup(0x0101, &WsaData);

	if (err == SOCKET_ERROR)
	{
		printf("WSAStartup() failed: %ld\n", GetLastError());
	}

	listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (listen_socket == INVALID_SOCKET)
	{
		std::cerr << "Cant create a socekt for listen_socket" << std::endl;
	}

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = (INADDR_ANY);
	address.sin_port = htons(PORT);
}

void Socket::Bind()
{
	if (bind(listen_socket, (struct sockaddr*)&address, sizeof(address)) != SOCKET_ERROR)
	{
		printf("Socket succed binded\n");
	}
}

void Socket::Listen()
{
	if (listen(listen_socket, SOMAXCONN) != SOCKET_ERROR)
	{
		printf("Start listenin at port %u\n", ntohs(address.sin_port));
	}
}

int Socket::GetCountClient()
{
	return clients.size();
}

void Socket::AddNewClient(client client)
{
	clients.push_back(client);
}

void Socket::ProcessingRequest(SOCKET &new_socket)
{
	CURL *curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_HEADER, 1);
	std::string wr_buf;
	
	if (curl_handle)
	{
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &wr_buf);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_perform(curl_handle);
		curl_easy_cleanup(curl_handle);
	}

	send(new_socket, wr_buf.data(), wr_buf.size(), 0);
	std::cout << wr_buf.data() << std::endl;
	getchar();

	shutdown(new_socket, SD_BOTH);
	closesocket(new_socket);
}

int Socket::SetNonblocking()
{
	return 0;
}

Socket::~Socket()
{
}

void Socket::Processing()
{
	while (true)
	{
		client client_info;
		int size = sizeof(client_info.addressClient);
		SOCKET new_socket = accept(listen_socket, (sockaddr *)&client_info.addressClient, &size);

		if (new_socket == INVALID_SOCKET)
		{
			std::cerr << "Cant create a client_socekt" << std::endl;
		}
		
		AddNewClient(client_info);
		std::cout << "ip - adress of client :" << client_info.addressClient.sin_addr.s_addr << std::endl;
		int recv_lenght = recv(new_socket, buf, MAX_SIZE_MESSAGE, NULL);
		buf[recv_lenght] = 0;
		std::cout << buf << std::endl;
		client_info.CountVolumeData(buf);
		//std::cout << client_info.GetVolumeData() << std:: endl << GetCountClient() << std::endl;
		ProcessingRequest(new_socket);
		shutdown(new_socket, SD_BOTH);
		closesocket(new_socket);
	}
}

