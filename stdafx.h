#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <string>
#include <Windows.h>
#include "jsonxx.h"
#include <iphlpapi.h>
#include <icmpapi.h>
#include <fstream>
#include <gdiplus.h>
#include <time.h>
#include"shlwapi.h"
#include "base64.h"
#include <objbase.h>
#include <fstream>
#include <vector>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment( lib, "gdiplus" )
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "gdiplus.lib")

#pragma warning(disable : 4996)

using namespace Gdiplus;
using namespace jsonxx;
using namespace std;


#define MaxPackSize     1400    //buffer
#define SERVER_PORT     8866   //server port #
#define serverIP    "192.168.31.46"