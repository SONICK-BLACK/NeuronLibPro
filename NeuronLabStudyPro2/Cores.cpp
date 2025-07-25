#include "Cores.h"
#include<iostream>
void Cores::operator ()  (int sizeY, int sizeX) {
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	Core = new double* [sizeY];
	CoreErr = new double* [sizeY];
	for (int i = 0; i < sizeY; i++) {
		Core[i] = new double [sizeX];
		CoreErr[i] = new double[sizeX];

	}
	for (int i = 0; i < sizeY; i++) {
		for (int j = 0; j < sizeX; j++) {
			Core[i][j]= (rand() % 100) * 0.03 / 35.0;
			
			CoreErr[i][j] = 0;
		}
	}
	biosCore = (rand() % 100) * 0.001;
}
void Cores::NullErrCore() {
	for (int i = 0; i < sizeY; i++) {
		for (int j = 0; j < sizeX; j++) {
			CoreErr[i][j] = 0;
		}
	}
	biosCoreErr = 0;
}

Cores:: ~Cores() {
	for (int i = 0; i < sizeY; i++) {
		delete[] Core[i];
		delete[] CoreErr[i];
	}
	delete[] Core;
	delete[] CoreErr;
}
