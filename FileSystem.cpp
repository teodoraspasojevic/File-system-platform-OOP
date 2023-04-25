#include "FileSystem.h"

int FileSystem::function_read_one_element_calls = 0;//inicijalizacija statickog polja
int FileSystem::counter = 0;//inicijalizacija statickog polja
int  FileSystem::calls_of_command_ls=0;//inicijalizacija statickog polja
int  FileSystem::calls_of_handle_function = 0;//inicijalizacija statickog polja

//destruktor
FileSystem::~FileSystem() {
	delete tree_;
	tree_ = nullptr;
	output_file_ = "";
}

//metoda koja ucitava hijerarhiju iz stringa fs_filepath
void FileSystem::loadHierarchy(const string& fs_filepath, const string& log_filepath) {

	//cuvamo putanju ka izlaznom fajlu u kome vrsimo ispis
	output_file_ = log_filepath;

	//ukoliko je prethodno vec ucitana hijerarhija, sve informacije o staroj hijerarhiji fajl sistema se brisu i ucitava se nova hijerarhija
	if (tree_->getRoot()) {
		cout << "Note: There was already one file system loaded. By loading new, old will be deleted." << endl;
		delete tree_;
		tree_ = new Tree();
	}

	//iz datoteke cije je ime u stringu fs_filepath ucitavamo string koji predstavlja fajl sistem koji ucitavamo
	ifstream input(fs_filepath);
	stringstream ss;
	ss << input.rdbuf(); 
	input.close();
	string str = ss.str();

	int position = 0;//brojac koji ce nam sluziti za kretanje kroz string

	while (position < str.length()) {//dokle god ne dodjemo do kraja stringa, ucitavamo element po element fajl sistema
		readOneElement(str, position);
	}
	tree_->setCurrentNode(tree_->getRoot());//posle kreiranja tekuci direktorijum treba da pokazuje na root_!!
}

//metoda koja pokrece komende upisane u fajlu sa putanjom filepath
void FileSystem::execute(const string& filepath) {

	//iz datoteke cije je ime u stringu filepath ucitavamo string koji predstavlja fajl sistem koji ucitavamo
	ifstream input(filepath);
	stringstream ss;
	ss << input.rdbuf();
	input.close();
	string str = ss.str();

	executeWithString(str);

	calls_of_command_ls = 0;
	calls_of_handle_function = 0;
}

//geter
Tree* FileSystem::getTree() {
	return tree_;
}

//metoda koja cita ime fajla/foldera
string FileSystem::readName(const string& str, int& position) {

	string name = "";
	//ukoliko ne pocinje putanja sa \ baca se izuzetak
	if (str[position++] != '\\') {
		throw new FolderNameDoesNotBeginWithBackslash();
	}
	//u imenu foldera se mogu naci samo slova, brojevi i _, citamo dok se ne nadje neki drugi karakter ili dok ne dodjemo do kraja stringa
	while (((str[position] >= 'a' && str[position] <= 'z') || (str[position] >= 'A' && str[position] <= 'Z') || (str[position] >= '0' && str[position] <= '9') || str[position] == '_') && position < str.length()) {
		name += str[position++];
	}
	if (str[position] != ' ' && str[position] != '\n' && str[position] != '.' && str[position] != '\\') throw new NoneValidDirectoryName();
	return name;
}

//metoda koja cita ekstenziju fajla
Extension FileSystem::readExtension(const string& str, int& position) {
	string extension = "";

	//ucitavamo tri karaktera koja predstavljaju ekstenziju
	for (int i = 0; i < 3; i++) {
		if (str[position] == 't' || str[position] == 'x' || str[position] == 'e') {
			extension += str[position++];
		}
		else {
			throw new NoneExistingFileExtension();//ukoliko ne postoji ovakva ekstenzija bacamo izuzetak
		}
	}

	//proveravamo da li posle ekstenzije sledi ' '
	if (str[position++] != ' ') {
		throw new NoBackspaceAfterExtension();
	}

	//proveravamo da li je ekstenzija txt ili exe
	if (extension == "txt") {
		return txt;
	}
	else if (extension == "exe") {
		return exe;
	}
	else {
		throw new NoneExistingFileExtension();//ukoliko ne postoji ovakva ekstenzija bacamo izuzetak
	}
}

