#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <windows.h>
#include <nlohmann/json.hpp>

using namespace std;
using namespace std::chrono_literals;
using json = nlohmann::json;

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

Address ReadAddressFromFile(string fileName)
{
    ifstream file(fileName);

    if (!file.is_open())
    {
        cout << "Не удалось открыть файл: " << fileName << endl;
        return Address();
    }

    json data;
    file >> data;

    Address a;
    a.country = data["country"];
    a.city = data["city"];
    a.street = data["street"];
    a.latitude = data["latitude"];
    a.longitude = data["longitude"];

    return a;
}

int main()
{
    system("chcp 65001 > nul");

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(console, &mode);
    SetConsoleMode(console, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    vector<TrafficLight> trafficLights;

    trafficLights.push_back(TrafficLight(ReadAddressFromFile("traffic_light1.json")));
    trafficLights.push_back(TrafficLight(ReadAddressFromFile("traffic_light2.json")));
    trafficLights.push_back(TrafficLight(ReadAddressFromFile("traffic_light3.json")));

    while (true)
    {
        for (int i = 0; i < trafficLights.size(); i++)
        {
            trafficLights[i].GetInfo();
        }

        std::this_thread::sleep_for(23000ms);
    }

    return 0;
}