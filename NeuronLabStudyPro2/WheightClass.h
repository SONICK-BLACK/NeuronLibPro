#pragma once
class WheightClass
{
public:
	double* Wheight;
	double* SetErrorWhPacket = 0;

	int sizeMatrix = 0;
	void InitWheightClass(int SizeWheight, int SizeNeuron);
	void WheightErrSetNull();
	void InitWheightErr();
	~WheightClass();
};

