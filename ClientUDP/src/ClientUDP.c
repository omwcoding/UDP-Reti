/*
 ============================================================================
 Name        : ClientUDP.c
 Author      : Balde Omar, De Santis Samantha
 Version     :
 Copyright   : Free for educational purposes
 Description : Source file for ClientUDP
 ============================================================================
 */

#if defined WIN32
#include <winsock.h>
#else
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket close
#endif
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include "ClientUDP.h"

#define PROTOPORT 48000 //Define default port number protocol
#define IP "localhost" //Define default IP
#define MAXBUFFER 255 //Define maximum buffer size

int main(int argc, char *argv[]) {

// Initializing Winsock
#if defined WIN32
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0 ) {
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif
	int c_socket;
	int check = 1;
	c_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (c_socket != -1) {
		//SOCKET ADDRESSES BUILD
		struct sockaddr_in sad = sockBuild(&check, argc, argv);
		struct sockaddr_in sadCheck;
		int sadCheckLen = sizeof(sadCheck);
		memset(&sadCheck, 0, sizeof(sadCheck));
		if (check) {
			char input[MAXBUFFER];
			char resultant[MAXBUFFER];
			char *rmvSpace;
			char safeString[MAXBUFFER];
			while (1) {
				memset(input, 0, sizeof(input));
				system("cls");
				printf(
						"Make an operation, inserting parameters in this order:\n*operator(+,-,*,/)* "
								"*integer_value_1* *integer_value 2*\nex: + 23 45\nPress = to quit connection\n\n\n");
				printf("Operation: ");
				gets(input);
				rmvSpace = leadingSpaces(input);
				//"=" IS THE QUIT COMMAND
				if ((rmvSpace[0] == '=') && (rmvSpace[1] == '\0')) {
					system("cls");
					sleep(1);
					closesocket(c_socket);
					clearWinSock();
					return 1;
				} else {
					strcpy(safeString, rmvSpace);
					if (sendto(c_socket, safeString, sizeof(safeString), 0,
							(struct sockaddr*) &sad, sizeof(sad))
							== sizeof(safeString)) {
						//CLIENTS WAITS FOR AN ANSWER
						recvfrom(c_socket, resultant, sizeof(resultant), 0,
								(struct sockaddr*) &sadCheck, &sadCheckLen);
						if (sad.sin_addr.s_addr == sadCheck.sin_addr.s_addr) {
							system("cls");
							printf(
									"\n\n\nResult received from server %s, ip %s: %s\n\n\n",
									translateIntoString(
											inet_ntoa(sad.sin_addr)),
									inet_ntoa(sad.sin_addr), resultant);
							system("PAUSE");
						} else {
							errorHandler("Failed to receive.\n");
							system("PAUSE");
							closesocket(c_socket);
							clearWinSock();
							return -1;
						}
					} else {
						errorHandler("Failed to send.\n");
						system("PAUSE");
						closesocket(c_socket);
						clearWinSock();
						return -1;
					}
				}
				memset(rmvSpace, 0, sizeof(char[MAXBUFFER]));
				memset(safeString, 0, sizeof(char[MAXBUFFER]));
			}
		} else {
			errorHandler("Socket Build failed.\n");
			clearWinSock();
			return -1;
		}
	} else {
		errorHandler("Socket Creation failed.\n");
		clearWinSock();
		return -1;
	}
	system("PAUSE");
}


// TRANSFORM INPUT LIKE "+      23   45"
// INTO "+ 23 45"
void extraSpaces(char *str) {
	int i, x;
	for (i = x = 0; str[i]; ++i)
		if (!isspace(str[i]) || ((i > 0) && !isspace(str[i - 1]))) {
			str[x++] = str[i];
		}
	str[x] = '\0';
}

// TRANSFORM INPUT LIKE "      + 23 45"
// INTO "+ 23 45"
char* leadingSpaces(char *str) {
	static char str1[MAXBUFFER];
	int count = 0, j, k;
	while (str[count] == ' ') {
		count++;
	}
	for (j = count, k = 0; str[j] != '\0'; j++, k++) {
		str1[k] = str[j];
	}
	str1[k] = '\0';
	extraSpaces(str1);
	return str1;
}



char* translateIntoInt(char *input) {
	struct hostent *host;
	host = gethostbyname(input);

	if (host == NULL) {
		errorHandler("Unavailable host.\n");
		exit(EXIT_FAILURE);
	} else {
		struct in_addr *ina = (struct in_addr*) host->h_addr_list[0];
		return inet_ntoa(*ina);
	}
}

char* translateIntoString(char *input) {
	struct in_addr addr;
	struct hostent *host;

	addr.s_addr = inet_addr(input);
	host = gethostbyaddr((char*) &addr, 4, AF_INET);
	char *canonical_name = host->h_name;
	return canonical_name;
}

int splitString(char *input, char *first, char *second) {
	int ok = 0;
	int i = 0;
	while (input[i] != '\0') {
		if (input[i] == ':' && (i != 0) && input[i + 1] != '\0') {
			ok = 1;
		}
		i++;
	}
	i = 0;
	if (ok) {
		while (input[i] != ':') {
			first[i] = input[i];
			i++;
		}
		i++;
		int j = 0;
		while (input[i] != '\0') {
			second[j] = input[i];
			i++;
			j++;
		}
	}
	return ok;
}

void errorHandler(char *errorMessage) {
	printf("%s", errorMessage);
}

void clearWinSock() {
#if defined WIN32
	WSACleanup();
#endif
}

// FILL SOCKET STRUCTURE: IP and PORT
void settingAddresses(struct sockaddr_in *sad, int port, char *ip) {
	if (isdigit(ip[0]) && (isdigit(ip[1]) || ip[1] == '.')
			&& (isdigit(ip[2]) || ip[2] == '.')
			&& (isdigit(ip[3]) || ip[3] == '.')) {
		sad->sin_addr.s_addr = inet_addr(ip);
	} else {
		sad->sin_addr.s_addr = inet_addr(translateIntoInt(ip));
	}
	sad->sin_port = htons(port);
}

// INSERT ARGVS IN SOCKET STRUCTURE
// IF ARGUMENTS WERE PASSED DURING SERVER RUN
struct sockaddr_in sockBuild(int *ok, int argc, char *argv[]) {
	struct sockaddr_in cad;
	memset(&cad, 0, sizeof(cad));
	cad.sin_family = AF_INET;
	if (argc == 1) {
		settingAddresses(&cad, PROTOPORT, IP);
	} else if (argc == 2) {
		char line[100];
		char canonical[90];
		char pNumber[10];
		strcpy(line, argv[1]);
		if (splitString(line, canonical, pNumber)) {
			int port = atoi(pNumber);
			if (port < 65536 && port > 0) {
				settingAddresses(&cad, port, canonical);
			} else {
				*ok = 0;
				memset(&cad, 0, sizeof(cad));
				errorHandler("Bad port number\n");
			}
		} else {
			*ok = 0;
			memset(&cad, 0, sizeof(cad));
			errorHandler("Invalid arguments\n");
		}
	} else {
		*ok = 0;
		memset(&cad, 0, sizeof(cad));
		errorHandler("Invalid arguments\n");
	}
	return cad;
}
