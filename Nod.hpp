#pragma once
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>
#include <queue>
#include <stack>

// Note generale: 
//  - Google C++ style for private members: name_ (with trailing underscore)
//

template <typename TVN> class Nod;

template <typename TVN> struct TreeWalkInfo {
	const int treeDepth; // adancimea maxima a arborelui
	const std::vector<std::tuple<Nod<TVN>*, int>> walkNodes; // tuple de forma (nod, adancime), 0-indexed

	/// <summary>
	/// Pretty print result of walk
	/// </summary>
	/// <param name="out"></param>
	void Print(std::ostream& out, bool brief = false) const {
		out << "Walk [treeLayers: " << treeDepth+1 << "]: ";
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
	bool negru_; // true daca nodul este negru, false daca este rosu (pentru arbori rosii-negri); DEFAULT false

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
	/// Insereaza valoarea respectand proprietatile unui arbore binar de cautare. Daca valoarea exista deja, incrementeaza contorul nodului existent.
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
#pragma endregion

#pragma region GETTERI_SETTERI
	TVN getValoare() const {
		return valoare_;
	}
	int getContor() const {
		return contor_;
	}
	bool isNegru() const {
		return negru_;
	}
	Nod<TVN>* getParinte() const {
		return parinte_;
	}
	Nod<TVN>* getCStanga() const {
		return c_stanga_;
	}
	Nod<TVN>* getCDreapta() const {
		return c_dreapta_;
	}
#pragma endregion

#pragma region AFISARE_STRING
	/// <summary>
	/// Afiseaza arborele incepand de la nodul curent, similar cu comanda tree din Linux
	/// </summary>
	/// <param name="valMaxStrLen">The maximum length of the value</param>
	void AfiseazaRecursiv(int valMaxStrLen) {
		// Pentru culoarea nod-ului
		constexpr const char* kColorRosu = "\033[91m"; // ANSI bright red
		constexpr const char* kColorReset = "\033[0m";  // ANSI reset (culoare implicita, pentru noduri negre)

		TreeWalkInfo<TVN> wBFS = this->walkBFS();
	}
#pragma endregion
};
