#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h> 
#include <string.h>

#define TIME_PORT	27015

#define GET_TIME  1
#define GET_TIME_WITHOUT_DATE  2
#define GET_TIME_SINCE_EPOCH  3
#define GET_CLIENT_TO_SERVER_DELAY_ESTIMATION  4
#define MEASURE_RTT 5
#define GET_TIME_WITHOUT_DATE_OR_SECONDES 6
#define GET_YEAR 7
#define GET_MONTH_AND_DAY 8 
#define GET_SECONDES_SINCE_BEGINING_OF_MONTH 9
#define GET_WEEK_OF_YEAR 10
#define GET_DAY_LIGHT_SAVINGS 11
#define GET_TIME_WITHOUT_DATE_IN_CITY 12
#define MEASURE_TIME_LAP 13
#define EXIT_PROGRAM 14

void clearScreen()
{
	system("cls");
}

void printUserMenu()
{
	std::cout << "Please choose an action:" << endl;
	std::cout << "1. What's the time and date?" << endl;
	std::cout << "2. What's the time now?" << endl;
	std::cout << "3. What's the time now in seconds since 1.1.1970?" << endl;
	std::cout << "4. what's the client to server delay estimation?" << endl;
	std::cout << "5. Measure RTT please" << endl;
	std::cout << "6. What's the time now? ('hour:minutes')" << endl;
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
	if (userInput == GET_TIME)
	{
		strcpy(sendBuff, "1");
	}
	else if (userInput == GET_TIME_WITHOUT_DATE)
	{
		strcpy(sendBuff, "2");
	}
	else if (userInput == GET_TIME_SINCE_EPOCH)
	{
		strcpy(sendBuff, "3");
	}
	else if (userInput == GET_CLIENT_TO_SERVER_DELAY_ESTIMATION)
	{
		strcpy(sendBuff, "4");
	}
	else if (userInput == MEASURE_RTT)
	{
		strcpy(sendBuff, "5");
	}
	else if (userInput == GET_TIME_WITHOUT_DATE_OR_SECONDES)
	{
		strcpy(sendBuff, "6");
	}
	else if (userInput == GET_YEAR)
	{
		strcpy(sendBuff, "7");
	}
	else if (userInput == GET_MONTH_AND_DAY)
	{
		strcpy(sendBuff, "8");
	}
	else if (userInput == GET_SECONDES_SINCE_BEGINING_OF_MONTH)
	{
		strcpy(sendBuff, "9");
	}
	else if (userInput == GET_WEEK_OF_YEAR)
	{
		strcpy(sendBuff, "10");
	}
	else if (userInput == GET_DAY_LIGHT_SAVINGS)
	{
		strcpy(sendBuff, "11");
	}
	else if (userInput == GET_TIME_WITHOUT_DATE_IN_CITY)
	{
		strcpy(sendBuff, "12");
	}
	else if (userInput == MEASURE_TIME_LAP)
	{
		strcpy(sendBuff, "13");
	}
	else
	{
		return 0;
	}

	return 1;
}

bool socketError(int bytes, SOCKET connSocket, string errorAt)
{
	if (SOCKET_ERROR == bytes)
	{
		cout << "Time Client: Error at" << errorAt <<":" << WSAGetLastError() << endl;
		closesocket(connSocket);
		WSACleanup();
		return true;
	}

	return false;
}

