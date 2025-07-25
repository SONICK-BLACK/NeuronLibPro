#include "Tensor.h"
using namespace Tsr;

Tensor::Tensor(int VallSloy, const int ArrSizeSloy[], const ActFuns ActFunc[]) {
	SizeValSloy = VallSloy;
	act = new ActFuns[VallSloy - 1];
	MatrixNeuron = new NeuronClass[SizeValSloy];
	MatrixWheight = new WheightClass[SizeValSloy - 1];
	MatrixBios = new BiosClass[SizeValSloy - 1];
	for (int i = 0; i < SizeValSloy; i++) {
		if (!(i == SizeValSloy - 1)) {

			act[i] = ActFunc[i];
			MatrixNeuron[i].InitNeuronClass(ArrSizeSloy[i]);
			MatrixWheight[i].InitWheightClass(ArrSizeSloy[i] * ArrSizeSloy[i + 1], ArrSizeSloy[i + 1]);
			MatrixBios[i].InitBiosClass(ArrSizeSloy[i + 1], ArrSizeSloy[i]);

		}
		else {
			MatrixNeuron[i].InitNeuronClass(ArrSizeSloy[i]);
		}
	}



}
void Tensor::StartDirect() {
	
	for (int i = 1; i < SizeValSloy; i++) {
		TasksNetwork::DirectDirection(MatrixNeuron[i - 1], MatrixNeuron[i], MatrixWheight[i - 1], MatrixBios[i - 1], act[i - 1]);
		
	}
}
void Tensor::StartDirectBase() {
	
	for (int i = 1; i < SizeValSloy; i++) {
		TasksNetwork::DirectDirectionBase(MatrixNeuron[i - 1], MatrixNeuron[i], MatrixWheight[i - 1], MatrixBios[i - 1], act[i - 1]);
	}
}
void Tensor::LoadData(double* Data) {
	for (int i = 0; i < MatrixNeuron[0].sizeMatrix; i++)
		MatrixNeuron[0].Neuron[i] = Data[i];
}
void Tensor::StartTrainingSet(double* VectorRight, ErrFuns erF) {
	for (int i = 0; i < SizeValSloy; i++) {
		MatrixNeuron[i].NeuronErrSetNull();
	}
	TasksNetwork::EvalutionError(VectorRight,MatrixNeuron[SizeValSloy-1],erF);//Verno
	for (int i = SizeValSloy-1; i > 1; i--) {
		TasksNetwork::EvalutionErrorLast(MatrixNeuron[i], MatrixNeuron[i - 1], MatrixWheight[i - 1], act[i-1]);//Verno
	}
	for (int i = SizeValSloy - 1; i > 0; i--) {
		TasksNetwork::ErrorTeachSloySet(MatrixNeuron[i - 1], MatrixNeuron[i], MatrixWheight[i - 1], MatrixBios[i - 1], act[i-1]);//Verno
	}
}

void Tensor::StartTrainingSetCNN(double* VectorRight, ErrFuns erF, double* ErrVector) {
	for (int i = 0; i < SizeValSloy; i++) {
		MatrixNeuron[i].NeuronErrSetNull();
	}
	TasksNetwork::EvalutionError(VectorRight, MatrixNeuron[SizeValSloy - 1], erF);
	for (int i = SizeValSloy - 1; i > 1; i--) {
		TasksNetwork::EvalutionErrorLast(MatrixNeuron[i], MatrixNeuron[i - 1], MatrixWheight[i - 1], act[i - 1]);    
	}
	for (int i = SizeValSloy - 1; i > 0; i--) {
		TasksNetwork::ErrorTeachSloySet(MatrixNeuron[i - 1], MatrixNeuron[i], MatrixWheight[i - 1], MatrixBios[i - 1], act[i - 1]);
	}
	TasksNetwork::EvalutionErrorLast(MatrixNeuron[1], MatrixNeuron[0], MatrixWheight[0], act[0]);
	
	for (int i = 0; i < MatrixNeuron[0].sizeMatrix; i++) {
		ErrVector[i] = MatrixNeuron[0].NeuronErr[i];
	}

}

void Tensor::InitClassesErr() {//Ðåàëèçàöèÿ ôóíêöèè â äðóãèõ StartTrainingov ñäåëàòü
	for (int i = 0; i < SizeValSloy; i++) {
		MatrixNeuron[i].InitNeuronClassErr();

	}
	for (int i = 0; i < SizeValSloy - 1; i++) {
		MatrixWheight[i].InitWheightErr();
		MatrixBios[i].InitErrBios();
	}
} 

