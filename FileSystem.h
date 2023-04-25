//-klasa FileSystem predstavlja sistem svih fajlova i foldera koje ucitavamo
//-pomocne metode su u private sekciji kako bismo sakrili implemetaciju krajnjeg proizvoda, u public sekciji zadrzavamo samo ono sto je deo interfejsa
//-zbog toga smo klasu Commands definisali kao prijateljsku klasu ove klase, da bi mogla da pristupi ovim metodama, a one ostale sakrivene u implementaciji

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "Commands.h"
#include "Exceptions.h"
#include "Tree.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

class FileSystem {
public:
	FileSystem() : output_file_("") { tree_ = new Tree(); };
	FileSystem(const FileSystem&) = delete;
	FileSystem(FileSystem&&) = delete;
	~FileSystem();

	//klasa Commands je prijateljska klasa klase FileSystem pa samim tim moze pristupati i privatnim i zasticenim poljima ove klase
	//kako se prijateljstvo ne nasledjuje, moramo ih sve definisati kao prijateljske klase klase FileSystem
	friend class Commands;
	friend class WriteSystem;
	friend class SetCurrentDirectory;
	friend class SetCurrentDirectoryByName;
	friend class SetCurrentDirectoryToParent;
	friend class NewDirectory;
	friend class NewFolder;
	friend class NewFile;
	friend class DeleteDirectory;
	friend class DeleteFolder;
	friend class DeleteFile;
	friend class Execute;

	void loadHierarchy(const string& fs_filepath, const string& log_filepath);
	void execute(const  string&  filepath);

private:
	Tree* tree_;
	string output_file_;//putanja ka izlaznom fajlu u kojem izvrsavamo ispis hijerarhije

	//pomocne metode
			//u sledecim metodama se parametar position prosledjuje po referenci kako bi se promene nad njim videle i van ovih metoda
	Tree* getTree();
	string readName(const string& str, int& position);
	Extension readExtension(const string& str, int& position);
	string readContent(const string& str, int& position);
	void readOneElement(const string& str, int& position);
	void moveToNextLine(const string& str, int& position);
	void handlingAnException(exception* e, const string& str, int& position);
	string getPath(TreeNode* node);
	void handlingAnExceptionInExecute(exception* e, const string& str, int& position, const string& command_line);
	void executeWithString(const string& str);
	string readNameInCommands(const string& str, int& position);
	bool isThisFileName(const string& name);
	string readCommandLine(const string& str, int position);

	static int function_read_one_element_calls;//staticko polje koje sluzi kao brojac poziva metode readOneElement
	static int counter;//brojac elemenata fajl sistema
	static int calls_of_command_ls;//staticko polje koje sluzi kao brojac poziva komande LS
	static int calls_of_handle_function;//staticko polje koje sluzi kao brojac poziva metode handlingAnExceptionInExecute
};

#endif