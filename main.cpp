#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "VectorUtils.hpp"
#include "ArboreBinar.hpp"

int main() {
	//std::srand(43); // seed rand function
	//std::vector<int> v = genereazaSecventa(10, 0, 20);
	//printeazaVector(v, "Secventa generata");

	//std::vector<int> v = { 10, 5, 8, 7};
	std::vector<int> v = { 3, 21, 32, 15 };

	ArboreBinar<int>* arbore = new ArboreBinar<int>();
	for (int i = 0; i < v.size(); i++) {
		arbore->insertRedBlackBST(v[i]);
		arbore->AfiseazaRecursiv(3);
	}

	return 0;
}
