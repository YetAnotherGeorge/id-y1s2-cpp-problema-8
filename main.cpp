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

	std::vector<int> v = { 
		100, 
		50, 150, 
		12, 75, 140, 170,
		1, 13, 51, 76, 130, 145, 160, 180};

	ArboreBinar<int>* arbore = new ArboreBinar<int>();
	for (int i = 0; i < v.size(); i++) {
		arbore->insertBST(v[i]);
	}

	arbore->AfiseazaRecursiv(3);
	return 0;
}
