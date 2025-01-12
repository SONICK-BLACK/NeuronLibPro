#pragma once
#include<iostream>
enum OptimizaterGradient { NullO = 0, Momentum=1, rmsprop, Adam};
enum Regulizators { NullR = 0, L1=1, L2, ElasticNet};
enum ActFuns {
	Sigmoida = 1, ReLU = 2, Softmax = 3, HypTang
};
enum ErrFuns
{
	MSR = 1, LogLoss = 2
};
class Function
{
public:
	static void FunctionUse(ActFuns set, double& value, double MidValue = 0);
	static double FunctionUseDer(ActFuns set, double value, double MidValue = 0);
	static double FunctionUseErDer(ErrFuns set, double valueEr, double valueCor);

};

