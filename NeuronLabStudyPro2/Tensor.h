#pragma once
#include "WheightClass.h"
#include "BiosClass.h"
#include "NeuronClass.h"
#include "Function.h"
#include"TasksNetwork.h"
#include"DataNeuron.h"
#include"Cores.h"
#include"Batch.h"
#include"DataHyperParametr.h"
#include<iostream>
#include<fstream>
using namespace std;
using namespace Data;



namespace Tsr {




	class Tensor
	{
		friend class CNN;
	private:
		WheightClass* MatrixWheight;
		BiosClass* MatrixBios;
		NeuronClass* MatrixNeuron;
		ActFuns* act;
		int SizeValSloy;
		void StartDirectSessionCNNBase(double* Data, double& val, double* Correctval);
		void StartDirect();
		void StartDirectBase();
		void LoadData(double* Data);
		void StartTrainingSet(double* VectorRight, ErrFuns erF);
		void StartTrainingSet(double* VectorRight, ErrFuns erF, OptimizaterGradient Optimizator);
		
		void StartGradient(int PacketSet, double SpeedTeach);
		void StartGradient(int PacketSet, double SpeedTeach, Regulizators regulizator, int SizeObservations);
		bool SetCorrectVal(double* SetCorrect);
		void SetCorrectVal(double* SetCorrect, double& val);

		bool StartTeachSessionÑNN(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch, double* VectorErr, int t);
		bool StartTeachSessionÑNN(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch, OptimizaterGradient Optimizator, Regulizators regulizator, double* VectorErr, int t, int sizeData);
		void StartTrainingSetCNN(double* VectorRight, ErrFuns erF, double* ErrVector);
		void StartTrainingSetCNN(double* VectorRight, ErrFuns erF, double* ErrVector, OptimizaterGradient Optimizator);
		void InitClassesErr();
	public:


		Tensor(int VallSloy, const int ArrSizeSloy[], const ActFuns ActFunc[]); //Standart Pepzetron;
		void SaveParametsNeurons(string pathW = "wheight.txt", string pathB = "ofset.txt");
		void LoadParametsNeurons(string pathW = "wheight.txt", string pathB = "ofset.txt");

		void StartTeachSession(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch);
		void StartTeachSession(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch, OptimizaterGradient Optimizator, Regulizators regulizator);
		void StartTeachSession(double SpeedTeach, int PacketSet, DataNeuron& Data, ErrFuns FunErr, int epoch, OptimizaterGradient Optimizator, Regulizators regulizator, bool StochasticSpeed);
		void StartDirectSession(DataNeuron& Data, void (*set_function)(double* setNeuron));
		void StartDirectSession(DataNeuron& Data, void (*set_function)(double* setNeuron, double* SetCorectVal));
		void StartDirectSession(double* Data, void (*set_function)(double* setNeuron));
		~Tensor();

	};

	class CNN {

	private:
		int SizeOutNeurons;
		Tensor* localPepzetron;
		BiosCNN** bios;
		Cores*** ñores;//Cores in layers
		bool SetInitDirect = false;
		bool SetInitTeach = false;
		int ValBat; //number of batches in the output convolutional layer
		Batch** batñh;//Many Batches are there in total
		Batch** batñhMax;// Many Batches are there in total
		int chanels; //Many color channels
		int* valCore;//cores for 1 batch in each layer
		int sloys;//many layers
		int Step;//The scan step
		void InputMat(Batch& MatBatch, int MaxSet);
		void VectorSweep(Batch* Grid, int valBat, double* VectorOutput);
		void SweepBatches(Batch* Grid, int SizeGridX, int SizeGridY);
		void VectorErrToMatrix(double* VectorErr, Batch* Batch, int valBat);
		int InitBatches(int SizeGridX, int SizeGridY);
		void StrartGradientCore(int PacketVal, double SpeedTeach);
		void StrartGradientCore(int PacketVal, double SpeedTeach, Regulizators regulizator, int SizeObservations);
		void NullBatchSet();
		void DirectCnnBase(DataCNN& GridData);
	public:
		//We make the same size of cores for the entire layer.

		int* PollingVal;
		CNN(int chanels, int  valCore[], int sloys, int** SizeCore, int Step);
		void StartTrainingCNN(DataCNN& GridData, double SpeedTeach, int PacketSet, ErrFuns FunErr, int epoch);
		void StartTrainingCNN(DataCNN& GridData, double SpeedTeach, int PacketSet, ErrFuns FunErr, int epoch, OptimizaterGradient Optimizator, Regulizators regulizator);
		void StartTrainingCNN(DataCNN& GridData,  double SpeedTeach, int PacketSet, ErrFuns FunErr, int epoch, OptimizaterGradient Optimizator, Regulizators regulizator, bool StochasticSpeed);
		void StartDirectCNN(Batch* Grid, void (*set_function)(double* SetOutput), int VallSloy, const int ArrSizeSloy[], const ActFuns ActFunc[]);
		void SaveParamentsCNN(string pathCore = "Cores.txt", string pathBios = "Bios.txt");
		void LoadParamentsCNN(string pathCore = "Cores.txt", string pathBios = "Bios.txt");
		void InitPepzetronCNNForTeach(int VallSloy, int ArrSizeSloy[], const ActFuns ActFunc[], int InputSizeGridX, int InputSizeGridY);
		void InitPepzetronCNNForDirect(int VallSloy, int ArrSizeSloy[], const ActFuns ActFunc[], int InputSizeGridX, int InputSizeGridY);
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

	void TestCNNModel(int chanels, int  valCore[], int sloys, int*** SizeCore, int Step, int SizeGridX, int SizeGridY);

	
}
