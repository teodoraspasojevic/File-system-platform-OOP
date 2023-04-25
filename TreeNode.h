//-klasa TreeNode je klasa koja predstavlja cvor stabla koje predstavlja FileSystem
//-kako svaki folder moze imati u sebi n drugih foldera, potrebno nam je n-arno stablo, medjutim koriscenjem jednog algoritma mozemo preci na binarno stablo
//-ideja je da svaki folder/fajl koji se nalazi u drugom folderu dodajemo kao levog sina (son_) trenutnog foldera,
//a folder koji se nalazi u istom nivou hijerarhije kao trenutni fajl dodajemo kao desnog sina (brother_)
//-TreeNode ima pokazivac na objekat klase SystemElement, pa se pod njim mogu nalaziti i folderi i fajlovi
//jer u C++ postoji implicitna konverzija iz pokazivaca na objekat izvedene klase u pokazivac na objekat osnovne klase
//-dodatno imamo i polje father_ jer zbog ovakvog nacina generisanja stabla, otac nije uvek jedan nivo iznad naseg cvora, pa je pogodno cuvati pokazivac na oca

#ifndef TREE_NODE_H
#define TREE_NODE_H

#include "Exceptions.h"
#include "SystemElement.h"

class TreeNode {
public:
	TreeNode();
	TreeNode(SystemElement* element);
	TreeNode(const TreeNode&) = delete;
	TreeNode(TreeNode&&) = delete;
	~TreeNode();

	//geteri
	TreeNode* getSon() const;
	TreeNode* getBrother() const;
	TreeNode* getFather() const;
	SystemElement* getElement() const;

	//seteri
	void setSon(TreeNode* node);
	void setBrother(TreeNode* node);
	void setFather(TreeNode* node);
	void setElement(SystemElement* element);

	//pomocne metode
	void reset();

private:
	SystemElement *element_;
	TreeNode *son_, *brother_, *father_;
};

#endif
