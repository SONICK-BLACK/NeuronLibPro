#pragma once
class Cores
{
public:
	int sizeX=0;
	int sizeY=0;
	double** Core=0;
	double biosCore=0;
	double biosCoreErr = 0;
	double** CoreErr;
	 void operator ()  (int sizeY, int sizeX);
	 void NullErrCore();
	 ~Cores();




};

