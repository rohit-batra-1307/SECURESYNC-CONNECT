
#include "Server.h"

int main() 
{
	/*
	 * Create an instance of the CTcpServer class, specifying the port number (12345)
	 */
    CTcpServer tcpServer(12345);

	/*
	 * Start listening for incoming client connections
	 */
    tcpServer.StartListening();

    system("pause");
    return 0;
}
