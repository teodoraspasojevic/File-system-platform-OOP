#include "Tree.h"

//inicijalizacija statickih polja
int Tree::function_add_brother_calls = 1;
int Tree::function_add_son_calls = 1;

//destruktor
Tree::~Tree() {

	deletePartOfTheTree(root_);
	root_ = nullptr;
	current_node_ = nullptr;
}

//geteri

TreeNode* Tree::getRoot() const {
	return root_;
}

TreeNode* Tree::getCurrentNode() const {
	return current_node_;
}

//seter

void Tree::setCurrentNode(TreeNode* node) {
	current_node_ = node;
}

//metoda koja dodaje brata trenutnom cvoru
void Tree::addBrother(TreeNode* node) {

	try {
		checkName(node);//metoda koja proverava da li vec postoji fajl/folder sa istim imenom u istom nivou hijerarhije

		if (function_add_brother_calls == 1 && root_) {//ukoliko je ovo prvi poziv ove metode za prosledjeni cvor, za oca mu stavljamo oca trenutnog cvora
			node->setFather(current_node_->getFather());
		}
		if (root_ == nullptr) {//ukoliko jos nismo poceli da pravimo stablo dodajemo root_
			root_ = current_node_ = node;
			function_add_brother_calls = 1;
			return;
		}
		if (current_node_->getBrother() == nullptr) {//ukoliko vec nema brata, dodajemo ga
			current_node_->setBrother(node);
			current_node_ = root_;
			function_add_brother_calls = 1;
			return;
		}
		else {
			function_add_brother_calls++;//posto ce biti rekurentnih poziva inkrementiramo brojac poziva
			current_node_ = current_node_->getBrother();//ukoliko vec ima brata, ulazimo u njega i njemu dodajemo brata
			addBrother(node);
		}
	}
	catch (FileWithThatNameAlreadyExists* e) {
		handleExceptionInAddMethods(node);
		throw e;//ponovo bacamo izuzetak da bismo ga kasnije ponovo uhvatili i pozvali metodu handlingAnException
	}
	catch (FolderWithThatNameAlreadyExists* e) {
		handleExceptionInAddMethods(node);
		throw e;//ponovo bacamo izuzetak da bismo ga kasnije ponovo uhvatili i pozvali metodu handlingAnException
	}
}

//metoda koja dodaje sina trenutnom cvoru
void Tree::addSon(TreeNode* node) {

	if (function_add_son_calls == 1) {//ukoliko prvi put pozivamo ovu metodu za ovaj cvor, otac mu je trenutni cvor
		node->setFather(current_node_);
	}

	try {
		checkName(node);//pozivamo metodu koja ce baciti izuzetak ukoliko vec postoji file/folder sa istim imenom

		if (!current_node_->getElement()->isFile()) {//ukoliko je cvor kome zelimo da dodelimo sina fajl baca se izuzetak
			if (current_node_->getSon() == nullptr) {//ukoliko vec nema sina, dodajemo ga
				current_node_->setSon(node);
				current_node_ = root_;
				function_add_son_calls = 1;
				return;
			}
			else {
				function_add_son_calls=1;
				current_node_ = current_node_->getSon();//ukoliko cvor vec ima sina, ulazimo u njega i njemu dodeljujemo brata
				addBrother(node);
			}
		}
		else {
			throw new FileCanNotHaveSon();
		}

	}
	catch (FileCanNotHaveSon* e) {
		handleExceptionInAddMethods(node);
		throw e;//ponovo bacamo izuzetak da bismo ga kasnije ponovo uhvatili i pozvali metodu handlingAnException
	}
	catch (FileWithThatNameAlreadyExists* e) {
		handleExceptionInAddMethods(node);
		throw e;//ponovo bacamo izuzetak da bismo ga kasnije ponovo uhvatili i pozvali metodu handlingAnException
	}
	catch (FolderWithThatNameAlreadyExists* e) {
		handleExceptionInAddMethods(node);
		throw e;//ponovo bacamo izuzetak da bismo ga kasnije ponovo uhvatili i pozvali metodu handlingAnException
	}
}

