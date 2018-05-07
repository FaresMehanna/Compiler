#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;


//hash a vector of strings
struct hasher
{
	unsigned long long operator() (const vector<string>& x) const{
		return 1;
	}
};

//test two vectors of string for equality
struct equall
{
	bool operator() (const vector<string>& x, const vector<string>& y) const{
		for(int i=1;i<x.size();i++){
			if(x[i] != y[i]){
				return false;
			}
		}
		return true;
	}
};

class Minimizer{

	private:
		unordered_map<vector<string>, string, hasher, equall> minimizeMap;
		unordered_map<string, string> mapingBetweenNames;

	public:
		vector<vector<string>> execute(const vector<vector<string>>& matrix){
			
			//does any minimization happened?
			bool minimizedBool = false;
			
			//-1 for not possible transitions
			mapingBetweenNames["-1"] = "-1";

			//set each name to itself
			for(int i=0;i<matrix.size();i++){
				mapingBetweenNames[matrix[i][2]] = matrix[i][2];
			}
			
			//Minimizing
			for(int i=0;i<matrix.size();i++){
				//if it's old then set this name to the first row's name
				if(minimizeMap.find(matrix[i]) != minimizeMap.end()){
					mapingBetweenNames[matrix[i][0]] = minimizeMap[matrix[i]];
					minimizedBool = true;
				}
				//if it's new row
				//then add this row to minimizeMap and to mapingBetweenNames
				else{
					minimizeMap[matrix[i]] = matrix[i][0];
					mapingBetweenNames[matrix[i][0]] = matrix[i][0];
				}
			}

			vector<vector<string>> minimizedVec;
			for(auto& x : minimizeMap){
				vector<string> row = x.first;
				for(int i=0;i<row.size();i++){
					row[i] = mapingBetweenNames[row[i]];
				}
				minimizedVec.push_back(row);
			}
			reverse(minimizedVec.begin(),minimizedVec.end());

			minimizeMap.clear();
			mapingBetweenNames.clear();


			if(minimizedBool){
				return execute(minimizedVec);
			}

			return minimizedVec;
		}
};