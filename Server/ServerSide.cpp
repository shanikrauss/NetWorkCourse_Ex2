#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include<windows.h>

#define TIME_PORT	27015

const char* dayNames[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
const char* monthNames[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };


void main()
{
	// Initialize Winsock (Windows Sockets).

	// Create a WSADATA object called wsaData.
	// The WSADATA structure contains information about the Windows 
	// Sockets implementation.
	WSAData wsaData;

	// Call WSAStartup and return its value as an integer and check for errors.
	// The WSAStartup function initiates the use of WS2_32.DLL by a process.
	// First parameter is the version number 2.2.
	// The WSACleanup function destructs the use of WS2_32.DLL by a process.
	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Time Server: Error at WSAStartup()\n";
		return;
	}

	// Server side:
	// Create and bind a socket to an internet address.

	// After initialization, a SOCKET object is ready to be instantiated.

	// Create a SOCKET object called m_socket. 
	// For this application:	use the Internet address family (AF_INET), 
	//							datagram sockets (SOCK_DGRAM), 
	//							and the UDP/IP protocol (IPPROTO_UDP).
	SOCKET m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// Check for errors to ensure that the socket is a valid socket.
	// Error detection is a key part of successful networking code. 
	// If the socket call fails, it returns INVALID_SOCKET. 
	// The "if" statement in the previous code is used to catch any errors that
	// may have occurred while creating the socket. WSAGetLastError returns an 
	// error number associated with the last error that occurred.
	if (INVALID_SOCKET == m_socket)
	{
		cout << "Time Server: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// For a server to communicate on a network, it must first bind the socket to 
	// a network address.

	// Need to assemble the required data for connection in sockaddr structure.

	// Create a sockaddr_in object called serverService. 
	sockaddr_in serverService;
	// Address family (must be AF_INET - Internet address family).
	serverService.sin_family = AF_INET;
	// IP address. The sin_addr is a union (s_addr is a unsigdned long (4 bytes) data type).
	// INADDR_ANY means to listen on all interfaces.
	// inet_addr (Internet address) is used to convert a string (char *) into unsigned int.
	// inet_ntoa (Internet address) is the reverse function (converts unsigned int to char *)
	// The IP address 127.0.0.1 is the host itself, it's actually a loop-back.
	serverService.sin_addr.s_addr = INADDR_ANY;	//inet_addr("127.0.0.1");
	// IP Port. The htons (host to network - short) function converts an
	// unsigned short from host to TCP/IP network byte order (which is big-endian).
	serverService.sin_port = htons(TIME_PORT);

	// Bind the socket for client's requests.

	// The bind function establishes a connection to a specified socket.
	// The function uses the socket handler, the sockaddr structure (which
	// defines properties of the desired connection) and the length of the
	// sockaddr structure (in bytes).
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

	// Get client's requests and answer them.
	// The recvfrom function receives a datagram and stores the source address.
	// The buffer for data to be received and its available size are 
	// returned by recvfrom. The fourth argument is an idicator 
	// specifying the way in which the call is made (0 for default).
	// The two last arguments are optional and will hold the details of the client for further communication. 
	// NOTE: the last argument should always be the actual size of the client's data-structure (i.e. sizeof(sockaddr)).
	cout << "Time Server: Wait for clients' requests.\n";

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
		int tm_sec = timeinfo->tm_sec;			/*seconds, range 0 to 59 */
		int tm_min = timeinfo->tm_min;         /* minutes, range 0 to 59           */
		int tm_hour = timeinfo->tm_hour;      /* hours, range 0 to 23             */
		int tm_mday = timeinfo->tm_mday;       /* day of the month, range 1 to 31  */
		int tm_mon = timeinfo->tm_mon;       /* month, range 0 to 11             */
		int tm_year = timeinfo->tm_year;        /* The number of years since 1900   */
		int tm_wday = timeinfo->tm_wday;       /* day of the week, range 0 to 6    */
		int tm_yday = timeinfo->tm_yday;       /* day in the year, range 0 to 365  */
		int tm_isdst = timeinfo->tm_isdst;


		if (strcmp(recvBuff, "1") == 0)
		{
			cout << "Time Server: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";
			     /* daylight saving time*/

			// Parse the current time to printable string.
			strcpy(sendBuff, ctime(&timer));
			sendBuff[strlen(sendBuff) - 1] = '\0'; //to remove the new-line from the created string


			time_t t = time(NULL);
			struct tm tm = *localtime(&t);
			printf("now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		
			time_t rawtime;
			struct tm * timeinfo;

			time(&rawtime);
			timeinfo = localtime(&rawtime);
			printf("Current local time and date: %s", asctime(timeinfo));
		}
		else if (strcmp(recvBuff, "2") == 0)
		{
			sprintf(sendBuff, "%d:%d:%d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

			//cout << "HUD Server: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";
		}
		else if (strcmp(recvBuff, "3") == 0)
		{
			sprintf(sendBuff, "%ld", secondes);
		}
		else if (strcmp(recvBuff, "4") == 0) // DID NOT DO YET
		{
			long int currTimeSecondes = GetTickCount();
			sprintf(sendBuff, "%ld", currTimeSecondes);
		}
		else if (strcmp(recvBuff, "5") == 0) // DID NOT DO YET
		{
			long int currTimeSecondes = GetTickCount();
			sprintf(sendBuff, "%ld", currTimeSecondes);
		}
		else if (strcmp(recvBuff, "6") == 0)
		{
			sprintf(sendBuff, "%d:%d", timeinfo->tm_hour, timeinfo->tm_min);
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

			sprintf(sendBuff, "secondes from begining of the month: %ld", secondesFromBegainOfMonth);
		}
		else if (strcmp(recvBuff, "10") == 0)
		{
			sprintf(sendBuff, "num of week from begin year is: %d", timeinfo->tm_yday/4);

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
		else if (strcmp(recvBuff, "12") == 0) // DID NOT DO YET
		{
			
		}
		else if (strcmp(recvBuff, "13") == 0) // DID NOT DO YET
		{
		}
		else // (recvBuff == "Whats the wheather today?")
		{
			
		}

		// Answer client's request by the current time.

		// Get the current time.


		// Sends the answer to the client, using the client address gathered
		// by recvfrom. 
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