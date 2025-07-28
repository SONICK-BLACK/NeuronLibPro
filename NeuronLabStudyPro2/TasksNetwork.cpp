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
	//	std::cout << std::endl;
	//	std::cout << Neuron2.NoActivateNeuron[u];
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
void TasksNetwork::DirectDirectionBase(NeuronClass& Neuron1, NeuronClass& Neuron2, WheightClass& Wheight, BiosClass& Bios, ActFuns& funs) {
	int k = 0;
	for (int i = 0; i < Neuron1.sizeMatrix; i++) {
		for (int u = 0; u < Neuron2.sizeMatrix; u++) {
			Neuron2.Neuron[u] += Neuron1.Neuron[i] * Wheight.Wheight[k];
			
			k += 1;
		}
	}
	for (int u = 0; u < Neuron2.sizeMatrix; u++) {
		Neuron2.Neuron[u] += Bios.Bios[u];
	
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

			Function::FunctionUse(funs, Neuron2.Neuron[u], valMid);
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



void TasksNetwork::DirectBatches(Batch& batch1, Cores& core, Batch& batch2, int step) {
	
	
Sweep(batch1.batch, core.Core, step, batch1.sizeY, batch1.sizeX, core.sizeY, core.sizeX, batch2.batch);
}
void TasksNetwork::MaxPooling(Batch& batch, int val, Batch& batchMax) {

	Batch* PadMat=0;
	int X= batch.sizeX / val;
	int Y= batch.sizeY / val;
	if (batch.sizeX % val && batch.sizeY % val) {
	
		int VALX = abs(((batch.sizeX / val + 1) * val) - batch.sizeX);
		int VALY = abs(((batch.sizeY / val + 1) * val) - batch.sizeY);
		int chetX = 0;
		int px = 0;
		int py = 0;
		int chetY = 0;
		if (VALX % 2) {
			chetX = (VALX - 1) / 2;
			px = 1;
			if (chetX == 0) {
				chetX = 1;
				px = 0;
			}
			
		}
		else {
			chetX = VALX / 2;
		}
		if (VALY % 2) {
			py = 1;
			chetY = (VALY - 1) / 2;
			if (chetY == 0) {
				chetY = 1;
				py = 0;
			}
		}
		else {
			chetY = VALY / 2;

		}
		
		
		X = batch.sizeX / val + 1;
		Y = batch.sizeY / val + 1;
		
		//MaxPooling
		int y;
		int x;
	
		double Max;
		for (int i = 0; i < Y; i++) {
			for (int j = 0; j < X; j++) {
				
			
			
				if (j && i) {
			
					Max = batch.batch[i * val - chetY][j * val - chetX];
					y = i * val  - chetY;
					x = j * val - chetX;
					
				}
				else if (i) {
				
					Max = batch.batch[i * val - chetY][j * val];
					y = i * val - chetY;
					x = j * val;
			
					
					
				}
				else if (j) {
				
					Max = batch.batch[i * val][j * val - chetX];
					x = j * val - chetX;
					y = i * val;
				
					
				}
				else {
					Max = batch.batch[i * val][j * val];
					y = i * val;
					x = j * val;
					
				}
				 
				for (int g = 0; g < val; g++) {
					
					if (i * val + g - chetY - py >= batch.sizeY) {
						break;
					}
					
					
					for (int h = 0; h < val; h++) {
						
						if (j * val + h  - chetX - px >= batch.sizeX) {
							break;
					}
				
				
				

						
						if ((i * val + g) >= chetY + py && (j * val + h) >= chetX + px) {
							if (Max < batch.batch[i * val + g - (chetY + py)][j * val + h - (chetX + px)]) {
								Max = batch.batch[i * val + g - (chetY + py)][j * val + h - (chetX + px)];
							
								y = i * val + g - (chetY + py);
								x = j * val + h - (chetX + px);
							}
								
							}
						


					}
				
				}
				batchMax.batch[i][j] = Max;
				
				batchMax.PositioningMax[i][j][0] = y;
				batchMax.PositioningMax[i][j][1] = x;
			}
		}
		
	}
	else if (batch.sizeX % val) {
	
		int VALX = abs(((batch.sizeX / val + 1) * val) - batch.sizeX);
		int chetX = 0;
		int px = 0;
	
		if (VALX % 2) {
			chetX = (VALX - 1) / 2;
			px = 1;
			if (chetX == 0) {
				chetX = 1;
				px = 0;
			}
		}
		else {
			chetX = VALX / 2;
		}
		

		X = batch.sizeX / val + 1;
		Y = batch.sizeY / val;

		//MaxPooling
	
		int y;
		int x;
		double Max;
		for (int i = 0; i < Y; i++) {
			for (int j = 0; j < X; j++) {
				 
			

				
				
				 if (j) {
					Max = batch.batch[i * val][j * val - chetX];
					x = j * val - chetX;
				}
				else {
					Max = batch.batch[i * val][j * val];
					y = i * val;
					x = j * val;
				}


				for (int g = 0; g < val; g++) {
					for (int h = 0; h < val; h++) {
					
						if (j * val + h - chetX-px >= batch.sizeX) {
							break;
						}
						

					
						if ((j * val + h) >= chetX + px) {
							if (Max < batch.batch[i * val + g][j * val + h - (chetX+ px)]) {
								Max = batch.batch[i * val + g][j * val + h - (chetX + px)];
								y = i * val + g;
								x = j * val + h - (chetX + px);
							}

						}


					}
				
					
					
				}
				batchMax.batch[i][j] = Max;
				
				batchMax.PositioningMax[i][j][0] = y;
				batchMax.PositioningMax[i][j][1] = x;
			}
		}
	
	}
	else if (batch.sizeY % val) {

	int VALY = abs(((batch.sizeY / val + 1) * val) - batch.sizeY);
	int chetY = 0;
	int py = 0;
	if (VALY % 2) {
		chetY = (VALY - 1) / 2;
		py = 1;
		if (chetY == 0) {
			chetY = 1;
			py = 0;
		}
	}
	else {
		chetY = VALY / 2;
	}
		X = batch.sizeX / val;
		Y = batch.sizeY / val + 1;
		//MaxPooling
	
		int y;
		int x;
		double Max;
		for (int i = 0; i < Y; i++) {
			for (int j = 0; j < X; j++) {
				
				
				
			
				 if (i) {
					Max = batch.batch[i * val - chetY][j * val];
					y = i * val - chetY;

				}
				else {
					Max = batch.batch[i * val][j * val];
					y = i * val;
					x = j * val;
				}
				for (int g = 0; g < val; g++) {
					
					if (i * val + g - chetY-py >= batch.sizeY) {
						break;
					}
					
					for (int h = 0; h < val ; h++) {
						
					
						

					
						if ((i * val + g) >= chetY + py) {
							if (Max < batch.batch[i * val + g - (chetY+py)][j * val + h]) {
								Max = batch.batch[i * val + g - (chetY+py)][j * val + h];
								y = i * val + g - (chetY + py);
								x = j * val + h;
							}

						}


					}


					}
				batchMax.batch[i][j] = Max;
				
				batchMax.PositioningMax[i][j][0] = y;
				batchMax.PositioningMax[i][j][1] = x;
				}
				
			}
		}
		
	

	//MaxPooling
	else {
		int y;
		int x;
		double Max;
		for (int i = 0; i < Y; i++) {
			for (int j = 0; j < X; j++) {
				 y = i * val;
				 x = j * val;
				 Max = batch.batch[i * val][j * val];
				for (int g = 0; g < val; g++) {
					for (int h = 0; h < val - 1; h++) {
						if (Max < batch.batch[i * val + g][j * val + h + 1]) {
							Max = batch.batch[i * val + g][j * val + h + 1];
							y = i * val + g;
							x = j * val + h + 1;
						}
					}
				}
				batchMax.batch[i][j] = Max;
				
				batchMax.PositioningMax[i][j][0] = y;
				batchMax.PositioningMax[i][j][1] = x;
				
			}

		}
	}
	
	
	

}


void TasksNetwork::Sweep(double** Grid, double** Yadro, int step, int SizeGridY, int SizeGridX, int SizeYadroY, int SizeYadroX, double** Sweeper) {
	
	
	for (int i = 0; i < (((SizeGridY - (SizeYadroY - 1)) - 1) / step + 1); i++) {
		for (int j = 0; j < (((SizeGridX - (SizeYadroX - 1)) - 1) / step + 1); j++) {
			for (int l = 0; l < SizeYadroY; l++) {
				for (int h = 0; h < SizeYadroX; h++) {
				
					Sweeper[i][j]+=Grid[(i * step) + l][(j * step) + h] * Yadro[l][h];
					
				}
			}
			
	
					
				
			
		}
	}

}

void TasksNetwork::EvalutionErrMaxToBatch(Batch& batchMax, Batch& batch) {
	for (int i = 0; i < batchMax.sizeY; i++) {
		for (int j = 0; j < batchMax.sizeX; j++) {
			
			batch.batchErr[batchMax.PositioningMax[i][j][0]][batchMax.PositioningMax[i][j][1]] = batchMax.batchErr[i][j];
			
		}
	}


}

void TasksNetwork::EvalutionErrBatchToMax(Batch& batch, Batch& batchMax, Cores& core, int step) {


	

	
	for (int i = 0; i < (((batchMax.sizeY - (core.sizeY - 1)) - 1) / step + 1); i++) {
		for (int j = 0; j < (((batchMax.sizeX - (core.sizeX - 1)) - 1) / step + 1); j++) {
			for (int l = 0; l < core.sizeY; l++) {
				for (int h = 0; h < core.sizeX; h++) {

					
					batchMax.batchErr[(i * step) + l][(j * step) + h] += batch.batchErr[i][j] * Function::FunctionUseDer(ReLU, batch.batchNoAct[i][j], 0) * core.Core[l][h];//?
					//batchMax.batchErr[(i * step) + l][(j * step) + h] += batch.batchErr[i][j] * core.Core[l][h];
				}
			}

		
		}
	}


}

void TasksNetwork::ErrorTeachCores(Batch& batch, Batch& batchMax, Cores& core, BiosCNN& bios, int step) {
	
	for (int i = 0; i < (((batchMax.sizeY - (core.sizeY - 1)) - 1) / step + 1); i++) {
		for (int j = 0; j < (((batchMax.sizeX - (core.sizeX - 1)) - 1) / step + 1); j++) {
			for (int l = 0; l < core.sizeY; l++) {
				for (int h = 0; h < core.sizeX; h++) {

				
					core.CoreErr[l][h] += batchMax.batch[(i * step) + l][(j * step) + h]*batch.batchErr[i][j] * Function::FunctionUseDer(ReLU, batch.batchNoAct[i][j], 0);///?
					
					
					//core.CoreErr[l][h] += batchMax.batch[(i * step) + l][(j * step) + h] * batch.batchErr[i][j];
					
					
					
					
					
				}
			}

			bios.biosErrPacket[i][j] += batch.batchErr[i][j] * Function::FunctionUseDer(ReLU, batch.batchNoAct[i][j], 0);
			
			//bios.biosErrPacket[i][j] += batch.batchErr[i][j];
			
		
		}
	}

	
}

 void TasksNetwork::ErrorTeachCores(Batch& batch, Batch& batchMax, Cores& core, BiosCNN& bios, int step, OptimizaterGradient Optimizator) {
	
	HypPar::DataHyperParametr HYpPar;
	double VCore = 0;
	double VBios= 0;
	if (Optimizator == Momentum) {
		for (int i = 0; i < (((batchMax.sizeY - (core.sizeY - 1)) - 1) / step + 1); i++) {
			for (int j = 0; j < (((batchMax.sizeX - (core.sizeX - 1)) - 1) / step + 1); j++) {
				for (int l = 0; l < core.sizeY; l++) {
					for (int h = 0; h < core.sizeX; h++) {

						VCore = VCore * HYpPar.SetM + batchMax.batch[(i * step) + l][(j * step) + h] * batch.batchErr[i][j] * Function::FunctionUseDer(ReLU, batch.batchNoAct[i][j], 0) * (1 - HYpPar.SetM);

						core.CoreErr[l][h] += VCore;


						





					}
				}
				
				VBios = VBios * HYpPar.SetM + (1 - HYpPar.SetM) * batch.batchErr[i][j] * Function::FunctionUseDer(ReLU, batch.batchNoAct[i][j], 0);
				bios.biosErrPacket[i][j] += VBios;


				


			}
		}

		
	}
	if (Optimizator == rmsprop) {
		for (int i = 0; i < (((batchMax.sizeY - (core.sizeY - 1)) - 1) / step + 1); i++) {
			for (int j = 0; j < (((batchMax.sizeX - (core.sizeX - 1)) - 1) / step + 1); j++) {
				for (int l = 0; l < core.sizeY; l++) {
					for (int h = 0; h < core.sizeX; h++) {

						VCore = VCore * HYpPar.SetMR + pow(batchMax.batch[(i * step) + l][(j * step) + h] * batch.batchErr[i][j] * Function::FunctionUseDer(ReLU, batch.batchNoAct[i][j], 0),2) * (1 - HYpPar.SetMR);

						core.CoreErr[l][h] += batchMax.batch[(i * step) + l][(j * step) + h] * batch.batchErr[i][j] * Function::FunctionUseDer(ReLU, batch.batchNoAct[i][j])/(sqrt(VCore)+HYpPar.dEconst);///?








					}
				}

				VBios = VBios * HYpPar.SetMR + (1 - HYpPar.SetMR) * pow(batch.batchErr[i][j] * Function::FunctionUseDer(ReLU, batch.batchNoAct[i][j], 0),2);
				bios.biosErrPacket[i][j] += batch.batchErr[i][j] * Function::FunctionUseDer(ReLU, batch.batchNoAct[i][j], 0)/(VBios+HYpPar.dEconst);





			}
		}
	}
	if (Optimizator == Adam) {
		double VCoreR = 0;
		int kC = 0;
		int kB = 0;
		double VCoreRN = 0;
		double VCoreM = 0;
		double VCoreMN = 0;
		double VBiosR = 0;
		double VBiosM = 0;
		double VBiosRN = 0;
		double VBiosMN = 0;
		for (int i = 0; i < (((batchMax.sizeY - (core.sizeY - 1)) - 1) / step + 1); i++) {
			for (int j = 0; j < (((batchMax.sizeX - (core.sizeX - 1)) - 1) / step + 1); j++) {
				for (int l = 0; l < core.sizeY; l++) {
					for (int h = 0; h < core.sizeX; h++) {

						VCoreR = VCoreR * HYpPar.SetMR + pow(batchMax.batch[(i * step) + l][(j * step) + h] * batch.batchErr[i][j] * Function::FunctionUseDer(ReLU, batch.batchNoAct[i][j], 0), 2) * (1 - HYpPar.SetMR);
						VCoreRN = VCoreR / (1-pow(HYpPar.SetMR, kC+1));
						VCoreM = VCoreM * HYpPar.SetM + batchMax.batch[(i * step) + l][(j * step) + h] * batch.batchErr[i][j] * Function::FunctionUseDer(ReLU, batch.batchNoAct[i][j], 0) * (1 - HYpPar.SetM);
						VCoreMN = VCoreM / (1 - pow(HYpPar.SetM, kC+1));
						core.CoreErr[l][h] += VCoreMN/(sqrt(VCoreRN)+HYpPar.dEconst);



						kC += 1;




					}
				}



				VBiosR = VBiosR * HYpPar.SetMR + pow(batch.batchErr[i][j] * Function::FunctionUseDer(ReLU, batch.batchNoAct[i][j],0), 2) * (1 - HYpPar.SetMR);
				VBiosRN = VBiosR / (1 - pow(HYpPar.SetMR, kB + 1));
				VBiosM = VBiosM * HYpPar.SetM + batch.batchErr[i][j] * Function::FunctionUseDer(ReLU, batch.batchNoAct[i][j], 0) * (1 - HYpPar.SetM);
				VBiosMN = VBiosM / (1 - pow(HYpPar.SetM, kB + 1));
				bios.biosErrPacket[i][j] += VBiosMN / (sqrt(VBiosRN) + HYpPar.dEconst);
				kB += 1;
				





			}
		}


	}
}

void TasksNetwork::actChann(Batch* batchChan, BiosCNN* bios, int valChan) {
	
	for (int i = 0; i < valChan; i++) {
		for (int j = 0; j < batchChan[i].sizeY; j++) {
			for (int l = 0; l < batchChan[i].sizeX; l++) {
				batchChan[i].batch[j][l] += bios[i].bios[j][l];
				
				batchChan[i].batchNoAct[j][l] = batchChan[i].batch[j][l];
				Function::FunctionUse(ReLU,batchChan[i].batch[j][l],0);
			}
		}
	}
}

void TasksNetwork::PaddingX(Batch& batch, int val, Batch& batchPad) {
	

	batchPad(batch.sizeY, batch.sizeX + val,0,0);


	int chet;
	if (val % 2) {
		chet = (val - 1) / 2;
		if (chet != 0) {
			for (int i = 0; i < batch.sizeY; i++) {
				for (int j = 0; j < chet; j++) {


					batchPad.batch[i][j] = 0;
					for (int g = 0; g < batch.sizeX; g++) {
						batchPad.batch[i][j+chet+1] = batch.batch[i][j];
					}
					batchPad.batch[i][j+ batch.sizeX+ chet + 1] = 0;


				}
				batchPad.batch[i][chet] = 0;
			}



			
			


		}
		else {
			for (int i = 0; i < batch.sizeY; i++) {
				batchPad.batch[i][chet] = 0;
				for (int j = 0; j < batch.sizeX; j++) {
					batchPad.batch[i][j+ 1 + chet] = batch.batch[i][j];
				}
			}

		

		}


	}
	else {
		chet = val / 2;
		for (int j = 0; j < chet; j++) {

			for (int i = 0; i < batch.sizeY; i++) {
				for (int j = 0; j < chet; j++) {
					batchPad.batch[i][j] = 0;
					batchPad.batch[i][j + batch.sizeX + chet] = 0;
				}
				for (int j = 0; j < batch.sizeX; j++) {
					batchPad.batch[i][j+chet] = batch.batch[i][j];
				}
			}

		}

		
	}









	


}
void TasksNetwork::PaddingY(Batch& batch, int val, Batch& batchPad) {

	batchPad(batch.sizeY+val, batch.sizeX,0,0);
	
	int chet;
	if (val % 2) {
		 chet = (val-1)/2;
		if (chet != 0) {
			for (int j = 0; j < chet; j++) {

				for (int i = 0; i < batch.sizeX; i++) {
					batchPad.batch[j][i] = 0;
					batchPad.batch[j + batch.sizeY+chet+1][i] = 0;
				}

			}


			for (int i = 0; i < batch.sizeX; i++) {
				batchPad.batch[chet][i] = 0;
			}

			for (int i = 0; i < batch.sizeY; i++) {
				for (int j = 0; j < batch.sizeX; j++) {
					batchPad.batch[i+1 + chet][j] = batch.batch[i][j];
				}
			}

			
		}
		else {
			for (int i = 0; i < batch.sizeX; i++) {
				batchPad.batch[chet][i] = 0;
			}

			for (int i = 0; i < batch.sizeY; i++) {
				for (int j = 0; j < batch.sizeX; j++) {
					batchPad.batch[i+1 + chet][j] = batch.batch[i][j];
				}
			}

		}
		

	}
	else {
		 chet = val/2;
		for (int j = 0; j < chet; j++) {

			for (int i = 0; i < batch.sizeX; i++) {
				batchPad.batch[j][i] = 0;
				batchPad.batch[j + batch.sizeY+ chet][i] = 0;
			}

		}

		for (int i = 0; i < batch.sizeY; i++) {
			for (int j = 0; j < batch.sizeX; j++) {
				batchPad.batch[i + chet][j] = batch.batch[i][j];
			}
		}
	}
	




		
	
	
}

void TasksNetwork::PaddingXY(Batch& batch, int valX, int valY, Batch& batchPad) {



	batchPad(batch.sizeY + valY, batch.sizeX+ valX,0,0);


	int chet;
	if (valX % 2) {
		chet = (valX - 1) / 2;
		if (chet != 0) {
			for (int i = 0; i < batch.sizeY; i++) {
				for (int j = 0; j < chet; j++) {


					batchPad.batch[i + 1 + chet][j] = 0;
					for (int g = 0; g < batch.sizeX; g++) {
						batchPad.batch[i + 1 + chet][j + chet + 1] = batch.batch[i][j];
						
					
					}
					batchPad.batch[i + 1 + chet][j + batch.sizeX + chet + 1] = 0;


				}
				batchPad.batch[i + 1 + chet][chet] = 0;
			}







		}
		else {
			for (int i = 0; i < batch.sizeY; i++) {
				batchPad.batch[i + 1 + chet][chet] = 0;
				for (int j = 0; j < batch.sizeX; j++) {
					batchPad.batch[i + 1 + chet][j + 1 + chet] = batch.batch[i][j];
				}
			}



		}


	}
	else {
		chet = valX / 2;
		for (int j = 0; j < chet; j++) {

			for (int i = 0; i < batch.sizeY; i++) {
				for (int j = 0; j < chet; j++) {
					batchPad.batch[i +  chet][j] = 0;
					batchPad.batch[i +  chet][j + batch.sizeX + chet] = 0;
				}
				for (int j = 0; j < batch.sizeX; j++) {
					batchPad.batch[i +  chet][j + chet] = batch.batch[i][j];
				}
			}

		}


	}
	
	if (valY % 2) {
		chet = (valY - 1) / 2;
		if (chet != 0) {
			for (int j = 0; j < chet; j++) {

				for (int i = 0; i < batch.sizeX+ valX; i++) {
					batchPad.batch[j][i] = 0;
					batchPad.batch[j + batch.sizeY + chet + 1][i] = 0;
				}

			}


			for (int i = 0; i < batch.sizeX+ valX; i++) {
				batchPad.batch[chet][i] = 0;
			}

		


		}
		else {
			for (int i = 0; i < batch.sizeX+ valX; i++) {
				batchPad.batch[chet][i] = 0;
			}

			

		}


	}
	else {
		chet = valY / 2;
		for (int j = 0; j < chet; j++) {

			for (int i = 0; i < batch.sizeX+ valX; i++) {
				batchPad.batch[j][i] = 0;
				batchPad.batch[j + batch.sizeY + chet][i] = 0;
			}

		}

		
	}

	
	

}