#pragma once
namespace Data {
	class DataNeuron
	{
	public:
		DataNeuron(int SizeDat, int SizeData2, int SizeDataVal);
		~DataNeuron();
		double** SetData;
		double** CorrectVal;
		int SizeData;
	};

	class DataRegression {
	public:
		double** X;
		double* Y;
		double* b;
		int SizeExperiens;
		DataRegression(int ValParametr, int SizeExperiens);
		~DataRegression();
	};

}

