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
int channal = 3;
DataCNN dataTest(20, 2, channal);
int pup = 0;
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
void Put(Batch& bat) {
    fstream out;
    out.open("123.txt");
    if (!out.is_open()) {
        cout << "Err" << endl;
    }
    for (int i = 0; i < bat.sizeY; i++) {
        for (int l = 0; l < bat.sizeX; l++) {
            out >> bat.batch[i][l];
        }
    }
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
void OutputData3(double* OutNeuron) {
    int val;
  //  cout << OutNeuron[1];
  //  cout << endl;
   // cout << OutNeuron[0];

    if (OutNeuron[1] > OutNeuron[0]) {
        val = 1;
        
        cout << "Output Data: " << "Parrot" << "\n";
    }
    if (OutNeuron[1] < OutNeuron[0]) {
        val = 0;

        cout << "Output Data: " << "Cat" << "\n";
    }
    
    if (dataTest.CorrectVal[pup][val] == 1.0) {
        Val1 += 1;


    }
    if (dataTest.CorrectVal[pup][0] == 0.0) {

        cout << "Corect Data: " << "Parrot" << "\n";
    }
    else {

        cout << "Corect Data: " << "Cat" << "\n";
    }


}


int main()
{
    setlocale(LC_ALL, "Russian");
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

//int Pixel = 10000;
//string path;

///NormolaizeData
/*
for (int i = 1; i < 61; i++) {
   

    path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/CatFiles/Cat";
    path += to_string(i);
    path += ".bmp";
    
    img::Image im(&path[0]);
  //  im.SmoothFilt();
 //  im.contourBlack(img::Prefity, img::InR);


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
   //  im.SmoothFilt();
    // im.contourBlack(img::Prefity, img::InR);


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
    //  im.SmoothFilt();
    //  im.contourBlack(img::Prefity, img::InR);


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
  //  im.SmoothFilt();
  //  im.contourBlack(img::Prefity, img::InR);


    im.ScaleImage(30, 30);

    path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/DataTest/Parrot";
    path += to_string(i);
    path += ".bmp";
    im.WriteImage(&path[0]);

}
*/
///

//0-cat. 1-Parrot

cout << endl;
int Set;
cout << "Teach paraments? (1/0)";
cin >> Set;



string path;
DataCNN data(120, 2, channal);

int Cat = 1;
int Parrot = 1;
int k = 0;
for (int i = 0; i < 120; i++) {
    for (int y = 0; y < channal; y++) {
        data.Grid[i][y](30, 30, 0, 0);
    }
}



for (int i = 0; i < 20; i++) {
    for (int y = 0; y < channal; y++) {
        dataTest.Grid[i][y](30, 30, 0, 0);
    }
}

for (int i = 0; i < 120; i++) {
    
    
        if (!(i%2)) {
            path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/DataCat/Cat";
            path += to_string(Cat);
            path += ".bmp";
            img::Image* im =new Image(&path[0]);
            for (int y = 0; y < channal; y++) {
                for (int j = 0; j < 30; j++) {
                    for (int l = 0; l < 30; l++) {
                        


                            data.Grid[i][y].batch[j][l] = im->data.rgb[j][l].RGB[y]/255.0;
                        


                        k += 1;
                    }
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
            for (int y = 0; y < channal; y++) {
                for (int j = 0; j < 30; j++) {
                    for (int l = 0; l < 30; l++) {


                        data.Grid[i][y].batch[j][l] = im->data.rgb[j][l].RGB[y] / 255.0;


                        k += 1;
                    }
                }
            }
            data.CorrectVal[i][0] = 0.0;
            data.CorrectVal[i][1] = 1.0;
            delete im;
            Parrot += 1;
        }
      
   
    }
const ActFuns Funns[] = { ReLU,Softmax };
int ArrSize[] = {0,220,2 };
int ValCore[] = {1,2};
int** SizeCore= new int*[2];
SizeCore[0] = new int[2];
SizeCore[1] = new int[2];
SizeCore[0][0] =3;
SizeCore[0][1] = 3;
SizeCore[1][0] = 3;
SizeCore[1][1] = 3;


if (!Set) {
    CNN cnn(channal, ValCore, 2, SizeCore, 2);
    cnn.PollingVal = new int[2];
    cnn.PollingVal[0] = 2;
    cnn.PollingVal[1] = 2;

    cnn.InitPepzetronCNNForDirect(3, ArrSize, Funns, data.Grid[0][0].sizeX, data.Grid[0][0].sizeY);
    cnn.LoadParamentsCNN();
    Cat = 1;
    Parrot = 1;
    k = 0;
    for (int i = 0; i < 20; i++) {


        if (!(i % 2)) {
            path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/DataTest/Cat";
            path += to_string(Cat);
            path += ".bmp";
            img::Image* im = new Image(&path[0]);
            for (int y = 0; y < channal; y++) {
                for (int j = 0; j < 30; j++) {
                    for (int l = 0; l < 30; l++) {


                        dataTest.Grid[i][y].batch[j][l] = im->data.rgb[j][l].RGB[y] / 255.0;


                        k += 1;
                    }
                }
            }
            dataTest.CorrectVal[i][0] = 1.0;
            dataTest.CorrectVal[i][1] = 0.0;
            delete im;
            Cat += 1;

        }
        else {

            path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/DataTest/Parrot";
            path += to_string(Parrot);
            path += ".bmp";
            img::Image* im = new Image(&path[0]);
            for (int y = 0; y < channal; y++) {
                for (int j = 0; j < 30; j++) {
                    for (int l = 0; l < 30; l++) {



                        dataTest.Grid[i][y].batch[j][l] = im->data.rgb[j][l].RGB[y] / 255.0;

                        k += 1;
                    }
                }
            }
            dataTest.CorrectVal[i][0] = 0.0;
            dataTest.CorrectVal[i][1] = 1.0;
            delete im;
            Parrot += 1;
        }



    }
    for (int i = 0; i < 20; i++) {
        cnn.StartDirectCNN(dataTest.Grid[i], OutputData3, 2, ArrSize, Funns);
        pup += 1;
    }

    cout << endl;
    cout << (Val1 / 20.0) * 100;
    return 0;
    
}
CNN cnn(channal, ValCore, 2, SizeCore, 2);

cnn.PollingVal = new int[2];
cnn.PollingVal[0] = 2;
cnn.PollingVal[1] = 2;

cnn.InitPepzetronCNNForTeach(3, ArrSize, Funns, data.Grid[0][0].sizeX, data.Grid[0][0].sizeY);
cnn.StartTrainingCNN(data, 0.001, 1, MSR,180,Adam, NullR,false);




 Cat = 1;
 Parrot = 1;
  k = 0;
for (int i = 0; i < 20; i++) {

   
    if (!(i % 2)) {
        path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/DataTest/Cat";
        path += to_string(Cat);
        path += ".bmp";
        img::Image* im = new Image(&path[0]);
        for (int y = 0; y < channal; y++) {
            for (int j = 0; j < 30; j++) {
                for (int l = 0; l < 30; l++) {


                    dataTest.Grid[i][y].batch[j][l] = im->data.rgb[j][l].RGB[y] / 255.0;


                    k += 1;
                }
            }
        }
        dataTest.CorrectVal[i][0] = 1.0;
        dataTest.CorrectVal[i][1] = 0.0;
        delete im;
        Cat += 1;

    }
    else {

        path = "B:/Work/LocalProject/VS/NeuronLabStudyPro2/NeuronLabStudyPro2/DataTest/Parrot";
        path += to_string(Parrot);
        path += ".bmp";
        img::Image* im = new Image(&path[0]);
        for (int y = 0; y < channal; y++) {
            for (int j = 0; j < 30; j++) {
                for (int l = 0; l < 30; l++) {



                    dataTest.Grid[i][y].batch[j][l] = im->data.rgb[j][l].RGB[y] / 255.0;

                    k += 1;
                }
            }
        }
        dataTest.CorrectVal[i][0] = 0.0;
        dataTest.CorrectVal[i][1] = 1.0;
        delete im;
        Parrot += 1;
    }



}
for (int i = 0; i < 20; i++) {
    cnn.StartDirectCNN(dataTest.Grid[i], OutputData3, 2, ArrSize, Funns);
    pup += 1;
}

cout << endl;
cout << (Val1 / 20.0) * 100;

cout << endl;
int set;
cout << "Save paraments? (1/0)";
cin >> set;
if (set) {
    cnn.SaveParamentsCNN();
}


/*
DataCNN data(1, 2, 3);
data.Grid[0][0](12, 12, 0, 0);
data.Grid[0][1](12, 12, 0, 0);
data.Grid[0][2](12, 12, 0, 0);
Put(data.Grid[0][0]);
Put(data.Grid[0][1]);
Put(data.Grid[0][2]);
int ValCore[] = { 2,1 };
int** SizeCore = new int* [2];
SizeCore[0] = new int[2];
SizeCore[1] = new int[2];
SizeCore[0][0] = 3;
SizeCore[0][1] = 3;
SizeCore[1][0] = 3;
SizeCore[1][1] = 3;
CNN cnn(3, ValCore, 2, SizeCore, 1);
cnn.PollingVal = new int[2];
cnn.PollingVal[0] = 1;
cnn.PollingVal[1] = 1;
cnn.StartTrainingCNN(data);
*/
}


