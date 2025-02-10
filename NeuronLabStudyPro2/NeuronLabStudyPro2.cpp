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
            
            dat.SetData[i][j] = any_cast<double>(Pn(i, Pn.VectorParamets[j]));
        }
       if(any_cast<double>(Pn(i, Pn.VectorParamets[Pn.ValParamets - 1]))==0){
           dat.CorrectVal[i][0] = 0;
           dat.CorrectVal[i][1] = 1;
       }
       else {
           dat.CorrectVal[i][0] = 1;
           dat.CorrectVal[i][1] = 0;
       }
        
      
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
    int val;
    if (OutNeuron[1] > OutNeuron[0]) {
        val = 1;
      
        cout << "Output Data: " << "No surv" << "\n";
  }
    if (OutNeuron[1] <  OutNeuron[0]) {
        val = 0;
        
        cout << "Output Data: " << "Surv" << "\n";
    }

    if (CorrectVal[val] == 1) {
        Val1 += 1;


    }
    if (CorrectVal[0] == 0) {
      
        cout << "Corect Data: " << "Surv" << "\n";
   }
    else {
       
        cout << "Corect Data: " << "No Surv" << "\n";
    }
    
    
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
 /*   int sizeData = 100;
    Pandos pn("accident.csv", sizeData);
    pn.SetMidleData();
    int b[6] = { 1,1,1,1,1,0 };
    pn.SubstitutionNullParamets(b);

    for (int i = 0; i < sizeData; i++) {
        if (any_cast<string>(pn(i, "Gender")) == "Female") {
            double v = 1;
            pn(i, "Gender") = v;


        }
        else {
            double v = 0;
            pn(i, "Gender") = v;
        }
        if (any_cast<string>(pn(i, "Helmet_Used")) == "Yes") {
            double v = 1;
            pn(i, "Helmet_Used") = v;
        }
        else {
            double v = 0;
            pn(i, "Helmet_Used") = v;
        }
        if (any_cast<string>(pn(i, "Seatbelt_Used")) == "Yes") {
            double v = 1;
            pn(i, "Seatbelt_Used") = v;
        }
        else {
            double v = 0;
            pn(i, "Seatbelt_Used") = v;
        }
    }
    int b1[6] = { 1,0,1,0,0,0 };
    pn.SetMormolazeDataOfOne(b1);
    DataNeuron data(sizeData, 5, 2);
    SetDatasetData(data, pn, sizeData);
    const ActFuns Funns[] = { ReLU,Softmax };
    const int ArrSize[] = { 5,100,2};
    Tensor T(3, ArrSize, Funns);

    T.StartTeachSession(0.6, 1, data, MSR, 100, Momentum, NullR);


    T.StartDirectSession(data, OutputData1);

   double C = Val1 / (double)sizeData;
   cout << "\n";
   cout << C * 100;


    */
int sizeDat=5;
int valpar=4;
Pandos pn("house.csv", sizeDat);
pn.SetDiversity();
for (int j = 0; j < pn.ValParamets; j++) {
    cout << pn.VectorParamets[j] << ": ";
    for (int i = 0; i < pn.ListDatDiversity[pn.VectorParamets[j]].size(); i++) {
        cout << any_cast<double>(pn.ListDatDiversity[pn.VectorParamets[j]][i]) << " ";

    }
    cout << "\n";
}
cout << "\n";
cout<<any_cast<double>(pn.SetModa("Area"));

cout << "\n";
DataRegression rg(valpar, sizeDat);
for (int i = 0; i < sizeDat; i++) {
    for (int j = 1; j < valpar; j++) {
        rg.X[i][j] = any_cast<double>(pn(i, pn.VectorParamets[j]));
     
    }

  rg.Y[i][0] = any_cast<double>(pn.Data[i]["Price"]);

}

RegressionModel reg;
reg.SetWheightsBParamets(rg);
cout << "\n";
for (int j = 0; j < valpar; j++) {
    cout << rg.b[j][0];
    cout << "\n";
}

reg.SetRcriteria(rg);

   

}


