#include "stdafx.h"

//#include <stdio.h>
//#include <windows.h>
//#include <time.h>
//#include <math.h>
//#include <stdbool.h>

HANDLE initCom(int comPortNumber) {
	DCB dcb;
	HANDLE hCom;
	BOOL fSuccess;
	char comPortName[20];
	sprintf(comPortName, "\\\\.\\COM%d", comPortNumber); //moet naar echte C++ gezet worden

//	std::string comPortName;
//	std::ostringstream out;

//	out << "\\\\.\\COM" << comPortNumber;

	hCom = CreateFileA(comPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hCom == INVALID_HANDLE_VALUE) {
//		MessageBoxA(0, "Fout: De seriële poort kan niet geopend worden!", "Fout", MB_OK | MB_ICONEXCLAMATION);
//		exit(-1);
		return false;
	}
	fSuccess = GetCommState(hCom, &dcb);
	if (!fSuccess) {
//		MessageBoxA(0, "Fout: De instellingen van de seriële poort kunnen niet gelezen worden!", "Fout", MB_OK | MB_ICONEXCLAMATION);
//		exit(-1);
		return false;
	}
	dcb.fParity = TRUE;
	dcb.BaudRate = CBR_57600;
	dcb.ByteSize = 8;
	dcb.Parity = EVENPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = FALSE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	fSuccess = SetCommState(hCom, &dcb);
	if (!fSuccess) {
//		MessageBoxA(0, "Fout: De instellingen van de seriële poort kunnen niet aangepast worden!", "Fout", MB_OK | MB_ICONEXCLAMATION);
//		exit(-1);
		return false;
	}

	return hCom;
}

char readCom(HANDLE hCom) {
	char inBuffer[2] = " ";
	unsigned long nBytesRead;
	ReadFile(hCom, &inBuffer, 1, &nBytesRead, NULL);
	if (inBuffer[0] && (nBytesRead == 0)){
		return false;
	}
	return inBuffer[0];
}

int writeCom(HANDLE hCom, char c) {
	char outBuffer[2] = " ";
	unsigned long nBytesWritten;
	outBuffer[0] = c;
	WriteFile(hCom, &outBuffer, 1, &nBytesWritten, NULL);
	if (nBytesWritten != 1) {
//		MessageBoxA(0, "Fout: De seriële poort kan niet beschreven worden!", "Fout", MB_OK | MB_ICONEXCLAMATION);
//		exit(-1);
		return false;
	}
	return true;
}

int writeComNumber(HANDLE hCom, UINT8 c) {
	char outBuffer[2] = " ";
	unsigned long nBytesWritten;
	outBuffer[0] = c;
	WriteFile(hCom, &outBuffer, 1, &nBytesWritten, NULL);
	if (nBytesWritten != 1) {
//		MessageBoxA(0, "Fout: De seriële poort kan niet beschreven worden!", "Fout", MB_OK | MB_ICONEXCLAMATION);
//		exit(-1);
		return false;
	}
	return true;
}

HANDLE detectPort(COMMTIMEOUTS timeouts){

	HANDLE comp = 0;
	int portnr = 0;
	char in;
	bool x = false;
	bool y = false;
	
	SYSTEMTIME st_start, st_end;

	while (y == false){
		while (x == false){
			if (portnr == 255){
				MessageBoxA(0, "Fout: De seriële poort kan niet gevonden worden!", "Fout", MB_OK | MB_ICONEXCLAMATION);
				exit(-1);
			}
			portnr++;
			if ((comp = initCom(portnr)) == 0){
				x = false;
				std::cout << "Kan port niet openen" << std::endl;
			}
			else {
				x = true;
			}
		}
		std::cout << "port open" << std::endl;
		if (!SetCommTimeouts(comp, &timeouts)){
			std::cout << "time out error" << std::endl;
			fflush(stdin);
			getchar();
		}
		GetSystemTime(&st_start);
		GetSystemTime(&st_end);
		if ((st_start.wMilliseconds + 7) >= 993){
			st_start.wMilliseconds = 7;
		}
		writeCom(comp, 'X');
		while ((st_end.wMilliseconds < st_start.wMilliseconds + 7) && y != true){
			in = readCom(comp);
			if (in == 'Z'){
				y = true;
				return comp;
			}
			GetSystemTime(&st_end);
		}
		if (y == false){
			std::cout << "fout" << std::endl;
			closeCom(comp);
			y = false;
			x = false;
		}
	}
}

void closeCom(HANDLE hCom) {
	CloseHandle(hCom);
}