//metoda koja cita sadrzaj fajla
string FileSystem::readContent(const string& str, int& position) {
	string content = "";//radice i ukoliko je fajl prazan, vratice se samo prazan string
	while (str[position] != '\n' && position < str.length()) {
		content += str[position++];
	}
	position++;
	return content;
}

//metoda koja cita jedan element fajl sistema, odnosno jedan red iz stringa
void FileSystem::readOneElement(const string& str, int& position) {

	try {
		function_read_one_element_calls++;//svaki put kada ulazimo u ovu metodu inkrementiramo brojac poziva ove metode (brojimo zbog rekurenzije)

		string name = readName(str, position);
		//razmatramo da li smo dosli do kraja stringa
		if (position == str.length()) {//slucaj kada hijerarhija ima samo jedan folder
			tree_->addBrother(new TreeNode(new Folder(name)));
			return;//zavrsili smo sa citanjem reda
		}
		//razmatramo kakav je karakter na kome smo stali
		switch (str[position]) {
			case '\\': {//slucaj kada jos uvek nismo ucitali celu putanju do foldera/fajla
				tree_->setCurrentNodeByName(name);//trenutni cvor setujemo na onaj koji smo upravo procitali
				readOneElement(str, position);//pozivamo rekurentno ovu metodu i prosledjujemo kao current_node cvor koji smo nasli po upravo ucitanom imenu
				break;
			}
			case '\n': {//slucaj kada smo dosli do kraja reda i dodajemo sina trenutnom folderu
				position++;
				if (function_read_one_element_calls == 1) {
					tree_->addBrother(new TreeNode(new Folder(name)));//ukoliko je ovo prvi poziv ove metode za ovaj element dodajemo folder kao brata
				}
				else {
					tree_->addSon(new TreeNode(new Folder(name)));//ukoliko ovo nije prvi poziv ove metode za ovaj element, folder dodajemo kao brata
				}
				function_read_one_element_calls = 0;
				counter++;//povecavamo brojac elemenata u stablu
				return;
			}
			case '.': {//treba da ucitamo fajl
				position++; //brojac pomeramo sa tacke
				Extension exstension = readExtension(str, position);
				string content = readContent(str, position);
				if (function_read_one_element_calls == 1) {
					tree_->addBrother(new TreeNode(new File(name, exstension, content)));//ukoliko je ovo prvi poziv ove metode za ovaj element dodajemo fajl kao brata
				}
				else {
					tree_->addSon(new TreeNode(new File(name, exstension, content)));//ukoliko ovo nije prvi poziv ove metode za ovaj element, fajl se sigurno nalazi u nekom folderu, pa ga dodajemo kao sina
				}
				function_read_one_element_calls = 0;
				counter++;//povecavamo brojac elemenata u stablu
				return;
			}
		}
	}
	catch (FolderDoesNotHaveExtension* e) {
		handlingAnException(e, str, position);
	}
	catch (FolderDoesNotHaveContent* e) {
		handlingAnException(e, str, position);
	}
	catch (FolderNameDoesNotBeginWithBackslash* e) {
		handlingAnException(e, str, position);
	}
	catch (NoneExistingFileExtension* e) {
		handlingAnException(e, str, position);
	}
	catch (NoBackspaceAfterExtension* e) {
		handlingAnException(e, str, position);
	}
	catch (ParentFolderDoesNotExist* e) {
		handlingAnException(e, str, position);
	}
	catch (FileCanNotHaveSon* e) {
		handlingAnException(e, str, position);
	}
	catch (FileWithThatNameAlreadyExists* e) {
		handlingAnException(e, str, position);
	}
	catch (FolderWithThatNameAlreadyExists* e) {
		handlingAnException(e, str, position);
	}
	catch (NoneValidDirectoryName* e) {
		handlingAnException(e, str, position);
	}
}

//metoda koja pomera brojac na sledecu liniju ili kraj stringa, ukoliko nema vise linija
void FileSystem::moveToNextLine(const string& str, int& position) {

	while (str[position] != '\n' && position < str.length()) {
		position++;
	}
	if (position < str.length())
		if (str[position] == '\n') position++;
}

//metoda koja se poziva pri hvatanju izuzetka, ona prelazi na sledeci red i ispisuje na stdout opis greske
void FileSystem::handlingAnException(exception* e, const string& str, int& position) {
	cout << e->what() << endl;
	moveToNextLine(str, position);
	tree_->setCurrentNode(tree_->getRoot());
	delete e;
}

