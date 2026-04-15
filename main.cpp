#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "VectorUtils.hpp"
#include "ArboreBinar.hpp"

void test_swap_1() {
	//       20     swap 20 with 10
	//      /  \    
	//    15    25  
	//    / \       
	//  10   16     
	//  / \
	// 5   12
	std::vector<int> v = { 20, 15, 10, 5, 25, 16, 12 };
	ArboreBinar<int>* arbore = new ArboreBinar<int>();
	arbore->insertValuesBST(v);
	arbore->AfiseazaRecursiv();

	Nod<int>* node_1 = arbore->searchBST(15);
	Nod<int>* node_2 = arbore->searchBST(10);

	std::cout << "Replacing nodes: " << node_1->getValoare() << " with " << node_2->getValoare() << std::endl;
	arbore->swapNodesInTree(node_1, node_2);
	std::cout << "After swap:" << std::endl;
	arbore->AfiseazaRecursiv();
}
void test_swap_2() {
	//       20     swap 20 with 10
	//      /  \    
	//    15    25  
	//    / \       
	//  10   16     
	//  / \
	// 5   12
	std::vector<int> v = { 20, 15, 10, 5, 25, 16, 12 };
	ArboreBinar<int>* arbore = new ArboreBinar<int>();
	arbore->insertValuesBST(v);
	arbore->AfiseazaRecursiv();

	arbore->swapNodesInTree(arbore->searchBST(10), nullptr);
	arbore->swapNodesInTree(arbore->searchBST(25), nullptr);
	//arbore->swapNodesInTree(arbore->bstSearch(15), nullptr);
	std::cout << "After swap:" << std::endl;
	arbore->AfiseazaRecursiv();
}
void test_bst_delete_1() {
	//       20     swap 20 with 10
	//      /  \    
	//    15    25  
	//    / \       
	//  10   16     
	//  / \
	// 5   12
	std::vector<int> v = { 20, 15, 10, 5, 25, 16, 12 };
	ArboreBinar<int>* arbore = new ArboreBinar<int>();
	arbore->insertValuesBST(v);
	arbore->AfiseazaRecursiv();

	arbore->deleteBST(arbore->searchBST(10));
	std::cout << "After delete:" << std::endl;
	arbore->AfiseazaRecursiv();

	arbore->deleteBST(arbore->searchBST(16));
	std::cout << "After delete:" << std::endl;
	arbore->AfiseazaRecursiv();

	arbore->deleteBST(arbore->searchBST(20));
	std::cout << "After delete:" << std::endl;
	arbore->AfiseazaRecursiv();
}
void test_bst_delete_2() {
	//       20     swap 20 with 10
	//      /      
	//    15    
	//    /        
	//  10        
	//  / 
	// 5  
	std::vector<int> v = { 20, 15, 10, 5 };
	ArboreBinar<int>* arbore = new ArboreBinar<int>();
	arbore->insertValuesBST(v);
	arbore->AfiseazaRecursiv();

	arbore->deleteBST(arbore->searchBST(20));
	std::cout << "After delete:" << std::endl;
	arbore->AfiseazaRecursiv();
}

int main() {
	test_bst_delete_2();
	return 0;
}