void Tensor::StartGradient(int PacketSet1, double SpeedTeach) {
	for (int i = 0; i < SizeValSloy - 1; i++) {
		for (int j = 0; j < MatrixWheight[i].sizeMatrix; j++) {
			MatrixWheight[i].Wheight[j] = MatrixWheight[i].Wheight[j] - MatrixWheight[i].SetErrorWhPacket[j] * SpeedTeach / PacketSet1;
			
	
		}
		for (int j = 0; j < MatrixBios[i].SizeMatrix; j++) {
			MatrixBios[i].Bios[j] = MatrixBios[i].Bios[j] - MatrixBios[i].SetErrorBiosPacket[j] * SpeedTeach / PacketSet1;
		}
	}

	for (int i = 0; i < SizeValSloy - 1; i++) {
		MatrixWheight[i].WheightErrSetNull();
		MatrixBios[i].BiosErrSetNull();
	}
}
void Tensor::StartTeachSession(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch) {
	for (int i = 0; i < SizeValSloy; i++) {
		MatrixNeuron[i].InitNeuronClassErr();

	}
	for (int i = 0; i < SizeValSloy - 1; i++) {
		MatrixWheight[i].InitWheightErr();
		MatrixBios[i].InitErrBios();
	}
	int SetFlagPacket = 0;

	for (int h = 0; h < epoch; h++) {
		for (int j = 0; j < Data.SizeData; j++) {

			LoadData(Data.SetData[j]);
			StartDirect();
	
			
			if (MatrixNeuron[SizeValSloy - 1].sizeMatrix == 1) {
				StartTrainingSet(Data.CorrectVal[j], FunErr);
			}

				else if (!SetCorrectVal(Data.CorrectVal[j])) {

					StartTrainingSet(Data.CorrectVal[j], FunErr);
				}
				SetFlagPacket += 1;


				if (SetFlagPacket == PacketSet) {


					StartGradient(PacketSet, SpeedTeach);
					
					SetFlagPacket = 0;


				}
				for (int i = 0; i < SizeValSloy; i++) {
					MatrixNeuron[i].NeuronSetNull();
				}

		}
		SetFlagPacket = 0;
		if (!(MatrixNeuron[SizeValSloy - 1].sizeMatrix == 1)) {
			double val = 0;
			for (int j = 0; j < Data.SizeData; j++) {
				LoadData(Data.SetData[j]);
				StartDirectBase();
				SetCorrectVal(Data.CorrectVal[j], val);
				for (int i = 0; i < SizeValSloy; i++) {
					MatrixNeuron[i].NeuronSetNulL();
				}


			}
			cout << endl;
			cout << "Epoch (" << h << "): " << (val / Data.SizeData) * 100 << "%" << "\n";
		}
		else {
			cout << "Epoch (" << h << "):\n ";

		}

	}
	cout << endl;
	cout << "End Teach Tensor\n";
}
bool Tensor::StartTeachSessionÑNN(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch,double* VectorErr, int t) {
	bool F = true;
	

	for (int h = 0; h < epoch; h++) {
		for (int j = 0; j < Data.SizeData; j++) {

			LoadData(Data.SetData[j]);
			StartDirect();
		
			if (MatrixNeuron[SizeValSloy - 1].sizeMatrix == 1) {
				StartTrainingSetCNN(Data.CorrectVal[j], FunErr,VectorErr);
			}

			else if (!SetCorrectVal(Data.CorrectVal[j])) {

				StartTrainingSetCNN(Data.CorrectVal[j], FunErr, VectorErr);

			}
			else {
				F = false;
			}
			


			if (t == PacketSet) {


				StartGradient(PacketSet, SpeedTeach);

			


			}
			for (int i = 0; i < SizeValSloy; i++) {
				MatrixNeuron[i].NeuronSetNull();
			}

		}
		
	

	}
	return F;

}

//
void Tensor::StartTeachSession(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch, OptimizaterGradient Optimizator, Regulizators regulizator) {
	for (int i = 0; i < SizeValSloy; i++) {
		MatrixNeuron[i].InitNeuronClassErr();

	}
	for (int i = 0; i < SizeValSloy - 1; i++) {
		MatrixWheight[i].InitWheightErr();
		MatrixBios[i].InitErrBios();
	}
	int SetFlagPacket = 0;
	
	for (int h = 0; h < epoch; h++) {
		for (int j = 0; j < Data.SizeData; j++) {

			LoadData(Data.SetData[j]);
			StartDirect();
	
			
			if(MatrixNeuron[SizeValSloy - 1].sizeMatrix == 1) {
				if (Optimizator != NullO) {
					StartTrainingSet(Data.CorrectVal[j], FunErr, Optimizator);
				}
				else {
					StartTrainingSet(Data.CorrectVal[j], FunErr);
				}
			}
			else if (!SetCorrectVal(Data.CorrectVal[j])) {

				if (Optimizator != NullO) {
					StartTrainingSet(Data.CorrectVal[j], FunErr, Optimizator);
				}
				else {
					StartTrainingSet(Data.CorrectVal[j], FunErr);
				}
			}
			SetFlagPacket += 1;


			if (SetFlagPacket == PacketSet) {

				if (regulizator != NullR) {
					StartGradient(PacketSet, SpeedTeach,regulizator, Data.SizeData);
				}
				else {
					StartGradient(PacketSet, SpeedTeach);
				}

				SetFlagPacket = 0;


			}
			for (int i = 0; i < SizeValSloy; i++) {
				MatrixNeuron[i].NeuronSetNull();
			}
			cout << endl;
			cout << 1;
		}
		SetFlagPacket = 0;
		if (!(MatrixNeuron[SizeValSloy - 1].sizeMatrix == 1)) {
			double val = 0;
			for (int j = 0; j < Data.SizeData; j++) {
				LoadData(Data.SetData[j]);
				StartDirectBase();
				SetCorrectVal(Data.CorrectVal[j], val);
				for (int i = 0; i < SizeValSloy; i++) {
					MatrixNeuron[i].NeuronSetNulL();
				}
				

			}
			cout << endl;
			cout << "Epoch (" << h << "): " << (val / Data.SizeData) * 100 << "%" << "\n";
		}
		else {
			cout << "Epoch (" << h << "):\n ";

		}

	}
	cout << endl;
	cout << "End Teach Tensor\n";
}
//
void Tensor::StartTrainingSet(double* VectorRight, ErrFuns erF, OptimizaterGradient Optimizator) {
	for (int i = 0; i < SizeValSloy; i++) {
		MatrixNeuron[i].NeuronErrSetNull();
	}
	TasksNetwork::EvalutionError(VectorRight, MatrixNeuron[SizeValSloy - 1], erF);//Verno
	for (int i = SizeValSloy - 1; i > 1; i--) {
		TasksNetwork::EvalutionErrorLast(MatrixNeuron[i], MatrixNeuron[i - 1], MatrixWheight[i - 1], act[i - 1]);//Verno
	}
	for (int i = SizeValSloy - 1; i > 0; i--) {
		TasksNetwork::ErrorTeachSloySet(MatrixNeuron[i - 1], MatrixNeuron[i], MatrixWheight[i - 1], MatrixBios[i - 1], act[i - 1],Optimizator);//Verno
	}
}