void printMessegeFromServer(int userInput, char* recvBuff, int bytesRecv)
{
	cout << "Time Client: Recieved: " << bytesRecv << " bytes" << endl;
	cout << "The message is:" << endl;

	if (userInput == GET_TIME)
	{
		cout << "The time is: " << recvBuff << endl;
	}
	else if (userInput == GET_TIME_WITHOUT_DATE)
	{
		cout << "The time (without date) is: " << recvBuff << endl;
	}
	else if (userInput == GET_TIME_SINCE_EPOCH)
	{
		cout << "The time now in seconds since 1.1.1970 is: " << recvBuff << " secondes" << endl; 
	}
	else if (userInput == GET_TIME_WITHOUT_DATE_OR_SECONDES)
	{
		cout << "The time (without date and secondes) is: " << recvBuff << endl;
	}
	else if (userInput == GET_YEAR)
	{
		cout << "The year is: " << recvBuff << endl;
	}
	else if (userInput == GET_MONTH_AND_DAY)
	{
		cout << "The month and day are: " << recvBuff << endl;
	}
	else if (userInput == GET_SECONDES_SINCE_BEGINING_OF_MONTH)
	{
		cout << "The amount of secondes from the begining of the month are: " << recvBuff << " secondes" << endl;
	}
	else if (userInput == GET_WEEK_OF_YEAR)
	{
		cout << "The number of the week from the beginning of the year is: " << recvBuff << endl; 
	}
	else if (userInput == GET_DAY_LIGHT_SAVINGS)
	{
		cout << "Day light savings is: " << recvBuff << endl;
	}
	else if (userInput == GET_TIME_WITHOUT_DATE_IN_CITY)
	{
		cout << recvBuff << endl;
	}
	else if (userInput == MEASURE_TIME_LAP)
	{
		if(strcmp(recvBuff, "-1") == 0)
		{
			cout << "Measure time started now." << endl << "The next request for measuring time will return the time measurement." << endl;
		}
		else
		{
			cout << "The time measurement between both requests is: " << recvBuff << " secondes" << endl;
		}
	}
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
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(TIME_PORT);

	int userInput = 0;

	while (userInput != EXIT_PROGRAM)
	{
		// Send and receive data.
		printUserMenu();
		cin >> userInput;

		char sendBuff[255];

		if (updateUserInput(userInput, sendBuff) == 0)
		{
			break;
		}

		int bytesSent = 0;
		int bytesRecv = 0;
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

		if (userInput == GET_CLIENT_TO_SERVER_DELAY_ESTIMATION)
		{
			for (int i = 0; i < 100; i++) // send 100 request for whats the time
			{
				bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr *)&server, sizeof(server)); // 2
				
				if (socketError(bytesSent, connSocket, "sendto()"))
				{
					return;
				}
				/*if (SOCKET_ERROR == bytesSent)
				{
					cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
					closesocket(connSocket);
					WSACleanup();
					return;
				}*/
			}

			bytesRecv = recv(connSocket, recvBuff, 255, 0);

			if (socketError(bytesRecv, connSocket, " recv()"))
			{
				return;
			}
			/*if (SOCKET_ERROR == bytesRecv)
			{
				cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
				closesocket(connSocket);
				WSACleanup();
				return;
			}*/

			recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
			char *ptr;
			long int first = strtol(recvBuff, &ptr, 10);
			double res = 0;
			double sum = 0;

			for (int i = 0; i < 99; i++) // send 100 request for whats the time
			{
				bytesRecv = recv(connSocket, recvBuff, 255, 0);

				if (socketError(bytesRecv, connSocket, " recv()"))
				{
					return;
				}
				/*if (SOCKET_ERROR == bytesRecv)
				{
					cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
					closesocket(connSocket);
					WSACleanup();
					return;
				}*/

				recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
				char *ptr;
				long int second = strtol(recvBuff, &ptr, 10);

				sum += second - first;
				first = second;
			}

			double avg = (double)sum / 99; // there are 99 pairs
			cout << "Client To Server Delay Estimation is: " << avg << " secondes" << endl;
		}

		else if (userInput == MEASURE_RTT)
		{
			double sum = 0;
			for (int i = 0; i < 100; i++) // send 100 request for whats the time
			{
				bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr *)&server, sizeof(server)); // 2
				long int timeClient = GetTickCount();
				if (socketError(bytesSent, connSocket, "sendto()"))
				{
					return;
				}

				/*if (SOCKET_ERROR == bytesSent)
				{
					cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
					closesocket(connSocket);
					WSACleanup();
					return;
				}*/

				bytesRecv = recv(connSocket, recvBuff, 255, 0);

				if (socketError(bytesRecv, connSocket, " recv()"))
				{
					return;
				}

				/*if (SOCKET_ERROR == bytesRecv)
				{
					cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
					closesocket(connSocket);
					WSACleanup();
					return;
				}*/

				recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
				char *ptr;
				long int timeServer = strtol(recvBuff, &ptr, 10);
				sum += timeServer - timeClient;
			}

			double avg = (double)sum / 100; // there are 100 requsets
			cout << "RTT: " << avg << " secondes" << endl;
		}


		else
		{
			// regular
			bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr *)&server, sizeof(server)); // 2
			if (socketError(bytesSent, connSocket, "sendto()"))
			{
				return;
			}

			/*if (SOCKET_ERROR == bytesSent)
			{
				cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
				closesocket(connSocket);
				WSACleanup();
				return;
			}*/
			cout << "Time Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";

			// Gets the server's answer using simple recieve (no need to hold the server's address).
			bytesRecv = recv(connSocket, recvBuff, 255, 0);

			if (socketError(bytesRecv, connSocket, " recv()"))
			{
				return;
			}

			/*if (SOCKET_ERROR == bytesRecv)
			{
				cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
				closesocket(connSocket);
				WSACleanup();
				return;
			}*/

			recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
			
			printMessegeFromServer(userInput, recvBuff, bytesRecv);

			//cout << "Time Client: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";
		}
		//Sleep(3);
		//clearScreen();
		cout << endl << endl;
	}

	// Closing connections and Winsock.
	cout << "Time Client: Closing Connection.\n";
	closesocket(connSocket);

	system("pause");
}