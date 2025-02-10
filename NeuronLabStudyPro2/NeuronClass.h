#pragma once
class NeuronClass
{
public:
	double* Neuron;
	double* NoActivateNeuron = 0;
	double* NeuronErr = 0;
	int sizeMatrix;
	void InitNeuronClass(int NeuronSize);
	void NeuronErrSetNull();
	void NeuronSetNulL();
	void NeuronSetNull();
	void InitNeuronClassErr();
	~NeuronClass();

};

