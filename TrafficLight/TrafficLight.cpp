#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <windows.h>

using namespace std;
using namespace std::chrono_literals;

enum TrafficLightsState
{
    RED,
    YELLOW,
    GREEN
};

struct Address
{
    string country;
    string city;
    string street;
    double latitude;
    double longitude;
};

class TrafficLight
{
private:
    Address address;

public:
    TrafficLight(Address a)
    {
        address = a;
    }

    TrafficLightsState GetCurrentState()
    {
        auto now = std::chrono::system_clock::now();
        auto now_sec = std::chrono::floor<std::chrono::seconds>(now).time_since_epoch();
        int seconds = (now_sec % 60).count();

        if (seconds >= 0 && seconds <= 5)
        {
            return YELLOW;
        }

        if (seconds >= 6 && seconds <= 30)
        {
            return RED;
        }

        return GREEN;
    }

    void GetInfo()
    {
        cout << "Адрес светофора:" << endl;
        cout << "Страна: " << address.country << endl;
        cout << "Город: " << address.city << endl;
        cout << "Улица: " << address.street << endl;
        cout << "Координаты: " << address.latitude << ", " << address.longitude << endl;

        TrafficLightsState state = GetCurrentState();

        cout << "Текущий сигнал: ";

        if (state == RED)
        {
            cout << "\033[31mкрасный\033[0m" << endl;
        }
        else if (state == YELLOW)
        {
            cout << "\033[33mжелтый\033[0m" << endl;
        }
        else
        {
            cout << "\033[32mзеленый\033[0m" << endl;
        }

        cout << endl;
    }
};

int main()
{
    system("chcp 65001 > nul");

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(console, &mode);
    SetConsoleMode(console, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    Address a;
    a.country = "Россия";
    a.city = "Москва";
    a.street = "Фестивальная улица, д. 24А";
    a.latitude = 55.8728;
    a.longitude = 37.5409;

    TrafficLight trafficLight(a);

    while (true)
    {
        trafficLight.GetInfo();
        std::this_thread::sleep_for(23000ms);
    }

    return 0;
}