void Tensor::StartGradient(int PacketSet, double SpeedTeach, Regulizators regulizator, int SizeObservations) {
	HypPar::DataHyperParametr HYpPar;
/*??*/	if (regulizator == L1) {
		for (int i = 0; i < SizeValSloy - 1; i++) {
			for (int j = 0; j < MatrixWheight[i].sizeMatrix; j++) {
				MatrixWheight[i].Wheight[j] = MatrixWheight[i].Wheight[j] - MatrixWheight[i].SetErrorWhPacket[j] * SpeedTeach / PacketSet -(SpeedTeach * HYpPar.hOptim1) / (PacketSet * SizeObservations);


			}
			for (int j = 0; j < MatrixBios[i].SizeMatrix; j++) {

				MatrixBios[i].Bios[j] = MatrixBios[i].Bios[j] - MatrixBios[i].SetErrorBiosPacket[j] * SpeedTeach / PacketSet-(SpeedTeach * HYpPar.hOptim1) / (PacketSet * SizeObservations);
			}
		}
	}			
	if(regulizator==L2){

		for (int i = 0; i < SizeValSloy - 1; i++) {
			for (int j = 0; j < MatrixWheight[i].sizeMatrix; j++) {
				MatrixWheight[i].Wheight[j] = (1- (SpeedTeach*HYpPar.hOptim2) / (PacketSet* SizeObservations))*MatrixWheight[i].Wheight[j] - MatrixWheight[i].SetErrorWhPacket[j] * SpeedTeach / PacketSet;


			}
			for (int j = 0; j < MatrixBios[i].SizeMatrix; j++) {
				MatrixBios[i].Bios[j] = (1 - (SpeedTeach * HYpPar.hOptim2) / (PacketSet * SizeObservations))* MatrixBios[i].Bios[j] - MatrixBios[i].SetErrorBiosPacket[j] * SpeedTeach / PacketSet;
			}
		}

		
	}

	if (regulizator == ElasticNet) {

		for (int i = 0; i < SizeValSloy - 1; i++) {
			for (int j = 0; j < MatrixWheight[i].sizeMatrix; j++) {
				MatrixWheight[i].Wheight[j] = (1 - (SpeedTeach * HYpPar.hOptim2) / (PacketSet * SizeObservations)) * MatrixWheight[i].Wheight[j] - MatrixWheight[i].SetErrorWhPacket[j] * SpeedTeach / PacketSet - (SpeedTeach * HYpPar.hOptim1) / (PacketSet * SizeObservations);;


			}
			for (int j = 0; j < MatrixBios[i].SizeMatrix; j++) {
				MatrixBios[i].Bios[j] = (1 - (SpeedTeach * HYpPar.hOptim2) / (PacketSet * SizeObservations)) * MatrixBios[i].Bios[j] - MatrixBios[i].SetErrorBiosPacket[j] * SpeedTeach / PacketSet - (SpeedTeach * HYpPar.hOptim1) / (PacketSet * SizeObservations);;
			}
		}


	}
	for (int i = 0; i < SizeValSloy - 1; i++) {
		MatrixWheight[i].WheightErrSetNull();
		MatrixBios[i].BiosErrSetNull();
	}
}


//
void Tensor::StartTeachSession(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch, OptimizaterGradient Optimizator, Regulizators regulizator, bool StochasticSpeed) {
	for (int i = 0; i < SizeValSloy; i++) {
		MatrixNeuron[i].InitNeuronClassErr();

	}
	for (int i = 0; i < SizeValSloy - 1; i++) {
		MatrixWheight[i].InitWheightErr();
		MatrixBios[i].InitErrBios();
	}
	int SetFlagPacket = 0;
	
	for (int h = 0; h < epoch; h++) {
		for (int j = 0; j < Data.SizeData; j++) {

			LoadData(Data.SetData[j]);
			StartDirect();
	

			if (MatrixNeuron[SizeValSloy - 1].sizeMatrix == 1) {
				if (Optimizator != NullO) {
					StartTrainingSet(Data.CorrectVal[j], FunErr, Optimizator);
				}
				else {
					StartTrainingSet(Data.CorrectVal[j], FunErr);
				}
			}

			else if (!SetCorrectVal(Data.CorrectVal[j])) {

				if (Optimizator != NullO) {
					StartTrainingSet(Data.CorrectVal[j], FunErr, Optimizator);
				}
				else {
					StartTrainingSet(Data.CorrectVal[j], FunErr);
				}
			}
			SetFlagPacket += 1;


			if (SetFlagPacket == PacketSet) {

				if (regulizator != NullR) {
					if(StochasticSpeed){
						StartGradient(PacketSet, SpeedTeach * exp(-h / 20.0), regulizator, Data.SizeData);
					}
					else {
						StartGradient(PacketSet, SpeedTeach, regulizator, Data.SizeData);
					}
				}
				else {
					if (StochasticSpeed) {
						StartGradient(PacketSet, SpeedTeach * exp(-h / 20.0));
					}
					else {
						StartGradient(PacketSet, SpeedTeach);
					}
				}

				SetFlagPacket = 0;


			}
			for (int i = 0; i < SizeValSloy; i++) {
				MatrixNeuron[i].NeuronSetNull();
			}

		}
		SetFlagPacket = 0;


		if (!(MatrixNeuron[SizeValSloy - 1].sizeMatrix == 1)) {
			double val = 0;
			for (int j = 0; j < Data.SizeData; j++) {
				LoadData(Data.SetData[j]);
				StartDirectBase();
				SetCorrectVal(Data.CorrectVal[j], val);
				for (int i = 0; i < SizeValSloy; i++) {
					MatrixNeuron[i].NeuronSetNulL();
				}
			

			}
			cout << endl;
			cout << "Epoch (" << h << "): " << (val / Data.SizeData) * 100 << "%" << "\n";
		}
		else {
			cout << "Epoch (" << h << "):\n ";

		}


	

		
		


		
	}
	cout << endl;
	cout << "End Teach Tensor\n";
}

//




