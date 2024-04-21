#include <iostream>
#include <WS2tcpip.h>
#include <winsock2.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

int main() {

    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {
        std::cerr << "Can't initialize Winsock! Quitting" << std::endl;
        return -1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Can't create socket! Quitting" << std::endl;
        return -1;
    }

    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000); // Порт сервера
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);

    bool connected = false;

    while (!connected) {

        int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
        if (connResult == SOCKET_ERROR) {
            std::cerr << "Can't connect to server! Retrying..." << std::endl;
            Sleep(1000);
        }
        else {
            std::cout << "Connected to server!" << std::endl;
            connected = true;
        }
    }

    char buf[4096];
    std::string userInput;

    do {

        std::cout << "> ";
        std::getline(std::cin, userInput);

        if (userInput.size() > 0) {


            int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResult != SOCKET_ERROR) {

                ZeroMemory(buf, 4096);
                int bytesReceived = recv(sock, buf, 4096, 0);
                if (bytesReceived > 0) {

                    std::cout << "SERVER> " << std::string(buf, 0, bytesReceived) << std::endl;
                }
            }
        }

    } while (userInput.size() > 0);

    closesocket(sock);
    WSACleanup();

    return 0;
}