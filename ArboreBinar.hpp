#pragma once
#include <iostream>
#include <sstream>
#include <algorithm>
#include "Nod.hpp"
#include "VectorUtils.hpp"

// #define DEBUG_INSERT_RED_BLACK

/// <summary>
/// Arbore binar. Poate fi complet gol
/// </summary>
/// <typeparam name="TVN">
/// Tip valoare nod. Trebuie sa implementeze =, ==, <, <=, >, >=
/// </typeparam>
template <typename TVN> class ArboreBinar {
private:
	Nod<TVN>* radacina_; // poate fi null

public:
	/// <summary>
	/// Arbore binar gol
	/// </summary>
	ArboreBinar() {
		radacina_ = nullptr;
	}
	/// <summary>
	/// Constructor de copiere (deep copy)
	/// </summary>
	ArboreBinar(const ArboreBinar<TVN>& other) {
		if (other.radacina_ != nullptr) {
			radacina_ = new Nod<TVN>(*other.radacina_);
		} else {
			radacina_ = nullptr;
		}
	}
	/// <summary>
	/// Creaza arbore binar cu o singura valoare
	/// </summary>
	/// <param name="val"></param>
	ArboreBinar(TVN val) {
		radacina_ = new Nod<TVN>(val, 1, true, nullptr, nullptr, nullptr);
	}
	/// <summary>
	/// Insereaza valorile folosind BST
	/// </summary>
	/// <param name="values"></param>
	ArboreBinar(const std::vector<TVN>& values) {
		radacina_ = nullptr;
		insertValuesBST(values);
	}
	/// <summary>
	/// Destructor
	/// </summary>
	~ArboreBinar() {
		if (radacina_ != nullptr) {
			delete radacina_;
		}
	}

#pragma region SEARCH / WALK
	/// <summary>
  /// 
  /// </summary>
  /// <param name="value"></param>
  /// <returns></returns>
	Nod<TVN>* searchBST(TVN value) {
		if (this->radacina_ == nullptr) return nullptr;
		return this->radacina_->searchBST(value);
	}
#pragma endregion

#pragma region INSERTION / CREATION OPS
	/// <summary>
	/// Seteaza radacina copacului, stergand radacina existenta daca este cazul.
	/// </summary>
	/// <param name="val"></param>
	/// <returns>returneaza radacina copacului, goala</returns>
	Nod<TVN>* initRadacina(TVN val) {
		if (this->radacina_ != nullptr) {
#ifdef BST_DEBUG_PRINT
			std::cout << "initRadacina: Radacina deja initializata, suprascriere" << std::endl;
#endif
			delete this->radacina_;
		}
		this->radacina_ = new Nod<TVN>(val);
#ifdef BST_DEBUG_PRINT
		std::cout << "initRadacina: Radacina initializata cu valoarea '" << val << "'" << std::endl;
#endif
		return this->radacina_;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <returns>radacina copacului. Poate fi nullptr daca nu a fost setata</returns>
	Nod<TVN>* getRadacina() const {
		return radacina_;
	}

	/// <summary>
	/// Insert respecting binary search tree properties. If the value already exists, increment the counter of the existing node.
	/// </summary>
	/// <returns>
	///	Returneaza nod-ul inserat
	/// </returns>
	Nod<TVN>* insertBST(TVN val) {
		if (radacina_ == nullptr) {
			// std::cout << "insertBST: Arbore gol, inserare ca radacina" << std::endl;
			Nod<TVN>* nod = new Nod<TVN>(val, 1, true, nullptr, nullptr, nullptr); // nod negru, contor 1
			this->radacina_ = nod;
			return this->radacina_;
		} else {
			return this->radacina_->insertValueBST(val);
		}
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="nod"></param>
	void deleteBST(Nod<TVN>* nod) {
		if (nod == nullptr) return;
#ifdef BST_DEBUG_PRINT
		std::cout << "deleteBST: delete node with value: " << nod->getValoare() << std::endl;
#endif

		// Sunt 3 cazuri: 
		// 1. nodul de sters este frunza => stergem nodul
		// 2. nodul de sters are un singur copil => stergem nodul si legam copilul la parintele nodului sters
		// 3. nodul de sters are 2 copii => gasim nodul cu cea mai mare valoare din subarborele stang, 
		//    va fi folosit in locul nod-ului sters
		// Check nod in acest copac
		if (nod->walkToTop() != this->radacina_) {
			std::cerr << "DeleteBST: Nodul de sters nu apartine acestui arbore" << std::endl;
			return;
		}

		if (nod->getContor() > 1) {
			nod->setContor(nod->getContor() - 1);
			return;
		}

		if (nod->getCStanga() != nullptr && nod->getCDreapta() != nullptr) {
			// ambii copii sunt prezenti: find smallest value in left subtree
			Nod<TVN>* rep = nod->getCDreapta()->walkToLeftmost(); // next after nod in inorder walk
			this->swapNodesInTree(nod, rep);
		}

		Nod<TVN>* child = nod->getCStanga() != nullptr ? nod->getCStanga() : nod->getCDreapta();
		if (nod->getParinte() == nullptr) {
			this->radacina_ = child;
			if (child != nullptr)
				child->setParinte(nullptr);
		} else {
			nod->getParinte()->replaceChild(nod, child);
		}

		nod->setParinte(nullptr);
		nod->setCStanga(nullptr);
		nod->setCDreapta(nullptr);
		delete nod;
	}
	/// <summary>
  /// 
  /// </summary>
  /// <param name="val"></param>
	void deleteBST(TVN val) {
		Nod<TVN>* nod = searchBST(val);
		if (nod == nullptr)
			throw std::runtime_error("deleteBST: Valoarea nu exista in arbore");

		deleteBST(nod);
	}

	/// <summary>
	/// Insereaza valoarea folosind algoritmul red-black
	/// </summary>
	/// <param name="val"></param>
	/// <returns></returns>
	Nod<TVN>* insertRedBlackBST(TVN val) {
		// 1. BST insert; new node is red
		Nod<TVN>* inserted = this->insertBST(val);
		if (radacina_ == inserted) {
			radacina_->setIsNegru(true); // root is always black
			return radacina_;
		}
		inserted->setIsNegru(false);

		// Fixup loop: runs while there is a red-red parent-child violation
		Nod<TVN>* current = inserted;
		while (current != radacina_ && !current->getParinte()->getIsNegru()) {
			Nod<TVN>* p = current->getParinte();
			Nod<TVN>* g = p->getParinte();
			if (g == nullptr) break;

			Nod<TVN>* uncle = g->getOtherChild(p);
			bool uncle_black = uncle == nullptr || uncle->getIsNegru();

#ifdef DEBUG_INSERT_RED_BLACK
			std::cout << "insertRedBlackBST: Parent is red for node: " << current->getValoare() << ", need to fix tree" << std::endl;
			if (uncle != nullptr)
				std::cout << "insertRedBlackBST: Uncle: " << uncle->getValoare() << ", uncle_black: " << uncle_black << std::endl;
			else
				std::cout << "insertRedBlackBST: Uncle: NULL (BLACK), uncle_black: " << uncle_black << std::endl;
#endif

			if (!uncle_black) {
				// Uncle is red: recolor P and U to black, G to red, then repeat from G
#ifdef DEBUG_INSERT_RED_BLACK
				std::cout << "insertRedBlackBST: Uncle is red for node: " << current->getValoare() << ", recoloring parent, uncle and grandparent" << std::endl;
#endif
				p->setIsNegru(true);
				uncle->setIsNegru(true);
				g->setIsNegru(false); // may violate at G's level; loop will handle it
				current = g;
			} else {
				// Uncle is black: one of 4 rotation cases (never need further loop iteration)
				Nod<TVN>* x = current;

				// 4.1 Left Left
				if (x == p->getCStanga() && p == g->getCStanga()) {
#ifdef DEBUG_INSERT_RED_BLACK
					std::cout << "insertRedBlackBST: Case 4.1 Left Left for node: " << x->getValoare() << std::endl;
#endif
					auto new_root = g->rightRotate();
					if (g == this->radacina_) this->radacina_ = new_root;
					bool g_black = g->getIsNegru();
					g->setIsNegru(p->getIsNegru());
					p->setIsNegru(g_black);
				}
				// 4.2 Left Right
				else if (x == p->getCDreapta() && p == g->getCStanga()) {
#ifdef DEBUG_INSERT_RED_BLACK
					std::cout << "insertRedBlackBST: Case 4.2 Left Right for node: " << x->getValoare() << std::endl;
#endif
					p->leftRotate();
					auto new_root = g->rightRotate();
					if (g == this->radacina_) this->radacina_ = new_root;
					bool g_black = g->getIsNegru();
					g->setIsNegru(x->getIsNegru());
					x->setIsNegru(g_black);
				}
				// 4.3 Right Right
				else if (x == p->getCDreapta() && p == g->getCDreapta()) {
#ifdef DEBUG_INSERT_RED_BLACK
					std::cout << "insertRedBlackBST: Case 4.3 Right Right for node: " << x->getValoare() << std::endl;
#endif
					auto new_root = g->leftRotate();
					if (g == this->radacina_) this->radacina_ = new_root;
					bool g_black = g->getIsNegru();
					g->setIsNegru(p->getIsNegru());
					p->setIsNegru(g_black);
				}
				// 4.4 Right Left
				else if (x == p->getCStanga() && p == g->getCDreapta()) {
#ifdef DEBUG_INSERT_RED_BLACK
					std::cout << "insertRedBlackBST: Case 4.4 Right Left for node: " << x->getValoare() << std::endl;
#endif
					p->rightRotate();
					auto new_root = g->leftRotate();
					if (g == this->radacina_) this->radacina_ = new_root;
					bool g_black = g->getIsNegru();
					g->setIsNegru(x->getIsNegru());
					x->setIsNegru(g_black);
				}

				break; // rotation cases never cascade further
			}
		}

		radacina_->setIsNegru(true); // root is always black
		return inserted;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="values"></param>
	void insertValuesBST(const std::vector<TVN>& values) {
		for (const TVN& val : values) {
			insertBST(val);
		}
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="values"></param>
	void insertValuesRedBlackBST(const std::vector<TVN>& values) {
		for (const TVN& val : values) {
			insertRedBlackBST(val);
		}
	}
	/// <summary>
	/// Citeste o linie de la tastatura cu valori separate prin spatiu sau virgula.
	/// Daca linia incepe cu /INSERT_RED_BLACK, foloseste inserare red-black in loc de BST simplu.
	/// </summary>
	void citireConsola() {
		std::cout << "Introduceti valorile pentru inserare, separate prin spatiu sau virgula. Pentru inserare red-black, prefixati cu /INSERT_RED_BLACK\n";
		
		std::string line;
		std::getline(std::cin, line);
		bool use_rb = false;
		const std::string prefix = "/INSERT_RED_BLACK";
		if (line.rfind(prefix, 0) == 0) {
			use_rb = true;
			line = line.substr(prefix.size());
		}
		std::replace(line.begin(), line.end(), ',', ' ');
		std::istringstream ss(line);
		TVN val;
		while (ss >> val) {
			if (use_rb) insertRedBlackBST(val);
			else insertBST(val);
		}

		this->AfiseazaRecursiv();
	}
	/// <summary>
	/// Returneaza secventa in-ordine cu repetare conform contorului fiecarui nod
	/// </summary>
	std::vector<TVN> inorderValueSequence() const {
		if (radacina_ == nullptr) return {};
		return radacina_->inorderTraversalValuesWithDup();
	}

	/// <summary>
	/// Does not respect BST properties after swap
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	void swapNodesInTree(Nod<TVN>* a, Nod<TVN>* b) {
#ifdef BST_DEBUG_PRINT
		std::cout << "Swap nodes in tree: ";
		if (a != nullptr) std::cout << a->getValoare();
		else std::cout << "NULL";
		std::cout << " <-> ";
		if (b != nullptr) std::cout << b->getValoare();
		else std::cout << "NULL";
		std::cout << std::endl;
#endif

		if ((a != nullptr && a->walkToTop() != this->radacina_) || (b != nullptr && b->walkToTop() != this->radacina_)) {
			std::cerr << "swapNodesInTree: One or both nodes do not belong to this tree" << std::endl;
			return;
		}
		if (a == b) return; // swapping the same node does nothing
		// make sure root is updated
		if (a == this->radacina_) {
			this->radacina_ = b;
		} else if (b == this->radacina_) {
			this->radacina_ = a;
		}
		Nod<TVN>::SwapNodesInTree(a, b);
	}
#pragma endregion

	/// <summary>
	/// Afiseaza arborele incepand de la radacina. Lungimea valorii printate ca string
	/// este pe moment limitata la 3
	/// </summary>
	/// <param name="valMaxStrLen">Lungimea maxima a valorii printata ca string</param>
	void AfiseazaRecursiv() const {
		if (radacina_ != nullptr) {
			radacina_->AfiseazaRecursiv();
		} else {
			std::cout << ">>>>>>>>>>>>>>>> ARBORE GOL" << std::endl;
		}
	}

#pragma region OPERATORS
	/// <summary>
	/// Operator de atribuire (deep copy).
	/// Sterge arborele curent si copiaza adanc arborele sursa.
	/// </summary>
	ArboreBinar<TVN>& operator=(const ArboreBinar<TVN>& other) {
		if (this == &other) return *this;
		if (radacina_ != nullptr) {
			delete radacina_;
			radacina_ = nullptr;
		}
		if (other.radacina_ != nullptr) {
			radacina_ = new Nod<TVN>(*other.radacina_);
		}
		return *this;
	}

	bool operator==(const ArboreBinar<TVN>& other) const {
		return comparaLexicografic(this->inorderValueSequence(), other.inorderValueSequence()) == 0;
	}
	bool operator!=(const ArboreBinar<TVN>& other) const {
		return comparaLexicografic(this->inorderValueSequence(), other.inorderValueSequence()) != 0;
	}
	bool operator<(const ArboreBinar<TVN>& other) const {
		return comparaLexicografic(this->inorderValueSequence(), other.inorderValueSequence()) < 0;
	}
	bool operator<=(const ArboreBinar<TVN>& other) const {
		return comparaLexicografic(this->inorderValueSequence(), other.inorderValueSequence()) <= 0;
	}
	bool operator>(const ArboreBinar<TVN>& other) const {
		return comparaLexicografic(this->inorderValueSequence(), other.inorderValueSequence()) > 0;
	}
	bool operator>=(const ArboreBinar<TVN>& other) const {
		return comparaLexicografic(this->inorderValueSequence(), other.inorderValueSequence()) >= 0;
	}
	ArboreBinar<TVN> operator+(const TVN& val) const {
		ArboreBinar<TVN> result(*this);
		result.insertBST(val);
		return result;
	}
	ArboreBinar<TVN> operator+(const ArboreBinar<TVN>& other) const {
		ArboreBinar<TVN> result(*this);
		for (const TVN& val : other.inorderValueSequence()) {
			result.insertBST(val);
		}
		return result;
	}
	ArboreBinar<TVN> operator-(const TVN& val) const {
		ArboreBinar<TVN> result(*this);
		Nod<TVN>* nod = result.searchBST(val);
		if (nod != nullptr) {
			result.deleteBST(nod);
		}
		return result;
	}
	ArboreBinar<TVN> operator-(const ArboreBinar<TVN>& other) const {
		ArboreBinar<TVN> result(*this);
		for (const TVN& val : other.inorderValueSequence()) {
			Nod<TVN>* nod = result.searchBST(val);
			if (nod != nullptr) {
				result.deleteBST(nod);
			}
		}
		return result;
	}
	friend ArboreBinar<TVN> operator+(const TVN& val, const ArboreBinar<TVN>& tree) {
		ArboreBinar<TVN> result(tree);
		result.insertBST(val);
		return result;
	}
#pragma endregion
};
