#include "TasksNetwork.h"
void TasksNetwork::DirectDirection(NeuronClass& Neuron1, NeuronClass& Neuron2, WheightClass& Wheight, BiosClass& Bios, ActFuns& funs) {
	int k = 0;
	for (int i = 0; i < Neuron1.sizeMatrix; i++) {
		for (int u = 0; u < Neuron2.sizeMatrix; u++) {
			Neuron2.Neuron[u] +=  Neuron1.Neuron[i] * Wheight.Wheight[k];
			Neuron2.NoActivateNeuron[u] += Neuron1.Neuron[i] * Wheight.Wheight[k];
			k += 1;
		}
	}
	for (int u = 0; u < Neuron2.sizeMatrix; u++) {
		Neuron2.Neuron[u] += Bios.Bios[u];
		Neuron2.NoActivateNeuron[u] += Bios.Bios[u];
	}
	if (funs != Softmax) {
		for (int u = 0; u < Neuron2.sizeMatrix; u++) {

			Function::FunctionUse(funs, Neuron2.Neuron[u]);
		}
	}
	else {
		double valMid = 0;
		for (int i = 0; i < Neuron2.sizeMatrix; i++) {
			valMid += exp(Neuron2.Neuron[i]);

		}
		for (int u = 0; u < Neuron2.sizeMatrix; u++) {

			Function::FunctionUse(funs, Neuron2.Neuron[u],valMid);
		}
	}

	
}
void  TasksNetwork::EvalutionError(double* VectorRight, NeuronClass& NeuronEnd, ErrFuns errFuns) {
	for (int i = 0; i < NeuronEnd.sizeMatrix; i++) {
		NeuronEnd.NeuronErr[i]= Function::FunctionUseErDer(errFuns, NeuronEnd.Neuron[i], VectorRight[i]);
	}
}
void TasksNetwork::EvalutionErrorLast(NeuronClass& Neuron2, NeuronClass& Neuron, WheightClass& Wheight, ActFuns& funs) {
	int k = 0;
	double Val = 0;
	if (funs == Softmax) {
		for (int j = 0; j < Neuron2.sizeMatrix; j++) {

			Val += exp(Neuron2.NoActivateNeuron[j]);
		}
		
	}
	for (int i = 0; i < Neuron.sizeMatrix; i++) {
		for (int j = 0; j < Neuron2.sizeMatrix; j++) {
			if (funs == Softmax) {
				Neuron.NeuronErr[i] += Neuron2.NeuronErr[j] * Wheight.Wheight[k] * Function::FunctionUseDer(funs, Neuron2.NoActivateNeuron[j],Val);
			}
			else {
				Neuron.NeuronErr[i] += Neuron2.NeuronErr[j] * Wheight.Wheight[k] * Function::FunctionUseDer(funs, Neuron2.NoActivateNeuron[j]);
			}
		
			k += 1;
		}
	}

}
void TasksNetwork::ErrorTeachSloySet(NeuronClass& Neuron2, NeuronClass& Neuron, WheightClass& Wheight, BiosClass& Bios, ActFuns& funs) {
	int k = 0;
	double Val = 0;
	if (funs == Softmax) {
		for (int j = 0; j < Neuron.sizeMatrix; j++) {

			Val += exp(Neuron.NoActivateNeuron[j]);
		}

	}
	for (int i = 0; i < Neuron2.sizeMatrix; i++) {
		for (int j = 0; j < Neuron.sizeMatrix; j++) {
			if (funs == Softmax) {
				Wheight.SetErrorWhPacket[k] += Neuron2.Neuron[i] * Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j],Val);
			}
			else {
				Wheight.SetErrorWhPacket[k] += Neuron2.Neuron[i] * Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j]);
			}
			k += 1;
		}
	}
	for (int j = 0; j < Neuron.sizeMatrix; j++) {
		if (funs == Softmax) {
			Bios.SetErrorBiosPacket[j] += Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j], Val);
		}
		else {
			Bios.SetErrorBiosPacket[j] += Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j]);
		}
	}
}