export module ArboreBinar;

/// <summary>
/// 
/// </summary>
/// <typeparam name="TVN">
/// Tip valoare nod. Trebuie sa implementeze =, ==, <, <=, >, >=
/// </typeparam>
template <typename TVN> class Nod {
private: 
	TVN valoare_;
	int contor_; // Google C++ style

	Nod<TVN>* parinte_; // NULL daca nodul este radacina
	Nod<TVN>* c_stanga_; // copilul din stanga (cu valoare mai mica)
	Nod<TVN>* c_dreapta_; // copilul din dreapta (cu valoare mai mare)

public: 
	/// <summary>
	/// Nod fara parinte, copil stanga/dreapta
	/// </summary>
	/// <param name="valoare"></param>
	Nod(TVN valoare) {
		valoare_ = valoare;
		contor_ = 1;
		parinte_ = nullptr;
		c_stanga_ = nullptr;
		c_dreapta_ = nullptr;
	}
	/// <summary>
	/// Nod cu parinte, fara copil stanga/dreapta
	/// </summary>
	/// <param name="valoare"></param>
	/// <param name="parinte"></param>
	Nod(TVN valoare, Nod<TVN>* parinte) {
		valoare_ = valoare;
		contor_ = 1;
		parinte_ = parinte;
		c_stanga_ = nullptr;
		c_dreapta_ = nullptr;
	}
};

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
};

