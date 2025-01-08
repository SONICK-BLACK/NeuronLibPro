#include <iostream>
#include <fstream>
#include <string>
#include"DataNeuron.h"
#include "Tensor.h"

using namespace std;

void ReadFile(string path, DataNeuron& dat, int Pixel, int InputSize) {
    fstream out;
    out.open(path);
    if (!out.is_open()) {
        cout << "Err" << endl;
    }
    int val;
    for (int i = 0; i < dat.SizeData; i++) {
        out >> val;
        for (int g = 0; g < InputSize; g++) {
            if (g == val) {

                dat.CorrectVal[i][g] = 1.0;
            }
            else {
                dat.CorrectVal[i][g] = 0.0;
            }
        }
        for (int j = 0; j < Pixel; j++) {
            out >> dat.SetData[i][j];
        }
    }
    cout << "End Read File\n";
    out.close();
}

int main()
{
    int NumberCol = 1000;
    int pixel = 784;
     DataNeuron data(NumberCol, pixel, 10);

    ReadFile("lib_MNIST_edit.txt", data, pixel, 10);

    int val = 3;
    const ActFuns Funns[] = { ReLU,Sigmoida};
    const int ArrSize[] = { pixel,256,10};

    Tensor T(3, ArrSize, Funns);

  T.StartTeachSession(0.001, 1, data, MSR, 10);





}


