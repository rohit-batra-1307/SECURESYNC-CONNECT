
/**************************HEADER FILES***********************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

/*
 * Winsock library for socket programming
 */
#include <winsock2.h>

/*
 * Functions for working with threads
 */
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

/*************************************************************************/

class CTcpServer 
{
private:
	/********************Member Variables******************************/
	/*
	 * port number used for communication in the TCP server. 
	 */
    int m_nPORT;

	/*
	 * represents the socket used by the TCP server for communication
	 */
    SOCKET m_serverSocket;

	/*
	 * Declaration of WSADATA structure
	 */
	WSADATA m_wsData;

	/*
	 * Server receives a file from the client
	 */
	void ReceiveFile(const char* pszFileName, SOCKET client);

	/*
	 * Function is responsible for saving the received file content
	 */
    void SaveReceivedFile(const char* pszFileName, const std::string& strFileContent);

public:

	/*
	 * Constructor
	 */
    CTcpServer(int nPort);

	/*
	 * Destructor
	 */
    ~CTcpServer();

    /*
	 * Function for handling communication with the connected client.
	 */
	void StartListening();

	 /*
	  * Function appears to be a static member function
      * that serves as a wrapper to call the non-static member function.
	  */
    static void HandleClient(void* clientSocket);

	/*
	 * Handle the server client communication and processing information.
	 */
	void HandleClientImpl();
};