#pragma once
#include "WheightClass.h"
#include "BiosClass.h"
#include "NeuronClass.h"
#include "Function.h"
#include"TasksNetwork.h"
#include"DataNeuron.h"
#include<iostream>
using namespace std;

	class Tensor
	{
	private:
		WheightClass* MatrixWheight;
		BiosClass* MatrixBios;
		NeuronClass* MatrixNeuron;
		ActFuns* act;
		int SizeValSloy;
		void StartDirect();
		void LoadData(double* Data);
		void StartTrainingSet(double* VectorRight, ErrFuns erF);
		void StartGradient(int PacketSet, double SpeedTeach);
		bool SetCorrectVal(double* SetCorrect, double& val);

	public:

		Tensor(int VallSloy, const int ArrSizeSloy[], const ActFuns ActFunc[]); //Standart Pepzetron;

		void StartTeachSession(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch);
		~Tensor();

	};
	

