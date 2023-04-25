#include "Commands.h"

void WriteSystem::execute(FileSystem* file_system, const string& name, const string& content) {

	file_system->calls_of_command_ls++;

	priority_queue <string, vector<string>, greater<string>> paths;//red u kojem cemo cuvati putanje svih elemenata stabla, koristimo prioritetni jer on automatski sortira red, pri cemu eksplicitno trazimo da ih sortiramo po principu greater

	//za ispis svih putanja obilazicemo stablo na sl. nacin: ROOT->BROTHER->SON kako bismo imali putanje svih foldera pre nego sto ispisemo njihove sinove

	TreeNode* node = file_system->tree_->getCurrentNode();//ispisuju se sadrzaj tekuceg direktorijuma i svih njehovih poddirektorijuma
	if (node != file_system->tree_->getRoot()) node = node->getSon();//ukoliko current_node nije root_ onda ne treba da se ispise i sam current_node pa ga prebacujemo na sina
	if (!node) throw new NoFoldersInCurrentDirectory();
	stack<TreeNode*> unvisitedNodes;
	while (node || !unvisitedNodes.empty()) {//dokle god je stek prazan ili node!=nullptr krecemo se kroz stablo
		if (node) {//nismo dosli do lista, ulazimo u sina, na steku cuvamo brata
			paths.push(file_system->getPath(node));//u red dodajemo putanju trenutnog cvora
			unvisitedNodes.push(node->getSon());
			node = node->getBrother();
		}
		else {//dosli smo do lista, pa prelazimo na cvor sa vrha steka
			node = unvisitedNodes.top();
			unvisitedNodes.pop();
		}
	}

	fstream output_file(file_system->output_file_, ios::app);//otvaramo fajl, ciju smo putanju zapamtili pri ucitavanju, za ispis, nastavjamo tamo gde smo stavili sa ispisom ili ga otvaramo tek ako vec nismo (app)

	if (file_system->calls_of_command_ls >1 || file_system->calls_of_handle_function>0)
		output_file<< endl;

	while (!paths.empty()) {
		if (paths.size() == 1) output_file << paths.top();
		else output_file << paths.top() << endl;
		paths.pop();//izbacujemo iz reda onog koga cemo sada ispisati
	}

	output_file.close();//zatvaramo fajl za ispis

}

void SetCurrentDirectoryByName::execute(FileSystem* file_system, const string& name, const string& content) {
	try {
		file_system->tree_->findNodeInCurrentDirectory(name);
		file_system->getTree()->setCurrentNodeByName(name);
	}
	catch (ParentFolderDoesNotExist* e) {
		throw new DirectoryWithThatNameDoesNotExistiInCurrentDirectory();//bacamo ipak ovaj zbog teksta, ali ne menjamo implementaciju setCurrentNodeByName
	}
}

void SetCurrentDirectoryToParent::execute(FileSystem* file_system, const string& name, const string& content) {
	if (!file_system->getTree()->getCurrentNode()->getFather())//ukoliko je otac nullptr
		if (file_system->tree_->getCurrentNode() != file_system->tree_->getRoot())//ako nije koren, onda koren postaje sada otac
			file_system->tree_->setCurrentNode(file_system->tree_->getRoot());//ukoliko je koren, ne mozemo ici vise u hijerarhiji i bacamo izuzetak
		else throw new NoParentFolder();
	else 
		file_system->getTree()->setCurrentNode(file_system->getTree()->getCurrentNode()->getFather());//ukoliko otac nije nullptr prelazimo na njega
}

void NewFolder::execute(FileSystem* file_system, const string& name, const string& content) {
	TreeNode* new_node = new TreeNode(new Folder(name));
	try {
		if (file_system->getTree()->getCurrentNode() != file_system->getTree()->getRoot())
			new_node->setFather(file_system->getTree()->getCurrentNode());//odmah postavljamo da je otac current_node
		else
			new_node->setFather(nullptr);
		file_system->getTree()->checkIfFolderByThatNameAlreadyExists(new_node);//proveravamo da li takav folder vec postoji u tom nivou hijerarhije
		if (!file_system->getTree()->getCurrentNode()) {
			throw new NoFoldersInCurrentDirectory();
		}
		else if (file_system->tree_->getCurrentNode()!=file_system->tree_->getRoot()) file_system->getTree()->addSon(new_node);
		else file_system->getTree()->addBrother(new_node);
		if (new_node->getFather()) file_system->tree_->setCurrentNode(new_node->getFather());//vracamo current_node na stari jer se vratio na root_pri dodavanju
		else file_system->tree_->setCurrentNode(file_system->tree_->getRoot());
	}
	catch (FolderWithThatNameAlreadyExists* e) {
		delete new_node;
		throw e;
	}
}

