/********************* COPYRIGHT SSS 2023-2024 ****************************
 *************************************************************************/

#include "Client.h"

int main() 
{
    CTcpClient tcpClient(12345);

    if (!tcpClient.Connect("127.0.0.1")) 
	{
        return 1;
    }

    tcpClient.SendFile("E:\\ program\\test.txt");
    tcpClient.ReceiveResult();

    system("pause");
    return 0;
}
