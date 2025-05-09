#pragma once
#include "WheightClass.h"
#include "BiosClass.h"
#include "NeuronClass.h"
#include "Function.h"
#include"TasksNetwork.h"
#include"DataNeuron.h"
#include"DataHyperParametr.h"
#include<iostream>
#include<fstream>
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
		void StartTrainingSet(double* VectorRight, ErrFuns erF, OptimizaterGradient Optimizator);

		void StartGradient(int PacketSet, double SpeedTeach);
		void StartGradient(int PacketSet, double SpeedTeach, Regulizators regulizator, int SizeObservations);
		bool SetCorrectVal(double* SetCorrect, double& val);

	public:
	

		Tensor(int VallSloy, const int ArrSizeSloy[], const ActFuns ActFunc[]); //Standart Pepzetron;
		void SaveParametsNeurons(string pathW, string pathB);
		void LoadParametsNeurons(string pathW, string pathB);
		void StartTeachSession(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch);
		void StartTeachSession(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch, OptimizaterGradient Optimizator, Regulizators regulizator);
		void StartTeachSession(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch, OptimizaterGradient Optimizator, Regulizators regulizator, bool StochasticSpeed);
		void StartDirectSession(DataNeuron& Data);

		~Tensor();

	};
	

