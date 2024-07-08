/********************* COPYRIGHT SSS 2023-2024 *********************
 *******************************************************************/

/*****************************HEADER FILES**************************/
#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <cstring>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")
/******************************************************************/

class CTcpClient 
{
private:
	/********************Member Variables***************************/

	/*
	 * port number used for communication in the TCP client. 
	 */
    int m_nPORT;

	/*
	 * represents the socket used by the TCP client for communication
	 */
    SOCKET m_clientSocket;

	/*
	 * Declaration of WSADATA structure
	 */
	WSADATA m_wsData;

public:

	/*
	 * Constructor
	 */
    CTcpClient(int nPort);
	
	/*
	 *Destructor
	 */
    ~CTcpClient();

    /*
	 * Establishing a connection to a server
	 */
	bool Connect(const char* pszServerAddress);

	/*
	 * Sending files to a server
	 */   
	bool SendFile(const char* pszFileName);

    /*
	 * receiving the result of a 
	 * binary operation from the connected server.
	 */  
	void ReceiveResult();
};
