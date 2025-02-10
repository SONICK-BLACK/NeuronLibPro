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
	///Verno
	for (int i = 1; i < SizeValSloy; i++) {
		TasksNetwork::DirectDirection(MatrixNeuron[i - 1], MatrixNeuron[i], MatrixWheight[i - 1], MatrixBios[i - 1], act[i - 1]);
	}
}
void Tensor::StartDirectBase() {
	///Verno
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