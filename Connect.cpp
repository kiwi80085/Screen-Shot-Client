#include "Connect.h"

UDP::~UDP()
{
    closesocket(sock);
    WSACleanup();
}

UDP::UDP()
{
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(MAKEWORD(2, 2), &data);
    if (wsResult != 0)
    {
        cerr << "WSAStartup failed, Err #" << wsResult << endl;
        cin.get();
    }

    //Creating Socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET)
    {
        cerr << "Can't create a socket, Err #" << WSAGetLastError() << endl;
        cin.get();
        WSACleanup();
    }

    int OptVal = 1;
    int OptLen = sizeof(int);
    int rc = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&OptVal, OptLen);
    if (rc == SOCKET_ERROR)
    {
        cerr << "Can't set a sockopt, Err #" << WSAGetLastError() << endl;
        cin.get();
        WSACleanup();
    }

    sockaddr_in client_addr;

    server_len = sizeof(server_addr);
    ZeroMemory(&server_addr, server_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, serverIP, &server_addr.sin_addr);

    int client_len = sizeof(client_addr);
    ZeroMemory(&client_addr, client_len);
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(0);
    client_addr.sin_addr.S_un.S_addr = INADDR_ANY;

    rc = bind(sock, (struct sockaddr*)&client_addr, client_len);
    if (rc == SOCKET_ERROR)
    {
        cerr << "Can't bind a socket, Err #" << WSAGetLastError() << endl;
        cin.get();
        WSACleanup();
    }
}

int UDP::Send(string userInput)
{
    if (userInput.size() > 0)
    {
        //Send the message
        int sendResult = sendto(sock, userInput.c_str(), MaxPackSize, 0, (sockaddr*)&server_addr, server_len);

        if (sendResult == SOCKET_ERROR)
        {
            cerr << "Can't send msg, Err #" << WSAGetLastError() << endl;
            cin.get();
            return 0;
        }
    }
}


string UDP::Response()
{
    ZeroMemory(buf, MaxPackSize);
    bytesRecieved = recvfrom(sock, buf, MaxPackSize, 0, (struct sockaddr*)NULL, NULL);
    if (bytesRecieved > 0)
    {
        //Echo the text to the console
        string info = (string)buf;
        return info;
        cerr << "SERVER> " << string(buf, 0, bytesRecieved) << endl;
    }
    else
    {
        cerr << "Can't read msg, Err #" << WSAGetLastError() << endl;
        cin.get();
    }
}


void UDP::Delivery_Warranty(string message, int partNum)
{
    string checkNumber = to_string(partNum);

    int check = 0;
    string DATA; // for check the packet number. The server returns it if it has received the packet.
    while (check == 0)
    {
        Send(message);
        DATA = Response();
        if (DATA == "OK")
        {
            check = 1;
        }
    }
}


void UDP::separator(string str, Object data)
{
    int SIZE = 1000;
    if (str.size() > SIZE)
    {
        int packCount = ceil((double)str.size() / (double)SIZE);
        string pack;
        int o = 0;
        for (int i = packCount; i > 0; i--)
        {
            pack.assign(str.begin() + o, i != 1 ? str.begin() + o + SIZE : str.end());
            o += SIZE;


            data << "part" << i;
            data << "str" << pack;

            string encodePACK = data.json();
            //string encodePACK = base64_encode(reinterpret_cast<const unsigned char*>(data.dump().c_str()), data.dump().size());
            Delivery_Warranty(encodePACK, i);
            //g_current.push_back(j.dump());
        }

    }
    else
    {
        data << "part" << 1;
        data << "str" << str;
        string encodePACK = data.json();
        //string encodePACK = base64_encode(reinterpret_cast<const unsigned char*>(data.dump().c_str()), data.dump().size());
        Delivery_Warranty(encodePACK, 1);
    }
}