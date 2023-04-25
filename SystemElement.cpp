#include "SystemElement.h"

//KLASA SYSTEM_ELEMENT

//konstruktor koji inicijalizuje objekat preko inicijalizatorske liste
SystemElement::SystemElement(const string& name) : name_(name) {}

//destruktor ce sam pravilno unistiti sva polja, nijedan podatak clan nije pokazivac na nesto, pa nece biti curenja memorije
SystemElement::~SystemElement() {}

//geter
string SystemElement::getName() const {
	return name_;
}

//seter
void SystemElement::setName(const string& name) {
	name_ = name;
}

//KLASA FOLDER

Folder::Folder(const string& name) : SystemElement(name) {}

//geteri

Extension Folder::getExtension() const {
	throw new FolderDoesNotHaveExtension();//ukoliko neko proba da pozove ovu metodu nad objektom klase Folder bacamo izuzetak jer folder nema ekstenziju
}

string Folder::getContent() const {
	throw new FolderDoesNotHaveContent();//ukoliko neko proba da pozove ovu metodu nad objektom klase Folder bacamo izuzetak jer folder nema sadrzaj
}

//seteri

void Folder::setExtension(Extension exstension) {
	throw new FolderDoesNotHaveExtension();//ukoliko neko proba da pozove ovu metodu nad objektom klase Folder bacamo izuzetak jer folder nema ekstenziju
}

void Folder::setContent(const string& content) {
	throw new FolderDoesNotHaveContent();//ukoliko neko proba da pozove ovu metodu nad objektom klase Folder bacamo izuzetak jer folder nema sadrzaj
}


bool Folder::isFile() const {
	return false;
}


//KLASA FILE

File::File(const string& name, Extension extension, const string& content): SystemElement(name), extension_(extension), content_(content) {}

//geteri

Extension File::getExtension() const {
	return extension_;
}

string File::getContent() const {
	return content_;
}

//seteri

void File::setExtension(Extension exstension) {
	extension_ = exstension;
}

void File::setContent(const string& content) {
	content_ = content;
}


bool File::isFile() const {
	return true;
}




