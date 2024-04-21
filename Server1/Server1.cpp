#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    WSAStartup(ver, &wsData);

    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        std::cerr << "Can't create a socket! Quitting" << std::endl;
        WSACleanup();
        return -1;
    }

    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000); 
    hint.sin_addr.S_un.S_addr = INADDR_ANY; 

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    listen(listening, SOMAXCONN);

    sockaddr_in client{};
    int clientSize = sizeof(client);
    SOCKET clientSocket;
    int clientCount = 0;

    while (true) {
        clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
        clientCount++;

        std::cout << "Client " << clientCount << " connected" << std::endl;

        char buf[4096];

        while (true) {
            ZeroMemory(buf, 4096);

            int bytesReceived = recv(clientSocket, buf, 4096, 0);
            if (bytesReceived == SOCKET_ERROR) {
                std::cerr << "Client " << clientCount << " disconnected" << std::endl;
                break;
            }

            std::cout << "Received: " << std::string(buf, 0, bytesReceived) << std::endl;

            send(clientSocket, buf, bytesReceived + 1, 0);
        }

        closesocket(clientSocket);
    }

    closesocket(listening);
    WSACleanup();

    return 0;
}