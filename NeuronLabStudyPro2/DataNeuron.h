#pragma once
class DataNeuron
{
	public:
		DataNeuron(int SizeDat, int SizeData2, int SizeDataVal);
		~DataNeuron();
		double** SetData;
		double** CorrectVal;
		int SizeData;
};

