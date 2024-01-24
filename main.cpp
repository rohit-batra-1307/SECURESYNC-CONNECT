/********************* COPYRIGHT SSS 2023-2024 ****************************
 ** MODULE  :  main.cpp
 **
 ** PURPOSE :  main file to control the console
 **
 ** AUTHOR  :  Rohit
 *************************************************************************/

#include "Client.h"

int main() 
{
    CTcpClient tcpClient(12345);

    if (!tcpClient.Connect("127.0.0.1")) 
	{
        return 1;
    }

    tcpClient.SendFile("E:\\Synergy program\\test.txt");
    tcpClient.ReceiveResult();

    system("pause");
    return 0;
}