//metoda koja nalazi putanju svakog elementa u stablu
string FileSystem::getPath(TreeNode* node) {

	string path = "";//u ovom stringu cuvamo putanju ka prosledjenom elementu fajl sistema
	while (node && tree_->isCurrentNodeAboveThisNode(node)) {
		string name = "";//u ovom stringu cuvamo ime cvora na kome se nalazimo trenutno
		name += '\\';//svaka putanja pocinje ovim znakom
		name += node->getElement()->getName();//dodajemo ime
		if (node->getElement()->isFile()) {//ukoliko je u putanju fajl dodajemo i ekstenziju
			name += '.';
			if (node->getElement()->getExtension() == txt) name += "txt";
			else name += "exe";
		}
		path = name + path;//posto mi ide od sinova ka ocevima, a putanja nam je potrebno da se ispise u suprotnom smeru, na dobijeno ime dodajemo ona vec postojeca, jer su to imena snova trenutnog cvora
		node = node->getFather();//prelazimo na oca
	}
	return path;
}

//metoda koja se poziva pri hvatanju izuzetaka NoneValidCallOfCommand
void FileSystem::handlingAnExceptionInExecute(exception* e, const string& str, int& position, const string& command_line) {
	moveToNextLine(str, position);
	fstream output_file(output_file_, ios::app);//otvaramo fajl, ciju smo putanju zapamtili pri ucitavanju, za ispis
	if (calls_of_command_ls>0 || calls_of_handle_function>0) output_file << endl;
	output_file <<"Error: "<< command_line;
	output_file.close();
	delete e;
	calls_of_handle_function++;
}

//metoda koja nam sluzi kako bismo mogli da je pozivamo i u EXE komandi, prosledjujemo joj gotov string a ne putanju ka datoteci u kojoj su komande
void FileSystem::executeWithString(const string& str) {

	int position = 0;
	while (position < str.length()) {
		string command_line = readCommandLine(str, position);
		try {
			switch (str[position]) {
				case 'L': {//slucaj kada ocekujemo LS
					if (str[position + 1] != 'S' || (str[position + 2] != '\n' && (position+2)<str.length()))
						throw new NoneValidCallOfCommand();//bacamo izuzetak u slucaju nevalidnog poziva komande
					WriteSystem command_write;
					command_write.execute(this);//pozivamo komandu LS
					moveToNextLine(str, position);//prelazimo na sledeci red komande
					break;
				}
				case 'E': {//slucaj kada ocekujemo EXE
					if (str[position + 1] != 'X' || str[position + 2] != 'E' || str[position + 3] != ' ' ) 
						throw new NoneValidCallOfCommand();//bacamo izuzetak u slucaju nevalidnog poziva komande
					position += 4;
					string name = "";
					//u imenu foldera se mogu naci samo slova, brojevi i _, citamo dok se ne nadje neki drugi karakter ili dok ne dodjemo do kraja stringa
					while (((str[position] >= 'a' && str[position] <= 'z') || (str[position] >= 'A' && str[position] <= 'Z') || (str[position] >= '0' && str[position] <= '9') || str[position] == '_') && position < str.length()) {
						name += str[position++];
					}
					if (str[position] != '.') throw new NoneValidDirectoryName();//bacamo izuzetak u slucaju nevalidnog imena foldera/fajla ili znaka posle njega
					position++;
					string extension = "";
					for (int i = 0; i < 3; i++) extension += str[position++];
					if (extension != "exe") throw new CanNotRunNotExeFile();//ukoliko fajl nema exe ekstenziju bacamo izuzetak
					Execute command;
					command.execute(this, name);//pozivamo EXE komandu
					moveToNextLine(str, position);//prelazimo na sledeci red komande
					break;
				}
				case 'C': {//ocekujemo ili CD .. ili CD name
					if (str[position + 1] != 'D' || str[position + 2] != ' ') throw new NoneValidCallOfCommand();//bacamo izuzetak u slucaju nevalidnog poziva komande
					position += 3;
					if (str[position] == '.' && str[position + 1] == '.') {
						if (str[position + 2] != '\n' && (position+2) < str.length()) throw new NoneValidCallOfCommand();//bacamo izuzetak ako ne sledi novi red ako ovo nije poslednja komanda
						SetCurrentDirectoryToParent command;
						command.execute(this);//pozivamo CD ..
						moveToNextLine(str, position);//prelazimo na sledeci red poziva komandi
					}
					else {//ocekujemo CD name
						string name = readNameInCommands(str, position);
						if (isThisFileName(name)) throw new CanNotSetFileForCurrentDirectory();
						SetCurrentDirectoryByName command;
						command.execute(this, name);//pozivamo CD name
						moveToNextLine(str, position);
					}
					break;
				}
				case 'N': {
					if (str[position + 1] != 'E' || str[position + 2] != 'W' || str[position + 3] != ' ') throw new NoneValidCallOfCommand();//bacamo izuzetak u slucaju nevalidnog poziva komande
					position += 4;
					string name = readNameInCommands(str, position);
					bool is_file = isThisFileName(name);
					//poziv metode
					if (is_file) {
						NewFile command;
						if (str[position] != ' ') throw new NoBackspaceAfterExtension();
						position++;
						string content = readContent(str, position);
						command.execute(this, name, content);
					}
					else {
						NewFolder command;
						if (str[position] != '\n') throw new NoneValidCallOfCommand();//bacamo izuzetak u slucaju nevalidnog poziva komande
						command.execute(this, name);
						moveToNextLine(str, position);
					}
					break;
				}
				case 'D': {
					if (str[position + 1] != 'E' || str[position + 2] != 'L' || str[position + 3] != ' ') throw new NoneValidCallOfCommand();//bacamo izuzetak u slucaju nevalidnog poziva komande
					position += 4;
					string name = readNameInCommands(str, position);
					bool is_file = isThisFileName(name);
					//poziv metode
					if (is_file) {
						DeleteFile command;
						command.execute(this, name);
					}
					else {
						DeleteFolder command;
						command.execute(this, name);
					}
					moveToNextLine(str, position);
					break;
				}
				default: throw new NoneValidCallOfCommand();
			}
		}
		catch (FileSystemIsEmpty* e) {//izdvajamo posebno jer ovde prekidamo program
			handlingAnExceptionInExecute(e, str, position, command_line);
			return;//ukoliko je prazan prekidamo execute
		}
		catch (exception* e) {//nismo pisali catch(...) jer nam treba da prihvatimo ovaj izuzetak kako kasnije ne bismo imali curenje memorije, jer sledece pozvana memorija brise e, ali je isti efekat jer se i ovde isto primaju svi ostali izuzeci
			handlingAnExceptionInExecute(e, str, position, command_line);
		}
	}
}

