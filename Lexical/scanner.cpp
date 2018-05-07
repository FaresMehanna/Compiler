#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <limits.h>

using namespace std;




class Scanner{
	public:
		
		vector< vector< int> > table;
		vector<vector <int> > scan;
		vector<string> name;
		vector<string> expressionsNames;
		string code;

		Scanner(string c){
			code =c;
		}

		void dataHandling (vector < vector <string> > minimiz){
			unordered_map<int , int> indexMap;


			for(int i=0;i<minimiz.size();i++ ){
				vector<int> v;
				for(int j=0;j<minimiz[0].size();j++){
					if(j==2)
						name.push_back(minimiz[i][j]);
					else{
						if(minimiz[i][j][0] == '-')
							v.push_back(-1);
						else
							v.push_back(atoi(minimiz[i][j].c_str()));
					}
				}
				scan.push_back(v);
				v.resize(0);
			}


			for(int i=0;i<scan.size();i++){
				indexMap[scan[i][0]] = i;
			}

			for(int i=0;i<scan.size();i++){
				for(int j=0;j<scan[i].size();j++){
					if(j!=1 && scan[i][j] !=-1 ){
						scan[i][j] = indexMap[scan[i][j]];
					}
				}
			}

			/* DEBUGGING
			for(int i=0;i<name.size();i++){
				cout << name[i] << endl;
			}

			for(int i=0;i<scan.size();i++){
				for(int j=0;j<scan[i].size();j++){
					cout << scan[i][j] << " ";
				}
				cout << endl;
			}
			*/	
		}

		vector<string> startScanning(){
			
			table = scan;
			expressionsNames = name;

			int lastAcceptedState=INT_MIN,curState=0,i=0, state = 0;

			vector<string> toReturn;

			for(int i=0;i<code.size();i++){
				//cout << table[curState][1] << endl;

				//if space
				if(code[i] == ' ' || code[i] == '\t'){
					//if first space
					if(lastAcceptedState == i-1){
						toReturn.push_back(expressionsNames[state]);
						curState = 0;
					}
					lastAcceptedState = INT_MIN;
					continue;
				}

				int transition = table[curState][code[i]+2];
				if(transition == -1 ){
					//if accepted series
					if(lastAcceptedState == i-1){
						toReturn.push_back(expressionsNames[state]);
						lastAcceptedState = INT_MIN;
						curState = 0;
						i--;
					}else{
						cout << "ERROR1" << endl;
						return toReturn;
					}
				}else{
					if(table[transition][code[i]+1] == 0){
						//if accepted series
						if(lastAcceptedState == i-1){
							toReturn.push_back(expressionsNames[state]);
							lastAcceptedState = INT_MIN;
							curState = 0;
							i--;
						}else{
							cout << "ERROR2" << endl;
							return toReturn;
						}
					}else{
						curState = transition;
						lastAcceptedState = i;
						state = transition;
					}
				}


			}

			if(lastAcceptedState != INT_MIN){
				toReturn.push_back(expressionsNames[state]);
			}

			return toReturn;

		}
};
