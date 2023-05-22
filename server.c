#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFLEN 512  // Определение размера буфера
#define PORT 8888   // Определение порта для прослушивания

// Функция для обработки ошибок
void die(char *s) {
    perror(s);
    exit(1);
}

int main() {
    struct sockaddr_in si_me, si_other;  // Структуры для хранения информации о адресе сервера и клиента
    int s;
    char buf[BUFLEN];  // Буфер для хранения отправляемых и получаемых данных
    socklen_t slen = sizeof(si_other);  // Размер структуры адреса клиента

    // Создание сокета
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        die("socket");
    }

    // Обнуление структуры адреса сервера
    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);  // Сервер будет прослушивать любой интерфейс

    // Включение режима широковещательной рассылки
    int broadcastEnable = 1;
    int ret = setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    if (ret) {
        die("Error: Could not open set socket to broadcast mode");
    }

    // Обнуление структуры адреса клиента
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.s_addr = htonl(INADDR_BROADCAST);  // Установка широковещательного адреса

    // Бесконечный цикл отправки сообщений
    while (1) {
        printf("Enter message : ");
        fgets(buf, BUFLEN, stdin);

        // Отправка сообщения
        if (sendto(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, slen) == -1) {
            die("sendto()");
        }
    }

    close(s);  // Закрытие сокета
    return 0;
}
