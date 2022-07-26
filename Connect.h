#include "stdafx.h"

class UDP
{
public:
    int Send(string SendInfo);
    UDP();
    ~UDP();
    string Response();
    int bytesRecieved = 0;
    void separator(string str, Object data);

private:
    char buf[MaxPackSize];
    SOCKET sock;
    sockaddr_in server_addr;
    int server_len;
    void Delivery_Warranty(string message, int partNum);

};