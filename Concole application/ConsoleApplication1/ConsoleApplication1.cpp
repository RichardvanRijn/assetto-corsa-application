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
//	int portnr = 0;
	char in;
	int zendbuffer = 0;
	char buffer = 'B';
//	bool x = false;
//	bool y = false;
	
//	SYSTEMTIME st_start, st_end;
	HANDLE comp = 0;

	initPhysics();
	initGraphics();
	initStatic();

	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 1;
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.ReadTotalTimeoutConstant = 1;
	timeouts.WriteTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 1;

	comp = detectPort(timeouts);

	cout << "goed" << endl;
	fflush(stdin);
	getchar();
	while (!_kbhit())
	{
		SPageFilePhysics* Ph = (SPageFilePhysics*)m_physics.mapFileBuffer;
		SPageFileGraphic* Gr = (SPageFileGraphic*)m_graphics.mapFileBuffer;
		SPageFileStatic* St = (SPageFileStatic*)m_static.mapFileBuffer;

		in = readCom(comp);

		if (in != 0){
			buffer = in;
			in = 0;
		}

		switch (buffer){
		case 'A':
			writeCom(comp, 'A');
			zendbuffer = Ph->rpms;
			//zendbuffer = 258;
			cout << zendbuffer << endl;
			writeComNumber(comp, zendbuffer);
			zendbuffer = (zendbuffer >> 8);
			cout << zendbuffer << endl;
			writeComNumber(comp, zendbuffer);
			cout << "Toeren " << Ph->rpms << " Rpm" << endl;
			break;
		case 'B':
			writeCom(comp, 'B');
			zendbuffer = Ph->speedKmh;
			//zendbuffer = 384;
			cout << zendbuffer << endl;
			writeComNumber(comp, zendbuffer);
			zendbuffer = (zendbuffer >> 8);
			cout << zendbuffer << endl;
			writeComNumber(comp, zendbuffer);
			cout << "Snelheid " << Ph->speedKmh << " Kmh" << endl;
			break;
		default:
			buffer = 'B';
			break;
		}
				
		system("cls");
//		cout << "Toeren "<< pf->rpms << " Rpm" << endl;
//		cout << "Snelheid " << pf->speed << " Mps" << endl;

		Sleep(2);
	}

	closeCom(comp);
	dismiss(m_graphics);
	dismiss(m_physics);
	dismiss(m_static);

	return 0;
}