//metoda koja setuje trenutni cvor na onaj cije ime odgovara prosledjenom parametru
void Tree::setCurrentNodeByName(const string& name) {

	//za pretragu binarnog stabla cemo koristiti sl. nacin obilaska stabla: ROOT->BROTHER->SON
	TreeNode* node;
	if (current_node_ == root_)
		node = current_node_;
	else node = current_node_->getSon();
	if (!node) throw new ParentFolderDoesNotExist();

	stack<TreeNode*> unvisitedNodes;
	while (node || !unvisitedNodes.empty()) {//dokle god je stek prazan ili node!=nullptr i nema ime kao ono koje trazimo se krecemo kroz stablo
		if (node)
			if (node->getElement()->getName() == name) break;//ukoliko smo nasli element koji ima isto ime kao trazeno, prekidamo potragu
		if (node) {//nismo dosli do lista, ulazimo u sina, na steku cuvamo brata
			unvisitedNodes.push(node->getSon());
			node = node->getBrother();
		}
		else {//dosli smo do lista, pa prelazimo na cvor sa vrha steka
			node = unvisitedNodes.top();
			unvisitedNodes.pop();
		}
	}
	if (!node) {//ukoliko je node==nullptr nismo nasli odgovarajuci folder pa bacamo izuzetak
		throw new ParentFolderDoesNotExist();
	}
	else {
		current_node_=node;//setujemo trenutni cvor na onaj koji smo nasli
	}
}

//metoda koja proverava da li u hijerarhiji vec postoji folder sa tim imenom, ako da pamti ga u prosledjenom cvoru 
void Tree::checkIfFolderByThatNameAlreadyExists(TreeNode* node_parameter) {
	
	//za pretragu binarnog stabla cemo koristiti preorder nacin obilaska stabla kojim se stablo obilazi na sl. nacin:
	//  ROOT->LEFT_CHILD->RIGHT_CHILD ----> u nasem slucaju: ROOT->SON->BROTHER

	TreeNode* node = current_node_;
	stack<TreeNode*> unvisitedNodes;
	while (node || !unvisitedNodes.empty()) {//dokle god je stek prazan ili node!=nullptr krecemo se kroz stablo
		if (node)
			if (node->getElement()->getName() == node_parameter->getElement()->getName() && !node->getElement()->isFile() && node->getFather() == node_parameter->getFather()) {
				node_parameter = node;//pamtimo taj cvor u parametru jer nam moze zatrebati van ove metode
				break;//ukoliko smo nasli folder koji ima isto ime kao trazeno i u istom je nivou (imaju istog oca), prekidamo potragu
			}
		if (node) {//nismo dosli do lista, ulazimo u sina, na steku cuvamo brata
			unvisitedNodes.push(node->getBrother());
			node = node->getSon();
		}
		else {//dosli smo do lista, pa prelazimo na cvor sa vrha steka
			node = unvisitedNodes.top();
			unvisitedNodes.pop();
		}
	}
	if (node) {//ukoliko je node!=nullptr nasli smo folder koji vec postoji sa tim imenom u istom nivou hijerarhije pa bacamo izuzetak
		throw new FolderWithThatNameAlreadyExists();
	}
}
	
//metoda koja proverava da li u hijerarhiji vec postoji fajl sa tim imenom, ako da pamti ga u prosledjenom cvoru 
void Tree::checkIfFileByThatNameAlreadyExists(TreeNode* node_parameter) {

	//za pretragu binarnog stabla cemo koristiti preorder nacin obilaska stabla kojim se stablo obilazi na sl. nacin:
	//  ROOT->LEFT_CHILD->RIGHT_CHILD ----> u nasem slucaju: ROOT->SON->BROTHER

	TreeNode* node = current_node_;
	stack<TreeNode*> unvisitedNodes;
	while (node || !unvisitedNodes.empty()) {//dokle god je stek prazan ili node!=nullptr krecemo se kroz stablo
		if (node)
			if (node->getElement()->getName() == node_parameter->getElement()->getName() && node->getElement()->isFile() && node->getFather() == node_parameter->getFather() && node->getElement()->getExtension() == node_parameter->getElement()->getExtension()) {
				node_parameter = node;//pamtimo taj cvor u parametru jer nam moze zatrebati van ove metode
				break;//ukoliko smo nasli fajl koji ima isto ime i ekstenziju kao trazeno i u istom je nivou (imaju istog oca), prekidamo potragu
			}
		if (node) {//nismo dosli do lista, ulazimo u sina, na steku cuvamo brata
			unvisitedNodes.push(node->getBrother());
			node = node->getSon();
		}
		else {//dosli smo do lista, pa prelazimo na cvor sa vrha steka
			node = unvisitedNodes.top();
			unvisitedNodes.pop();
		}
	}
	if (node) {//ukoliko je node!=nullptr nasli smo fajl koji vec postoji sa tim imenom i istom ekstenzijom u istom nivou hijerarhije pa bacamo izuzetak
		throw new FileWithThatNameAlreadyExists();
	}
}

//metoda koja poziva, na osnovu toga da li je node file ili folder, odgovarajucu metodu za proveru postojanja cvora sa istim imenom
void Tree::checkName(TreeNode* node) {

	if (node->getElement()->isFile())
		checkIfFileByThatNameAlreadyExists(node);
	else
		checkIfFolderByThatNameAlreadyExists(node);

}

