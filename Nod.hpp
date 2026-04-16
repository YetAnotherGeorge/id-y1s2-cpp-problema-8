#pragma once
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>
#include <queue>
#include <stack>
#include <stdexcept>
#include <cmath>
#include <numbers>
#include <optional>
#include <functional>

// Note generale: 
//  - Google C++ style for private members: name_ (with trailing underscore)
//

template <typename TVN> class Nod;

template <typename TVN> struct TreeWalkInfo {
	const int treeDepth; // adancimea maxima a arborelui
	const std::vector<std::tuple<Nod<TVN>*, int>> walkNodes; // tuple de forma (nod, adancime), adancimea este 0-indexed

	/// <summary>
	/// Pretty print result of walk
	/// </summary>
	/// <param name="out"></param>
	void Print(std::ostream& out, bool brief = false) const {
		out << "Walk [treeLayers: " << treeDepth + 1 << "]: ";
		for (size_t i = 0; i < walkNodes.size(); i++) {
			const Nod<TVN>* node = std::get<0>(walkNodes[i]);
			int nodeDepth = std::get<1>(walkNodes[i]);

			if (!brief)
				out << "{" << nodeDepth << ", '" << node->getValoare() << "'}";
			else
				out << "'" << node->getValoare() << "'";

			if (i < walkNodes.size() - 1) {
				out << ", ";
			}
		}
		out << std::endl;
	}
};

