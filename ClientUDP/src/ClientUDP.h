/*
 ============================================================================
 Name        : ClientUDP.h
 Author      : Balde Omar, De Santis Samantha
 Version     :
 Copyright   : Free for educational purposes
 Description : Header file for ClientUDP
 ============================================================================
 */

#ifndef CLIENTUDP_H_
#define CLIENTUDP_H_

void errorHandler(char*);
void clearWinSock();

void extraSpaces(char*);
char* leadingSpaces(char*);

void settingAddresses(struct sockaddr_in *, int, char*);
struct sockaddr_in sockBuild(int *, int, char **);

char* translateIntoString(char *);
char* translateIntoInt(char *);

int splitString(char *, char *, char *);

#endif /* CLIENTUDP_H_ */
