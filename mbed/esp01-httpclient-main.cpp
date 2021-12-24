#include "mbed.h"

WiFiInterface *wifi;
Serial computer(USBTX, USBRX, 9600);

const char *sec2str(nsapi_security_t sec)
{
    switch (sec) {
        case NSAPI_SECURITY_NONE:
            return "None";
        case NSAPI_SECURITY_WEP:
            return "WEP";
        case NSAPI_SECURITY_WPA:
            return "WPA";
        case NSAPI_SECURITY_WPA2:
            return "WPA2";
        case NSAPI_SECURITY_WPA_WPA2:
            return "WPA/WPA2";
        case NSAPI_SECURITY_UNKNOWN:
        default:
            return "Неизвестно";
    }
}

int scan_demo(WiFiInterface *wifi)
{
    WiFiAccessPoint *ap;
    computer.printf("Ищем доступные сети:\n");
    int count = wifi->scan(NULL,0);
    if (count <= 0) {
        computer.printf("Ошибка: scan() вернула код возврата: %d\n", count);
        return 0;
    }
    count = count < 15 ? count : 15; // ограничимся 15 сетями
    ap = new WiFiAccessPoint[count];
    count = wifi->scan(ap, count);
    if (count <= 0) {
        computer.printf("Ошибка  scan() вернула код возврата: %d\n", count);
        return 0;
    }
    for (int i = 0; i < count; i++) {
        computer.printf("Сеть: %s шифрование: %s BSSID: %hhX:%hhX:%hhX:%hhx:%hhx:%hhx RSSI: %hhd Канал: %hhd\n", ap[i].get_ssid(),
               sec2str(ap[i].get_security()), ap[i].get_bssid()[0], ap[i].get_bssid()[1], ap[i].get_bssid()[2],
               ap[i].get_bssid()[3], ap[i].get_bssid()[4], ap[i].get_bssid()[5], ap[i].get_rssi(), ap[i].get_channel());
    }
    computer.printf("Найдено %d сетей.\n", count);
    delete[] ap;
    return count;
}

void http_demo(NetworkInterface *net)
{
    TCPSocket socket;
    socket.open(net);
    SocketAddress a;
    net->gethostbyname("ident.me", &a);
    a.set_port(80);
    socket.connect(a);
    char sbuffer[] = "GET / HTTP/1.1\r\nHost: ident.me\r\n\r\n";
    int scount = socket.send(sbuffer, sizeof sbuffer);
    char rbuffer[256];
    int rcount = socket.recv(rbuffer, sizeof rbuffer);
    if (rcount > 0)
        computer.printf("%s", rbuffer);
    socket.close();
}

int main()
{
    computer.printf("WiFi пример для ESP-01 и Mbed OS\n");
#ifdef MBED_MAJOR_VERSION
    computer.printf("Версия Mbed OS %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif
    wifi = WiFiInterface::get_default_instance();
    if (!wifi) {
        computer.printf("Ошибка: WiFiInterface на найден.\n");
        return -1;
    }
    int count = scan_demo(wifi);
    if (count == 0) {
        computer.printf("Ошибка: точки доступа Wi-Fi не найдены, продолжение невозможно.\n");
        return -1;
    }
    computer.printf("\nПодключаемся к точке доступа %s...\n", MBED_CONF_APP_WIFI_SSID);
    int ret = wifi->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
        computer.printf("\nОшибка подключения: %d\n", ret);
        return -1;
    }
    computer.printf("Подключились к сети, получили IP = %s\n", wifi->get_ip_address());
    computer.printf("MAC: %s\n", wifi->get_mac_address());
    // делаем GET-запрос
    computer.printf("Подключаемся...\n");
    http_demo(wifi);
    wifi->disconnect();
}