void Tensor::SaveParametsNeurons(string pathW, string pathB) {
	ofstream outW;
	outW.open(pathW);
	ofstream outB;
	outB.open(pathB);
	for (int i = 0; i < SizeValSloy - 1; i++) {
		for (int j = 0; j < MatrixWheight[i].sizeMatrix; j++) {
			outW << MatrixWheight[i].Wheight[j]<<"\n";


		}
		for (int j = 0; j < MatrixBios[i].SizeMatrix; j++) {

			outB << MatrixBios[i].Bios[j] << "\n";
		}
	}
	outW.close();
	outB.close();
	cout << endl;
	cout << "End Save Paramets";
}
void Tensor::LoadParametsNeurons(string pathW , string pathB) {
	fstream outW;
	fstream outB;
	outW.open(pathW);
	outB.open(pathB);
	if (!outW.is_open()) {
		cout << endl;
		cout << "Error open File...";
	}
	if (!outB.is_open()) {
		cout << endl;
		cout << "Error open File...";
	}
	for (int i = 0; i < SizeValSloy - 1; i++) {
		for (int j = 0; j < MatrixWheight[i].sizeMatrix; j++) {
			outW >> MatrixWheight[i].Wheight[j];


		}
		for (int j = 0; j < MatrixBios[i].SizeMatrix; j++) {

			outB >> MatrixBios[i].Bios[j];
		}
	}
	outW.close();
	outB.close();
	cout << endl;
	cout << "End load Paramets";
}
void Tensor::StartDirectSession(DataNeuron& Data, void (*set_function)(double* setNeuron)) {
	for (int j = 0; j < Data.SizeData; j++) {
		LoadData(Data.SetData[j]);
		StartDirect();
		set_function(MatrixNeuron[SizeValSloy - 1].Neuron);
		for (int i = 0; i < SizeValSloy; i++) {
			MatrixNeuron[i].NeuronSetNull();
		}
	}
}

void Tensor::StartDirectSessionCNNBase(double* Data, double& val, double* Correctval) {
	
		LoadData(Data);
		StartDirect();
		


		int number = 0;
		///???
		double max = MatrixNeuron[SizeValSloy - 1].Neuron[0];
	/*	cout << endl;
		cout << "Âûõîä: ";
		for (int i = 0; i < MatrixNeuron[SizeValSloy - 1].sizeMatrix; i++) {


			cout << MatrixNeuron[SizeValSloy - 1].Neuron[i] << " ";
		}
		cout << endl;
		cout << "Âåðíûå çíà÷åíèÿ: ";
		for (int i = 0; i < MatrixNeuron[SizeValSloy - 1].sizeMatrix; i++) {
			cout << Correctval[i] << " ";
		}
		*/
		for (int i = 0; i < MatrixNeuron[SizeValSloy - 1].sizeMatrix-1; i++) {

			if (MatrixNeuron[SizeValSloy - 1].Neuron [i + 1] > max) {
				number = (i + 1);
				max = MatrixNeuron[SizeValSloy - 1].Neuron[i + 1];
			}


		}
		if ((int)Correctval[number] == 1) {
			val+= 1.0;


		}
		



		for (int i = 0; i < SizeValSloy; i++) {
			MatrixNeuron[i].NeuronSetNull();
		}
	
}
void Tensor::StartDirectSession(DataNeuron& Data, void (*set_function)(double* setNeuron, double* SetCorectVal)) {
	for (int j = 0; j < Data.SizeData; j++) {
		LoadData(Data.SetData[j]);
		StartDirectBase();
		set_function(MatrixNeuron[SizeValSloy - 1].Neuron,Data.CorrectVal[j]);
		
		for (int i = 0; i < SizeValSloy; i++) {
			MatrixNeuron[i].NeuronSetNull();
		}
	}
}

void Tensor::StartDirectSession(double* Data, void (*set_function)(double* setNeuron)) {
	
		LoadData(Data);
		StartDirectBase();
		set_function(MatrixNeuron[SizeValSloy - 1].Neuron);

		for (int i = 0; i < SizeValSloy; i++) {
			MatrixNeuron[i].NeuronSetNull();
		}
	
}
bool Tensor::SetCorrectVal(double* SetCorrect) {



	int number=0;
	///???
	double max = MatrixNeuron[SizeValSloy - 1].Neuron[0];
	
	for (int i = 0; i < MatrixNeuron[SizeValSloy - 1].sizeMatrix-1; i++) {
	
		if (MatrixNeuron[SizeValSloy - 1].Neuron[i + 1] > max) {
			number = (i + 1);
			max = MatrixNeuron[SizeValSloy - 1].Neuron[i+1];
		}
		
	
	}
	if ((int)SetCorrect[number]==1) {
		
		return true;

	}
	else {
		return false;
	}
}

void Tensor::SetCorrectVal(double* SetCorrect, double& val) {



	int number = 0;
	///???
	double max = MatrixNeuron[SizeValSloy - 1].Neuron[0];
	for (int i = 0; i < MatrixNeuron[SizeValSloy - 1].sizeMatrix - 1; i++) {

		if (MatrixNeuron[SizeValSloy - 1].Neuron[i + 1] > max) {
			number = (i + 1);
			max = MatrixNeuron[SizeValSloy - 1].Neuron[i + 1];
		}


	}
	if ((int)SetCorrect[number] == 1) {
		val += 1;
		

	}
	else {
		
	}
}


Tensor::~Tensor() {
	delete[] act;
	delete[] MatrixBios;
	delete[] MatrixWheight;
	delete[] MatrixNeuron;

}
double** RegressionModel::TransportMat(double** mat, int col, int str) {
	double val;
	double** matT = new double*[col];
	for (int i = 0; i < col; i++)
		matT[i] = new double[str];
	for (int i = 0; i < col; i++) {
		for (int j = 0; j < str; j++) {
			matT[i][j] = mat[j][i];
		}
	}

	return matT;

}
void RegressionModel::SetWheightsBParamets(DataRegression& dataReg) {
	double** xT = TransportMat(dataReg.X, dataReg.SizeParametrs, dataReg.SizeExperiens);

	double** mat = MultiMatrix(xT, dataReg.X, dataReg.SizeParametrs, dataReg.SizeExperiens, dataReg.SizeParametrs, dataReg.SizeExperiens);
	if (mat == 0) {
		cout << endl;
		cout << "Error Matrix";
	}
	else {
		double** matA= new double*[dataReg.SizeParametrs];
		for (int i = 0; i < dataReg.SizeParametrs; i++) {
			matA[i] = new double[dataReg.SizeParametrs];

		}
		for (int i = 0; i < dataReg.SizeParametrs; i++) {
			for (int j = 0; j < dataReg.SizeParametrs; j++) {
				matA[i][j] = pow(-1, (j + 1) + (i + 1)) * def_SetMatrixAlgibration(mat, dataReg.SizeParametrs, i, j);
			}
		}
		double** matAT = TransportMat(matA, dataReg.SizeParametrs, dataReg.SizeParametrs);
		double detmat;
		detmat = def_determination(mat, dataReg.SizeParametrs);
		
		for (int i = 0; i < dataReg.SizeParametrs; i++) {
			for (int j = 0; j < dataReg.SizeParametrs; j++) {
				matAT[i][j] *= (1 / detmat);
			}
		}
		double** mat2 = MultiMatrix(matAT, xT, dataReg.SizeParametrs, dataReg.SizeParametrs, dataReg.SizeExperiens, dataReg.SizeParametrs);
		dataReg.b = MultiMatrix(mat2, dataReg.Y, dataReg.SizeParametrs, dataReg.SizeExperiens, 1, dataReg.SizeExperiens);
		///Delete Matrixs
		for (int i = 0; i < dataReg.SizeParametrs; i++) {
			delete[] xT[i];
			delete[] matAT[i];
			delete[] mat2[i];
			delete[] matA[i];
			delete[] mat[i];
		}
			
		delete[] xT;
		delete[] matAT;
		delete[] mat2;
		delete[] matA;
		delete[] mat;


	}
}

