#pragma once
#include<iostream>
enum ActFuns {
	Sigmoida = 1, ReLU = 2, Softmax = 3
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

