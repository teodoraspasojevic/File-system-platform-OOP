//klasa SystemElement je apstraktna klasa koja predstavlja elemente sistema (fajlove i foldere)

//-ona sadrzi clanove podatke koji su zajednicki i fajlovima i folderima
//-ona sadzi funkcije clanice koje sadrze obe izvedene klase, virtualne ce biti one cija definicija vec u ovoj klasi nam moze biti potrebna, a ostale ce biti apstraktne
//-klasa je apstraktna ako ima makar jednu apstraktnu metodu, sto je ovde slucaj
//-cilj nam je bio da napravimo uopstenu klasu iz koje cemo moci da izvedemo klase fajl i folder, pa time pod pokazivacem na ovu klasu moci da cuvamo
//i objekte klasa File i Folder, jer C++ ima implicitnu konverziju pokazivaca iz pokazivaca na objekte izvedene klase u pokazivace na objekte osnovne klase
//-takodje, samim tim sto je ova klasa apstraktna nije moguce praviti njene objekte, sto ni ne zelimo jer u fajl sistemu mozemo imati samo fajlove i foldere

#ifndef SYSTEM_ELEMENT_H
#define SYSTEM_ELEMENT_H

#include "Exceptions.h"
#include <string>
using namespace std;

//enum koji se koristi za lakse dodavanje novih ekstenzija fajlova, samim tim i novih fajlova
enum Extension { txt, exe };


class SystemElement {
public:
	SystemElement() = default; //zahtevamo da imamo i podrazumevani konstruktor, jer inace automatski ne postoji deklarisanjem novog konstruktora/destruktora
	SystemElement(const string& name);
	virtual ~SystemElement();//virtualni destruktor

	//geteri
	string getName() const;
	virtual Extension getExtension() const = 0;
	virtual string getContent() const = 0;

	//seteri
	void setName(const string& name);
	virtual void setExtension(Extension exstension) = 0;
	virtual void setContent(const string& content) = 0;


	virtual bool isFile() const = 0;//metoda koja nam govori da li je objekat folder ili file

protected:
	//ovi clanovi podaci su u protected sekciji kako bi izvedene klase mogle da im pristupe
	string name_;
};


//klasu Folder javno izvodimo iz klase SystemElement

class Folder : public SystemElement {
public:
	Folder() = default;
	Folder(const string& name);

	//geteri
	Extension getExtension() const override;
	string getContent() const override;

	//seteri
	void setExtension(Extension exstension) override;
	void setContent(const string& content) override;

	bool isFile() const override;

};


class File : public SystemElement {
public:
	File() = default;
	File(const string& name, Extension extension, const string& content);

	//geter
	Extension getExtension() const override;
	virtual string getContent() const override;

	//seteri
	void setExtension(Extension exstension) override;
	void setContent(const string& content) override;

	bool isFile() const override;

private:
	Extension extension_;
	string content_;
};


#endif
