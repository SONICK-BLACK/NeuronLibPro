#include "Function.h"
#include "Function.h"

void Function::FunctionUse(ActFuns set, double& value, double MidValue) {

	switch (set)
	{
	case Sigmoida:
		value = 1 / (1 + exp(-value));
		break;
	case ReLU:
		if (value < 0)
			value *= 0.01;
		else if (value > 1)
			value = 1. + 0.01 * (value - 1);
		break;
	case Softmax:
		if (MidValue == NULL) {
			std::cout << "Mid is NULL\n";
		}
		else {
			value = exp(value) / MidValue;
		}


		break;
	default:
		break;
	}
}
double Function::FunctionUseDer(ActFuns set, double value, double MidValue) {
	switch (set)
	{
	case Sigmoida:
		if (value < 1e-7) {
			value = 0; //Quite optimal constraints for this activation function
		}
		if (value > 1e+6) {
			value = 1e+6;
		}
		return exp(-value) / pow((1 + exp(-value)), 2);

		break;
	case ReLU:
		if (value < 0 || value>1)
			return 0.01;
		else
			return 1;
		break;
	case Softmax:
		if (MidValue == 0) {
			std::cout << "Mid is NULL\n";
		}
		else {

			return ((exp(value) * MidValue - pow(exp(value), 2)) / pow(MidValue, 2));
		}
		break;
	default:
		break;
	}
}
double Function::FunctionUseErDer(ErrFuns set, double valueEr, double valueCor) {
	switch (set) {
	case MSR:
		return (valueEr - valueCor);
		break;
	case LogLoss:
		return -((valueCor * (1 - valueEr) + valueEr * (1 - valueCor)) / (valueEr * (1 - valueEr)));

		break;
	default:
		break;


	}
}