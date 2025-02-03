#include "Pandos.h"
#include <fstream>
using namespace std;
Pandos::Pandos(string path, int SizeData) {
	///Set Paramets;
	this->SizeData = SizeData;
	Data = new map<string, any>[SizeData];

	int k = 0;
	ifstream out(path);
	if (!out.is_open()) {
		cout << "Error";
	}
	string ParametsStr;
	getline(out, ParametsStr);
	stringstream StreamStr(ParametsStr);
	string Paramet;
	while (getline(StreamStr, Paramet,',')) {
		for (int i = 0; i < SizeData; i++) {
			Data[i][Paramet];
			MidleData[Paramet];
		}
		
		VectorParamets.push_back(Paramet);
		k += 1;
	}
	StreamStr.clear();
	ValParamets = k;

	//Filling in the parameters
	string DataStr;
	int i = 0;
	int g = 0;
	bool F1 = true;
	bool F2 = true;
	while (getline(out, DataStr)) {
		stringstream StreamDataStr(DataStr);
		string Dat;
		
		while (getline(StreamDataStr, Dat, ',')) {

			if (Dat == "") {
				if (TypeParamets[i] == "(int)") {
					int v = 0;
					Data[g][VectorParamets[i]] = v;
				}
				else {
					string nul = "null";
					Data[g][VectorParamets[i]] = nul;
				}
				
				i += 1;
				continue;
			}
			if (!(Dat.find('"') == string::npos)) {
				string val;
				getline(StreamDataStr, val, '"');
				Dat += val;
				Dat += '"';
				
				getline(StreamDataStr, val, ',');
				Dat += val;
				Data[g][VectorParamets[i]] = Dat;
			
				if (F2) {
					TypeParamets.push_back("(string)");
					mapTypeParamets[VectorParamets[i]] = "(string)";
				}
				i += 1;
				continue;

			}
			/// Possible Optimization

			if (SetDigitStr((signed char*)&Dat[0], Dat.length())) {
				
				Data[g][VectorParamets[i]] = stoi(Dat);
				if (F1) {
				TypeParamets.push_back("(int)");
				mapTypeParamets[VectorParamets[i]] = "(int)";
					
				}
				
		

			}
			else {
				Data[g][VectorParamets[i]] = Dat;
				if (F2) {
					TypeParamets.push_back("(string)");
					mapTypeParamets[VectorParamets[i]] = "(string)";
				}
				
			}

		
			i += 1;
		}
		F1 = false;
		F2 = false;
		g += 1;
		if (g == SizeData) {
			break;
		}
		StreamDataStr.clear();
		i = 0;
	}
	out.close();


}
map<string, any> Pandos::operator () (int n) {
	
	return Data[n];

}
any& Pandos::operator () (int n, string parametr) {
	
	return Data[n][parametr];
}
Pandos::~Pandos() {
	delete[] Data;
}
bool Pandos::SetDigitStr( signed char* str, int size) {
	for (int i = 0; i < size; i++) {
		if (!isdigit(str[i]) && str[i]!='.') {
			return false;
		}
	}
	return true;

}
void Pandos::SetMidleData() {
	
	for (int j = 0; j < ValParamets; j++) {
		if (TypeParamets[j] == "(string)") {
			

			////Set Vector Val for Paraments;
			vector<string> VectorSet;
			bool B = true;
			if (any_cast<string>(Data[0][VectorParamets[j]]) != "null") {
				VectorSet.push_back(any_cast<string>(Data[0][VectorParamets[j]]));
				B = false;
			}
			
			for (int i = 1; i < SizeData; i++) {
				if (any_cast<string>(Data[i][VectorParamets[j]]) != "null") {
					if (B) {
						VectorSet.push_back(any_cast<string>(Data[i][VectorParamets[j]]));
						B = false;
					}
					else if (SetStringBool(VectorSet, (string&)any_cast<string>(Data[i][VectorParamets[j]]))) {

						VectorSet.push_back(any_cast<string>(Data[i][VectorParamets[j]]));

					}
				}
				

			}
			/////
			int* SizePar = new int[VectorSet.size()];
			for (int i = 0; i < VectorSet.size(); i++) {
				SizePar[i] = 0;
			}
			/// Set SizePar
			for (int i = 0; i < SizeData; i++) {
				if (any_cast<string>(Data[i][VectorParamets[j]]) != "null") {
					SizePar[SetStringint(VectorSet, (string&)any_cast<string>(Data[i][VectorParamets[j]]))] += 1;
				}
				

			}
			///
			int Val = SizePar[0];
			int k = 0;
			for (int i = 0; i < VectorSet.size()-1; i++) {
				if (Val < SizePar[i + 1]) {
					Val = SizePar[i+1];
					k = i + 1;
				}
			}
			MidleData[VectorParamets[j]] = VectorSet[k];
			delete[] SizePar;
		}
		else if (TypeParamets[j] == "(int)") {
			int ValMid = 0;
			for (int i = 0; i < SizeData; i++) {
				ValMid += any_cast<int>(Data[i][VectorParamets[j]]);

			}
			
			ValMid = ValMid / SizeData;
			MidleData[VectorParamets[j]] = ValMid;
			
		}
	

	}
}

