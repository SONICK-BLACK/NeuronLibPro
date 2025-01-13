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

void OutputData(double* OutNeuron, double* CorrectVal) {
    int CorectNumber;
    for (int i = 0; i < 10; i++) {
        if (CorrectVal[i] == 1.0) {
            CorectNumber = i;
            break;
        }
    }
    cout << endl;
    cout << "Corect Number: " << CorectNumber<<"\n";
    for (int i = 0; i < 10; i++) {
        cout << "( " << i << " ) [ " << OutNeuron[i] << " ]" << "  ";
    }
}
int main()
{
    int t;
    cout << "Begin Study Network Neurons? If zero, load Paramets Neurons Network and Test (1/0)\n";
    cin >> t;
    if (t) {
        int NumberCol = 1000;
        int pixel = 784;
        DataNeuron data(NumberCol, pixel, 10);

        ReadFile("lib_MNIST_edit.txt", data, pixel, 10);

        int val = 3;
        const ActFuns Funns[] = { ReLU,Softmax };
        const int ArrSize[] = { pixel,256,10 };

        Tensor T(3, ArrSize, Funns);

        T.StartTeachSession(0.001, 1, data, MSR, 10, rmsprop, NullR);
        cout << endl;
        cout << "Save Paramets? (1/0)\n";
        cin >> t;
        if (t) {
            T.SaveParametsNeurons();
        }

    }
    else {
        int TestNumber = 10000;
        int pixel = 784;
        DataNeuron dataTest(TestNumber, pixel, 10);
        ReadFile("lib_10k.txt", dataTest, pixel, 10);
    
        const ActFuns Funns[] = { ReLU,Softmax };
        const int ArrSize[] = { pixel,256,10 };
        Tensor T(3, ArrSize, Funns);
        T.LoadParametsNeurons();
        T.StartDirectSession(dataTest, OutputData);


    }



}


