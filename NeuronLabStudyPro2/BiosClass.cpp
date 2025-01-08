#include "BiosClass.h"
#include<iostream>
void BiosClass::InitBiosClass(int SizeBios, int SizeNeuron) {
	Bios = new double[SizeBios];
	SizeMatrix = SizeBios;
	for (int i = 0; i < SizeMatrix; i++) {
		Bios[i] = (rand() % 50) * 0.03 / (SizeNeuron + 15);
	}
}
void BiosClass::InitErrBios() {
	SetErrorBiosPacket = new double[SizeMatrix];
	for (int i = 0; i < SizeMatrix; i++) {
		SetErrorBiosPacket[i] = 0;
	}

}
void BiosClass::BiosErrSetNull() {
	for (int i = 0; i < SizeMatrix; i++) {
		SetErrorBiosPacket[i] = 0;
	}

}
BiosClass::~BiosClass() {
	delete[] Bios;
	if (SetErrorBiosPacket != 0) {
		delete[] SetErrorBiosPacket;
	}
}