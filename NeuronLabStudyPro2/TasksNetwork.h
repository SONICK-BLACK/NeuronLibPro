#pragma once
#include "WheightClass.h"
#include "BiosClass.h"
#include "NeuronClass.h"
#include"Cores.h"
#include "BiosCNN.h"
#include"Batch.h"
#include "Function.h"
#include "DataHyperParametr.h"
class TasksNetwork
{
private:

	static void Sweep(double** Grid, double** Yadro, int step, int SizeGridY, int SizeGridX, int SizeYadroY, int SizeYadroX, double** Sweeper);
	static void PaddingX(Batch& batch, int val,Batch& batchPad);
	static void PaddingY(Batch& batch, int val, Batch& batchPad);
	static void PaddingXY(Batch& batch, int valX, int valY, Batch& batchPad);

	
public:
	static void EvalutionErrMaxToBatch(Batch& batchMax, Batch& batch);
	static void actChann(Batch* batchChan, BiosCNN* bios, int valChan);
	static void EvalutionErrBatchToMax(Batch& batch, Batch& batchMax, Cores& core, int step);
	static void ErrorTeachCores(Batch& batch, Batch& batchMax, Cores& core, BiosCNN& bios, int step);
	static void MaxPooling(Batch& batch, int val, Batch& batchMax);
	static void DirectDirection(NeuronClass& Neuron1, NeuronClass& Neuron2,WheightClass& Wheight, BiosClass& Bios, ActFuns& funs);
	static void  DirectDirectionBase(NeuronClass& Neuron1, NeuronClass& Neuron2, WheightClass& Wheight, BiosClass& Bios, ActFuns& funs);
	static void EvalutionError(double* VectorRight, NeuronClass& NeuronEnd, ErrFuns errFuns);
	static void EvalutionErrorLast(NeuronClass& Neuron2, NeuronClass& Neuron, WheightClass& Wheight, ActFuns& funs);
	static void ErrorTeachSloySet(NeuronClass& Neuron2, NeuronClass& Neuron, WheightClass& Wheight, BiosClass& Bios, ActFuns& funs);
	
	static void ErrorTeachSloySet(NeuronClass& Neuron2, NeuronClass& Neuron, WheightClass& Wheight, BiosClass& Bios, ActFuns& funs, OptimizaterGradient Optimizator);

	static void DirectBatches(Batch& batch1, Cores& core, Batch& batch2, int step);
};

