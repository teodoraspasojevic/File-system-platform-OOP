//-klasa koja predstavlja apstrakciju svih komandi koje se mogu vrsiti nad fajl sistemom
//-klasa Commands je prijateljska klasa klase FileSystem pa samim tim moze pristupati i privatnim i zasticenim poljima ove klase
//-ona je sablon iz koga izvodimo sve konkretne komande koje su nam potrebne
//-kako se prijateljstvo ne nasledjuje, potrebno je da ih sve definisemo kao prijateljske klase, klase FileSystem

#ifndef COMMANDS_H
#define COMMANDS_H

#include "FileSystem.h"
#include <fstream>
#include <queue>
#include <stack>
#include <string>
#include <vector>
using namespace std;

class FileSystem;

class Commands {
public:
	virtual void execute(FileSystem* file_system, const string& name, const string& content) = 0;
};

//LS
class WriteSystem : public Commands{
public:
	void execute(FileSystem* file_system, const string& name="", const string& content="") override;
};


class SetCurrentDirectory : public Commands {
public:
	virtual void execute(FileSystem* file_system, const string& name, const string& content = "") = 0;
};
//CD name
class SetCurrentDirectoryByName : public SetCurrentDirectory {
public:
	void execute(FileSystem* file_system, const string& name, const string& content = "") override;
};
//CD ..
class SetCurrentDirectoryToParent : public SetCurrentDirectory {
public:
	void execute(FileSystem* file_system, const string& name="", const string& content = "") override;
};


class NewDirectory : public Commands {
public:
	virtual void execute(FileSystem* file_system, const string& name, const string& content) = 0;
};
// NEW name
class NewFolder : public  NewDirectory{
public:
	void execute(FileSystem* file_system, const string& name, const string& content = "") override;
};
// NEW name.ext content
class NewFile : public  NewDirectory {
public:
	void execute(FileSystem* file_system, const string& name, const string& content) override;
};
	

class DeleteDirectory : public Commands {
public:
	virtual void execute(FileSystem* file_system, const string& name, const string& content) = 0;
};
//DEL name
class DeleteFolder : public DeleteDirectory{
public:
	void execute(FileSystem* file_system, const string& name, const string& content = "") override;
};
//DEL name.ext
class DeleteFile : public DeleteDirectory {
public:
	void execute(FileSystem* file_system, const string& name, const string& content="") override;
};

//EXE name.exe
class Execute : public Commands{
public:
	void execute(FileSystem* file_system, const string& name, const string& content = "") override;
private:
	string fillTextWithNewLines(const string& str);
};

#endif
