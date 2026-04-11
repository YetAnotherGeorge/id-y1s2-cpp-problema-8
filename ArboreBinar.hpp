#pragma once
#include "Nod.hpp"
#include <iostream>

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
   /// Destructor
   /// </summary>
   ~ArboreBinar() {
      if (radacina_ != nullptr) {
         delete radacina_;
      }
   }

#pragma region INSERTION / CREATION OPS
   /// <summary>
   /// Seteaza radacina copacului, stergand radacina existenta daca este cazul.
   /// </summary>
   /// <param name="val"></param>
   /// <returns>returneaza radacina copacului, goala</returns>
   Nod<TVN>* initRadacina(TVN val) {
      if (this->radacina_ != nullptr) {
         std::cout << "initRadacina: Radacina deja initializata, suprascriere" << std::endl;
         delete this->radacina_;
      }
      this->radacina_ = new Nod<TVN>(val);
      std::cout << "initRadacina: Radacina initializata cu valoarea '" << val << "'" << std::endl;
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
         std::cout << "insertBST: Arbore gol, inserare ca radacina" << std::endl;
         Nod<TVN>* nod = new Nod<TVN>(val, 1, true, nullptr, nullptr, nullptr); // nod negru, contor 1
         this->radacina_ = nod;
         return this->radacina_;
      } else {
         return this->radacina_->insertValueBST(val);
      }
   }
#pragma endregion

   /// <summary>
   /// Afiseaza arborele incepand de la radacina
   /// </summary>
   void AfiseazaRecursiv() const {
      if (radacina_ != nullptr) {
         radacina_->AfiseazaRecursiv();
      } else {
         std::cout << "Arbore gol!" << std::endl;
      }
   }
};
