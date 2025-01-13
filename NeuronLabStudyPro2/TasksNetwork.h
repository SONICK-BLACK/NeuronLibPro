#pragma once
#include "WheightClass.h"
#include "BiosClass.h"
#include "NeuronClass.h"
#include "Function.h"
#include "DataHyperParametr.h"
class TasksNetwork
{
public:
	
	
	static void DirectDirection(NeuronClass& Neuron1, NeuronClass& Neuron2,WheightClass& Wheight, BiosClass& Bios, ActFuns& funs);
	static void  DirectDirectionBase(NeuronClass& Neuron1, NeuronClass& Neuron2, WheightClass& Wheight, BiosClass& Bios, ActFuns& funs);
	static void EvalutionError(double* VectorRight, NeuronClass& NeuronEnd, ErrFuns errFuns);
	static void EvalutionErrorLast(NeuronClass& Neuron2, NeuronClass& Neuron, WheightClass& Wheight, ActFuns& funs);
	static void ErrorTeachSloySet(NeuronClass& Neuron2, NeuronClass& Neuron, WheightClass& Wheight, BiosClass& Bios, ActFuns& funs);
	
	static void ErrorTeachSloySet(NeuronClass& Neuron2, NeuronClass& Neuron, WheightClass& Wheight, BiosClass& Bios, ActFuns& funs, OptimizaterGradient Optimizator);
};

