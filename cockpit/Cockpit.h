#pragma once

class Cockpit
{
public:
	Cockpit() {};
	~Cockpit() {};

	void init(OrbitalHauler *vessel);
	void defineAreas();

	MESHHANDLE mesh;

};

