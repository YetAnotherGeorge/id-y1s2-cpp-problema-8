#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "VectorUtils.hpp"
#include "ArboreBinar.hpp"

namespace bst_tests {

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
	void test_bst_delete_3() {
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

		arbore->deleteBST(arbore->searchBST(5));
		std::cout << "After delete:" << std::endl;
		arbore->AfiseazaRecursiv();

		arbore->deleteBST(arbore->searchBST(15));
		std::cout << "After delete:" << std::endl;
		arbore->AfiseazaRecursiv();
	}
	void test_bst_delete_4() {
		//       20     swap 20 with 10
		//      /      
		//    15    
		//    /        
		//  10        
		//  / 
		// 5  
		std::vector<int> v = { 5, 6, 7, 8, 9 };
		ArboreBinar<int>* arbore = new ArboreBinar<int>();
		arbore->insertValuesBST(v);
		arbore->AfiseazaRecursiv();

		arbore->deleteBST(arbore->searchBST(9));
		std::cout << "After delete:" << std::endl;
		arbore->AfiseazaRecursiv();

		arbore->deleteBST(arbore->searchBST(6));
		std::cout << "After delete:" << std::endl;
		arbore->AfiseazaRecursiv();
	}

	void printTestHeader(const std::string& name) {
		std::cout << "\n================ " << name << " ================\n";
	}

	void printInorder(const ArboreBinar<int>& arbore, const std::string& name) {
		printeazaVector(arbore.inorderValueSequence(), name);
	}

	void test_delete_counter_decrement() {
		printTestHeader("test_delete_counter_decrement");
		ArboreBinar<int> arbore;
		arbore.insertValuesBST({ 10, 10, 10, 5, 15 });
		std::cout << "Contor initial pentru 10: " << arbore.searchBST(10)->getContor() << std::endl;
		arbore.deleteBST(10);
		std::cout << "Contor dupa un delete pentru 10: " << arbore.searchBST(10)->getContor() << std::endl;
		printInorder(arbore, "Inorder dupa decrement contor");
	}

	void test_nod_copy_constructor() {
		printTestHeader("test_nod_copy_constructor");
		ArboreBinar<int> arbore;
		arbore.insertValuesBST({ 20, 15, 10, 5, 16, 25, 12 });
		Nod<int> copie_nod(*arbore.searchBST(15));
		copie_nod.AfiseazaRecursiv();
	}

	void test_arbore_copy_constructor() {
		printTestHeader("test_arbore_copy_constructor");
		ArboreBinar<int> original;
		original.insertValuesBST({ 20, 15, 10, 5, 16, 25, 12 });
		ArboreBinar<int> copie(original);
		printInorder(original, "Original");
		printInorder(copie, "Copie");
		std::cout << "Original == Copie: " << (original == copie) << std::endl;
	}

	void test_operator_plus_tree_key() {
		printTestHeader("test_operator_plus_tree_key");
		ArboreBinar<int> arbore;
		arbore.insertValuesBST({ 10, 5, 20 });
		ArboreBinar<int> rezultat = arbore + 15;
		printInorder(rezultat, "arbore + 15");
	}

	void test_operator_plus_key_tree() {
		printTestHeader("test_operator_plus_key_tree");
		ArboreBinar<int> arbore;
		arbore.insertValuesBST({ 10, 5, 20 });
		ArboreBinar<int> rezultat = 15 + arbore;
		printInorder(rezultat, "15 + arbore");
	}

	void test_operator_plus_tree_tree() {
		printTestHeader("test_operator_plus_tree_tree");
		ArboreBinar<int> a;
		ArboreBinar<int> b;
		a.insertValuesBST({ 10, 5, 20 });
		b.insertValuesBST({ 15, 7, 20 });
		ArboreBinar<int> rezultat = a + b;
		printInorder(rezultat, "a + b");
	}

	void test_operator_minus_tree_key() {
		printTestHeader("test_operator_minus_tree_key");
		ArboreBinar<int> arbore;
		arbore.insertValuesBST({ 10, 10, 5, 20, 15 });
		ArboreBinar<int> rezultat = arbore - 10;
		printInorder(rezultat, "arbore - 10");
		Nod<int>* nod = rezultat.searchBST(10);
		std::cout << "Contor ramas pentru 10: " << (nod != nullptr ? nod->getContor() : 0) << std::endl;
	}

	void test_operator_minus_tree_tree() {
		printTestHeader("test_operator_minus_tree_tree");
		ArboreBinar<int> a;
		ArboreBinar<int> b;
		a.insertValuesBST({ 10, 10, 5, 20, 15, 25 });
		b.insertValuesBST({ 10, 25, 99 });
		ArboreBinar<int> rezultat = a - b;
		printInorder(rezultat, "a - b");
	}

	void run_all_tests() {
		// test_swap_1();
		// test_swap_2();
		// test_bst_delete_1();
		// test_bst_delete_2();
		// test_bst_delete_3();
		// test_bst_delete_4();
		
		 test_delete_counter_decrement();
		 test_nod_copy_constructor();
		 test_arbore_copy_constructor();
		 test_operator_plus_tree_key();
		 test_operator_plus_key_tree();
		 test_operator_plus_tree_tree();
		 test_operator_minus_tree_key();
		 test_operator_minus_tree_tree();
	}
}