void RegressionModel::SetRcriteria(DataRegression& dataReg) {
	double Qr = 0;
	double Qe = 0;
	double Mid=0;
	for (int i = 0; i < dataReg.SizeExperiens; i++) {
		double Val = 0;
		for (int j = 0; j < dataReg.SizeParametrs; j++) {
			
			Val += dataReg.X[i][j] * dataReg.b[j][0];

		}
		Mid += Val;

	}
	Mid = Mid / (double)dataReg.SizeExperiens;


	for (int i = 0; i < dataReg.SizeExperiens; i++) {
		double Val = 0;
		for (int j = 0; j < dataReg.SizeParametrs; j++) {

			Val += dataReg.X[i][j] * dataReg.b[j][0];

		}
		Qe += pow((dataReg.Y[i][0] - Val), 2);
		Qr += pow((dataReg.Y[i][0] - Mid), 2);

	}
	cout << "\n";
	cout << "R Criteria: " << Qr / (Qe + Qr);
	cout << "\n";
}


double RegressionModel::def_SetMatrixAlgibration(double** a, int Size, int i, int j) {
	const int size = Size - 1;
	double Amod;
	double** A;
	A = new double* [size];
	for (int o = 0; o < size; o++) {
		A[o] = new double[size];
	}
	int k = 0;
	int h = 0;
	for (int r = 0; r < Size; r++) {
		for (int g = 0; g < Size; g++) {
			if (!(r == i || g == j)) {
				A[h][k] = a[r][g];
				k += 1;
			}
			if (k == size) {
				k = 0;
				h += 1;
			}
		}
	}

	Amod = def_determination(A, size);
	for (int o = 0; o < size; o++) {
		delete[] A[o];
	}
	delete[] A;

	return Amod;
	}
double RegressionModel::def_determination(double** M, int size) {
	if (size == 1) {
		return M[0][0];
	}
	double SumA = 0;
	if (size != 2) {
		double** A = new double* [size - 1];
		for (int o = 0; o < size - 1; o++) {
			A[o] = new double[size - 1];
		}
		int k = 0;
		int h = 0;
		for (int j = 0; j < size; j++) {
			for (int r = 0; r < size; r++) {
				for (int g = 0; g < size; g++) {
					if (!(r == 0 || g == j)) {
						A[h][k] = M[r][g];
						k += 1;
					}
					if (k == size - 1) {
						k = 0;
						h += 1;
					}
				}
			}
			h = 0;
			SumA += M[0][j] * pow(-1, 1 + j + 1) * def_determination(A, size - 1);
		}
		for (int i = 0; i < size - 1; i++)
			delete[] A[i];
		delete[] A;
	}
	else {
		SumA = def_DeterminateTwoMat(M);
	}



return SumA;
}
double RegressionModel::def_DeterminateTwoMat(double** M2) {
	double SumA = 0;
	SumA = (M2[0][0] * M2[1][1]) - (M2[0][1] * M2[1][0]);
	return SumA;
}
double** RegressionModel::MultiMatrix(double** M1, double** M2, int str1, int col1, int col2, int str2) {
	if (col1 != str2) {
		cout << endl;
		cout << "Error MultyMatrix";
		return 0;
	}
	else {
		double** Mat = new double* [str1];
		for (int i = 0; i < str1; i++) {
			Mat[i] = new double[col2];
		}
		for (int i = 0; i < str1; i++) {
			for (int j = 0; j < col2; j++) {
				Mat[i][j] = 0;
			}

		}


		for (int i = 0; i < str1; i++) {

			for (int g = 0; g < col2; g++) {
				for (int j = 0; j < col1; j++) {
					Mat[i][g] += M1[i][j] * M2[j][g];

				}

			}
		}

		return Mat;
	}

}

