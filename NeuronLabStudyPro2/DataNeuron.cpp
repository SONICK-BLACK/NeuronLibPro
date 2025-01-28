#include "DataNeuron.h"
using namespace Data;
DataNeuron::DataNeuron(int SizeDat, int SizeData2, int SizeDataVal) {
	DataNeuron::SizeData = SizeDat;
	DataNeuron::SetData = new double* [SizeDat];
	DataNeuron::CorrectVal = new double* [SizeDat];
	for (int i = 0; i < SizeDat; i++) {
		DataNeuron::SetData[i] = new double[SizeData2];

	}
	for (int i = 0; i < SizeDat; i++) {
		DataNeuron::CorrectVal[i] = new double[SizeDataVal];

	}
}

DataNeuron::~DataNeuron() {


		for (int i = 0; i < DataNeuron::SizeData; i++) {
			delete[] DataNeuron::SetData[i];

			delete[] DataNeuron::CorrectVal[i];

		}
		delete[] DataNeuron::SetData;
		delete[] DataNeuron::CorrectVal;
	

}
DataRegression::DataRegression(int ValParametr, int SizeExperiens) {
	this->SizeExperiens = SizeExperiens;
	SizeParametrs = ValParametr;
	X = new double* [SizeExperiens];
	for (int i = 0; i < SizeExperiens; i++)
		X[i] = new double[ValParametr];
	Y = new double*[SizeExperiens];
	for (int i = 0; i < 1; i++) {
		Y[i] = new double[1];
	}

	for (int i = 0; i < SizeExperiens; i++)
		X[i][0] = 1;
	
}
DataRegression::~DataRegression() {
	for (int i = 0; i < SizeExperiens; i++)
		delete[] X[i];
	delete[] X;
	for (int i = 0; i < 1; i++) {
		delete[] Y[i];
	}
	delete[] Y;
	if (b != 0) {
		for (int i = 0; i < 1; i++) {
			delete[] b[i];
		}
		delete[] b;
	}
	
}
