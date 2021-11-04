#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h> 
#include <string.h>

#define TIME_PORT	27015
//#define HUD_PORT	27016
//#define WHEATHER_PORT	27017

#define GetTime  1
#define GetTimeWithoutDate  2
#define GetTimeSinceEpoch  3
#define GetClientToServerDelayEstimation  4
#define MeasureRTT 5
#define GetTimeWithoutDateOrSeconds 6
#define GetYear 7
#define GetMonthAndDay 8 
#define GetSecondsSinceBeginingOfMonth 9
#define GetWeekOfYear 10
#define GetDaylightSavings 11
#define GetTimeWithoutDateInCity 12
#define MeasureTimeLap 13
#define EXIT_PROGRAM 14


void printUserMenu()
{
	std::cout << "Please choose an action:" << endl;
	std::cout << "1. What's the time and date?" << endl;
	std::cout << "2. What's the time now?" << endl;
	std::cout << "3. What's the time now in seconds since 1.1.1970?" << endl;
	std::cout << "4. what's the client to server delay estimation?" << endl;
	std::cout << "5. Measure RTT please" << endl;
	std::cout << "6. What's the time now? ('hour:seconds')" << endl;
	std::cout << "7. What year is it?" << endl;
	std::cout << "8. What day and month is it today?" << endl;
	std::cout << "9. How many seconds have passed since the beginning of the month?" << endl;
	std::cout << "10. How many weeks have passed since the beginning of the year?" << endl;
	std::cout << "11. Is it Daylight saving time or winter clock?" << endl; // is it good?
	std::cout << "12. What's the time now in different cities of the world?" << endl;
	std::cout << "13. Please tell me the measure time lap" << endl;
	std::cout << "14. Exit Program" << endl;
}

int updateUserInput(int userInput, char* sendBuff)
{
	if (userInput == GetTime)
	{
		strcpy(sendBuff, "1");
	}
	else if (userInput == GetTimeWithoutDate)
	{
		strcpy(sendBuff, "2");
	}
	else if (userInput == GetTimeSinceEpoch)
	{
		strcpy(sendBuff, "3");
	}
	else if (userInput == GetClientToServerDelayEstimation)
	{
		strcpy(sendBuff, "4");
	}
	else if (userInput == MeasureRTT)
	{
		strcpy(sendBuff, "5");
	}
	else if (userInput == GetTimeWithoutDateOrSeconds)
	{
		strcpy(sendBuff, "6");
	}
	else if (userInput == GetYear)
	{
		strcpy(sendBuff, "7");
	}
	else if (userInput == GetMonthAndDay)
	{
		strcpy(sendBuff, "8");
	}
	else if (userInput == GetSecondsSinceBeginingOfMonth)
	{
		strcpy(sendBuff, "9");
	}
	else if (userInput == GetWeekOfYear)
	{
		strcpy(sendBuff, "10");
	}
	else if (userInput == GetDaylightSavings)
	{
		strcpy(sendBuff, "11");
	}
	else if (userInput == GetTimeWithoutDateInCity)
	{
		strcpy(sendBuff, "12");
	}
	else if (userInput == MeasureTimeLap)
	{
		strcpy(sendBuff, "13");
	}
	else
	{
		return 0;
	}

	return 1;
}

void main()
{

	// Initialize Winsock (Windows Sockets).

	WSAData wsaData;
	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Time Client: Error at WSAStartup()\n";
		return;
	}

	// Client side:
	// Create a socket and connect to an internet address.

	SOCKET connSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == connSocket)
	{
		cout << "Time Client: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// For a client to communicate on a network, it must connect to a server.

	// Need to assemble the required data for connection in sockaddr structure.

	// Create a sockaddr_in object called server. 
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("13.52.80.36");
	server.sin_port = htons(TIME_PORT);

	int userInput = 0;

	while (userInput != EXIT_PROGRAM)
	{
		// Send and receive data.
		printUserMenu();
		cin >> userInput;

		//char sendBuff[255];
		char sendBuff[255];// = new char[255];

		if (updateUserInput(userInput, sendBuff) == 0)
		{
			break;
		}

		int bytesSent = 0;
		int bytesRecv = 0;
		//char sendBuff[255] = "Whats the time?";
		char recvBuff[255];

		// Asks the server what's the currnet time.
		// The send function sends data on a connected socket.
		// The buffer to be sent and its size are needed.
		// The fourth argument is an idicator specifying the way in which the call is made (0 for default).
		// The two last arguments hold the details of the server to communicate with. 
		// NOTE: the last argument should always be the actual size of the client's data-structure (i.e. sizeof(sockaddr)).
		//int x = 1; // 3
		//x = htonl(x); // 3

		//bytesSent = sendto(connSocket, (char*)&x, 4, 0, (const sockaddr *)&server, sizeof(server)); //3

		bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr *)&server, sizeof(server)); // 2
		if (SOCKET_ERROR == bytesSent)
		{
			cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
			closesocket(connSocket);
			WSACleanup();
			return;
		}
		cout << "Time Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";

		// Gets the server's answer using simple recieve (no need to hold the server's address).
		bytesRecv = recv(connSocket, recvBuff, 255, 0);
		if (SOCKET_ERROR == bytesRecv)
		{
			cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
			closesocket(connSocket);
			WSACleanup();
			return;
		}

		recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
		cout << "Time Client: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";
	}


	// Closing connections and Winsock.
	cout << "Time Client: Closing Connection.\n";
	closesocket(connSocket);

	system("pause");
}