void NewFile::execute(FileSystem* file_system, const string& name, const string& content) {
	TreeNode* new_node = new TreeNode(new File("", txt, content));
	try {
		if (file_system->getTree()->getCurrentNode() != file_system->getTree()->getRoot())
			new_node->setFather(file_system->getTree()->getCurrentNode());//odmah postavljamo da je otac current_node
		else
			new_node->setFather(nullptr);
		string only_name = "";
		int position = 0;
		while (name[position] != '.') only_name += name[position++];//izdvajamo samo ime
		position++;
		string only_extension = "";
		for (int i = 0; i < 3; i++) only_extension += name[position++];
		Extension extension;
		if (only_extension == "txt") extension = txt;//izdvajamo samo ekstenziju
		else extension = exe;
		new_node->getElement()->setName(only_name);
		new_node->getElement()->setExtension(extension);
		file_system->getTree()->checkIfFileByThatNameAlreadyExists(new_node);//proveravamo da li takav fajl vec postoji
		if (!file_system->getTree()->getCurrentNode()) {
			throw new NoFoldersInCurrentDirectory();
		}
		else if (file_system->tree_->getCurrentNode() == file_system->tree_->getRoot()) file_system->getTree()->addBrother(new_node);
		else file_system->getTree()->addSon(new_node);
		if (new_node->getFather()) file_system->tree_->setCurrentNode(new_node->getFather());//vracamo current_node na stari jer se vratio na root_pri dodavanju
		else file_system->tree_->setCurrentNode(file_system->tree_->getRoot());
	}
	catch (FileWithThatNameAlreadyExists* e) {
		delete new_node;
		throw e;
	}
}

void DeleteFolder::execute(FileSystem* file_system, const string& name, const string& content) {
	
	TreeNode* node = file_system->tree_->findNodeInCurrentDirectory(name);
	//brisemo sve puteve do ovog cvora u stablu
	file_system->tree_->removePathsToThisNode(node);
	//brisemo sve njegove sinove
	if (node->getSon())
		file_system->tree_->deletePartOfTheTree(node->getSon());
	//brisemo njega 
	delete node;
	if (!file_system->tree_->getRoot()) throw new FileSystemIsEmpty();//ukoliko smo obrisali poslednji cvor bacamo izzetak kako bismo prekinuli execute
}

void DeleteFile::execute(FileSystem* file_system, const string& name, const string& content) {

	string only_name = "";
	int position = 0;
	while (name[position] != '.') only_name += name[position++];//izdvajamo samo ime
	position++;
	string only_extension = "";
	for (int i = 0; i < 3; i++) only_extension += name[position++];
	Extension extension;
	if (only_extension == "txt") extension = txt;//izdvajamo samo ekstenziju
	else extension = exe;
	TreeNode* node = file_system->tree_->findNodeInCurrentDirectory(only_name, extension);
	//brisemo sve puteve do ovog cvora u stablu
	file_system->tree_->removePathsToThisNode(node);
	//brisemo sve njegove sinove
	if (node->getSon())
		file_system->tree_->deletePartOfTheTree(node->getSon());
	//brisemo njega 
	delete node;
	if (!file_system->tree_->getRoot()) throw new FileSystemIsEmpty();//ukoliko smo obrisali poslednji cvor bacamo izzetak kako bismo prekinuli execute
}

void Execute::execute(FileSystem* file_system, const string& name, const string& content) {

	TreeNode* node = file_system->tree_->findNodeInCurrentDirectory(name, exe);
	string str = fillTextWithNewLines(node->getElement()->getContent());//menjamo "\n" sa '\n' u stringu, kako bi se uklopilo sa metodom executeWithString iz FileSystem klase
	file_system->executeWithString(str);

}

//metoda koja menja "\n" sa '\n' u stringu, kako bi se uklopilo sa metodom executeWithString iz FileSystem klase
string Execute::fillTextWithNewLines(const string& str) {
	int position = 0;
	string new_string = "";
	while (position < str.length()) {
		if (str[position] == '\\' && str[position + 1] == 'n') {
			position += 2;
			string isNextCommand = "";
			int i = 0;
			if ((position + 2) < str.length()) {
				for (i = 0; i < 2; i++) isNextCommand += str[position++];
				if (isNextCommand == "LS" || isNextCommand == "CD")
					new_string += '\n';
				else {
					isNextCommand += str[position++];
					if (isNextCommand == "NEW" || isNextCommand == "DEL" || isNextCommand == "EXE")
						new_string += '\n';
					else {
						new_string += '\\';//slucaj kada content nekof file koji dodajemo ima vise redova pa ostavljamo "\n" a ne dodajemo '\n'
						new_string += 'n';
					}
				}
				new_string += isNextCommand;
			}
			else new_string += '\n';
		}
		else new_string += str[position++];
	}
	return new_string;
}
