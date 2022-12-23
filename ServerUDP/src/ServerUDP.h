/*
 ============================================================================
 Name        : ServerUDP.h
 Author      : Balde Omar, De Santis Samantha
 Version     :
 Copyright   : Free for educational purposes
 Description : Header file for ServerUDP
 ============================================================================
 */

#ifndef SERVERUDP_H_
#define SERVERUDP_H_

void clearWinSock();
void errorHandler(char*);
void leave(int);

int legitOperator(char);
int legitInput(char*);
int numericCheck(char*, char*);

void populateValues(char*, char*, char*);

char* sum(int, int);
char* sub(int, int);
char* mult(int, int);
char* division(int, int);
char* calculation(char*, char*, char*);

void settingAddresses(struct sockaddr_in *, int, char*);
struct sockaddr_in sockBuild(int *, int, char **);

char* translateIntoString(char *);
char* translateIntoInt(char *);

int splitString(char *, char *, char *);

#endif /* SERVERUDP_H_ */
