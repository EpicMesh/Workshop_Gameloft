#pragma once
#include<string>

class Obiect
{
public:
	Obiect();
	int idObiect;
	std::string numeObiect;
	std::string tipObiect;
	int valoareObiect;
	std::string numeFolder;
	int valoareTextura;
	Obiect(int idObiect, std::string numeObiect, std::string tipObiect, int valoareObiect, std::string numeFolder, int valoareTextura);
	~Obiect();
};

