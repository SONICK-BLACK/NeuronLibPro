#include <iostream>
#include <fstream>
#include <string>
#include"DataNeuron.h"
#include "Tensor.h"
#include"Pandos.h"
#include "Image.h"
#include <span>


using namespace std;
using namespace Tsr;
using namespace Data;
using namespace img;
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
//void InitData(DataNeuron& dat, int InputSize, int Pixel, int) {
    


//}

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

int Pixel = 900;
string path;

///NormolaizeData

for (int i = 1; i < 61; i++) {
   

    path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/CatFiles/Cat";
    path += to_string(i);
    path += ".bmp";
    
    img::Image im(&path[0]);
    im.SmoothFilt();
   im.contourBlack(img::Prefity, img::InR);


 im.ScaleImage(30, 30);
    
    path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/DataCat/Cat";
    path += to_string(i);
    path += ".bmp";
    im.WriteImage(&path[0]);

}
for (int i = 1; i < 61; i++) {


    path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/ParrotFiles/Parrot";
    path += to_string(i);
    path += ".bmp";

    img::Image im(&path[0]);
    im.SmoothFilt();
    im.contourBlack(img::Prefity, img::InR);


    im.ScaleImage(30, 30);

    path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/DataParrot/Parrot";
    path += to_string(i);
    path += ".bmp";
    im.WriteImage(&path[0]);

}
for (int i = 1; i < 11; i++) {


    path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/TestFiles/Cat";
    path += to_string(i);
    path += ".bmp";

    img::Image im(&path[0]);
    im.SmoothFilt();
    im.contourBlack(img::Prefity, img::InR);


    im.ScaleImage(30, 30);

    path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/DataTest/Cat";
    path += to_string(i);
    path += ".bmp";
    im.WriteImage(&path[0]);

}
for (int i = 1; i < 11; i++) {


    path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/TestFiles/Parrot";
    path += to_string(i);
    path += ".bmp";

    img::Image im(&path[0]);
    im.SmoothFilt();
    im.contourBlack(img::Prefity, img::InR);


    im.ScaleImage(30, 30);

    path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/DataTest/Parrot";
    path += to_string(i);
    path += ".bmp";
    im.WriteImage(&path[0]);

}

///

//0-cat. 1-Parrot
DataNeuron data(120, Pixel, 2);

int Cat = 1;
int Parrot = 1;
for (int i = 0; i < 120; i++) {
    
    int k = 0;
        if (!(i%2)) {
            path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/DataCat/Cat";
            path += to_string(Cat);
            path += ".bmp";
            img::Image* im =new Image(&path[0]);
            for (int j = 0; j < 30; j++) {
                for (int l = 0; l < 30; l++) {
                    data.SetData[i][k] = im->data.rgb[j][l].RGB[0]/250.0;
                    k += 1;
               }
            }
            data.CorrectVal[i][0] = 1.0;
            data.CorrectVal[i][1] = 0.0;
            delete im;
            Cat += 1;

        }
        else {
            
            path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/DataParrot/Parrot";
            path += to_string(Parrot);
            path += ".bmp";
            img::Image* im = new Image(&path[0]);
            for (int j = 0; j < 30; j++) {
                for (int l = 0; l < 30; l++) {
                 
                    data.SetData[i][k] = im->data.rgb[j][l].RGB[0] / 250.0;
                    k += 1;
                }
            }
            data.CorrectVal[i][0] = 0.0;
            data.CorrectVal[i][1] = 1.0;
            delete im;
            Parrot += 1;
        }
        

   
    }
const ActFuns Funns[] = { ReLU,Softmax };
const int ArrSize[] = { Pixel,300,2 };

Tensor T(3, ArrSize, Funns);



T.StartTeachSession(0.001, 1, data, MSR, 2, rmsprop, NullR);


}


