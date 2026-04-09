## Problema 8
Clasa pentru arbori binari de căutare stricţi cu contor, având chei de un tip neprecizat, prevăzut cu o ordine totală (şi cu operatorii relaţionali, operatorii de ordine fiind corespunzători respectivei ordini totale), cu supraîncărcarea operatorilor indicaţi mai jos:

\=, \=\=; <, <\=, >, >\= (operatorii de ordine întorc 1 sau conform ordonării lexicografice a şirurilor ordonate care rezultă prin parcurgerea în ordine a operanzilor, cu repetarea fiecărei chei conform contorului);

"+" se supraîncarcă în 3 moduri: 
- a) primul operand cheie, al doilea operand arbore binar de căutare strict cu contor; 
- b) primul operand arbore binar de căutare strict cu contor, al doilea operand cheie; 
- c) ambii operanzi arbori binari de căutare stricţi cu contor; rezultatul operaţiilor a) b) este arborele obţinut prin inserarea cheii în arbore; rezultatul operaţiei c) este arborele obţinut prin inserarea în primul arbore a cheilor arborelui al doilea, parcurse în inordine, cu repetarea fiecărei chei conform contorului; 

"-" se supraîncarcă în 2 moduri: 
- a) primul operand arbore binar de căutare strict cu contor, al doilea operand cheie; b) ambii operanzi arbori binari de căutare stricţi cu contor; rezultatul operaţiei a) este arborele obţinut prin ştergerea cheii din arbore; rezultatul operaţiei 
- b) este arborele obţinut prin ştergerea din primul arbore a cheilor arborelui al doilea, parcurse în inordine, cu repetarea fiecărei chei conform contorului.

Cerinţe comune
Toate clasele vor trebui să aibă constructor fără argumente, constructor de copiere, cel puţin încă un constructor, destructor şi metode de citire a datelor de la consolă.