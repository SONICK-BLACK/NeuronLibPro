#include "Batch.h"
#include<iostream>
void Batch::operator ()  (int sizeY, int sizeX, int SetMax, int setErr) {
	this->sizeX = sizeX;
	this->sizeY = sizeY;

	batch = new double * [sizeY];
	batchNoAct = new double* [sizeY];
	for (int i = 0; i < sizeY; i++) {
		batch[i] = new double[sizeX];
		batchNoAct[i] = new double[sizeX];
	}

	for (int i = 0; i < sizeY; i++) {
		for (int j = 0; j < sizeX; j++) {
			batch[i][j] = 0;
			batchNoAct[i][j] = 0;
		}
	}
	if (SetMax) {
		PositioningMax = new int** [sizeY];
		for (int i = 0; i < sizeY; i++) {
			PositioningMax[i] = new int*[sizeX];
			for (int j = 0; j < sizeX; j++) {
				PositioningMax[i][j] = new int[2];
			}
		}
		

		
	}
	if (setErr) {
		batchErr = new double* [sizeY];
		for (int i = 0; i < sizeY; i++) {
			batchErr[i] = new double[sizeX];
		}
		for (int i = 0; i < sizeY; i++) {
			for (int j = 0; j < sizeX; j++) {
				batchErr[i][j] = 0;
			}
		}
	}

	
	

	

}

void Batch::SetNull() {
	for (int i = 0; i < sizeY; i++) {
		for (int j = 0; j < sizeX; j++) {
			batch[i][j] = 0;
			batchNoAct[i][j] = 0;
		}
	}
	if (batchErr != 0) {
		for (int i = 0; i < sizeY; i++) {
			for (int j = 0; j < sizeX; j++) {
				batchErr[i][j] = 0;
			}
		}
	}
	
	
}

void Batch::SetNullErr() {
	for (int i = 0; i < sizeY; i++) {
		for (int j = 0; j < sizeX; j++) {
			batchErr[i][j] = 0;
		}
	}

}

Batch:: ~Batch() {
	for (int i = 0; i < sizeY; i++) {
		delete[] batch[i];
}
	delete[] batch;
	if (PositioningMax != 0) {
		for (int i = 0; i < sizeY; i++) {
			for (int j = 0; j < sizeX; j++) {
				delete[] PositioningMax[i][j];
			}
			delete[] PositioningMax[i];
		}
		delete[] PositioningMax;
	}
	if (batchErr != 0) {
		for (int i = 0; i < sizeY; i++) {
			delete[] batchErr[i];
			
		}
		delete[] batchErr;
		
	}
	
	
}