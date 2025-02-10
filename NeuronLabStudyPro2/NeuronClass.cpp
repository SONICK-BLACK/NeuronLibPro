#include "NeuronClass.h"
void NeuronClass::InitNeuronClass(int NeuronSize) {
	sizeMatrix = NeuronSize;
	Neuron = new double[sizeMatrix];
	for (int i = 0; i < sizeMatrix; i++) {
		Neuron[i] = 0;
	}

}
void NeuronClass::InitNeuronClassErr() {
	NeuronErr = new double[sizeMatrix];
	for (int i = 0; i < sizeMatrix; i++) {
		NeuronErr[i] = 0;
	}
	NoActivateNeuron = new double[sizeMatrix];
	for (int i = 0; i < sizeMatrix; i++) {
		NoActivateNeuron[i] = 0;
	}

}
void NeuronClass::NeuronErrSetNull() {
	for (int i = 0; i < sizeMatrix; i++) {
		NeuronErr[i] = 0;
	}


}
void NeuronClass:: NeuronSetNull() {
	for (int i = 0; i < sizeMatrix; i++) {
		Neuron[i] = 0;
	}
	if (NeuronErr != 0) {
		for (int i = 0; i < sizeMatrix; i++) {
			NoActivateNeuron[i] = 0;
		}
	}
}
//
void NeuronClass::NeuronSetNulL() {
	for (int i = 0; i < sizeMatrix; i++) {
		Neuron[i] = 0;
	}
	
}

//

NeuronClass::~NeuronClass() {
	delete[] Neuron;
	if (NeuronErr != 0) {
		delete[] NeuronErr;
		delete[] NoActivateNeuron;
	}

}