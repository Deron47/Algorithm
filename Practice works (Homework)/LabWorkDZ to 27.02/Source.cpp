#include <iostream>
#include <unordered_map>
#include <vector>
#include <chrono>
#include "../LabWorkDZ to 27.02/Testik.h"

using namespace std;


int main() {
	setlocale(LC_ALL, "Russian");

	vector<int> sizes = { 10, 100, 1000, 10000, 100000, 50000, 1000000, 5000000, 10000000 };
	cout << " Начинаем замерчики скорости поиска...." << endl;

	for (int i = 0; i < sizes.size(); i++) {
		int CurSize = sizes[i];
		unordered_map<int, int> dict;

		for (int j = 0; j < CurSize; j++) {
			dict[j] = j * 2;
		}

		long long TotalT = 0;
		int Attempts = 1000;



		for (int k = 0; k < Attempts; k++) {
			int randomKey = rand() % CurSize;

			auto start = chrono::steady_clock::now();
			TestikPOISK(dict, randomKey);
			auto end = chrono::steady_clock::now();

			TotalT += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
		}
		long long avgTime = TotalT / Attempts;
		cout << "Элементов: " << CurSize << "\t Вермя поиска: " << avgTime << " нс" << endl;
	}
	return 0;
}

