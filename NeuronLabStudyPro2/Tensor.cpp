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



