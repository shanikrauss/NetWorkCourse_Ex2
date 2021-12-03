#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include<windows.h>

#define TIME_PORT	27015

#define TOKYO (+9)
#define MELBOURNE (+11)
#define SAN_FRANCISCO (-8)
#define PORTO (0)
#define UTC (0)

#define THREE_MIN 180

const char* dayNames[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
const char* monthNames[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

void main()
{
	WSAData wsaData;

	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Time Server: Error at WSAStartup()\n";
		return;
	}

	SOCKET m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (INVALID_SOCKET == m_socket)
	{
		cout << "Time Server: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	sockaddr_in serverService;
	serverService.sin_family = AF_INET;
	serverService.sin_addr.s_addr = INADDR_ANY;	
	serverService.sin_port = htons(TIME_PORT);

	if (SOCKET_ERROR == bind(m_socket, (SOCKADDR *)&serverService, sizeof(serverService)))
	{
		cout << "Time Server: Error at bind(): " << WSAGetLastError() << endl;
		closesocket(m_socket);
		WSACleanup();
		return;
	}

	// Waits for incoming requests from clients.
	// Send and receive data.
	sockaddr client_addr;
	int client_addr_len = sizeof(client_addr);
	int bytesSent = 0;
	int bytesRecv = 0;
	char sendBuff[255];
	char recvBuff[255];

	cout << "Time Server: Wait for clients' requests.\n";

	bool isMeasureTimeLapOn = false;
	time_t startTimeMeasure;

	while (true)
	{
		bytesRecv = recvfrom(m_socket, recvBuff, 255, 0, &client_addr, &client_addr_len);
		if (SOCKET_ERROR == bytesRecv)
		{
			cout << "Time Server: Error at recvfrom(): " << WSAGetLastError() << endl;
			closesocket(m_socket);
			WSACleanup();
			return;
		}

		recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
		time_t timer;
		time_t secondes;
		secondes = time(NULL);

		struct tm * timeinfo;
		time(&timer);
		timeinfo = localtime(&timer);
		
		if (strcmp(recvBuff, "1") == 0)
		{
			cout << "Time Server: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";
			
			// Parse the current time to printable string.
			strcpy(sendBuff, ctime(&timer));
			sendBuff[strlen(sendBuff) - 1] = '\0'; //to remove the new-line from the created string
		}
		else if (strcmp(recvBuff, "2") == 0)
		{
			sprintf(sendBuff, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
		}
		else if (strcmp(recvBuff, "3") == 0)
		{
			sprintf(sendBuff, "%ld", secondes);
		}
		else if (strcmp(recvBuff, "4") == 0 || strcmp(recvBuff, "5") == 0)
		{
			long int currTimeSecondes = GetTickCount();
			sprintf(sendBuff, "%ld", currTimeSecondes);
		}
		else if (strcmp(recvBuff, "6") == 0)
		{
			sprintf(sendBuff, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
		}
		else if (strcmp(recvBuff, "7") == 0)
		{
			sprintf(sendBuff, "%d", timeinfo->tm_year + 1900);
		}
		else if (strcmp(recvBuff, "8") == 0)
		{
			sprintf(sendBuff, "%s %s", dayNames[timeinfo->tm_wday], monthNames[timeinfo->tm_mon]);
		}
		else if (strcmp(recvBuff, "9") == 0)
		{
			int secondesInADay = 86400;
			int secondesInHour = 3600;
			int secondesFromBegainOfMonth = (timeinfo->tm_mday - 1) * secondesInADay + timeinfo->tm_hour*secondesInHour + timeinfo->tm_min * 60 + timeinfo->tm_sec;

			sprintf(sendBuff, "%ld", secondesFromBegainOfMonth);
		}
		else if (strcmp(recvBuff, "10") == 0)
		{
			sprintf(sendBuff, "%d", timeinfo->tm_mon * 4 + (timeinfo->tm_mday % 7) + 1);
			
		}
		else if (strcmp(recvBuff, "11") == 0)
		{
			if (timeinfo->tm_isdst != -1)
			{
				sprintf(sendBuff, "%d", timeinfo->tm_isdst);
			}
			else
			{
				strcpy(sendBuff, "This information is currently unavailable");
			}
		}
		else if (strcmp(recvBuff, "1T") == 0)
		{
			gmtime(&timer);
			sprintf(sendBuff, "%02d:%02d:%02d", (timeinfo->tm_hour + TOKYO) % 24, timeinfo->tm_min, timeinfo->tm_sec);
		}
		else if (strcmp(recvBuff, "2M") == 0)
		{
			gmtime(&timer);
			sprintf(sendBuff, "%02d:%02d:%02d", (timeinfo->tm_hour + MELBOURNE) % 24, timeinfo->tm_min, timeinfo->tm_sec);
		}
		else if (strcmp(recvBuff, "3S") == 0)
		{
			gmtime(&timer);
			sprintf(sendBuff, "%02d:%02d:%02d",(timeinfo->tm_hour + SAN_FRANCISCO) % 24, timeinfo->tm_min, timeinfo->tm_sec);
		}
		else if (strcmp(recvBuff, "4P") == 0)
		{
			gmtime(&timer);
			sprintf(sendBuff, "%02d:%02d:%02d", (timeinfo->tm_hour + PORTO) % 24, timeinfo->tm_min, timeinfo->tm_sec);
		}
		else if (strcmp(recvBuff, "5U") == 0)
		{
			gmtime(&timer);
			sprintf(sendBuff, "%02d:%02d:%02d", (timeinfo->tm_hour + UTC) % 24, timeinfo->tm_min, timeinfo->tm_sec);
		}
		else if (strcmp(recvBuff, "13") == 0) 
		{
			if (!isMeasureTimeLapOn || timer - startTimeMeasure > THREE_MIN)
			{
				isMeasureTimeLapOn = true;
				startTimeMeasure = timer;
				sprintf(sendBuff, "-1");
			}
			else
			{
				isMeasureTimeLapOn = false;
				sprintf(sendBuff, "%d", timer - startTimeMeasure);
				startTimeMeasure = timer;
			}
		}
		else 
		{
			sprintf(sendBuff, "The server does not support this request");
		}

		bytesSent = sendto(m_socket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr *)&client_addr, client_addr_len);
		if (SOCKET_ERROR == bytesSent)
		{
			cout << "Time Server: Error at sendto(): " << WSAGetLastError() << endl;
			closesocket(m_socket);
			WSACleanup();
			return;
		}

		cout << "Time Server: Sent: " << bytesSent << "\\" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";
	}

	// Closing connections and Winsock.
	cout << "Time Server: Closing Connection.\n";
	closesocket(m_socket);
	WSACleanup();
}