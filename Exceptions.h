#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
using namespace std;

class FolderDoesNotHaveExtension : public exception {
public:
	FolderDoesNotHaveExtension(): exception("Error: Folder does not have extension."){}
};

class FolderDoesNotHaveContent : public exception {
public:
	FolderDoesNotHaveContent() : exception("Error: Folder does not have content.") {}
};

class FolderNameDoesNotBeginWithBackslash : public exception {
public:
	FolderNameDoesNotBeginWithBackslash() : exception("Error: Folder name does not begin with '\\'.") {}
};

class NoneValidDirectoryName : public exception {
public:
	NoneValidDirectoryName() : exception("Error: Directory name is not valid.") {}
};

class NoneExistingFileExtension : public exception {
public:
	NoneExistingFileExtension() : exception("Error: Non existing file extension."){}
};

class NoBackspaceAfterExtension : public exception {
public:
	NoBackspaceAfterExtension() : exception("Error: No backspace after extension."){}
};

class ParentFolderDoesNotExist : public exception {
public:
	ParentFolderDoesNotExist() : exception("Error: We can not add new folder/file in non existing folder.") {}
};

class FileCanNotHaveSon : public exception {
public:
	FileCanNotHaveSon() : exception("Error: Another file/folder can not be added in a file.") {}
};

class FolderWithThatNameAlreadyExists : public exception {
public:
	FolderWithThatNameAlreadyExists() : exception("Error: Folder with that name already exists in current folder.") {}
};

class FileWithThatNameAlreadyExists : public exception {
public:
	FileWithThatNameAlreadyExists() : exception("Error: File with that name already exists in current folder.") {}
};

class NoneValidCallOfCommand : public exception {
public:
	NoneValidCallOfCommand() : exception("Error: None valid call of command.") {}
};

class CanNotRunNotExeFile : public exception {
public:
	CanNotRunNotExeFile() : exception("Error: You can not execute file which is not exe file.") {}
};

class DirectoryWithThatNameDoesNotExistiInCurrentDirectory : public exception {
public:
	DirectoryWithThatNameDoesNotExistiInCurrentDirectory() : exception("Error: Directory with that name does not exist in current directory.") {}
};

class NoFoldersInCurrentDirectory : public exception {
public:
	NoFoldersInCurrentDirectory() : exception("Error: There are no folders in current directory.") {}
};

class CanNotSetFileForCurrentDirectory : public exception {
public:
	CanNotSetFileForCurrentDirectory() : exception("Error: Can not set file for current directory.") {}
};

class FileSystemIsEmpty : public exception {
public:
	FileSystemIsEmpty() : exception("Error: File system is empty.") {}
};

class NoParentFolder : public exception {
public:
	NoParentFolder() : exception("Error: Parent folder does not exist.") {}
};

#endif
