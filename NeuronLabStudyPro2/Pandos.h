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
public:
	map<string, any>* Data;
	map<string, any> MidleData;
	vector<string> VectorParamets;
	vector<string> TypeParamets;
	int ValParamets = 0;
	int SizeData=0;
	Pandos(string path, int SizeData);
	void SetMidleData();
	void SubstitutionNullParamets();
	void SubstitutionNullParamets(int b[]);
	void SubstitutionNullParamets(map<string, any>& MiddleDat);

	map<string, any> operator () (int n);
	~Pandos();
	
		
	

};

