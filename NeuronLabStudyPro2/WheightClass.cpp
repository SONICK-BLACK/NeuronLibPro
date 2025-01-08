#include "WheightClass.h"
#include<iostream>
void WheightClass::InitWheightClass(int SizeWheight, int SizeNeuron) {
	Wheight = new double[SizeWheight];
	sizeMatrix = SizeWheight;
	for (int i = 0; i < sizeMatrix; i++) {
		Wheight[i] = (rand() % 100) * 0.03 / (SizeNeuron + 35);
	}

}
void WheightClass::InitWheightErr() {
	SetErrorWhPacket = new double[sizeMatrix];
	for (int i = 0; i < sizeMatrix; i++) {
		SetErrorWhPacket[i] = 0;
	}
}
void WheightClass::WheightErrSetNull() {
	for (int i = 0; i < sizeMatrix; i++) {
		SetErrorWhPacket[i] = 0;
	}
}
WheightClass::~WheightClass() {
	delete[] Wheight;
	if (SetErrorWhPacket != 0) {
		delete[] SetErrorWhPacket;

	}
}
