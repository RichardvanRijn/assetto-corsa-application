#include "stdafx.h"
#include "SerialCPP.h"


SerialCPP::SerialCPP(int comPortNumber)
{
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
//		return false;
	}
	fSuccess = GetCommState(hCom, &dcb);
	if (!fSuccess) {
		//		MessageBoxA(0, "Fout: De instellingen van de seriële poort kunnen niet gelezen worden!", "Fout", MB_OK | MB_ICONEXCLAMATION);
		//		exit(-1);
//		return false;
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
//		return false;
	}

//	return hCom;
}



SerialCPP::~SerialCPP()
{
}