CNN::CNN(int chanels, int  valCore[], int sloys, int** SizeCore, int Step) {
	// For channels, the size and number of cores are the same
// For each batch, the size of the cores is the same
	
	PollingVal = new int[sloys];
	for (int i = 0; i < sloys; i++) {
		PollingVal[i] = 1;
	}
	this->chanels = chanels;
	this->sloys = sloys;
	this->Step = Step;
	batñh = new Batch * [sloys];
	bios = new BiosCNN * [sloys];
	batñhMax = new Batch * [sloys];
	//Initialization of the number of cores in each layer per 1 batch
	this->valCore = new int[sloys];
	for (int i = 0; i < sloys; i++) {
		this->valCore[i] = valCore[i];
	}
	///Initialization of Cores + Batches will need to do an additional check later
	ñores = new Cores **[sloys];
	ñores[0] = new Cores * [chanels];
	for (int i = 0; i < chanels; i++) {
		ñores[0][i] = new Cores[valCore[0]];
		for (int j = 0; j < valCore[0]; j++) {
			ñores[0][i][j](SizeCore[0][0], SizeCore[0][1]);
		}

	}




	
	ValBat = valCore[sloys-1];
	batñh[0] = new Batch[valCore[0]];
	bios[0] = new BiosCNN[valCore[0]];
	batñhMax[0]= new Batch[valCore[0]];
	for (int i = 1; i < sloys; i++) {
		
		
			batñh[i] = new Batch[valCore[i]];
			bios[i] = new BiosCNN[valCore[i]];
			batñhMax[i] = new Batch[valCore[i]];
			ñores[i] = new Cores * [valCore[i - 1]];
			for (int l = 0; l < valCore[i - 1]; l++) {
				ñores[i][l] = new Cores[valCore[i]];
				for (int j = 0; j < valCore[i]; j++) {
					ñores[i][l][j](SizeCore[i][0], SizeCore[i][1]);
					
				}
			}
			
			
	

		}
	

	



}
void CNN::StartTrainingCNN(DataCNN& GridData, int VallSloy, int ArrSizeSloy[], const ActFuns ActFunc[], double SpeedTeach, int PacketSet, ErrFuns FunErr, int epoch) {
	SizeOutNeurons = ArrSizeSloy[VallSloy - 1];
	SetInit = false;
	//The error vector from peptsetron
	double* VectorErr;
	//Output vector
	double* VectorOut;

	int t = 0; //For packages
	// Batch initialization, if the return of 1 is successful //Let's make the same scale for everyone in X and Y

	if (!InitBatches( GridData.Grid[0][0].sizeX, GridData.Grid[0][0].sizeY)) {
		return;
	}
	else {
		
	ArrSizeSloy[0]= ValBat * batñhMax[sloys - 1][0].sizeY * batñhMax[sloys - 1][0].sizeX;
	localPepzetron = new Tensor(VallSloy, ArrSizeSloy, ActFunc);
	localPepzetron->InitClassesErr();
	}
int SizeVectOut = ValBat * batñhMax[sloys - 1][0].sizeY * batñhMax[sloys - 1][0].sizeX;
VectorErr = new double[SizeVectOut];
VectorOut = new double[SizeVectOut];

	DataNeuron data(1, SizeVectOut, GridData.OutSize);
	
	for (int i = 0; i < epoch; i++) {

		for (int j = 0; j < GridData.SizeData; j++) {
			///Batch Scan
			SweepBatches(GridData.Grid[j], GridData.Grid[j][0].sizeX, GridData.Grid[j][0].sizeY);

			//Scan to vector
			VectorSweep(batñhMax[sloys - 1], valCore[sloys - 1], VectorOut);
			
			
		

			
			

			for (int o = 0; o < GridData.OutSize; o++) {
			data.CorrectVal[0][o] = GridData.CorrectVal[j][o];
			}
			for (int h = 0; h < SizeVectOut; h++) {
				data.SetData[0][h] = VectorOut[h];
			}

			


			//Starting the propagation of the error in the pepcetron and getting the input layer error, if correction is not required, we switch to another epoch
			if (!localPepzetron->StartTeachSessionÑNN(SpeedTeach, PacketSet, data, FunErr, 1, VectorErr, t + 1)) {
				
				
		
				t += 1;
				if (t == PacketSet) {
					StrartGradientCore(PacketSet, SpeedTeach);

					t = 0;
				}
				NullBatchSet();
				 continue;
			}
			
			//Translation of the error vector into the batch error matrix
			VectorErrToMatrix(VectorErr, batñhMax[sloys - 1], valCore[sloys - 1]);
			//Distribution of batch errors
			

			   for (int d = 0; d < valCore[sloys-1]; d++) {
				   TasksNetwork::EvalutionErrMaxToBatch(batñhMax[sloys - 1][d], batñh[sloys - 1][d]);
			   }



			for (int f = sloys - 1; f > 0; f--) {
				
			
					for (int l = 0; l < valCore[f-1]; l++) {
						for (int h = 0; h <  valCore[f]; h++) {
							TasksNetwork::EvalutionErrBatchToMax(batñh[f][h], batñhMax[f - 1][l], ñores[f][l][h], Step);
					
						}
					}
				
					
					
				
			

				for (int d = 0; d < valCore[f - 1]; d++) {
					TasksNetwork::EvalutionErrMaxToBatch(batñhMax[f-1][d], batñh[f-1][d]);
				}


				
			

			}




			for (int f = sloys - 1; f >= 0; f--) {
				
				if (f == 0) {
					
					for (int l = 0; l <chanels; l++) {
						for (int h = 0; h < valCore[f]; h++) {
							TasksNetwork::ErrorTeachCores(batñh[f][h],GridData.Grid[j][l], ñores[f][l][h],bios[f][h], Step);
						
						}
					}
					

				}
				else {

					for (int l = 0; l < valCore[f - 1]; l++) {
						for (int h = 0; h < valCore[f]; h++) {
							TasksNetwork::ErrorTeachCores(batñh[f][h], batñhMax[f - 1][l], ñores[f][l][h], bios[f][h], Step);

						}
					}


				}

				

			}





			t += 1;
			if (t == PacketSet) {
				StrartGradientCore(PacketSet, SpeedTeach);
				
				t = 0;
			}
			NullBatchSet();
			
		}
		



		
		cout << "epoch [" << i+1 << "]: ";
		DirectCnnBase(GridData);
	}
	delete[] VectorErr;
	delete[] VectorOut;
}

