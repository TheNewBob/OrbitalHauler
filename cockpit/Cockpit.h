#pragma once

class Cockpit
{
public:
	Cockpit() {};
	~Cockpit() {};

	void init(VESSEL4 *vessel);
	void defineAreas();

	MESHHANDLE mesh;

};

