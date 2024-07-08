
#include "Server.h"

/*---------------------------------------------------------------------------
----------------------------------------------------------------------------*/
CTcpServer::CTcpServer(int nPort)
{
	m_nPORT = nPort;

    if (WSAStartup(MAKEWORD(2, 2), &m_wsData) != 0) 
	{
        std::cerr << "Failed to initialize Winsock\n";

        return;
    }

    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverSocket == INVALID_SOCKET) 
	{
        std::cerr << "Failed to create server socket\n";
        WSACleanup();

        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_nPORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), 
		sizeof(serverAddr)) == SOCKET_ERROR) 
	{
        std::cerr << "Failed to bind server socket\n";
        closesocket(m_serverSocket);
        WSACleanup();

        return;
    }

    if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR) 
	{
        std::cerr << "Failed to listen on server socket\n";
        closesocket(m_serverSocket);
        WSACleanup();

        return;
    }

    std::cout << "Server listening on port " << m_nPORT << std::endl;
}

/*-----------------------------------------------------------------------------
----------------------------------------------------------------------------*/
CTcpServer::~CTcpServer() 
{
    if (m_serverSocket != INVALID_SOCKET) 
	{
        closesocket(m_serverSocket);
    }
    WSACleanup();
}

/*---------------------------------------------------------------------------
----------------------------------------------------------------------------*/
void CTcpServer::StartListening() 
{
    while (true) 
	{
		/*
		 *Accept incoming client connections
		 */
        SOCKET clientSocket = accept(m_serverSocket, 0, 0);
        if (clientSocket == INVALID_SOCKET) 
		{
            std::cerr << "Failed to accept client connection\n";
            continue;
        }

        /*
		 * Create a new thread to handle the client
		 */
        _beginthread(HandleClient, 0, reinterpret_cast<void*>(clientSocket));
    }
}

/*----------------------------------------------------------------------------------
-----------------------------------------------------------------------------------*/
void CTcpServer::HandleClient(void* pvClientSocket) 
{
    /*
	 * reinterpret cast of the clientSocket from a void* to a pointer 
	 * to an instance of CTcpServer.
	 */
	reinterpret_cast<CTcpServer*>(pvClientSocket)->HandleClientImpl();
}


/*-------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------*/
void CTcpServer::HandleClientImpl() 
{
	/*
	 *Accept incoming client connection
	 */
    SOCKET clientSocket = accept(m_serverSocket, 0, 0);

    if (clientSocket == INVALID_SOCKET) 
	{
        std::cerr << "Failed to accept client connection\n";
        return;
    }

    /*
	 * Receive the file name from the client
	 */
    char cArrFileName[256];
    int nBytesRead = recv(clientSocket, cArrFileName, sizeof(cArrFileName), 0);

    if (nBytesRead <= 0) 
	{
        std::cerr << "Failed to receive file name\n";
        closesocket(clientSocket);
        return;
    }
    cArrFileName[nBytesRead] = '\0';
    std::cout << "File Received: " << cArrFileName << std::endl;

    /*
	 * Opens the file specified by the received file name and reads its content in chunks 
	 * using a loop. The content is appended to the strFileContent string.
	 */
    std::string strFileContent;
    const int nBUFFER_SIZE = 1024;
    char cBuffer[nBUFFER_SIZE];

    std::ifstream inputFile(cArrFileName, std::ios::binary);

    while (true) 
	{
        inputFile.read(cBuffer, nBUFFER_SIZE);
        int nBytesRead = static_cast<int>(inputFile.gcount());

        if (nBytesRead <= 0) 
		{
            break;
        }

        strFileContent.append(cBuffer, nBytesRead);
    }

    inputFile.close();

    std::cout << "File content received" << std::endl;

    /*
	 * Save the received file
	 */
    SaveReceivedFile(cArrFileName, strFileContent);

    /*
	 * Perform binary operations (e.g., count the number of characters)
	 */
    int nCharCount = strFileContent.size();

    /*
	 * Send the result back to the client
	 */
    send(clientSocket, reinterpret_cast<char*>(&nCharCount), sizeof(nCharCount), 0);

    closesocket(clientSocket);

}

/*----------------------------------------------------------------------------------
-----------------------------------------------------------------------------------*/
void CTcpServer::ReceiveFile(const char* pszFileName, SOCKET client) 
{
    /*
	 * Open an output file stream for writing the received file in binary mode
	 */
	std::ofstream outputFile(pszFileName, std::ios::binary);

    /*
	 * Check if the file stream is successfully opened
	 */

	if (!outputFile.is_open()) 
	{
        std::cerr << "Failed to open output file for receiving\n";
        closesocket(client);
        return;
    }

	/*
	 * Set the buffer size for receiving data
	 */
    const int nBUFFER_SIZE = 1024;
    char cBuffer[nBUFFER_SIZE];

	/*
	 * Loop to receive and write file content
	 */
    while (true) 
	{
		/*
		 * Receive data from the client into the buffer
		 */
        int nBytesRead = recv(client, cBuffer, nBUFFER_SIZE, 0);

		/*
		 * Check if there was an error or if no more data is read
		 */
        if (nBytesRead <= 0) 
		{
            std::cerr << "Error occurred while receiving file\n";

			/*
			 * Break the loop if no more data is read or an error occurs
			 */
            break;  
        }

        /*
		 * Write the received data to the output file
		 */
		outputFile.write(cBuffer, nBytesRead);

    }

    outputFile.close();
    std::cout << "File received and saved: " << pszFileName << std::endl;
    closesocket(client);
}

/*----------------------------------------------------------------------------------
-----------------------------------------------------------------------------------*/
void CTcpServer::SaveReceivedFile(const char* pszFileName, 
								  const std::string& strFileContent) 
{
    /*
	 * Open an output file stream for writing the received file in binary mode
	 */
    std::ofstream outputFile(pszFileName, std::ios::binary);

    if (outputFile.is_open()) 
	{
        /*
		 * Write the received file content to the output file
		 */
		outputFile.write(strFileContent.c_str(), strFileContent.size());
        outputFile.close();
        std::cout << "File received and saved: " << pszFileName << std::endl;
    }
	else 
	{
        std::cerr << "Failed to open output file\n";
    }
}