void CNN::SweepBatches(Batch* Grid, int SizeGridX, int SizeGridY) {
	
	//For optimization, you don't need to make an additional matrix for maxspooling if val=1



	for (int i = 0; i < chanels; i++) {
		for (int l = 0; l < valCore[0]; l++) {
			TasksNetwork::DirectBatches(Grid[i], ñores[0][i][l], batñh[0][l],Step);
		
		}
	
	}

	

	TasksNetwork::actChann(batñh[0],bios[0], valCore[0]);
	
	




	
	for (int l = 0; l < valCore[0]; l++) {
		TasksNetwork::MaxPooling(batñh[0][l], PollingVal[0], batñhMax[0][l]);

	}
	


	for (int i = 1; i < sloys; i++) {
	
			 for (int l = 0; l < valCore[i - 1]; l++) {
				 for (int j = 0; j < valCore[i]; j++) {

					 TasksNetwork::DirectBatches(batñhMax[i - 1][l], ñores[i][l][j], batñh[i][j], Step);


					 


				 }


				 TasksNetwork::actChann(batñh[i],bios[i], valCore[i]);
			for (int j = 0; j < valCore[i]; j++){
				TasksNetwork::MaxPooling(batñh[i][j], PollingVal[i], batñhMax[i][j]);
			 }


			}

		
		

	
	
	}

}
void CNN::StrartGradientCore(int PacketVal, double SpeedTeach) {
	
	
	for (int i = 0; i < sloys; i++) {
		if (i == 0) {
			for (int g = 0; g < chanels; g++) {
				for (int j = 0; j < valCore[0]; j++) {
					for (int y = 0; y < ñores[0][g][j].sizeY; y++) {
						for (int x = 0; x < ñores[0][g][j].sizeX; x++) {
							ñores[i][g][j].Core[y][x] = ñores[i][g][j].Core[y][x] - ñores[i][g][j].CoreErr[y][x]/ (double)PacketVal* SpeedTeach;
						
						
						}
					}
					
				}
			}
		
		}
		else {
			for (int l = 0; l < valCore[i - 1]; l++) {

				for (int j = 0; j < valCore[i]; j++) {
					for (int y = 0; y < ñores[i][l][j].sizeY; y++) {
						for (int x = 0; x < ñores[i][l][j].sizeX; x++) {
							ñores[i][l][j].Core[y][x] = ñores[i][l][j].Core[y][x] - ñores[i][l][j].CoreErr[y][x] / (double)PacketVal* SpeedTeach;
							
						}
					}
					
				}
			}
			
		}
		
	}


	for (int i = 0; i < sloys; i++) {
		if (i == 0) {
			for (int g = 0; g < chanels; g++) {
				for (int j = 0; j < valCore[0]; j++) {
					
							ñores[i][g][j].NullErrCore();
						
					
				}
			}
		
		}
		else {
			for (int l = 0; l < valCore[i - 1]; l++) {

				for (int j = 0; j < valCore[i]; j++) {
					
						
							ñores[i][l][j].NullErrCore();
					

				}
			}
			
		}
		
	}
	

	
	
	
	for (int j = 0; j < valCore[0]; j++) {
		for (int y = 0; y < bios[0][j].sizeY; y++) {
			for (int x = 0; x < bios[0][j].sizeX; x++) {
				bios[0][j].bios[y][x] -= bios[0][j].biosErrPacket[y][x] * SpeedTeach / (double)PacketVal;
				
			}
		}
	}



	for (int i = 1; i < sloys; i++) {


		for (int l = 0; l <  valCore[i]; l++) {



			for (int y = 0; y < bios[i][l].sizeY; y++) {
				for (int x = 0; x < bios[i][l].sizeX; x++) {
					bios[i][l].bios[y][x] -= bios[i][l].biosErrPacket[y][x] * SpeedTeach / (double)PacketVal;
				}
			}
			



		}
		







	}




	for (int j = 0; j < valCore[0]; j++) {
		
		bios[0][j].SetNullErr();
			
		
	}



	for (int i = 1; i < sloys; i++) {


		for (int l = 0; l < valCore[i]; l++) {



			
					bios[i][l].SetNullErr();
				
			




		}







	}


	
}


void CNN::StartDirectCNN(Batch* Grid, void (*set_function)(double* SetOutput), int VallSloy, const int ArrSizeSloy[], const ActFuns ActFunc[]) {
	if (SetInit) {
		if (!InitBatches(Grid[0].sizeX, Grid[0].sizeY)) {
			return;
		}
		else {
			SetInit = true;
			localPepzetron = new Tensor(VallSloy, ArrSizeSloy, ActFunc);
		}

	}
	int SizeVectOut = ValBat * batñhMax[sloys - 1][0].sizeY * batñhMax[sloys - 1][0].sizeX;
	double* VectorOut = new double[SizeVectOut];
	SweepBatches(Grid, Grid[0].sizeX, Grid[0].sizeY);
	VectorSweep(batñhMax[sloys - 1], ValBat, VectorOut);
	localPepzetron->StartDirectSession(VectorOut,set_function);
	NullBatchSet();
	delete[]  VectorOut;
}

