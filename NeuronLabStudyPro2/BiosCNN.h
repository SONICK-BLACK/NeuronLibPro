#pragma once
class BiosCNN
{
public:
	int sizeY;
	int sizeX;
	double** bios;
	double** biosErrPacket=0;
	void operator () (int y, int x, bool setErr);
	void SetNullErr();
	~BiosCNN();
};

