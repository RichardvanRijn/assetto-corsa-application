#ifndef _Serial_H_
#define _Serial_H_

HANDLE initCom(int comPortNumber);

char readCom(HANDLE hCom);

int writeCom(HANDLE hCom, char c);

int writeComNumber(HANDLE hCom, UINT8 c);

HANDLE detectPort(COMMTIMEOUTS timeouts);

void closeCom(HANDLE hCom);

#endif