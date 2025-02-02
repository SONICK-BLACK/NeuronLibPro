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

    int sizeDat = 200;
    Pandos pn("accident.csv", sizeDat);
    for (int i = 0; i < pn.ValParamets; i++) {
        cout << pn.VectorParamets[i] << " ";
    }
    int AgeMid = 0;
    int SpeedMid = 0;
    string modaGend;
    string modaHelment;
    string modaSeatbelt;

    int SizeMod1G = 0;
    int SizeMod2G = 0;
    int SizeMod1H = 0;
    int SizeMod2H = 0;
    int SizeMod1S = 0;
    int SizeMod2S = 0;


    pn.SetMidleData();

    int k = 0;

    for (int i = 0; i < sizeDat; i++) {
        if (!(any_cast<int>(pn(i)["Age"]) == 0)) {
            AgeMid += any_cast<int>(pn(i)["Age"]);
            k += 1;
        }
        if (!(any_cast<int>(pn(i)["Speed_of_Impact"]) == 0)) {
            SpeedMid += any_cast<int>(pn(i)["Speed_of_Impact"]);


        }
        if (!(any_cast<string>(pn(i)["Gender"]) == "null")) {
            if (any_cast<string>(pn(i)["Gender"]) == "Female") {
                SizeMod1G += 1;
            }
            else {
                SizeMod2G += 1;
            }
        }
    
        if (!(any_cast<string>(pn(i)["Helmet_Used"]) == "null")) {
            if (any_cast<string>(pn(i)["Helmet_Used"]) == "Yes") {
                SizeMod1H += 1;
            }
            else {
                SizeMod2H += 1;
            }
        }


        if (!(any_cast<string>(pn(i)["Seatbelt_Used"]) == "null")) {
            if (any_cast<string>(pn(i)["Seatbelt_Used"]) == "Yes") {
                SizeMod1S += 1;
            }
            else {
                SizeMod2S += 1;
            }
        }





    }
    if (SizeMod1G > SizeMod2G) {
        modaGend = "Female";

    }
    else if (SizeMod1G < SizeMod2G) {
        modaGend = "Male";
    }
    else {
        modaGend = "Female";
    }


    if (SizeMod1H > SizeMod2H) {
        modaHelment = "Yes";

    }
    else if (SizeMod1H < SizeMod2H) {
        modaHelment = "No";
    }
    else {
        modaHelment = "Yes";
    }



    if (SizeMod1S > SizeMod2S) {
        modaSeatbelt = "Yes";

    }
    else if (SizeMod1S < SizeMod2S) {
        modaSeatbelt = "No";
    }
    else {
        modaSeatbelt = "Yes";
    }








    AgeMid = AgeMid / k;
    SpeedMid = SpeedMid / k;
    cout << "\n";
    cout << "Speed Mid:" << "\n";

    cout << SpeedMid;
    cout << "\n";
    cout << "Age Mid:" << "\n";
    cout << AgeMid;
    cout << "\n";
    cout << "moda Gender:" << "\n";
    cout << modaGend;
    cout << "\n";
    cout << "moda Helmet_Used:" << "\n";
    cout << modaHelment;
    cout << "\n";
    cout << "moda Seatbelt_Used:" << "\n";
    cout << modaSeatbelt;


    cout << "\n\n";

  //  pn.SetMidleData();
    cout << "\n";
    cout << "Speed Mid:" << "\n";

    cout << any_cast<int>(pn.MidleData["Speed_of_Impact"]);
    cout << "\n";
    cout << "Age Mid:" << "\n";
    cout << any_cast<int>(pn.MidleData["Age"]);
    cout << "\n";
    cout << "moda Gender:" << "\n";
    cout << any_cast<string>(pn.MidleData["Gender"]);
    cout << "\n";
    cout << "moda Helmet_Used:" << "\n";
    cout << any_cast<string>(pn.MidleData["Helmet_Used"]);
    cout << "\n";
    cout << "moda Seatbelt_Used:" << "\n";
    cout << any_cast<string>(pn.MidleData["Seatbelt_Used"]);


    int b[6] = { 1,1,1,1,1,0};
    pn.SubstitutionNullParamets(b);


    

   

}


