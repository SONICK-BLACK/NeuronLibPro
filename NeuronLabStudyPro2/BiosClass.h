#pragma once
class BiosClass
{
public:
	double* Bios;
	double* SetErrorBiosPacket = 0;
	int SizeMatrix;
	void InitBiosClass(int SizeBios, int SizeNeuron);
	void BiosErrSetNull();
	void InitErrBios();
	~BiosClass();
};

