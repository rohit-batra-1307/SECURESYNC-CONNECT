/********************* COPYRIGHT SSS 2023-2024 ******************
 ** MODULE  :  Client.cpp
 **
 ** PURPOSE :  Solution for Class CTcpClient
 **
 ** AUTHOR  :  Rohit
 **************************************************************/

#include "Client.h"

/*----------------------------------------------------------
-- NAME      : CTcpClient::CTcpClient
-- PURPOSE   : Constructor for Class
-- PARAMETER : Interger for port number

---------------------------------------------------------*/
CTcpClient::CTcpClient(int nPort)
{
	m_nPORT = nPort;

	if (WSAStartup(MAKEWORD(2, 2), &m_wsData) != 0) 
	{
		std::cerr << "Failed to initialize Winsock\n";
		return;
	}
	m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (m_clientSocket == INVALID_SOCKET) 
	{
		std::cerr << "Failed to create client socket\n";
		WSACleanup();
		return;
	}
}

/*----------------------------------------------------------
-- NAME      : CTcpClient::CTcpClient
-- PURPOSE   : Destructor for Class
-- PARAMETER : NA
---------------------------------------------------------*/
CTcpClient::~CTcpClient()
{
	if (m_clientSocket != INVALID_SOCKET) 
	{
		closesocket(m_clientSocket);
	}
	WSACleanup();
}

/*-----------------------------------------------------------------------
-- NAME      : CTcpClient::CTcpClient
-- PURPOSE   : Establishing a connection to a server using the provided 
			   server address and port number.
-- PARAMETER : char pointer of server address
------------------------------------------------------------------------*/
bool CTcpClient::Connect(const char* pszServerAddress) 
{
    /*
	 * Create a sockaddr_in structure to represent the server's address
	 */
	sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_nPORT);

	/*
	 * Converts the server IP address from string format (pszServerAddress) 
	 * to binary form and sets it in the structure.
	 */
    serverAddr.sin_addr.s_addr = inet_addr(pszServerAddress);

	/*
	 * Attempt to connect to the server using the client socket and server address
	 */
    if (connect(m_clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), 
		sizeof(serverAddr)) == SOCKET_ERROR) 
	{
        std::cerr << "Failed to connect to the server\n";
        return false;
    }

    std::cout << "Connected to server on port " << m_nPORT << std::endl;
    return true;
}

/*---------------------------------------------------------------------
-- NAME      : CTcpClient::CTcpClient
-- PURPOSE   : Responsible for sending a file to the connected server.
-- PARAMETER : char pointer of file name
----------------------------------------------------------------------*/
bool CTcpClient::SendFile(const char* pszFileName) 
{
    /*
	 * Send file name
	 * Use the send function to send the file name to the server
	 */
    send(m_clientSocket, pszFileName, strlen(pszFileName), 0);
    std::cout << "File name sent: " << pszFileName << std::endl;

    /*
	 * Send file content
	 * Open the file in binary mode for reading
	 */
    std::ifstream inputFile(pszFileName, std::ios::binary);
    std::ostringstream strFileContent;
    strFileContent << inputFile.rdbuf();
    inputFile.close();

    /*
	 * Convert the ostringstream to a string
	 */
	std::string strContentString = strFileContent.str();
    send(m_clientSocket, strContentString.c_str(), strContentString.size(), 0);
    std::cout << "File content sent\n";

    return true;
}

/*-----------------------------------------------------------
-- NAME      : CTcpClient::CTcpClient
-- PURPOSE   : Responsible for receiving the result of a 
			   binary operation from the connected server.
-- PARAMETER : NA
------------------------------------------------------------*/
void CTcpClient::ReceiveResult() 
{
    /*
	 *Receive the result from the server
	 */
    int nResult;
    int nBytesRead = recv(m_clientSocket, reinterpret_cast<char*>(&nResult), 
		             sizeof(nResult), 0);
    /*
	 * Check if bytes were read successfully
	 */
	if (nBytesRead > 0) 
	{
        std::cout << "Binary operation result: " << nResult << std::endl;
    } 
	else 
	{
        std::cerr << "Failed to receive result from server\n";
    }
}


