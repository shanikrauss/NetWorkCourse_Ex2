#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h> 
#include <string.h>

using namespace std;


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

#define TOKYO 1
#define MELBOURNE 2
#define SAN_FRANCISCO 3
#define PORTO 4
#define UTC 5


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
	std::cout << "11. Is it Daylight saving time or winter clock?" << endl;
	std::cout << "12. What's the time now in different cities of the world?" << endl;
	std::cout << "13. Please tell me the measure time lap" << endl;
	std::cout << "14. Exit Program" << endl;
}

void updateSendBuffByUserInput(int userInput, char* sendBuff, int city)
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
		if (city == TOKYO)
		{
			strcpy(sendBuff, "1T");
		}
		else if (city == MELBOURNE)
		{
			strcpy(sendBuff, "2M");
		}
		else if (city == SAN_FRANCISCO)
		{
			strcpy(sendBuff, "3S");
		}
		else if (city == PORTO)
		{
			strcpy(sendBuff, "4P");
		}
		else // city == UTC
		{
			strcpy(sendBuff, "5U");
		}
	}
	else // (userInput == MEASURE_TIME_LAP)
	{
		strcpy(sendBuff, "13");
	}
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

void printMessegeFromServer(int userInput, char* recvBuff, int bytesRecv, int city, char* cityInput)
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
		if (city == TOKYO)
		{
			cout << "TOKYO ";
		}
		else if (city == MELBOURNE)
		{
			cout << "MELBOURNE ";
		}
		else if (city == SAN_FRANCISCO)
		{
			cout << "SAN_FRANCISCO ";
		}
		else if (city == PORTO)
		{
			cout << "PORTO ";
		}
		else //(city == UTC)
		{
			cout << "The time in " << cityInput <<"that you asked is not avilable." << endl;
			cout << "UTC ";
		}
		cout <<"time is: " << recvBuff << endl;
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
	else
	{
		cout << recvBuff << endl;
	}
}

bool notInRange(int input)
{
	if (input < 1 || input > 14)
	{
		return false;
	}

	return true;
}

void getUserInput(int* userInput)
{
	int input;
	cin >> input;

	while (!notInRange(input))
	{
		cout << "The input is not valid. Please choose a number from the options in the menu" << endl << endl;
		printUserMenu();
		cin >> input;
	}

	*userInput = input;
}

void printUserMenuCities()
{
	std::cout << "Please choose a city (enter a number or a name city thats not on the list):" << endl;
	std::cout << "1. Tokyo - Japan" << endl;
	std::cout << "2. Melbourne - Australia" << endl;
	std::cout << "3. San Francisco - US" << endl;
	std::cout << "4. Porto - Portugal " << endl;
	std::cout << "5. Cities not listed above - Please indicate which city (up to 20 letters!!) " << endl;
}
void getUserInputCities(int* city, char* cityInput)
{

	char temp;
	scanf("%c", &temp); // temp statement to clear buffer
	fgets(cityInput, 21, stdin);

	if (strcmp(cityInput, "1\n") == 0)
	{
		*city = TOKYO;
	}
	else if (strcmp(cityInput, "2\n") == 0)
	{
		*city = MELBOURNE;
	}
	else if (strcmp(cityInput, "3\n") == 0)
	{
		*city = SAN_FRANCISCO;
	}
	else if (strcmp(cityInput, "4\n") == 0)
	{
		*city = PORTO;
	}
	else {
		*city = UTC;
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

	printUserMenu();
	getUserInput(&userInput);
	int city = 0;
	char cityInput[25];
	if (userInput == GET_TIME_WITHOUT_DATE_IN_CITY)
	{
		printUserMenuCities();
		getUserInputCities(&city, cityInput);
	}

	while (userInput != EXIT_PROGRAM)
	{
		// Send and receive data.
		char sendBuff[255];
		
		updateSendBuffByUserInput(userInput, sendBuff, city);

		int bytesSent = 0;
		int bytesRecv = 0;
		char recvBuff[255];
		// Asks the server what's the currnet time.
		// The send function sends data on a connected socket.
		// The buffer to be sent and its size are needed.
		// The fourth argument is an idicator specifying the way in which the call is made (0 for default).
		// The two last arguments hold the details of the server to communicate with. 
		// NOTE: the last argument should always be the actual size of the client's data-structure (i.e. sizeof(sockaddr)).

		if (userInput == GET_CLIENT_TO_SERVER_DELAY_ESTIMATION)
		{
			for (int i = 0; i < 100; i++) // send 100 request for whats the time
			{
				bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr *)&server, sizeof(server)); // 2
				
				if (socketError(bytesSent, connSocket, "sendto()"))
				{
					return;
				}
			}

			bytesRecv = recv(connSocket, recvBuff, 255, 0);

			if (socketError(bytesRecv, connSocket, " recv()"))
			{
				return;
			}
	
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
			
				recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
				char *ptr;
				long int second = strtol(recvBuff, &ptr, 10);

				sum += second - first;
				first = second;
			}

			double avg = (double)sum / 99; // there are 99 pairs
			cout << "Client To Server Delay Estimation is: " << avg << endl;
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

				bytesRecv = recv(connSocket, recvBuff, 255, 0);
				if (socketError(bytesRecv, connSocket, " recv()"))
				{
					return;
				}

				recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
				char *ptr;
				long int timeServer = strtol(recvBuff, &ptr, 10);
				sum += timeServer - timeClient;
			}

			double avg = (double)sum / 100; // there are 100 requsets
			cout << "RTT: " << avg << endl;
		}

		else
		{
			// regular
			bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr *)&server, sizeof(server)); // 2
			if (socketError(bytesSent, connSocket, "sendto()"))
			{
				return;
			}

			cout << "Time Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";

			// Gets the server's answer using simple recieve (no need to hold the server's address).
			bytesRecv = recv(connSocket, recvBuff, 255, 0);
			if (socketError(bytesRecv, connSocket, " recv()"))
			{
				return;
			}

			recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
			printMessegeFromServer(userInput, recvBuff, bytesRecv, city, cityInput);
		}
		//Sleep(3);
		//clearScreen();
		cout << endl;

		printUserMenu();
		getUserInput(&userInput);
		if (userInput == GET_TIME_WITHOUT_DATE_IN_CITY)
		{
			printUserMenuCities();
			getUserInputCities(&city, cityInput);
		}
	}

	// Closing connections and Winsock.
	cout << "Time Client: Closing Connection.\n";
	closesocket(connSocket);
	system("pause");
}