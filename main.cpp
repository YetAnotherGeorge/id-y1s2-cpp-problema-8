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
	std::vector<int> v = { 22, 12, 30, 8, 20, 25, 40, 4, 9, 18, 21, 24, 60};
	ArboreBinar<int>* arbore = new ArboreBinar<int>();
	arbore->insertValuesRedBlackBST(v);
	arbore->AfiseazaRecursiv();

	std::vector<int> inorder_traversal = arbore->getRadacina()->inorderTraversalValues();
	printeazaVector(inorder_traversal, "Inorder traversal values: ");
	return 0;
}
