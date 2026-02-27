#pragma once
#include <unordered_map>

using namespace std;

int TestikPOISK(unordered_map<int, int>& dict, int ToFind) {

	auto it = dict.find(ToFind);

	if (it == dict.end()) {
		return -1;
	}
	else {
		return it->second;
	}
}


