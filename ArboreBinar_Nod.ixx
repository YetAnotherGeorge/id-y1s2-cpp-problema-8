module;
#include <iostream>
#include <string>

export module ArboreBinar:Nod;
// Notes: 
// Google C++ style for private members: name_ (with trailing underscore)

/// <summary>
/// 
/// </summary>
/// <typeparam name="TVN">
/// Tip valoare nod. Trebuie sa implementeze =, ==, <, <=, >, >=
/// </typeparam>
export template <typename TVN> class Nod {
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
	Nod(TVN val): Nod(val, 1, false, nullptr, nullptr, nullptr) {}
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
				// <CAZ 1>, nu exista copil stanga
				this->c_stanga_ = new Nod<TVN>(val, 1, false, this); // nod rosu, contor 1
				return this->c_stanga_;
			} else {
				// <CAZ 2> daca exista copil stanga, exista 2 cazuri: 
				//   - c_stanga_->valoare_ < val -> inserare nod care va inlocui c_stanga_ (cazul 1)
				//   - c_stanga_->valoare_ >= val -> delegare insertValueBST catre c_stanga_ (cazul 2)
				if (this->c_stanga_->valoare_ < val) {
					// insert between current node and c_stanga_
					Nod<TVN>* c_stanga_old = this->c_stanga_;
					Nod<TVN>* c_stanga_new = new Nod<TVN>(val, 1, false, this);
					c_stanga_new->c_stanga_ = c_stanga_old;
					c_stanga_old->parinte_ = c_stanga_new;
					this->c_stanga_ = c_stanga_new;
					return c_stanga_new;
				} else {
					return this->c_stanga_->insertValueBST(val);
				}
			}

		} else { // val > this->valoare_
			if (this->c_dreapta_ == nullptr) {
				// <CAZ 1>, nu exista copil dreapta
				this->c_dreapta_ = new Nod<TVN>(val, 1, false, this); // nod rosu, contor 1
				return this->c_dreapta_;
			} else {
				// <CAZ 2> daca exista copil dreapta, exista 2 cazuri: 
				//   - c_dreapta_->valoare_ < val -> inserare nod care va inlocui c_dreapta_ (cazul 1)
				//   - c_dreapta_->valoare_ >= val -> delegare insertValueBST catre c_dreapta_ (cazul 2)
				if (this->c_dreapta_->valoare_ < val) {
					// insert between current node and c_dreapta_
					Nod<TVN>* c_dreapta_old = this->c_dreapta_;
					Nod<TVN>* c_dreapta_new = new Nod<TVN>(val, 1, false, this);
					c_dreapta_new->c_dreapta_ = c_dreapta_old;
					c_dreapta_old->parinte_ = c_dreapta_new;
					this->c_dreapta_ = c_dreapta_new;
					return c_dreapta_new;
				} else {
					return this->c_dreapta_->insertValueBST(val);
				}
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
	/// <param name="prefix">Prefix acumulat pentru indentare</param>
	/// <param name="isLast">True daca nodul curent este ultimul copil al parintelui</param>
	void AfiseazaRecursiv(const std::string& prefix = "", bool isLast = true) const {
		constexpr const char* kBifurcatie = "\u251c"; // ├ - ramura cu continuare
		constexpr const char* kColt       = "\u2514"; // └ - ultima ramura
		constexpr const char* kLinieOriz  = "\u2500"; // ─ - linie orizontala
		constexpr const char* kLinieVert  = "\u2502"; // │ - linie verticala
		constexpr const char* kColorRosu  = "\033[91m"; // ANSI bright red
		constexpr const char* kColorReset = "\033[0m";  // ANSI reset (culoare implicita, pentru noduri negre)

		const char* culoare = negru_ ? kColorReset : kColorRosu;

		if (prefix.empty()) {
			std::cout << culoare << valoare_ << kColorReset << "\n";
		} else {
			std::cout << prefix << (isLast ? kColt : kBifurcatie) << kLinieOriz << kLinieOriz << " " << culoare << valoare_ << kColorReset << "\n";
		}

		bool hasLeft = c_stanga_ != nullptr;
		bool hasRight = c_dreapta_ != nullptr;

		if (!hasLeft && !hasRight) return;

		std::string childPrefix = prefix.empty() ? "" : prefix + (isLast ? "    " : std::string(kLinieVert) + "   ");

		if (hasLeft && hasRight) {
			c_stanga_->AfiseazaRecursiv(childPrefix, false);
			c_dreapta_->AfiseazaRecursiv(childPrefix, true);
		} else if (hasLeft) {
			c_stanga_->AfiseazaRecursiv(childPrefix, true);
		} else {
			c_dreapta_->AfiseazaRecursiv(childPrefix, true);
		}
	}
#pragma endregion
};

