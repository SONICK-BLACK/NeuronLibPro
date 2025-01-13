#include "Tensor.h"

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
	double Val = 0;
	for (int h = 0; h < epoch; h++) {
		for (int j = 0; j < Data.SizeData; j++) {

			LoadData(Data.SetData[j]);
			StartDirect();
			/////.....
	//	cout << "ttttttttt "<<endl;
		//		for (int u = 0; u < 10; u++) {
				//	cout << MatrixNeuron[SizeValSloy - 1].Neuron[u] << "  ";
		//	}
			
		//	cout << " ttttttttt " << endl;
				//////////


				if (!SetCorrectVal(Data.CorrectVal[j], Val)) {

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
		cout << endl;
		cout << "Epoch (" << h << "): " << (Val / Data.SizeData) * 100 << "%" << "\n";
		

		Val = 0;
	}
	cout << endl;
	cout << "End Teach Tensor\n";
}
void Tensor::StartTeachSession(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch, OptimizaterGradient Optimizator, Regulizators regulizator) {
	for (int i = 0; i < SizeValSloy; i++) {
		MatrixNeuron[i].InitNeuronClassErr();

	}
	for (int i = 0; i < SizeValSloy - 1; i++) {
		MatrixWheight[i].InitWheightErr();
		MatrixBios[i].InitErrBios();
	}
	int SetFlagPacket = 0;
	double Val = 0;
	for (int h = 0; h < epoch; h++) {
		for (int j = 0; j < Data.SizeData; j++) {

			LoadData(Data.SetData[j]);
			StartDirect();
			/////.....
	//	cout << "ttttttttt "<<endl;
		//		for (int u = 0; u < 10; u++) {
				//	cout << MatrixNeuron[SizeValSloy - 1].Neuron[u] << "  ";
		//	}

		//	cout << " ttttttttt " << endl;
				//////////


			if (!SetCorrectVal(Data.CorrectVal[j], Val)) {

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
		cout << endl;
		cout << "Epoch (" << h << "): " << (Val / Data.SizeData) * 100 << "%" << "\n";


		Val = 0;
	}
	cout << endl;
	cout << "End Teach Tensor\n";
}
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
void Tensor::StartTeachSession(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch, OptimizaterGradient Optimizator, Regulizators regulizator, bool StochasticSpeed) {
	for (int i = 0; i < SizeValSloy; i++) {
		MatrixNeuron[i].InitNeuronClassErr();

	}
	for (int i = 0; i < SizeValSloy - 1; i++) {
		MatrixWheight[i].InitWheightErr();
		MatrixBios[i].InitErrBios();
	}
	int SetFlagPacket = 0;
	double Val = 0;
	for (int h = 0; h < epoch; h++) {
		for (int j = 0; j < Data.SizeData; j++) {

			LoadData(Data.SetData[j]);
			StartDirect();
			/////.....
	//	cout << "ttttttttt "<<endl;
		//		for (int u = 0; u < 10; u++) {
				//	cout << MatrixNeuron[SizeValSloy - 1].Neuron[u] << "  ";
		//	}

		//	cout << " ttttttttt " << endl;
				//////////


			if (!SetCorrectVal(Data.CorrectVal[j], Val)) {

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
		cout << endl;
		cout << "Epoch (" << h << "): " << (Val / Data.SizeData) * 100 << "%" << "\n";


		Val = 0;
	}
	cout << endl;
	cout << "End Teach Tensor\n";
}
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
	outB.open(pathW);
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
bool Tensor::SetCorrectVal(double* SetCorrect, double& val) {



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
		val += 1;
		return true;

	}
	else {
		return false;
	}
}
Tensor::~Tensor() {
	delete[] act;
	delete[] MatrixBios;
	delete[] MatrixWheight;
	delete[] MatrixNeuron;

}



