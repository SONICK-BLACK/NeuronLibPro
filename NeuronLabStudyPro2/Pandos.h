#pragma once
#include<iostream>
#include<vector>
#include<any>
#include<map>
#include<span>
#include<optional>

#include<sstream>
using namespace std;
class Pandos
{
private:
	bool SetDigitStr(signed char* str, int size);
	int SetStringint(span<string> VectorStr, string& Par);
	bool SetStringBool(span<string> VectorStr,string& Par);
	bool Ff = true;
	map<string, string> mapTypeParamets;
public:
	map<string, any>* Data;
	map<string, any> MidleData;
	map<string, vector<any>> ListDatDiversity;
	vector<string> VectorParamets;
	vector<string> TypeParamets;

	int ValParamets = 0;
	int SizeData=0;
	Pandos(string path, int SizeData);
	void SetMidleData();
	void SetDiversity();
	void SubstitutionNullParamets();
	void SubstitutionNullParamets(int b[]);
	void SubstitutionNullParamets(map<string, any>& MiddleDat);
	any SetModa(string parametr);

	map<string, any> operator () (int n);
	any&  operator () (int n, string parametr);
	~Pandos();
	
		
	

};

