#include <iostream>
#include <string>
#include <locale>
#include <Windows.h>

class Counter {
public:
	Counter(int h) {
		n = h;
	}
	void printData() {
		std::cout << "value: " << n << std::endl;
	}

	bool operator == (const Counter& counter) {
		return n == counter.n;
	}
	bool operator != (const Counter& counter) {
		return n != counter.n;
	}
	
	friend class Observer;
private:
	int n;
};

class Observer {
public:
	static void GetInfo(const Counter& obj) {
		std::cout << "value: " << obj.n << std::endl;
	}
};

int main() {
	Counter obs1(1);

	Observer::GetInfo(obs1);

	return 0;
}