void CNN::DirectCnnBase(DataCNN& GridData) {
	int SizeVectOut = ValBat * batñhMax[sloys - 1][0].sizeY * batñhMax[sloys - 1][0].sizeX;
	double* VectorOut = new double[SizeVectOut];
	double* NeuronOut = new double[SizeOutNeurons];
	double Val = 0;
	for (int j = 0; j < GridData.SizeData; j++) {
		SweepBatches(GridData.Grid[j], GridData.Grid[j][0].sizeX, GridData.Grid[j][0].sizeY);
		VectorSweep(batñhMax[sloys - 1], ValBat, VectorOut);
		localPepzetron->StartDirectSessionCNNBase(VectorOut,  Val, GridData.CorrectVal[j]);
		NullBatchSet();
		
	}
	cout << Val / GridData.SizeData*100<<"%"<<endl;
	
	delete[]  VectorOut;
	delete[] NeuronOut;
}
int CNN::InitBatches(int SizeGridX, int SizeGridY) {

	for (int i = 0; i < sloys; i++) {
		if (PollingVal[i] <= 0) {
			cout << "\nMaxPooling cannot be negative or zero\n";
			return 0;
		}
	}
	//Compatibility check can be checked for 1 batch of 1 core, it is also possible to check for 1 channel only, since in all channels the cores are the same size
	///Initialization of Batches
	if (((SizeGridX - (ñores[0][0][0].sizeX - 1)) <= 0) || ((SizeGridY - (ñores[0][0][0].sizeY - 1)) <= 0)) {
		std::cout << "\n Size Yadro bigger Batch \n";
		return 0;
	}
	int ost = 0;
	if ((((SizeGridX - (ñores[0][0][0].sizeX - 1)) - 1) % Step) || (((SizeGridY - (ñores[0][0][0].sizeY - 1)) - 1) % Step)) {
	
		ost = Step - (((SizeGridX - (ñores[0][0][0].sizeX - 1)) - 1) % Step);

	}


	
	int VALX = ( (((SizeGridX - (ñores[0][0][0].sizeX - 1)) - 1)+ ost) / Step + 1);
	int VALY = ( (((SizeGridY - (ñores[0][0][0].sizeY - 1)) - 1) + ost) / Step + 1);
	for (int j = 0; j < valCore[0]; j++) {
		batñh[0][j](VALY, VALX,0,1);
		bios[0][j](VALY, VALX,true);
		


	if(VALX % PollingVal[0] && VALY % PollingVal[0]) {
		batñhMax[0][j](VALY / PollingVal[0] + 1, VALX / PollingVal[0] + 1,1,1);
		
	}
	else if (VALY % PollingVal[0]) {
		batñhMax[0][j](VALY / PollingVal[0]+1, VALX / PollingVal[0],1,1);
		
	}
	else if (VALX % PollingVal[0]) {
		batñhMax[0][j](VALY / PollingVal[0], VALX / PollingVal[0]+1,1,1);
	}
	else {
		batñhMax[0][j](VALY / PollingVal[0], VALX / PollingVal[0],1,1);
	}

		
	}

	if (VALX % PollingVal[0] && VALY % PollingVal[0]) {
		VALX = VALX / PollingVal[0] + 1;
		VALY = VALY / PollingVal[0] + 1;
	}
	else if (VALY % PollingVal[0]) {
		VALX = VALX / PollingVal[0];
		VALY = VALY / PollingVal[0] + 1;
	}
	else if (VALX % PollingVal[0]) {
		VALX = VALX / PollingVal[0] + 1;
		VALY = VALY / PollingVal[0];

	}
	else {
		VALX = VALX / PollingVal[0];
		VALY = VALY / PollingVal[0];
	}



	//Compatibility check can be checked for 1 batch of 1 core
	int valX = VALX;
	int valY = VALY;

	

	for (int i = 1; i < sloys; i++) {
	
			
			ost = 0;
			if (((valX - (ñores[i][0][0].sizeX - 1)) <= 0) || ((valY - (ñores[i][0][0].sizeY - 1)) <= 0)) {
				std::cout << "\n Size Yadro bigger Batch \n";
				return 0;
			}
			if ((((valX - (ñores[i][0][0].sizeX - 1)) - 1) % Step) || (((valY - (ñores[i][0][0].sizeY - 1)) - 1) % Step)) {
			
				ost = Step - (((SizeGridX - (ñores[0][0][0].sizeX - 1)) - 1) % Step);
			
			}


			valX = ((((valX - (ñores[i][0][0].sizeX - 1)) - 1)+ ost) / Step + 1);
			valY = ((((valY - (ñores[i][0][0].sizeY - 1)) - 1)+ost) / Step + 1);

			
			
			

			for (int l = 0; l <valCore[i]; l++) {

				


				batñh[i][l](valY, valX,0,1);
				bios[i][l](valY, valX, true);
				
				if (valX % PollingVal[i]&& valY % PollingVal[i]) {
					batñhMax[i][l](valY  / PollingVal[i]+1, valX / PollingVal[i]+1,1,1);
					
				}
				else if (valY % PollingVal[i]) {
					batñhMax[i][l](valY / PollingVal[i] + 1, valX / PollingVal[i],1,1);
					
				}
				else if (valX % PollingVal[i]) {
					batñhMax[i][l](valY / PollingVal[i], valX / PollingVal[i] + 1,1,1);
					
				}
				else {
					batñhMax[i][l](valY / PollingVal[i], valX / PollingVal[i],1,1);
					
				}

			



			}

			if (valX % PollingVal[i] && valY % PollingVal[i]) {
				valX = valX / PollingVal[i] + 1;
				valY= valY / PollingVal[i] + 1;
			
			}
			else if (valY % PollingVal[i]) {
				valX = valX / PollingVal[i];
				valY = valY / PollingVal[i] + 1;
				
			}
			else if (valX % PollingVal[i]) {
				valX = valX / PollingVal[i]+1;
				valY = valY / PollingVal[i];
				

			}
			else {
				valX = valX / PollingVal[i];
				valY = valY / PollingVal[i];
				
			}


			
		
			


	}


	return 1;
	//(SizeGridX-(SizeCore[0][i][1]-1))-1%Step) if 0 -> good else bad

}

void CNN::VectorSweep(Batch* Batch,int valBat, double* VectorOutput) {
	int Y = Batch[0].sizeY;
	int X = Batch[0].sizeX;
	int k = 0;
	for (int l = 0; l < valBat; l++) {
		for (int j = 0; j < Y; j++) {
			for (int g = 0; g < X; g++) {
				VectorOutput[k] = Batch[l].batch[j][g];
				k += 1;
			}
		}
	}
	
}
void CNN::VectorErrToMatrix(double* VectorErr, Batch* Batch, int valBat) {
	int Y = Batch[0].sizeY;
	int X = Batch[0].sizeX;
	int k = 0;
	for (int l = 0; l < valBat; l++) {
		for (int j = 0; j < Y; j++) {
			for (int g = 0; g < X; g++) {
				Batch[l].batchErr[j][g]= VectorErr[k];
				
				k += 1;
			}
		}
	}
	

}
void CNN::NullBatchSet() {
	
	for (int i = 0; i < sloys; i++){
		for (int l = 0; l < valCore[i]; l++) {
			batñh[i][l].SetNull();
			batñhMax[i][l].SetNull();
		}
		
}

}

void Tsr::TestCNNModel(int chanels, int  valCore[], int sloys, int*** SizeCore, int Step, int SizeGridX, int SizeGridY) {
	
}
void CNN::InputMat(Batch& MatBatch,int MaxSet) {
	for (int i = 0; i < MatBatch.sizeY; i++) {
		for (int l = 0; l < MatBatch.sizeX; l++) {
			cout<<" "<<MatBatch.batch[i][l];
		}
		cout << "\n";
	}
	if (MaxSet) {
		cout << endl;
		cout << "Positions: " << endl;
		for (int i = 0; i < MatBatch.sizeY; i++) {
			for (int l = 0; l < MatBatch.sizeX; l++) {
				cout << "(" << MatBatch.PositioningMax[i][l][0] << "," << MatBatch.PositioningMax[i][l][1] << ") ";
			}
			cout << "\n";
		}
	}
}