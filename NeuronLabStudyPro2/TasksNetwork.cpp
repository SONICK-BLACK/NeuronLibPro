#include "TasksNetwork.h"
#include "DataHyperParametr.h"
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
void TasksNetwork::ErrorTeachSloySet(NeuronClass& Neuron2, NeuronClass& Neuron, WheightClass& Wheight, BiosClass& Bios, ActFuns& funs, OptimizaterGradient Optimizator) {
	int k = 0;
	double Val = 0;
	HypPar::DataHyperParametr HYpPar;
	
		if (Optimizator == Momentum) {
			if (funs == Softmax) {
				for (int j = 0; j < Neuron.sizeMatrix; j++) {

					Val += exp(Neuron.NoActivateNeuron[j]);
				}

			}
			double V = 0;
			for (int i = 0; i < Neuron2.sizeMatrix; i++) {
				for (int j = 0; j < Neuron.sizeMatrix; j++) {
					if (funs == Softmax) {
						V = V * HYpPar.SetM + (1 - HYpPar.SetM) * Neuron2.Neuron[i] * Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j], Val);
						Wheight.SetErrorWhPacket[k] += V;
					}
					else {
						V = V * HYpPar.SetM + (1 - HYpPar.SetM) * Neuron2.Neuron[i] * Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j]);
						Wheight.SetErrorWhPacket[k] += V;
					}
					k += 1;
				}
			}
			V = 0;
			for (int j = 0; j < Neuron.sizeMatrix; j++) {
				if (funs == Softmax) {

					V = V * HYpPar.SetM + (1 - HYpPar.SetM) * Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j], Val);
					Bios.SetErrorBiosPacket[j] += V;
				}
				else {

					V = V * HYpPar.SetM + (1 - HYpPar.SetM) * Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j]);
					Bios.SetErrorBiosPacket[j] += V;
				}
			}
		}
		if (Optimizator == rmsprop) {
			if (funs == Softmax) {
				for (int j = 0; j < Neuron.sizeMatrix; j++) {

					Val += exp(Neuron.NoActivateNeuron[j]);
				}

			}
			double V = 0;
			for (int i = 0; i < Neuron2.sizeMatrix; i++) {
				for (int j = 0; j < Neuron.sizeMatrix; j++) {
					if (funs == Softmax) {
						V = V * HYpPar.SetMR + (1 - HYpPar.SetMR) * pow(Neuron2.Neuron[i] * Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j], Val), 2);
						Wheight.SetErrorWhPacket[k] += Neuron2.Neuron[i] * Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j], Val) / (sqrt(V) + HYpPar.dEconst);
					}
					else {
						V = V * HYpPar.SetMR + (1 - HYpPar.SetMR) * pow(Neuron2.Neuron[i] * Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j]), 2);
						Wheight.SetErrorWhPacket[k] += Neuron2.Neuron[i] * Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j], Val) / (sqrt(V) + HYpPar.dEconst);
					}
					k += 1;
				}
			}
			V = 0;
			for (int j = 0; j < Neuron.sizeMatrix; j++) {
				if (funs == Softmax) {

					V = V * HYpPar.SetMR + (1 - HYpPar.SetMR) * pow(Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j], Val), 2);
					Bios.SetErrorBiosPacket[j] += Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j], Val) / (sqrt(V) + HYpPar.dEconst);
				}
				else {

					V = V * HYpPar.SetMR + (1 - HYpPar.SetMR) * pow(Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j]), 2);
					Bios.SetErrorBiosPacket[j] += Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j]) / (sqrt(V) + HYpPar.dEconst);
				}
			}
		}
		if (Optimizator == Adam) {
			if (funs == Softmax) {
				for (int j = 0; j < Neuron.sizeMatrix; j++) {

					Val += exp(Neuron.NoActivateNeuron[j]);
				}

			}
			double V1 = 0;
			double V2 = 0;
			double Vscer1 = 0;
			double Vscer2 = 0;
			for (int i = 0; i < Neuron2.sizeMatrix; i++) {
				for (int j = 0; j < Neuron.sizeMatrix; j++) {
					if (funs == Softmax) {
						V1 = V1 * HYpPar.SetMR + (1 - HYpPar.SetMR) * pow(Neuron2.Neuron[i] * Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j], Val), 2);
						V2 = V2 * HYpPar.SetM + (1 - HYpPar.SetM) * Neuron2.Neuron[i] * Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j], Val);
						Vscer1 = V1 / (1 - pow(HYpPar.SetMR, k+1));
						Vscer2 = V2 / (1 - pow(HYpPar.SetM, k+1));
						Wheight.SetErrorWhPacket[k] += Vscer2 / (sqrt(Vscer1) + HYpPar.dEconst);
					}
					else {
						V1 = V1 * HYpPar.SetMR + (1 - HYpPar.SetMR) * pow(Neuron2.Neuron[i] * Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j]), 2);
						V2 = V2 * HYpPar.SetM + (1 - HYpPar.SetM) * Neuron2.Neuron[i] * Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j]);
						Vscer1 = V1 / (1 - pow(HYpPar.SetMR, k+1));
						Vscer2 = V2 / (1 - pow(HYpPar.SetM, k+1));
						Wheight.SetErrorWhPacket[k] += Vscer2 / (sqrt(Vscer1) + HYpPar.dEconst);
					}
					k += 1;
				}
			}
			V1 = 0;
			V2 = 0;
			Vscer1 = 0;
			Vscer2 = 0;
			for (int j = 0; j < Neuron.sizeMatrix; j++) {
				if (funs == Softmax) {
					V1 = V1 * HYpPar.SetMR + (1 - HYpPar.SetMR) * pow(Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j], Val), 2);
					V2 = V2 * HYpPar.SetM + (1 - HYpPar.SetM) * Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j], Val);
					Vscer1 = V1 / (1 - pow(HYpPar.SetMR, j+1));
					Vscer2 = V2 / (1 - pow(HYpPar.SetM, j+1));
					Bios.SetErrorBiosPacket[j] += Vscer2 / (sqrt(Vscer1) + HYpPar.dEconst);


				}
				else {

					V1 = V1 * HYpPar.SetMR + (1 - HYpPar.SetMR) * pow(Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j]), 2);
					V2 = V2 * HYpPar.SetM + (1 - HYpPar.SetM) * Neuron.NeuronErr[j] * Function::FunctionUseDer(funs, Neuron.NoActivateNeuron[j]);
					Vscer1 = V1 / (1 - pow(HYpPar.SetMR, j+1));
					Vscer2 = V2 / (1 - pow(HYpPar.SetM, j+1));
					Bios.SetErrorBiosPacket[j] += Vscer2 / (sqrt(Vscer1) + HYpPar.dEconst);

				}
			}
		}



	




}