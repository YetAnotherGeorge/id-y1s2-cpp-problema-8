#pragma once
#include <iostream>
#include <vector>
#include <algorithm>


/// <summary>
/// 
/// </summary>
/// <param name="v"></param>
/// <param name="nume"></param>
/// <returns></returns>
void printeazaVector(const std::vector<int>& v, const std::string& nume) {
	std::cout << nume << ": [";
	for (int i = 0; i < v.size(); i++) {
		std::cout << v[i];
		if (i < v.size() - 1)
			std::cout << ", ";
	}
	std::cout << "]" << std::endl;
}
/// <summary>
/// 
/// </summary>
/// <param name="nrElem"></param>
/// <param name="min">Valoarea minima a elementelor (inclusiv)</param>
/// <param name="max">Valoarea maxima a elementelor (exclusiv)</param>
/// <returns>
/// In output pot fi prezente duplicate
/// </returns>
std::vector<int> genereazaSecventaOrdonata(int nrElem, int min, int max) {
	// Algoritm
	// arr va avea valori in intervalul [0, nrElem)
	// 
	// 1. genereaza numar random [0, nrElem) => n
	// 2. verifica daca poate fi adaugat la pozitia n
	// 3. daca da, adauga-l, daca nu, verifica inapoi (while arr[k] == empty || arr[k] >= n)
	// 4. daca nu, verifica inainte (while arr[k] == empty || arr[k] <= n)
	// Daca nu exista loc pentru n, scoate n din secventa de numere aleatoare posibila
	//
	// maparea valorilor: [0, nrElem) => [min, max]

	std::vector<int> arr(nrElem, -1); // -1 inseamna "gol"

	// numere aleatoare disponibile; valori in intervalul [0, nrElem)
	std::vector<int> nrAlDisp(nrElem);
	for (int i = 0; i < nrElem; i++) {
		nrAlDisp[i] = i;
	}

	while (nrAlDisp.size() > 0) {
		int nrAlIdx = rand() % nrAlDisp.size();
		int nrAl = nrAlDisp[nrAlIdx];
		bool adaugat = false;

		// try to add
		if (arr[nrAl] == -1) {
			arr[nrAl] = nrAl;
			adaugat = true;
		} else {
			// 1. inserare inapoi
			for (int i = nrAl - 1; i >= 0; i--) {
				if (arr[i] == -1) { // gasit -> insereaza
					arr[i] = nrAl;
					adaugat = true;
					break;
				} else if (arr[i] < nrAl) {  // nu se poate insera si pastra ordinea
					break;
				}
			}

			// 2. inserare inainte
			if (!adaugat) {
				for (int i = nrAl + 1; i < arr.size(); i++) {
					if (arr[i] == -1) { // gasit -> insereaza
						arr[i] = nrAl;
						adaugat = true;
						break;
					} else if (arr[i] > nrAl) {  // nu se poate insera si pastra ordinea
						break;
					}
				}
			}
		}

		if (!adaugat)
			nrAlDisp.erase(nrAlDisp.begin() + nrAlIdx);
	}

	//// verificare
	//for (int i = 0; i < arr.size() - 1; i++) {
	//	if (arr[i] == -1 || arr[i + 1] == -1) {
	//		std::cout << "Secventa nu este completa!" << std::endl;
	//		return {};
	//	}
	//	if (arr[i] > arr[i + 1]) {
	//		std::cout << "Secventa nu este ordonata!" << std::endl;
	//		return {};
	//	}
	//}


	// mapare [0, nrElem) => [min, max]
	int nrOut = max - min;
	double scale = (double)(nrOut) / nrElem;
	for (int i = 0; i < arr.size(); i++) {
		arr[i] = (int)(arr[i] * scale) + min;
	}

	// daca max - min > nrElem => vor lipsi mereu elemente din intervalul [min, max)
	if (nrOut > nrElem) {
		int cap = max; // exclusive end
		for (size_t i = arr.size(); i-- > 0;) {
			int n = arr[i];
			int d = cap - n;
			if (d > 0) {
				int n_add = rand() % d;
				arr[i] = n + n_add;
			}
			cap = arr[i];
		}
	}
	return arr;
}
/// <summary>
/// 
/// </summary>
/// <param name="nrElem"></param>
/// <param name="min"></param>
/// <param name="max"></param>
/// <returns>
/// In output pot fi prezente duplicate
/// </returns>
std::vector<int> genereazaSecventa(int nrElem, int min, int max) {
	std::vector<int> arr(nrElem);
	for (int i = 0; i < nrElem; i++) {
		arr[i] = rand() % (max - min) + min;
	}
	return arr;
}

/// <summary>
/// 
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns>
///  <br/> a &lt; b => -1
///  <br/> a == b => 0
///  <br/> a &gt; b => 1
/// </returns>
template <typename T>
int comparaLexicografic(const std::vector<T>& a, const std::vector<T>& b) {
	size_t lmin = std::min(a.size(), b.size());
	for (size_t i = 0; i < lmin; i++) {
		if (a[i] < b[i])
			return -1;
		else if (a[i] > b[i])
			return 1;
	}

	if (a.size() < b.size())
		return -1;
	else if (a.size() > b.size())
		return 1;
	else
		return 0;
}