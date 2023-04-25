#include "TreeNode.h"

TreeNode::TreeNode() {
	reset();
}

TreeNode::TreeNode(SystemElement* element): element_(element), son_(nullptr), brother_(nullptr) {}

//kako objekat klase TreeNode u sebi ima pokazivace na druge objekte, ukoliko ne obrisemo objekte na koje oni pokazuju doci ce do curenja memorije, pa moramo napisati svoj destruktor
TreeNode::~TreeNode() {
	delete element_;//ne brisemo i objekte na koje pokazuju son_ i brother_ jer cemo stablo brisati postorder obilaskom stabla, pa ce oni vec biti obrisani kada dodjemo do trenutnog cvora koji brisemo
	reset();
}

//geteri

TreeNode* TreeNode::getSon() const {
	return son_;
}

TreeNode* TreeNode::getBrother() const {
	return brother_;
}

TreeNode* TreeNode::getFather() const {
	return father_;
}

SystemElement* TreeNode::getElement() const {
	return element_;
}

//seteri

void TreeNode::setSon(TreeNode* node) {
	son_ = node;
}

void TreeNode::setBrother(TreeNode* node) {
	brother_ = node;
}

void TreeNode::setFather(TreeNode* node) {
	father_ = node;
}

void TreeNode::setElement(SystemElement* element) {
	element_ = element;
}


//pomocne metode

//metoda resetuje pokazivace objekta TreeNode
void TreeNode::reset() {
	element_ = nullptr;
	son_ = brother_ = father_ = nullptr;
}
