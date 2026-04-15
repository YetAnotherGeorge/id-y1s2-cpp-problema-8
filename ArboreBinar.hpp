#pragma once
#include <iostream>
#include "Nod.hpp"

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
         // std::cout << "insertBST: Arbore gol, inserare ca radacina" << std::endl;
         Nod<TVN>* nod = new Nod<TVN>(val, 1, true, nullptr, nullptr, nullptr); // nod negru, contor 1
         this->radacina_ = nod;
         return this->radacina_;
      } else {
         return this->radacina_->insertValueBST(val);
      }
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
};