//metoda koja hvata izuzetak i prosledjuje ga dalje i brise napravljen cvor koji ne ubacujemo
void Tree::handleExceptionInAddMethods(TreeNode* node) {
	current_node_ = root_;//ukoliko je doslo do bacanja izuzetka, vracamo current_node_ na root_
	delete node;
}

//pronalazi element stabla u trenutnom direktorijumu (samo tom jednom nivou)
TreeNode* Tree::findNodeInCurrentDirectory(const string& name, Extension extension) {
	
	TreeNode* node;
	if (current_node_ == root_) node = root_;
	else node = current_node_->getSon();
	if (!node) throw new NoFoldersInCurrentDirectory();
	while (node) {
		if (node->getElement()->getName() == name) {
			if (node->getElement()->isFile() && node->getElement()->getExtension() == extension) {
				return node;
			}
			else if (!node->getElement()->isFile()) return node;
		}
		node = node->getBrother();//proveravamo samo u prvom nivou hijerarhije
	}
	if (!node) throw new DirectoryWithThatNameDoesNotExistiInCurrentDirectory();

}

//brise proslednjeni cvor i celo stablo ispod njega
void Tree::deletePartOfTheTree(TreeNode* part) {
	//za brisanje binarnog stabla mozemo koristiti postorder obilazak stabla kojim se stablo obilazi na sl. nacin:
	//  LEFT_CHILD->RIGHT_CHILD->ROOT ----> u nasem slucaju: SON->BROTHER->ROOT

	TreeNode* node = part;
	TreeNode* lastVisited = nullptr;//poslednji obidjen cvor
	stack<TreeNode*> unvisitedNodes; //svaki put kada dodjemo do cvora koji ima jos dece prelazimo na decu, a na stek stavljamo roditeljski cvor, a ako postoje oba detete prvo ulazimo u levi
	while (node || !unvisitedNodes.empty()) {
		if (node) {
			unvisitedNodes.push(node);
			node = node->getSon(); //prelazimo na sina, cuvamo roditelja na steku
		}
		else {
			TreeNode* topNode = unvisitedNodes.top();//uzimamo cvor sa vrha steka jer je node=nullptr
			if (topNode->getBrother() && topNode->getBrother() != lastVisited) {
				node = topNode->getBrother();//ako brat postoji i on nije bio poslednji obilazen (vec je bio roditelj) prelazimo na njega
			}
			else {
				//dosli smo do lista i brisemo ga
				lastVisited = topNode;
				delete topNode;
				unvisitedNodes.pop();
			}
		}
	}
}

//metoda koja se proverava da li se neki cvor nalati u bilo kojem poddirektorijumu trenutnog direktorijuma
bool Tree::isCurrentNodeAboveThisNode(TreeNode* node_parameter) {
	
	TreeNode* node;
	if (current_node_ == root_) node = current_node_;
	else node = current_node_->getSon();
	stack<TreeNode*> unvisitedNodes;
	while (node || !unvisitedNodes.empty()) {//dokle god je stek prazan ili node!=nullptr krecemo se kroz stablo
		if (node) {
			if (node == node_parameter)
				break;
		}
		if (node) {//nismo dosli do lista, ulazimo u sina, na steku cuvamo brata
			unvisitedNodes.push(node->getBrother());
			node = node->getSon();
		}
		else {//dosli smo do lista, pa prelazimo na cvor sa vrha steka
			node = unvisitedNodes.top();
			unvisitedNodes.pop();
		}
	}
	if (node) return true;
	else return false;
}

//metoda koja uklanja sve puteve u stablu do nekog cvora
void Tree::removePathsToThisNode(TreeNode* node_parameter) {

	if (node_parameter == root_) {
		root_ = node_parameter->getBrother();
		return;
	}

	TreeNode* node =  root_;
	stack<TreeNode*> unvisitedNodes;
	while (node || !unvisitedNodes.empty()) {//dokle god je stek prazan ili node!=nullptr krecemo se kroz stablo
		if (node && node->getBrother()==node_parameter) {
			node->setBrother(node_parameter->getBrother());//ukoliko je ovaj cvor nekome brat, pokazivac azuriramo na sledeceg brata
			break;
		}
		if (node && node->getSon() == node_parameter) {
			node->setSon(node_parameter->getBrother());//ukoliko je ovaj cvor nekome sin, pokazivac na sina onoga ciji je on sin azuriramo na sledeceg sina
			break;
		}
		if (node) {//nismo dosli do lista, ulazimo u sina, na steku cuvamo brata
			unvisitedNodes.push(node->getBrother());
			node = node->getSon();
		}
		else {//dosli smo do lista, pa prelazimo na cvor sa vrha steka
			node = unvisitedNodes.top();
			unvisitedNodes.pop();
		}
	}
}
