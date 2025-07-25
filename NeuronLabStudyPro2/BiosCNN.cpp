#include "BiosCNN.h"
#include<iostream>
void BiosCNN::operator () (int y, int x, bool setErr) {
	sizeY = y;
	sizeX = x;
	bios = new double* [y];
	for (int i = 0; i < y; i++) {
		bios[i] = new double[x];
	}
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			bios[i][j]= (rand() % 50) * 0.03 / 15.0;
		}
	}
	if (setErr) {
		biosErrPacket = new double* [y];
		for (int i = 0; i < y; i++) {
			biosErrPacket[i] = new double[x];
		}
		for (int i = 0; i < y; i++) {
			for (int j = 0; j < x; j++) {
				biosErrPacket[i][j] = 0;
			}
		}
	}
}
void  BiosCNN::SetNullErr() {
	for (int i = 0; i < sizeY; i++) {
		for (int j = 0; j < sizeX; j++) {
			biosErrPacket[i][j] = 0;
		}
	}
}
BiosCNN::~BiosCNN() {
	for (int y = 0; y < sizeY; y++)
		delete[] bios[y];

	delete[] bios;
	if (biosErrPacket != 0) {
		for (int y = 0; y < sizeY; y++)
			delete[] biosErrPacket[y];

		delete[] biosErrPacket;
	}
}