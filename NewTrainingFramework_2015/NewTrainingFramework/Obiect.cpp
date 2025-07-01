#include "stdafx.h"
#include "Obiect.h"

Obiect::Obiect() {};
Obiect::Obiect(int idObiect, std::string numeObiect, std::string tipObiect, int valoareObiect, std::string numeFolder, int valoareTextura)
	:idObiect{ idObiect }, numeObiect{ numeObiect }, tipObiect{ tipObiect }, valoareObiect{ valoareObiect }, numeFolder{ numeFolder }, valoareTextura{ valoareTextura } {};
Obiect::~Obiect() {};