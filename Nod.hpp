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
#include <numbers>
#include <optional>

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
		// <METHOD> print char matrix (without color insertion)
		auto pr_char_matrix = [](const std::vector<std::vector<char>>& char_matrix) {
			for (const std::vector<char>& line : char_matrix) {
				for (char c : line) {
					std::cout << c;
				}
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
			std::cout << "Walk: start(" << s_x << ", " << s_y << "), end(" << e_x << ", " << e_y << ")" << ", angle: " << angle << ", dist: " << dist << ", step: "<< step << std::endl;
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
				std::cout << " - walk step t=" << t << ": visit ("<< c_x_d << ", " << c_y_d << ") -> (" << c_x << ", " << c_y << ")" << std::endl;
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

			for (int j = 0; j < rows_insert; j++)
				pr_lines.insert(pr_lines.begin(), std::vector<char>(lr_line.size(), ' '));

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


		std::cout << std::string(lr_sz / 2 - 2, '>') << " BST " << std::string(lr_sz / 2 - 2, '>') << "\n";
		pr_char_matrix(pr_lines);
		std::cout << std::string(lr_sz, '<') << "\n";
	}

#pragma endregion
};
