#include <iostream>
#include <fstream>
#include <string>
#include"DataNeuron.h"
#include "Tensor.h"
#include"Pandos.h"
#include <span>


using namespace std;
using namespace Tsr;
using namespace Data;
int Val1 = 0;
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
void SetDatasetData(DataNeuron& dat, Pandos& Pn, int SizeData) {
    for (int i = 0; i < SizeData; i++) {
        for (int j = 0; j < Pn.ValParamets-1; j++) {
            
            dat.SetData[i][j] = any_cast<int>(Pn(i, Pn.VectorParamets[j]));
        }
        dat.CorrectVal[i][0] = any_cast<int>(Pn(i, Pn.VectorParamets[Pn.ValParamets - 1]));
        
      
    }

}

void OutputData(double* OutNeuron, double* CorrectVal) {
    int CorectNumber=0;

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
    double max = OutNeuron[0];
    int maxNumber = 0;
    for (int i = 0; i < 9; i++) {
        if (max < OutNeuron[i + 1]) {
            maxNumber = i + 1;
            max = OutNeuron[i + 1];
        }
    }
    if (maxNumber == CorectNumber) {
        Val1 += 1;
    }
    
}
void OutputData1(double* OutNeuron, double* CorrectVal) {
    cout << "Corect Znach: " << CorrectVal[0]<<"\n";
    cout << "Output Data: " << OutNeuron[0] << "\n";
    
    
}


int main()
{

    /*  int t;
      cout << "Begin Study Network Neurons? If zero, load Paramets Neurons Network and Test (1/0)\n";
      cin >> t;
      if (t) {
          int NumberCol = 1000;
          int pixel = 784;
          DataNeuron data(NumberCol, pixel, 10);

          ReadFile("lib_MNIST_edit.txt", data, pixel, 10);

          int val = 3;
          const ActFuns Funns[] = { ReLU,Softmax };
          const int ArrSize[] = { pixel,256,10};

          Tensor T(3, ArrSize, Funns);



          T.StartTeachSession(0.001, 1, data, MSR, 10, rmsprop, NullR);


          ////

          int TestNumber = 10000;
          DataNeuron dataTest(TestNumber, pixel, 10);
          ReadFile("lib_10k.txt", dataTest, pixel, 10);


          T.StartDirectSession(dataTest, OutputData);




          cout << endl;
          cout << "Number Corect: " << Val1;

          ////
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
          cout << endl;
          cout << "Number Corect: " << Val1;


      }
      */
    int sizeData = 200;
    Pandos pn("accident.csv", sizeData);
    pn.SetMidleData();
    int b[6] = { 1,1,1,1,1,0 };
    pn.SubstitutionNullParamets(b);

    for (int i = 0; i < sizeData; i++) {
        if (any_cast<string>(pn(i, "Gender")) == "Female") {
            int v = 1;
            pn(i, "Gender") = v;


        }
        else {
            int v = 0;
            pn(i, "Gender") = v;
        }
        if (any_cast<string>(pn(i, "Helmet_Used")) == "Yes") {
            int v = 1;
            pn(i, "Helmet_Used") = v;
        }
        else {
            int v = 0;
            pn(i, "Helmet_Used") = v;
        }
        if (any_cast<string>(pn(i, "Seatbelt_Used")) == "Yes") {
            int v = 1;
            pn(i, "Seatbelt_Used") = v;
        }
        else {
            int v = 0;
            pn(i, "Seatbelt_Used") = v;
        }
    }

    DataNeuron data(sizeData, 5, 1);
    SetDatasetData(data, pn, sizeData);
    const ActFuns Funns[] = { ReLU,ReLU };
    const int ArrSize[] = { 5,50,1};
    Tensor T(3, ArrSize, Funns);

    T.StartTeachSession(0.3, 1, data, MSR, 10, NullO, NullR);


    T.StartDirectSession(data, OutputData1);



    

   

}


