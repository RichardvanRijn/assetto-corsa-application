// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <windows.h>
//#include <tchar.h>
//#include <iostream>
//#include "SharedFileOut.h"
//#include "Serial.h"

using namespace std;

HANDLE hMapFile;
unsigned char* mapFileBuffer;

template <typename T, unsigned S>
inline unsigned arraysize(const T(&v)[S])
{
	return S;
}

struct SMElement
{
	HANDLE hMapFile;
	unsigned char* mapFileBuffer;
};

SMElement m_graphics;
SMElement m_physics;
SMElement m_static;

void initPhysics()
{
	TCHAR szName[] = TEXT("Local\\acpmf_physics");
	m_physics.hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SPageFilePhysics), szName);
	if (!m_physics.hMapFile)
	{
		MessageBoxA(GetActiveWindow(), "CreateFileMapping failed", "ACS", MB_OK);
	}
	m_physics.mapFileBuffer = (unsigned char*)MapViewOfFile(m_physics.hMapFile, FILE_MAP_READ, 0, 0, sizeof(SPageFilePhysics));
	if (!m_physics.mapFileBuffer)
	{
		MessageBoxA(GetActiveWindow(), "MapViewOfFile failed", "ACS", MB_OK);
	}
}

void initGraphics()
{
	TCHAR szName[] = TEXT("Local\\acpmf_graphics");
	m_graphics.hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SPageFileGraphic), szName);
	if (!m_graphics.hMapFile)
	{
		MessageBoxA(GetActiveWindow(), "CreateFileMapping failed", "ACS", MB_OK);
	}
	m_graphics.mapFileBuffer = (unsigned char*)MapViewOfFile(m_graphics.hMapFile, FILE_MAP_READ, 0, 0, sizeof(SPageFileGraphic));
	if (!m_graphics.mapFileBuffer)
	{
		MessageBoxA(GetActiveWindow(), "MapViewOfFile failed", "ACS", MB_OK);
	}
}

void initStatic()
{
	TCHAR szName[] = TEXT("Local\\acpmf_static");
	m_static.hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SPageFileStatic), szName);
	if (!m_static.hMapFile)
	{
		MessageBoxA(GetActiveWindow(), "CreateFileMapping failed", "ACS", MB_OK);
	}
	m_static.mapFileBuffer = (unsigned char*)MapViewOfFile(m_static.hMapFile, FILE_MAP_READ, 0, 0, sizeof(SPageFileStatic));
	if (!m_static.mapFileBuffer)
	{
		MessageBoxA(GetActiveWindow(), "MapViewOfFile failed", "ACS", MB_OK);
	}
}

void dismiss(SMElement element)
{
	UnmapViewOfFile(element.mapFileBuffer);
	CloseHandle(element.hMapFile);
}

int _tmain(int argc, _TCHAR* argv[])
{
	UINT8 i = 0;
	int portnr = 0;
	int zendbuffer = 0;
	char in;
	char buffer = 'A';
	bool x = false;
	bool y = false;

	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 1;
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.ReadTotalTimeoutConstant = 1;
	timeouts.WriteTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 1;

	SYSTEMTIME st_start, st_end;

	initPhysics();
	initGraphics();
	initStatic();

	HANDLE comp = 0;

	while (y == false){
		while (x == false){
//			cout << "insert comPortNumber" << endl;
//			fflush(stdin);
//			cin >> portnr;
			if (portnr == 255){
				MessageBoxA(0, "Fout: De port kan niet gevonden worden", "Fout", MB_OK | MB_ICONEXCLAMATION);
				exit(-1);
			}
			portnr++;
			if ((comp = initCom(portnr)) == 0){
				x = false;
				cout << "Kan port niet openen" << endl;
			}
			else {
				x = true;
			}
		}
		cout << "port open" << endl;
		if (!SetCommTimeouts(comp, &timeouts)){
			cout << "time out error" << endl;
			fflush(stdin);
			getchar();
		}
		GetSystemTime(&st_start);
		GetSystemTime(&st_end);
		if ((st_start.wMilliseconds + 7) >= 993){
			st_start.wMilliseconds = 7;
		}
//		cout << st_start.wMilliseconds << " " << st_start.wMilliseconds + 7 << endl;
		writeCom(comp, 'X');
		while ((st_end.wMilliseconds < st_start.wMilliseconds + 7) && y != true){
			in = readCom(comp);
			if (in == 'Z'){
				y = true;
			}
			GetSystemTime(&st_end);
//			cout << st_end.wMilliseconds << endl;
		}
		if (y == false){
			cout << "fout" << endl;
			closeCom(comp);
			y = false;
			x = false;
		}
	}
			cout << "goed" << endl;
			fflush(stdin);
			getchar();
	while(true)
	{
		SPageFilePhysics* pf = (SPageFilePhysics*)m_physics.mapFileBuffer;
		SPageFileGraphic* pf = (SPageFileGraphic*)m_graphics.mapFileBuffer;
		SPageFileStatic* pf = (SPageFileStatic*)m_static.mapFileBuffer;

		in = readCom(comp);

		if (in != 0){
			buffer = in;
			in = 0;
		}


		switch (buffer){
		case 'A':
			writeCom(comp, 'A');
			//zendbuffer = pf->rpms;
			zendbuffer = 258;
			writeComNumber(comp, zendbuffer);
			zendbuffer = (zendbuffer >> 8);
			writeComNumber(comp, zendbuffer);
			cout << "Toeren " << pf->rpms << " Rpm" << endl;
			break;
		case 'B':
			writeCom(comp, 'B');
			//zendbuffer = pf->speedKmh;
			zendbuffer = 384;
			writeComNumber(comp, zendbuffer);
			zendbuffer = (zendbuffer >> 8);
			writeComNumber(comp, zendbuffer);
			cout << "Snelheid " << pf->speedKmh << " Kmh" << endl;
			break;
		default:
			buffer = 'A';
			break;
		}
				
		system("cls");
//		cout << "Toeren "<< pf->rpms << " Rpm" << endl;
//		cout << "Snelheid " << pf->speed << " Mps" << endl;

//		Sleep(2);
	}

	closeCom(comp);
	dismiss(m_graphics);
	dismiss(m_physics);
	dismiss(m_static);

	return 0;
}