/// <summary>
/// 
/// </summary>
/// <typeparam name="TVN">
/// Tip valoare nod. Trebuie sa implementeze =, ==, <, <=, >, >=
/// </typeparam>
template <typename TVN> class Nod {
private:
	TVN valoare_;
	int contor_; // DEFAULT 1
	bool negru_; // true daca nodul este negru, false daca este rosu (pentru arbori rosii-negri); DEFA.setULT false

	Nod<TVN>* parinte_; // NULL daca nodul este radacina
	Nod<TVN>* c_stanga_; // copilul din stanga (cu valoare mai mica)
	Nod<TVN>* c_dreapta_; // copilul din dreapta (cu valoare mai mare)

public:
	/// <summary>
	/// Constructor de baza
	/// </summary>
	/// <param name="val"></param>
	/// <param name="contor">default 1</param>
	/// <param name="negru">default false</param>
	/// <param name="parinte">default null</param>
	/// <param name="c_stanga">default null</param>
	/// <param name="c_dreapta">default null</param>
	Nod(TVN val, int contor, bool negru, Nod<TVN>* parinte, Nod<TVN>* c_stanga, Nod<TVN>* c_dreapta) {
		valoare_ = val;
		contor_ = contor;
		negru_ = negru;
		parinte_ = parinte;
		c_stanga_ = c_stanga;
		c_dreapta_ = c_dreapta;
	}
	/// <summary>
	/// Defaults applied: contor = 1, negru = false, parinte = null, c_stanga = null, c_dreapta = null
	/// </summary>
	/// <param name="val"></param>
	Nod(TVN val) : Nod(val, 1, false, nullptr, nullptr, nullptr) {}
	/// <summary>
	/// Defaults applied: c_stanga = null, c_dreapta = null
	/// </summary>
	/// <param name="val"></param>
	/// <param name="contor"></param>
	/// <param name="negru"></param>
	/// <param name="parinte"></param>
	Nod(TVN val, int contor, bool negru, Nod<TVN>* parinte) : Nod(val, contor, negru, parinte, nullptr, nullptr) {}
	/// <summary>
	/// Constructor de copiere (deep copy pentru subarbore)
	/// </summary>
	Nod(const Nod<TVN>& other) : valoare_(other.valoare_), contor_(other.contor_), negru_(other.negru_), parinte_(nullptr), c_stanga_(nullptr), c_dreapta_(nullptr) {
		if (other.c_stanga_ != nullptr) {
			c_stanga_ = new Nod<TVN>(*other.c_stanga_);
			c_stanga_->parinte_ = this;
		}
		if (other.c_dreapta_ != nullptr) {
			c_dreapta_ = new Nod<TVN>(*other.c_dreapta_);
			c_dreapta_->parinte_ = this;
		}
	}

	/// <summary>
	/// Sterge recursiv tot subarborele pornind de la nodul curent
	/// </summary>
	~Nod() {
		if (c_stanga_ != nullptr) {
			delete c_stanga_;
		}
		if (c_dreapta_ != nullptr) {
			delete c_dreapta_;
		}
	}

#pragma region TREE OPS
	/// <summary>
	/// Walks tree in breadth-first order
	/// </summary>
	/// <returns></returns>
	TreeWalkInfo<TVN> walkBFS() {
		int treeDepth = 0; // adancimea maxima a arborelui
		std::vector<std::tuple<Nod<TVN>*, int>> walkNodes; // tuple de forma (nod, adancime), 0-indexed

		std::unordered_set<Nod<TVN>*> visited; // pentru a evita ciclurile (in cazul in care exista)
		std::queue<std::tuple<Nod<TVN>*, int>> queue; // coada pentru BFS: (nod, adancime)
		queue.push(std::tuple<Nod<TVN>*, int>(this, 0));

		while (!queue.empty()) {
			std::tuple<Nod<TVN>*, int> currentEntry = queue.front();
			queue.pop();

			Nod<TVN>* current = std::get<0>(currentEntry);
			int currentDepth = std::get<1>(currentEntry);

			if (visited.find(current) != visited.end()) {
				continue; // nodul a fost vizitat deja, sarim
			}
			visited.insert(current); // marcam nodul ca vizitat

			walkNodes.push_back({ current, currentDepth }); // adaugam nodul curent in rezultat
			if (currentDepth > treeDepth) {
				treeDepth = currentDepth;
			}

			// adaugare copii in coada
			if (current->c_stanga_ != nullptr) {
				queue.push(std::tuple<Nod<TVN>*, int>(current->c_stanga_, currentDepth + 1));
			}
			if (current->c_dreapta_ != nullptr) {
				queue.push(std::tuple<Nod<TVN>*, int>(current->c_dreapta_, currentDepth + 1));
			}
		}

		return TreeWalkInfo<TVN>{ treeDepth, walkNodes };
	}
	/// <summary>
	/// Walks tree in depth-first order (left to right)
	/// </summary>
	/// <returns></returns>
	TreeWalkInfo<TVN> walkDFS() {
		int treeDepth = 0; // adancimea maxima a arborelui
		std::vector<std::tuple<Nod<TVN>*, int>> walkNodes; // tuple de forma (nod, adancime), 0-indexed

		std::unordered_set<Nod<TVN>*> visited; // pentru a evita ciclurile (in cazul in care exista)
		std::stack<std::tuple<Nod<TVN>*, int>> stack; // stiva pentru DFS: (nod, adancime)
		stack.push(std::tuple<Nod<TVN>*, int>(this, 0));

		while (!stack.empty()) {
			std::tuple<Nod<TVN>*, int> currentEntry = stack.top();
			stack.pop();

			Nod<TVN>* current = std::get<0>(currentEntry);
			int currentDepth = std::get<1>(currentEntry);

			if (visited.find(current) != visited.end())
				continue; // nodul a fost vizitat deja
			visited.insert(current); // vizitat

			walkNodes.push_back({ current, currentDepth }); // adaugam nodul curent in rezultat
			if (currentDepth > treeDepth)
				treeDepth = currentDepth;

			// adaugare copii in stiva (ordine inversa pentru a procesa de la stanga la dreapta)
			if (current->c_dreapta_ != nullptr) {
				stack.push(std::tuple<Nod<TVN>*, int>(current->c_dreapta_, currentDepth + 1));
			}
			if (current->c_stanga_ != nullptr) {
				stack.push(std::tuple<Nod<TVN>*, int>(current->c_stanga_, currentDepth + 1));
			}
		}

		return TreeWalkInfo<TVN>{ treeDepth, walkNodes };
	}
	/// <summary>
	/// Walks tree in depth-first order (left to right)
	/// </summary>
	/// <param name="lambda">function<bool(Nod<TVN>*)> lambda: lambda receives a node and returns true if the walk should continue, false if it should stop</param>
	/// <returns></returns>
	TreeWalkInfo<TVN> walkDFSLambda(std::function<bool(Nod<TVN>*)> lambda) {
		int treeDepth = 0; // adancimea maxima a arborelui
		std::vector<std::tuple<Nod<TVN>*, int>> walkNodes; // tuple de forma (nod, adancime), 0-indexed

		std::unordered_set<Nod<TVN>*> visited; // pentru a evita ciclurile (in cazul in care exista)
		std::stack<std::tuple<Nod<TVN>*, int>> stack; // stiva pentru DFS: (nod, adancime)
		stack.push(std::tuple<Nod<TVN>*, int>(this, 0));

		while (!stack.empty()) {
			// pop nod + depth
			std::tuple<Nod<TVN>*, int> currentEntry = stack.top();
			stack.pop();
			Nod<TVN>* current = std::get<0>(currentEntry);
			int currentDepth = std::get<1>(currentEntry);

			if (visited.find(current) != visited.end())
				continue; // nodul a fost vizitat deja
			visited.insert(current); // vizitat

			walkNodes.push_back({ current, currentDepth }); // adaugam nodul curent in rezultat
			if (!lambda(current)) {
				break; // stop the walk if lambda returns false
			}

			if (currentDepth > treeDepth) // update tree depth while walking
				treeDepth = currentDepth;	// update tree depth while walking

			// adaugare copii in stiva (ordine inversa pentru a procesa de la stanga la dreapta)
			if (current->c_dreapta_ != nullptr) {
				stack.push(std::tuple<Nod<TVN>*, int>(current->c_dreapta_, currentDepth + 1));
			}
			if (current->c_stanga_ != nullptr) {
				stack.push(std::tuple<Nod<TVN>*, int>(current->c_stanga_, currentDepth + 1));
			}
		}

		return TreeWalkInfo<TVN>{ treeDepth, walkNodes };
	}

	/// <summary>
	/// Node must not connect to any CYCLE otherwise this method will not stop
	/// </summary>
	/// <returns>
	/// <br/> Returns a matrix of the form: 
	/// <br/>   [0], 
	/// <br/>   [_,2],
	/// <br/>   [_,_,5,6], // _ = null
	/// <br/>   ...
	/// <br/> Entries can be null if the tree is not complete
	/// </returns>
	std::vector<std::vector<Nod<TVN>*>> getBinTreeLayers() {
		std::vector<std::vector<Nod<TVN>*>> layers;

		int layer_idx = 0;
		std::vector<Nod<TVN>*> layer_current = { this };

		bool continue_walk = true;
		while (continue_walk) {
			size_t layer_sz = size_t(1) << layer_idx; // 2**(layer_idx + 1) = numarul de noduri posibile in layer-ul curent
			if (layer_current.size() != layer_sz) {
				std::cerr << "Warning: walkBFSAsLayers - layer " << layer_idx << " has " << layer_current.size() << " nodes, but expected " << layer_sz << " => INVALID" << std::endl;
				throw std::runtime_error("walkBFSAsLayers - invalid layer size");
			}

			continue_walk = false;
			std::vector<Nod<TVN>*> layer_next(layer_sz * 2, nullptr); // pregateste layer-ul urmator (initial gol)
			for (size_t i = 0; i < layer_current.size(); i++) {
				Nod<TVN>* n = layer_current[i];
				if (n != nullptr) {
					if (n->c_stanga_ != nullptr) {
						continue_walk = true;
						layer_next[i * 2] = n->c_stanga_; // copil stanga
					}
					if (n->c_dreapta_ != nullptr) {
						continue_walk = true;
						layer_next[i * 2 + 1] = n->c_dreapta_; // copil dreapta
					}
				}
			}

			layers.push_back(layer_current); // adauga layer-ul curent in rezultat
			layer_idx++;

			layer_current = layer_next; // trece la layer-ul urmator
		}

		return layers;
	}

	/// <summary>
	/// Insereaza valoarea respectand proprietatile unui arbore binar de cautare. Daca valoarea exista deja, incrementeaza contorul nodului existent.
	/// Nodul inserat va avea culoarea rosie pentru a respecta red-black tree insertion
	/// </summary>
	/// <param name="val"></param>
	/// <returns>
	///   returneaza nod-ul inserat
	/// </returns>
	Nod<TVN>* insertValueBST(TVN val) {
		if (val == this->valoare_) { // nu are loc nicio inserare, doar cresc contor
			this->contor_ += 1;
			return this;
		} else if (val < this->valoare_) {
			if (this->c_stanga_ == nullptr) {
				// <CAZ 1> nu exista copil stanga
				this->c_stanga_ = new Nod<TVN>(val, 1, false, this); // nod rosu, contor 1
				return this->c_stanga_;
			} else {
				// <CAZ 2> exista copil stanga
				return this->c_stanga_->insertValueBST(val);
			}

		} else { // val > this->valoare_
			if (this->c_dreapta_ == nullptr) {
				// <CAZ 1> nu exista copil dreapta
				this->c_dreapta_ = new Nod<TVN>(val, 1, false, this); // nod rosu, contor 1
				return this->c_dreapta_;
			} else {
				// <CAZ 2> exista copil dreapta
				return this->c_dreapta_->insertValueBST(val);
			}
		}
	}
	/// <summary>
	/// 
	/// </summary>
	/// <returns>
	///	nullptr if no uncle
	/// </returns>
	Nod<TVN>* getUncle() {
		Nod<TVN>* p = this->parinte_;
		if (p == nullptr) return nullptr;

		Nod<TVN>* g = p->parinte_;
		if (g == nullptr) return nullptr;

		return g->getOtherChild(p);
	}
	/// <summary>
	/// Updates parent for newChild
	/// </summary>
	/// <param name="oldChild">must not be null. Parent will be set to null</param>
	/// <param name="newChild">can be null. Parent will be set to this</param>
	void replaceChild(Nod<TVN>* oldChild, Nod<TVN>* newChild) {
		if (oldChild == nullptr)
			throw std::runtime_error("replaceChild error: oldChild is null");

		if (this->c_stanga_ == oldChild) {
			this->c_stanga_ = newChild;
		} else if (this->c_dreapta_ == oldChild) {
			this->c_dreapta_ = newChild;
		} else {
			throw std::runtime_error("replaceChild error: oldChild is not a child of this node");
		}

		// Parent new child & unlink from its current parent
		if (newChild != nullptr) {
			// now unlink newChild from its current parent manually
			Nod<TVN>* newChParent = newChild->parinte_;
			if (newChParent != nullptr) {
				if (newChParent->c_stanga_ == newChild) {
					newChParent->c_stanga_ = nullptr;
				} else if (newChParent->c_dreapta_ == newChild) {
					newChParent->c_dreapta_ = nullptr;
				}
			}
			// parent newChild to this
			newChild->parinte_ = this;
		}

		// Unparent old child
		oldChild->parinte_ = nullptr;
	}

	/// <summary>
	/// set left child: remove newLeftChild from parent and set it as left child of this
	/// null values are allowed
	/// </summary>
	/// <param name="newLeftChild"></param>
	void linkChildLeft(Nod<TVN>* newLeftChild) {
		if (this->c_stanga_ != nullptr) {
			this->replaceChild(this->c_stanga_, newLeftChild);
		} else {
			// unlink from parent
			if (newLeftChild != nullptr) {
				if (newLeftChild->parinte_ != nullptr)
					newLeftChild->parinte_->replaceChild(newLeftChild, nullptr);
				newLeftChild->parinte_ = this;
			}
			this->c_stanga_ = newLeftChild;
		}
	}
	/// <summary>
	/// set right child: remove newLeftChild from parent and set it as left child of this
	/// null values are allowed
	/// </summary>
	/// <param name="newLeftChild"></param>
	void linkChildRight(Nod<TVN>* newRightChild) {
		if (this->c_dreapta_ != nullptr) {
			this->replaceChild(this->c_dreapta_, newRightChild);
		} else {
			// unlink from parent
			if (newRightChild != nullptr) {
				if (newRightChild->parinte_ != nullptr)
					newRightChild->parinte_->replaceChild(newRightChild, nullptr);
				newRightChild->parinte_ = this;
			}
			this->c_dreapta_ = newRightChild;
		}
	}


	/// <summary>
	/// its left child becomes the new root of the subtree, and the original node becomes the right child of the new root.
	/// </summary>
	/// <returns>New tree root</returns>
	Nod<TVN>* rightRotate() {
		// Before: G (this)          After:     P
		//        / \                          / \
      //       P   U                        X   G
		//      / \                              / \
      //     X   Y                            Y   U

		if (this->c_stanga_ == nullptr)
			throw std::runtime_error("RightRotate error: left child is null");

		// G - P link -> reverse
		// G - U link remains intact
		// P - X link remains intact
		// P - Y link -> takes P's place

		Nod<TVN>* g = this; // not null
		Nod<TVN>* p = g->c_stanga_; // not null
		Nod<TVN>* x = p->c_stanga_; // nullable
		Nod<TVN>* y = p->c_dreapta_; // nullable

		// replace G with P in the eyes of G's parent
		if (g->parinte_ != nullptr)
			g->parinte_->replaceChild(g, p);

		p->linkChildRight(g);
		g->linkChildLeft(y);

		return p;
	}
	/// <summary>
	/// its right child becomes the new root of the subtree, and the original node becomes the left child of the new root.
	/// </summary>
	/// <returns>New tree root</returns>
	Nod<TVN>* leftRotate() {
		// Before:   G (this)    After:    P
		//          / \                   / \
		//         U   P                 G   X
		//            / \               / \
		//           Y   X             U   Y		

		if (this->c_dreapta_ == nullptr)
			throw std::runtime_error("LeftRotate error: right child is null");

		Nod<TVN>* g = this;
		Nod<TVN>* p = g->c_dreapta_; // not null
		Nod<TVN>* x = p->c_dreapta_; // nullable
		Nod<TVN>* y = p->c_stanga_; // nullable

		// replace G with P in the eyes of G's parent
		if (g->parinte_ != nullptr)
			g->parinte_->replaceChild(g, p);

		p->linkChildLeft(g);
		g->linkChildRight(y);

		return p;
	}

	/// <summary>
	/// Walk down the tree to the leftmost node
	/// </summary>
	/// <returns>will return this if no left child</returns>
	Nod<TVN>* walkToLeftmostDepth(int& depth) {
		depth = 0;
		Nod<TVN>* current = this;
		while (current->c_stanga_ != nullptr) {
			current = current->c_stanga_;
			depth++;
		}
		return current;
	}
	/// <summary>
	/// Walk down the tree to the rightmost node
	/// </summary>
	/// <returns>will retun this if no right child</returns>
	Nod<TVN>* walkToRightmostDepth(int& depth) {
		Nod<TVN>* current = this;
		depth = 0;
		while (current->c_dreapta_ != nullptr) {
			current = current->c_dreapta_;
			depth++;
		}
		return current;
	}
	/// <summary>
	/// Walk down the tree to the leftmost node
	/// </summary>
	/// <returns>will return this if no left child</returns>
	Nod<TVN>* walkToLeftmost() {
		Nod<TVN>* current = this;
		while (current->c_stanga_ != nullptr) {
			current = current->c_stanga_;
		}
		return current;
	}
	/// <summary>
	/// Walk down the tree to the rightmost node
	/// </summary>
	/// <returns>will retun this if no right child</returns>
	Nod<TVN>* walkToRightmost() {
		Nod<TVN>* current = this;
		while (current->c_dreapta_ != nullptr) {
			current = current->c_dreapta_;
		}
		return current;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	Nod<TVN>* walkToTop() {
		Nod<TVN>* current = this;
		while (current->parinte_ != nullptr) {
			current = current->parinte_;
		}
		return current;
	}

	/// <summary>
	/// Will not add null nodes to the walk
	/// </summary>
	/// <param name="walk_nodes"></param>
	void inorderTraversal_Helper(std::vector<Nod<TVN>*>& walk_nodes) {
		// explore left
		Nod<TVN>* left = this->c_stanga_;
		if (left != nullptr) {
			left->inorderTraversal_Helper(walk_nodes);
		}

		// parent
		walk_nodes.push_back(this);

		// explore right
		Nod<TVN>* right = this->c_dreapta_;
		if (right != nullptr) {
			right->inorderTraversal_Helper(walk_nodes);
		}
	}
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::vector<Nod<TVN>>* inorderTraversal() {
		std::vector<Nod<TVN>*> walk_nodes;
		this->inorderTraversal_Helper(walk_nodes);
		return walk_nodes;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::vector<TVN> inorderTraversalValues() {
		std::vector<TVN> values;
		std::vector<Nod<TVN>*> walk_nodes;
		this->inorderTraversal_Helper(walk_nodes);
		for (Nod<TVN>* n : walk_nodes) {
			values.push_back(n->getValoare());
		}
		return values;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <returns>secventa in ordine cu repetare in functie de contor</returns>
	std::vector<TVN> inorderTraversalValuesWithDup() {
		std::vector<TVN> values;
		std::vector<Nod<TVN>*> walk_nodes;
		this->inorderTraversal_Helper(walk_nodes);
		for (size_t i = 0; i < walk_nodes.size(); i++) {
			for (int j = 0; j < walk_nodes[i]->getContor(); j++) {
				values.push_back(walk_nodes[i]->getValoare());
			}
		}
		return values;
	}

	/// <summary>
	/// Nu se uita in sus la parinte, doar in jos la copii. Daca exista cicluri, metoda va intra in loop infinit
	/// </summary>
	/// <returns></returns>
	Nod<TVN>* deepCopyBST() {
		Nod<TVN>* copy = new Nod<TVN>(this->valoare_, this->contor_, this->negru_, nullptr, nullptr, nullptr);
		if (this->c_stanga_ != nullptr) {
			copy->c_stanga_ = this->c_stanga_->deepCopyBST();
			copy->c_stanga_->parinte_ = copy;
		}
		if (this->c_dreapta_ != nullptr) {
			copy->c_dreapta_ = this->c_dreapta_->deepCopyBST();
			copy->c_dreapta_->parinte_ = copy;
		}
		return copy;
	}

	/// <summary>
	/// 
	/// </summary>
	void cutAllTies() {
		if (this->parinte_ != nullptr) {
			this->parinte_->replaceChild(this, nullptr);
			this->parinte_ = nullptr;
		}

		if (this->c_stanga_ != nullptr)
			this->replaceChild(this->c_stanga_, nullptr);
		if (this->c_dreapta_ != nullptr)
			this->replaceChild(this->c_dreapta_, nullptr);
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	Nod<TVN>* searchBST(TVN value) {
		if (this->valoare_ == value) {
			return this;
		} else if (value < this->valoare_) {
			if (this->c_stanga_ != nullptr) {
				return this->c_stanga_->searchBST(value);
			} else {
				return nullptr;
			}
		} else { // value > this->valoare_
			if (this->c_dreapta_ != nullptr) {
				return this->c_dreapta_->searchBST(value);
			} else {
				return nullptr;
			}
		}
	}

	/// <summary>
	/// Replaces nodes and all links in tree. Does not respect BST properties or 
	/// red-black tree coloring, just performs a pure swap
	/// </summary>
	/// <param name="n1"></param>
	/// <param name="n2"></param>
	static void SwapNodesInTree(Nod<TVN>* n1, Nod<TVN>* n2) {
		// 1. Swap parents
		Nod<TVN>* p_temp = (n1 == nullptr) ? nullptr : n1->parinte_;
		if (n1 != nullptr) {
			// upd parent to point to n2
			if (n1->parinte_ != nullptr) {
				if (n1->parinte_->c_stanga_ == n1) {
					n1->parinte_->c_stanga_ = n2;
				} else if (n1->parinte_->c_dreapta_ == n1) {
					n1->parinte_->c_dreapta_ = n2;
				}
			}
			// update n1's parent to point to n2's parent
			n1->parinte_ = (n2 == nullptr) ? nullptr : n2->parinte_;
		}
		if (n2 != nullptr) {
			// upd parent to point to n1
			if (n2->parinte_ != nullptr) {
				if (n2->parinte_->c_stanga_ == n2) {
					n2->parinte_->c_stanga_ = n1;
				} else if (n2->parinte_->c_dreapta_ == n2) {
					n2->parinte_->c_dreapta_ = n1;
				}
			}

			// update n2's parent to point to n1's parent
			n2->parinte_ = p_temp;
		}

		// 2. Swap left children
		Nod<TVN>* l_temp = (n1 == nullptr) ? nullptr : n1->c_stanga_;
		if (n1 != nullptr) {
			// upd left child to point to n2
			if (n1->c_stanga_ != nullptr)
				n1->c_stanga_->parinte_ = n2;

			// update n1's left child to point to n2's left child
			n1->c_stanga_ = (n2 == nullptr) ? nullptr : n2->c_stanga_;
		}
		if (n2 != nullptr) {
			// upd left child to point to n1
			if (n2->c_stanga_ != nullptr)
				n2->c_stanga_->parinte_ = n1;

			// update n2's left child to point to n1's left child
			n2->c_stanga_ = l_temp;
		}

		// 3. Swap right children
		Nod<TVN>* r_temp = (n1 == nullptr) ? nullptr : n1->c_dreapta_;
		if (n1 != nullptr) {
			// upd right child to point to n2
			if (n1->c_dreapta_ != nullptr)
				n1->c_dreapta_->parinte_ = n2;
			// update n1's right child to point to n2's right child
			n1->c_dreapta_ = (n2 == nullptr) ? nullptr : n2->c_dreapta_;
		}
		if (n2 != nullptr) {
			// upd right child to point to n1
			if (n2->c_dreapta_ != nullptr)
				n2->c_dreapta_->parinte_ = n1;
			// update n2's right child to point to n1's right child
			n2->c_dreapta_ = r_temp;
		}
	}
#pragma endregion

#pragma region GETTERI_SETTERI
	TVN getValoare() const {
		return valoare_;
	}
	int getContor() const {
		return contor_;
	}
	void setContor(int contor) {
		this->contor_ = contor;
	}

	bool getIsNegru() const {
		return negru_;
	}
	void setIsNegru(bool negru) {
		this->negru_ = negru;
	}

	Nod<TVN>* getParinte() const {
		return parinte_;
	}
	void setParinte(Nod<TVN>* parinte) {
		this->parinte_ = parinte;
	}

	Nod<TVN>* getCStanga() const {
		return c_stanga_;
	}
	void setCStanga(Nod<TVN>* c_stanga) {
		this->c_stanga_ = c_stanga;
	}

	Nod<TVN>* getCDreapta() const {
		return c_dreapta_;
	}
	void setCDreapta(Nod<TVN>* c_dreapta) {
		this->c_dreapta_ = c_dreapta;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns>nr. copii: 0, 1, 2</returns>
	int getChildCount() const {
		int count = 0;
		if (c_stanga_ != nullptr) {
			count++;
		}
		if (c_dreapta_ != nullptr) {
			count++;
		}
		return count;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="child"></param>
	/// <returns></returns>
	Nod<TVN>* getOtherChild(Nod<TVN>* child) {
		if (c_stanga_ == child) return c_dreapta_;
		if (c_dreapta_ == child) return c_stanga_;
		return nullptr;
	}
#pragma endregion

#pragma region AFISARE_STRING
	/// <summary>
	/// Afiseaza arborele binar incepand de la nodul curent, similar cu comanda tree din Linux
	/// </summary>
	/// <param name="valMaxStrLen">The maximum length of the value</param>
	void AfiseazaRecursiv() {
		// obtine randuri de noduri
		std::vector<std::vector<Nod<TVN>*>> layers = this->getBinTreeLayers();

#pragma region Print Config
		constexpr const char* kColorRosu = "\033[91m"; // ANSI bright red
		constexpr const char* kColorReset = "\033[0m"; // ANSI reset (culoare implicita, pentru noduri negre)

		const int pr_space_min = 2; // print space min (intre nod-uri)
		const int pr_val_sz = 3; // print value size (in chars)

		int lr_elem = 1 << (layers.size() - 1); // last row elements count
		int lr_sz = lr_elem * pr_val_sz + (lr_elem - 1) * pr_space_min; // last row width in chars

#pragma endregion
		// <METHOD> constrain string representation of value to pr_val_sz chars
		auto pr_val_constrained = [&pr_val_sz](TVN value) {
			std::string val_str = std::to_string(value);
			if (val_str.size() > pr_val_sz) {
				return val_str.substr(0, pr_val_sz - 2) + "..";
			} else if (val_str.size() < pr_val_sz) {
				return std::string(pr_val_sz - val_str.size(), ' ') + val_str;
			} else {
				return val_str;
			}
		};
		// <METHOD> print char matrix; for each cell, print color from pr_colors (if set) then char from char_matrix
		// color_matrix rows have one extra trailing slot (index row.size()) for a post-row color code
		auto pr_char_matrix = [](const std::vector<std::vector<char>>& char_matrix, const std::vector<std::vector<std::optional<std::string>>>& color_matrix) {
			for (size_t row = 0; row < char_matrix.size(); row++) {
				for (size_t col = 0; col < char_matrix[row].size(); col++) {
					if (col < color_matrix[row].size() && color_matrix[row][col].has_value())
						std::cout << color_matrix[row][col].value();
					std::cout << char_matrix[row][col];
				}
				size_t trailing = char_matrix[row].size();
				if (trailing < color_matrix[row].size() && color_matrix[row][trailing].has_value())
					std::cout << color_matrix[row][trailing].value();
				std::cout << std::endl;
			}
		};
		// <METHOD> print to row; returns center of printed string
		// row = row to print to
		// str = string to print
		// @returns: the index of the center of the printed string in the row
		auto pr_to_row = [](std::vector<char>& row, const std::string& str) {
			size_t ch_center = row.size() + str.length() / 2;
			for (int i = 0; i < str.length(); i++) {
				row.push_back(str[i]);
			}
			return ch_center;
		};
		// <METHOD> print to row in insert only mode; nothing is added to the vector, only overridden
		// row = row to print to
		// str = string to print
		// center = the index to print the center of the string to; if the string has even length, it will be printed with 1 char more on the left side of the center
		// @returns: the index of the center of the printed string in the row (should be equal to the center parameter)
		auto pr_to_row_override = [](std::vector<char>& row, const std::string& str, size_t center) {
			size_t idx_start = center - str.length() / 2, idx_end = center + str.length() / 2 - (str.length() % 2 == 0 ? 1 : 0);
			if (idx_start < 0 || idx_end >= row.size()) {
				std::cerr << "Error: pr_to_row_override - string '" << str << "' with center " << center << " does not fit in row of size " << row.size() << std::endl;
				throw std::runtime_error("pr_to_row_override - string does not fit in row");
			}
			for (size_t i = 0; i < str.length(); i++) {
				row[idx_start + i] = str[i];
			}
			return center;
		};

		// <METHOD> 
		// unghi: (0, 90): '/'
		// unghi: (90, 180): '\'
		// unghi: 90: '-'
		// unghi: 0: '|'
		auto get_angle_char = [](double angle) {
			if (angle > 0.0 && angle < 90.0) {
				return '/';
			} else if (angle > 90.0 && angle < 180.0) {
				return '\\';
			} else if (angle == 90.0) {
				return '-';
			} else if (angle == 0.0 || angle == 180.0) {
				return '|';
			} else {
				return '?'; // unghi neasteptat
			}
		};
		// <METHOD>
		// mat = matrix to print to; 
		// start = (col, row) to start the line from (nu deseneaza la aceste coord)
		// end = (col, row) to end the line at (nu deseneaza la aceste coord)
		auto pr_walk_override = [get_angle_char](std::vector<std::vector<char>>& mat, std::tuple<size_t, size_t> start, std::tuple<size_t, size_t> end) {
			// x = col, y = row
			double s_x = get<0>(start), s_y = get<1>(start);
			double e_x = get<0>(end), e_y = get<1>(end);

			double dist = sqrt(pow(e_x - s_x, 2) + pow(e_y - s_y, 2));
			double angle = abs(atan2(s_y - e_y, s_x - e_x)) * 180 / std::numbers::pi;

			double step = 1.0 / (dist * sqrt(2)); // sa nu rateze nicio casuta
#ifdef DEBUG_AFISEAZA_RECURSIV
			std::cout << "Walk: start(" << s_x << ", " << s_y << "), end(" << e_x << ", " << e_y << ")" << ", angle: " << angle << ", dist: " << dist << ", step: " << step << std::endl;
#endif

			size_t prev_x = s_x, prev_y = s_y; // prev visited cell (go from start to end)
			std::vector<std::tuple<size_t, size_t>> path_taken; // for correcting L into /
			for (double t = step; t <= 1.0; t += step) {
				double c_x_d = s_x + (e_x - s_x) * t;
				double c_y_d = s_y + (e_y - s_y) * t;
				size_t c_x = round(c_x_d);
				size_t c_y = round(c_y_d);

				if (c_x == s_x && c_y == s_y)
					continue; // do not touch start cell
				if (c_x == e_x && c_y == e_y)
					continue; // do not touch end cell
				if (c_x == prev_x && c_y == prev_y)
					continue; // skip visited cell

#ifdef DEBUG_AFISEAZA_RECURSIV
				std::cout << " - walk step t=" << t << ": visit (" << c_x_d << ", " << c_y_d << ") -> (" << c_x << ", " << c_y << ")" << std::endl;
#endif
				if (mat[c_y][c_x] != ' ') { // do not override non-empty
#ifdef DEBUG_AFISEAZA_RECURSIV
					std::cout << " - skip cell (" << c_x << ", " << c_y << "); not empty (contains '" << mat[c_y][c_x] << "')" << std::endl;
#endif
					continue;
				}
				path_taken.push_back({ c_x, c_y });
				char ch_place = get_angle_char(angle);

				if (c_y == prev_y) { // same row, different col -> '-'
					ch_place = '-';
				} else if (c_x == prev_x) { // same col, different row -> '|'
					ch_place = '|';
				} else { // different row, different col -> '/' or '\'
					ch_place = get_angle_char(angle);
				}
				mat[c_y][c_x] = ch_place;
#ifdef DEBUG_AFISEAZA_RECURSIV
				std::cout << " - place char '" << ch_place << "' at (" << c_x << ", " << c_y << ")" << std::endl;
#endif

				// Check if char was placed in the 9x9 grid around the end point; if yes, stop the walk
				int d_end_x = abs(e_x - c_x), d_end_y = abs(e_y - c_y);
				if (std::max(d_end_x, d_end_y) <= 1) {
#ifdef DEBUG_AFISEAZA_RECURSIV
					std::cout << " - reached end vicinity, stop walk\n";
#endif
					break;
				}

				prev_x = c_x;
				prev_y = c_y;
			}

			// now look into path_taken, and only leave diagonal cells
			for (int i = 1; i < path_taken.size(); i++) {
				size_t path_a_x = std::get<0>(path_taken[i - 1]);
				size_t path_a_y = std::get<1>(path_taken[i - 1]);
				char path_a_ch = mat[path_a_y][path_a_x];

				size_t path_b_x = std::get<0>(path_taken[i]);
				size_t path_b_y = std::get<1>(path_taken[i]);
				char path_b_ch = mat[path_b_y][path_b_x];

				if (path_a_ch == '|' && path_b_ch == '-') {
					if (path_b_x < path_a_x) { // -> /
						mat[path_b_y][path_b_x] = '/';
						mat[path_a_y][path_a_x] = ' ';
					} else {
						mat[path_b_y][path_b_x] = ' ';
						mat[path_a_y][path_a_x] = '\\';
					}

				}
			}

#ifdef DEBUG_AFISEAZA_RECURSIV
			std::cout << "Walk end\n\n";
#endif
		};

		std::vector<std::vector<char>> pr_lines; // char matrix
		std::vector<std::vector<std::optional<std::string>>> pr_colors; // color matrix, mirrors pr_lines
		std::vector<size_t> lr_centers; // tine minte centrul fiecarui nod din ultimul rand 
		// indiferent daca este desenat sau nu

#pragma region Construieste Ultimul Rand
		std::vector<char> lr_line;
		std::vector<Nod<TVN>*> lr_vals = layers.back();
		std::string lr_pad_str = std::string(pr_space_min, ' ');

		for (int i = 0; i < lr_vals.size(); i++) {
			std::string n_str;
			if (lr_vals[i] != nullptr) {
				n_str = pr_val_constrained(lr_vals[i]->getValoare());
			} else {
				n_str = std::string(pr_val_sz, ' ');
			}

			size_t ch_center = pr_to_row(lr_line, n_str); // print value
			lr_centers.push_back(ch_center); // tine minte centrul nodului curent

			if (i < lr_vals.size() - 1)
				pr_to_row(lr_line, lr_pad_str); // print padding

			//std::cout << "lr_centers: push back " << ch_center << std::endl;
		}
		pr_lines.push_back(lr_line);
		// adauga coloring pentru ultimul rand
		{
			std::vector<std::optional<std::string>> lr_colors_row(lr_line.size() + 1, std::nullopt);
			size_t lr_pos = 0;
			for (int i = 0; i < (int)lr_vals.size(); i++) {
				if (lr_vals[i] != nullptr) {
					lr_colors_row[lr_pos] = std::string(lr_vals[i]->getIsNegru() ? kColorReset : kColorRosu);
					lr_colors_row[lr_pos + pr_val_sz] = std::string(kColorReset);
				}
				lr_pos += pr_val_sz;
				if (i < (int)lr_vals.size() - 1)
					lr_pos += pr_space_min;
			}
			pr_colors.push_back(lr_colors_row);
		}
#pragma endregion

#pragma region Construieste randurile Superioare
		// fiecare nod superior trebuie sa fie in centrul nodurilor copil din randul inferior
		std::vector<size_t> line_centers_prev = lr_centers;
		int logical_height = 1; // height from base
		for (int i = layers.size() - 2; i >= 0; i--) {
			std::vector<Nod<TVN>*> line = layers[i];

			std::vector<size_t> line_centers; // -1 pentru noduri nule
			// push a number of empty lines into pr_lines, at 0
			size_t rows_insert = logical_height == 1 ? 2 : 3; // daca e primul rand deasupra ultimului rand, avem nevoie de 1 rand pentru noduri si 1 rand pentru legaturi; 
			// altfel, avem nevoie de 1 rand pentru noduri si 2 randuri pentru legaturi
			size_t link_rows = rows_insert - 1;

			for (int j = 0; j < rows_insert; j++) {
				pr_lines.insert(pr_lines.begin(), std::vector<char>(lr_line.size(), ' '));
				pr_colors.insert(pr_colors.begin(), std::vector<std::optional<std::string>>(lr_line.size() + 1, std::nullopt));
			}

			// now print values on row 0 and draw legaturi
			for (int j = 0; j < line.size(); j++) {
				Nod<TVN>* n = line[j];

				size_t ch_left_center = line_centers_prev[j * 2];
				size_t ch_right_center = line_centers_prev[j * 2 + 1];
				size_t center = (ch_left_center + ch_right_center) / 2;

#ifdef DEBUG_AFISEAZA_RECURSIV
				if (n != nullptr)
					std::cout << "layer: " << i << ", node: " << n->getValoare()
					<< ", unite with ch.left(pos): " << ch_left_center << ", ch.right(pos): " << ch_right_center << std::endl;
#endif

				std::string n_str = (n != nullptr) ? pr_val_constrained(n->getValoare()) : std::string(pr_val_sz, ' ');
				pr_to_row_override(pr_lines[0], n_str, center); // print value to row 0 of layer_rows
				if (n != nullptr) {
					size_t idx_start = center - pr_val_sz / 2;
					if (!n->getIsNegru()) {
						pr_colors[0][idx_start] = std::string(kColorRosu);
						pr_colors[0][idx_start + pr_val_sz] = std::string(kColorReset);
					}
				}
				line_centers.push_back(center);
#ifdef DEBUG_AFISEAZA_RECURSIV
				std::cout << "Node center: " << center << std::endl;
#endif

				// Print legaturi 
				if (n != nullptr) {
					// positions are in (col, row) format
					if (n->getCStanga() != nullptr) {

						std::tuple<size_t, size_t> start, end;
						if (link_rows == 1) { // start and end for only 1 row for conn
							start = std::tuple<size_t, size_t>(center, 0); // start at center of node
							end = std::tuple<size_t, size_t>(ch_left_center + (pr_val_sz / 2 - 1), rows_insert); // end at right corner of child node

						} else { // start and end for 2 rows for conn
							start = std::tuple<size_t, size_t>(center - pr_val_sz / 2, 0);
							end = std::tuple<size_t, size_t>(ch_left_center + (pr_val_sz / 2), rows_insert);
						}

						pr_walk_override(pr_lines, start, end);
					}
					if (n->getCDreapta() != nullptr) {

						std::tuple<size_t, size_t> start, end;
						if (link_rows == 1) { // start and end for only 1 row for conn
							start = std::tuple<size_t, size_t>(center, 0); // start at center of node
							end = std::tuple<size_t, size_t>(ch_right_center - (pr_val_sz / 2), rows_insert); // end at left corner of child node

						} else { // start and end for 2 rows for conn
							start = std::tuple<size_t, size_t>(center + pr_val_sz / 2, 0);
							end = std::tuple<size_t, size_t>(ch_right_center - (pr_val_sz / 2), rows_insert);
						}

						pr_walk_override(pr_lines, start, end);
					}
				}
			}

			line_centers_prev = line_centers;
			logical_height++;
		}
#pragma endregion


		int pad_h = std::max(lr_sz / 2 - 2, 5);
		std::cout << std::string(pad_h, '>') << " BST " << std::string(pad_h, '>') << "\n";
		pr_char_matrix(pr_lines, pr_colors);
		std::cout << std::string(pad_h, '<') << std::string(5, '<') << std::string(pad_h, '<') << "\n";
	}
#pragma endregion

	/// <summary>
	/// Citeste un arbore binar din consola
	/// </summary>
	static Nod<TVN>* CreateFromConsole() {
		std::string line;
		std::getline(std::cin, line);
		std::replace(line.begin(), line.end(), ',', ' ');
		std::istringstream ss(line);
		TVN val;
		Nod<TVN>* root = nullptr;
		while (ss >> val) {
			if (root == nullptr) {
				root = new Nod<TVN>(val, 1, true, nullptr, nullptr, nullptr);
			} else {
				root->insertValueBST(val);
			}
		}
		return root;
	}
};
