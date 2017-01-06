#include "TcpClient.h"

TcpClient::TcpClient(SOCKET socket)
{
	_socket = socket;
}

TcpClient::~TcpClient()
{	
	closesocket(_socket);
}

TcpClient::TcpClient()
{
}


SOCKET TcpClient::GetSocket()
{
	return _socket;
}

int TcpClient::Recv(char** message, int* msgLength)
{	
	int messageLength;
	int actual_len = 0;

	if (SOCKET_ERROR == (actual_len = recv(_socket, (char*)&messageLength, sizeof(int), 0)))
	{
		return WSAGetLastError();		
	}	

	*message = new char[messageLength];
	if (SOCKET_ERROR == (actual_len = recv(_socket, *message, messageLength, 0)))
	{
		return WSAGetLastError();				
	}		
	*msgLength = actual_len;
	return 0;
}

int TcpClient::Send(char* message, int messageLength)
{
	char* msgLen = new char[sizeof(int)];
	itoa(messageLength, msgLen, 10);

	if (SOCKET_ERROR == (send(_socket, msgLen, sizeof(int), 0)))
	{
		return WSAGetLastError();
	}
	if (SOCKET_ERROR == (send(_socket, message, messageLength, 0)))
	{
		return WSAGetLastError();
	}
	return 0;
}

int TcpClient::Connect(char* ip, int port)
{	
	// ������� ���������� ��� �������� ������ 
	sockaddr_in s_address;
	// �������� ��:
	ZeroMemory(&s_address, sizeof(s_address));
	// ��� ������ (TCP/IP)
	s_address.sin_family = AF_INET;
	// ����� �������. �.�. TCP/IP ������������ ������ � �������� ����, �� ��� �������� 
	// ������ ���������� ������� inet_addr.	
	s_address.sin_addr.S_un.S_addr = inet_addr(ip);
	// ����. ���������� ������� htons ��� �������� ������ ����� �� �������� � //TCP/IP �������������.
	s_address.sin_port = htons(port);
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	// ������ ��������� ����������:
	if (SOCKET_ERROR == (connect(_socket, (sockaddr *)&s_address, sizeof(s_address))))
	{
		// Error...
		return ERROR;		
	}
	return 0;
}
