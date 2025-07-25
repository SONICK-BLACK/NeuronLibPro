#pragma once
class Batch
{
public:
	int sizeX=0;
	int sizeY=0;
	int*** PositioningMax = 0;
	double** batch;
	double** batchNoAct;
	double** batchErr=0;


	void operator ()  (int sizeY, int sizeX, int SetMax, int setErr);
	void SetNull();
	void SetNullErr();
	~Batch();

};

