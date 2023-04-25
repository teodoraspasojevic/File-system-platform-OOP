//-klasa Tree predstavlja binarno stablo koje predstavlja hijerarhiju fajlova i foldera u nasem fajl sistemu
//-kako FileSystem zapravo predstavlja n-arno stablo (jer svaki folder moze imati n fajlova/foldera u sebi), koristimo algoritam koji kaze da
//fajl/folder koji se nalazi u tekucem folderu dodajemo kao levog sina(son_), a fajl/folder koji je u istom nivou kao tekuci dodajemo kao desnog sina(brother_)
//-stablo je potpuno odredjeno svojim korenom, a dodatno imamo i polje current_node_ koje nam moze zatrebati u implementaciji odredjenih metoda

#ifndef TREE_H
#define TREE_H

#include "Exceptions.h"
#include "TreeNode.h"
#include <stack>
using namespace std;

class Tree {
public:
	Tree() : root_(nullptr), current_node_(root_) {};
	Tree(const Tree&) = delete;
	Tree(Tree&&) = delete;
	~Tree();

	//geteri

	TreeNode* getRoot() const;
	TreeNode* getCurrentNode() const;

	//seter
	void setCurrentNode(TreeNode* node);

	void addBrother(TreeNode* node);
	void addSon(TreeNode* node);
	//pomocne metode
	void setCurrentNodeByName(const string& name);
	void checkIfFolderByThatNameAlreadyExists(TreeNode* node_parameter);
	void checkIfFileByThatNameAlreadyExists(TreeNode* node_parameter);
	void checkName(TreeNode* node);
	void handleExceptionInAddMethods(TreeNode* node);
	TreeNode* findNodeInCurrentDirectory(const string& name, Extension extension = txt);
	void deletePartOfTheTree(TreeNode* part);
	bool isCurrentNodeAboveThisNode(TreeNode* node_parameter);
	void removePathsToThisNode(TreeNode* node_parameter);

private:
	TreeNode* root_;//koren stabla
	TreeNode* current_node_;//tekuci direktorijum

	//staticka polja koja sluze kao brojaci poziva metoda addBrother i addSon
	static int function_add_brother_calls;
	static int function_add_son_calls;
};

#endif
