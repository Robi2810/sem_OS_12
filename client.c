#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
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
    socklen_t slen = sizeof(si_other);  // Размер структуры адреса сервера
    char buf[BUFLEN]; // Буфер для хранения отправляемых и получаемых данных
    // Создание сокета
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        die("socket");
    }

    // Обнуление структуры адреса клиента
    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);  // Клиент будет прослушивать любой интерфейс

    // Привязка сокета к адресу
    if (bind(s, (struct sockaddr *) &si_me, sizeof(si_me)) == -1) {
        die("bind");
    }

    // Бесконечный цикл приема сообщений
    while (1) {
        // Получение сообщения
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1) {
            die("recvfrom()");
        }

        puts(buf);  // Вывод полученного сообщения
    }

    close(s);  // Закрытие сокета
    return 0;
}