//metoda za citanje imena u komandama, jer su drugacijeg formata(ne pocinju sa \) i drugacije ih cuvamo (cuvamo ih sa sve ekstenzijom)
string FileSystem::readNameInCommands(const string& str, int& position) {
	string name = "";
	//u imenu foldera se mogu naci samo slova, brojevi i _, citamo dok se ne nadje neki drugi karakter ili dok ne dodjemo do kraja stringa
	while (((str[position] >= 'a' && str[position] <= 'z') || (str[position] >= 'A' && str[position] <= 'Z') || (str[position] >= '0' && str[position] <= '9') || str[position] == '_') && position < str.length()) {
		name += str[position++];
	}
	if (str[position] != ' ' && str[position] != '\n' && str[position] != '.') throw new NoneValidDirectoryName();//bacamo izuzetak u slucaju nevalidnog imena foldera/fajla ili znaka posle njega
	if (str[position] == '.') {
		position++;
		string extension = "";
		for (int i = 0; i < 3; i++) extension += str[position++];
		if (extension != "exe" && extension!="txt") throw new NoneExistingFileExtension();//ukoliko fajl nema postojecu ekstenziju bacamo izuzetak
		name += '.';
		name+=extension;//dodajemo ekstenziju u ime
	}
	return name;
}

//metoda koja proverava da li je prosledjeno ime ime fajla
bool FileSystem::isThisFileName(const string& name) {
	int i = 0;
	while (name[i] != '.' && i<name.length()) i++;//trazimo znak '.' u imenu-znamo da je onda fajl
	if (i < name.length()) return true;//ako nismo dosli do kraja stringa nasli smo je 
	else return false;//ako smo dosli do kraka stringa nismo je nasli
}

//metoda koja cita jednu komandnu liniju, ne prima brojac po referenci, jer ne zelimo da se promene van ove metode vide
string FileSystem::readCommandLine(const string& str, int position) {
	
	string line = "";
	while (position < str.length() && str[position] != '\n')
		line += str[position++];
	return line;
}