bool Pandos::SetStringBool(span<string> VectorStr,string& Par) {
	for (string Val : VectorStr) {
		if (Val == Par) {
			return false;
		}
	}
	return true;
	
}

int Pandos::SetStringint(span<string> VectorStr, string& Par) {
	int i = 0;
	for (string Val : VectorStr) {
		
		if (Val == Par) {
			return i;
		}
		i += 1;
	}


	//for (int i = 0; i < VectorStr.size(); i++) {
		//if (VectorStr[i] == Par) {
	//		return i;
		//}
	//}
	
}
///???
void Pandos::SubstitutionNullParamets(map<string, any>& MiddleDat) {

}
///
void Pandos::SubstitutionNullParamets() {
	for (int i = 0; i < SizeData; i++) {
		for (int j = 0; j < ValParamets; j++) {
			if (TypeParamets[j] == "(string)") {
		
				
				if (any_cast<string>(Data[i][VectorParamets[j]]) == "null") {
				
					string val = any_cast<string>(MidleData[VectorParamets[j]]);
					Data[i][VectorParamets[j]] = val;
				}
			}
			else if (TypeParamets[j] == "(int)") {
				
				if (any_cast<int>(Data[i][VectorParamets[j]]) == 0) {
					int val = any_cast<int>(MidleData[VectorParamets[j]]);
					
					Data[i][VectorParamets[j]] =val;

				}

			}
		}
	}
}

void Pandos::SubstitutionNullParamets(int b[]) {
	for (int i = 0; i < SizeData; i++) {
		for (int j = 0; j < ValParamets; j++) {
			if (TypeParamets[j] == "(string)" && b[j] != 0) {


				if (any_cast<string>(Data[i][VectorParamets[j]]) == "null") {

					string val = any_cast<string>(MidleData[VectorParamets[j]]);
					Data[i][VectorParamets[j]] = val;
				}
			}
			else if (TypeParamets[j] == "(int)" && b[j]!=0) {

				if (any_cast<int>(Data[i][VectorParamets[j]]) == 0) {
					int val = any_cast<int>(MidleData[VectorParamets[j]]);

					Data[i][VectorParamets[j]] = val;

				}

			}
		}
	}

}
//A dubious function in the public. Is it needed at all?
void Pandos::SetDiversity() {
	
	for (int j = 0; j < ValParamets; j++) {
		if (TypeParamets[j] == "(int)") {
			ListDatDiversity[VectorParamets[j]].push_back(any_cast<int>(Data[0][VectorParamets[j]]));
		}
		else if (TypeParamets[j] == "(string)") {
			ListDatDiversity[VectorParamets[j]].push_back(any_cast<string>(Data[0][VectorParamets[j]]));
		}
		
	}

	for (int i = 1; i < SizeData; i++) {
		for (int j = 0; j < ValParamets; j++) {
			if (TypeParamets[j] == "(int)") {
				bool F = false;
				span<any> intList = ListDatDiversity[VectorParamets[j]];
				for (any val : intList) {
					if (any_cast<int>(val) == any_cast<int>(Data[i][VectorParamets[j]])) {
						goto t;
					}
				}
				F = true;
				if (F) {
					ListDatDiversity[VectorParamets[j]].push_back(any_cast<int>(Data[i][VectorParamets[j]]));

				}
			t:
				continue;
			
			}
			else if (TypeParamets[j] == "(string)") {
				bool F = false;
				span<any> intList = ListDatDiversity[VectorParamets[j]];
				for (any val : intList) {
					if (any_cast<string>(val) == any_cast<string>(Data[i][VectorParamets[j]])) {
						goto r;
					}
				}

				F = true;
				if (F) {
					ListDatDiversity[VectorParamets[j]].push_back(any_cast<string>(Data[i][VectorParamets[j]]));

				}
			r:
				continue;

			}
			
		}
	}
	Ff = false;
}
//It is possible to make optimizations if we need to find only a small part of the mod parameters.
any Pandos::SetModa(string parametr) {
	if (Ff) {
		SetDiversity();
	}
	
	int* intSize = new int[ListDatDiversity[parametr].size()];
	for (int i = 0; i < ListDatDiversity[parametr].size(); i++) {
		intSize[i] = 0;
	}

	if (mapTypeParamets[parametr] == "(int)") {
		
		for (int i = 0; i < SizeData; i++) {
			
			int k = 0;
			while (true) {
				if (any_cast<int>(Data[i][parametr]) == any_cast<int>(ListDatDiversity[parametr][k])) {
					break;
				}
				k += 1;
			}
			intSize[k] += 1;
		}

		

	}
	else if (mapTypeParamets[parametr] == "(string)") {
		for (int i = 0; i < SizeData; i++) {

			int k = 0;
			while (true) {
				if (any_cast<string>(Data[i][parametr]) == any_cast<string>(ListDatDiversity[parametr][k])) {
					break;
				}
				
				k += 1;

			}
			intSize[k] += 1;
		}
	}

	int val=intSize[0];
	int k=0;
	for (int i = 0; i < ListDatDiversity[parametr].size()-1; i++) {
		if (val < intSize[i + 1]) {
			val = intSize[i + 1];
			k = i + 1;
		}
	}
	delete[] intSize;
	return ListDatDiversity[parametr][k];
}


