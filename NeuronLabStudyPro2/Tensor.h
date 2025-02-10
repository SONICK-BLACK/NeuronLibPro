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
using namespace Data;



namespace Tsr {
	class Tensor
	{
	private:
		WheightClass* MatrixWheight;
		BiosClass* MatrixBios;
		NeuronClass* MatrixNeuron;
		ActFuns* act;
		int SizeValSloy;
		void StartDirect();
		void StartDirectBase();
		void LoadData(double* Data);
		void StartTrainingSet(double* VectorRight, ErrFuns erF);
		void StartTrainingSet(double* VectorRight, ErrFuns erF, OptimizaterGradient Optimizator);

		void StartGradient(int PacketSet, double SpeedTeach);
		void StartGradient(int PacketSet, double SpeedTeach, Regulizators regulizator, int SizeObservations);
		bool SetCorrectVal(double* SetCorrect);
		void SetCorrectVal(double* SetCorrect, double& val);

	public:


		Tensor(int VallSloy, const int ArrSizeSloy[], const ActFuns ActFunc[]); //Standart Pepzetron;
		void SaveParametsNeurons(string pathW = "wheight.txt", string pathB = "bios.txt");
		void LoadParametsNeurons(string pathW = "wheight.txt", string pathB = "bios.txt");
		void StartTeachSession(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch);
		void StartTeachSession(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch, OptimizaterGradient Optimizator, Regulizators regulizator);
		void StartTeachSession(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch, OptimizaterGradient Optimizator, Regulizators regulizator, bool StochasticSpeed);
		void StartDirectSession(DataNeuron& Data, void (*set_function)(double* setNeuron));
		void StartDirectSession(DataNeuron& Data, void (*set_function)(double* setNeuron, double* SetCorectVal));

		~Tensor();

	};
	class RegressionModel {
	private:
		double** TransportMat(double** mat, int col, int str);
		double def_SetMatrixAlgibration(double** a, int Size, int i, int j);
		double def_determination(double** M, int size);
		double def_DeterminateTwoMat(double** M2);
		double** MultiMatrix(double** M1, double** M2, int str1, int col1, int col2, int str2);
	public:
		void SetWheightsBParamets(DataRegression& dataReg);
		void SetRcriteria(DataRegression& dataReg);
	

	};
}
