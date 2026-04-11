#pragma once
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>
#include <queue>
#include <stack>
#include <stdexcept>
#include <cmath>

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
	/// Afiseaza arborele binar incepand de la nodul curent, similar cu comanda tree din Linux
	/// </summary>
	/// <param name="valMaxStrLen">The maximum length of the value</param>
	void AfiseazaRecursiv(int valMaxStrLen) {
		// Pentru culoarea nod-ului
		constexpr const char* kColorRosu = "\033[91m"; // ANSI bright red
		constexpr const char* kColorReset = "\033[0m";  // ANSI reset (culoare implicita, pentru noduri negre)

		std::vector<std::vector<Nod<TVN>*>> layers = this->getBinTreeLayers();
		const int space_min = 2; // spatiul minim intre noduri
		std::string space_min_pad(space_min, ' '); // padding pentru spatiul minim

		int lr_elem = 1 << (layers.size() - 1); // last row elements count

		// last row width in chars; all items must be uniformly distributed in this width
		int lr_sz = lr_elem * valMaxStrLen + (lr_elem - 1) * space_min; 

		int ch_count_header_half = std::max(abs(lr_sz / 2), 10);
		std::cout << std::string(ch_count_header_half - 4, '<') << " ARBORE " << std::string(ch_count_header_half - 4, '<') << "\n";
		// print each row 
		for (int i = 0; i < layers.size(); i++) {
			std::vector<Nod<TVN>*> r = layers[i]; // row
			int r_sz = r.size() * valMaxStrLen + (r.size() - 1) * space_min; // current row width in chars
			int r_extra_pad = (lr_sz - r_sz) / (r.size() + 1); 
			std::string r_extra_pad_str(r_extra_pad, ' '); // extra padding to distribute in row

			// Print values row
			for (int j = 0; j < r.size(); j++) {
				// get string representation of node value, or empty string if null
				std::string n_str;
				Nod<TVN>* n = r[j];
				if (n == nullptr) {
					n_str = std::string(valMaxStrLen, ' ');
				} else {
					n_str = std::to_string(n->getValoare());
					if (n_str.size() < valMaxStrLen) {
						// pad left with spaces to reach valMaxStrLen
						n_str = std::string(valMaxStrLen - n_str.size(), ' ') + n_str;
					}
				}

				// print node with color and padding
				std::cout << r_extra_pad_str;
				if (n != nullptr && n->negru_) {
					std::cout << n_str;
				} else {
					std::cout << kColorRosu << n_str << kColorReset;
				}
				std::cout << space_min_pad;
			}
			std::cout << "\n";

			// Print / \ connections row:
			// - each / must start under the space before the first char
			// - each \ must start under the last char
			if (i < layers.size() - 1) {
				std::vector<Nod<TVN>*> r = layers[i]; // row
				std::vector<Nod<TVN>*> r_next = layers[i + 1]; // next row
				
				int c_extra_pad = r.size() % 2 == 0 ? r_extra_pad : r_extra_pad - 1;
				std::string c_extra_pad_str(c_extra_pad, ' '); 

				for (int j = 0; j < r.size(); j++) {
					//std::cout << c_extra_pad 
					Nod<TVN>* n = r[j];
					std::cout << c_extra_pad_str;

					if (n == nullptr) {
						std::cout << std::string(valMaxStrLen + 2, ' ');
					} else {
						char ch_stanga = n->c_stanga_ != nullptr ? '/' : ' ';
						char ch_dreapta = n->c_dreapta_ != nullptr ? '\\' : ' ';
						std::cout << ch_stanga << std::string(valMaxStrLen, ' ') << ch_dreapta;
					}
				}

				std::cout << "\n";
			}
		}

		std::cout << std::string(ch_count_header_half * 2, '>') << std::endl;
	}
#pragma endregion
};
