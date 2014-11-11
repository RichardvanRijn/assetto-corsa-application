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
	sprintf(comPortName, "\\\\.\\COM%d", comPortNumber);

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
	dcb.BaudRate = CBR_9600;
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
}

void closeCom(HANDLE hCom) {
	CloseHandle(hCom);
}