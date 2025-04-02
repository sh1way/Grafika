#include <iostream>
#include <string>
#include <locale>
#include <Windows.h>

using namespace std;

class Counter;

class Train {
public:
    string destination;
    int time[2];
    int totalSeats;
    int number;

    void enterData() {
        cout << "Пункт назначения: ";
        cin >> destination;
        cout << "Номер поезда: ";
        cin >> number;
        cout << "Время отправления (часы минуты): ";
        cin >> time[0] >> time[1];
        cout << "Общее число мест: ";
        cin >> totalSeats;
        cout << "Места купе: ";
        cin >> coupeSeats;
        cout << "Места плацкарт: ";
        cin >> platzkartSeats;
    }

    void printInfo() const {
        cout << "Поезд №" << number << " в " << destination
            << ", отправление в " << time[0] << ":" << time[1]
            << "\nМеста: общие=" << totalSeats
            << ", купе=" << coupeSeats
            << ", плацкарт=" << platzkartSeats << endl;
    }

    bool operator==(const Train& other) const {
        return number == other.number;
    }
    bool operator!=(const Train& other) const {
        return !(*this == other);
    }

    friend class Observer;
    friend class Counter;

private:
    int coupeSeats;
    int platzkartSeats;
};

class Counter {
public:
    Counter(int initial) : count(initial) {}

    void decreaseSeats(int value) {
        count -= value;
    }

    void printCount() const {
        cout << "Осталось мест: " << count << endl;
    }

    bool operator==(const Counter& other) const {
        return count == other.count;
    }
    bool operator!=(const Counter& other) const {
        return count != other.count;
    }

private:
    int count;
};

class Observer {
public:
    static void displayTrainInfo(const Train& train) {
        cout << "[Observer] Поезд №" << train.number
            << " в " << train.destination
            << ", мест купе: " << train.coupeSeats << endl;
    }
};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const int N = 3;
    Train trains[N];
    Counter seatCounter(0);

    cout << "Введите данные о " << N << " поездах:\n";
    for (int i = 0; i < N; i++) {
        trains[i].enterData();
    }

    int choice;
    cout << "\nВыберите тип поиска:\n"
        << "1. По номеру поезда\n"
        << "2. По пункту назначения\n"
        << "3. По времени отправления\n"
        << "Ваш выбор: ";
    cin >> choice;

    switch (choice) {
    case 1: {
        int trainNumber;
        cout << "Введите номер поезда: ";
        cin >> trainNumber;
        bool found = false;
        for (const auto& train : trains) {
            if (train.number == trainNumber) {
                train.printInfo();
                found = true;
                break;
            }
        }
        if (!found) cout << "Поезд не найден!\n";
        break;
    }
    case 2: {
        string city;
        cout << "Введите город: ";
        cin >> city;
        for (const auto& train : trains) {
            if (train.destination == city) {
                train.printInfo();
                Observer::displayTrainInfo(train);
            }
        }
        break;
    }
    case 3: {
        int hours, minutes;
        cout << "Введите время (часы минуты): ";
        cin >> hours >> minutes;
        for (const auto& train : trains) {
            if (train.time[0] > hours ||
                (train.time[0] == hours && train.time[1] >= minutes)) {
                train.printInfo();
            }
        }
        break;
    }
    default:
        cout << "Некорректный выбор!\n";
    }

    return 0;
}