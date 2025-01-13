#include "DataNeuron.h"
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