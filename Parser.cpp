#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include "TableIndex.h"
#include <unordered_map>

using namespace std;

void usage_msg(){
	
}

bool r_err_handler(int length){
	if (length!=4){
		cout<<"Your command R is invalid, It's should have 4 arguments, including R itself."<<endl;
		return true;
	}
	else
		return false;
}

bool i_err_handler(int length, int record_num, vector<int> delimiter){
	if (length<4){
		cout<<"Your command I is invalid, It's should have at least 4 arguments, including I itself."<<endl;
		return true;
	}
	for (int i=0; i<delimiter.size(); i++)
		if (i<3){
			if (delimiter[i]!=1){
				cout<<"Your data format should be key-type and record for each record."<<endl;
				return true;
			}
		}
		else{
			if ((delimiter[i]-(i%2))!=1){
				cout<<"Your data format should be key-type and record for each record."<<endl;
				return true;
			}
		}
	if (((length/2)-2-record_num)!=0){
		cout<<"Your record should be key value pair respectively."<<endl;
		return true;
	}

	return false;
}

bool d_err_handler(int length){
	if (length!=3){
		cout<<"Your command D is invalid, It's should have 3 arguments, including D itself."<<endl;
		return true;
	}
	else
		return false;
}

bool s_err_handler(int length){
	if (length!=2){
		cout<<"Your command Scan is invalid, It's should have 2 arguments, including Scan itself."<<endl;
		return true;
	}
	else
		return false;
}

bool q_err_handler(int length){
	if (length!=3||length!=4){
		cout<<"Your command q is invalid, It's should have 3 arguments for single value, 4 arguments for range query, including q itself."<<endl;
		return true;
	}
	else
		return false;
}

bool c_err_handler(int length){
	if (length!=2){
		cout<<"Your command c is invalid, It's should have 2 arguments, including c itself."<<endl;
		return true;
	}
	else
		return false;
}

bool p_err_handler(int length){
	if (length!=3){
		cout<<"Your command p is invalid, It's should have 3 arguments, including p itself."<<endl;
		return true;
	}
	else
		return false;
}

int main(){
	while (1){
		usage_msg();

		string str;
		getline(cin, str);

		vector<int> delimiter;
		for (int i=0; i<str.size(); i++)
			if (str[i] == ',')
				delimiter.push_back(1);
			else if (str[i] == ';')
				delimiter.push_back(2);

		//replace( str.begin(), str.end(), ',', ' '); 
		str.erase(remove(str.begin(), str.end(), ','), str.end());//eliminate ,
		size_t record_n = count(str.begin(), str.end(), ';'); //count record
		//replace( str.begin(), str.end(), ';', ' '); 
		str.erase(remove(str.begin(), str.end(), ';'), str.end());//eliminate ;
		size_t string_n = count(str.begin(), str.end(), '"');
		if (string_n%2!=0)
			continue;

		vector<string> tokens;
		int last_s = 0;
		for (int i=0; i<str.size(); i++){
			if (str[i] == ' '){
				tokens.push_back(str.substr(last_s, i-last_s));
				last_s = i+1;
			}
			if (i==str.size()-1){
				tokens.push_back(str.substr(last_s, i-last_s+1));
				break;
			}
			if (str[i] == '"'){
				i++;
				last_s = i;
				while (str[i]!='"'){
					i++;
				}
				tokens.push_back(str.substr(last_s, i-last_s));
				i++;
			}
		}

		unordered_map <string, TableIndex<int>*>  int_tables;
		unordered_map <string, TableIndex<KeyString>*>  str_tables;
		
		if (tokens[0]=="R"){
			if (r_err_handler(tokens.size())) //size error
				continue;
			if (tokens[2]=="Integer"){ //Command execution here
				//convert = stoi(tokens[3]);
				TableIndex<int> t((unsigned int)stoi(tokens[3]), (tokens[1]).c_str());
				int_tables.insert(make_pair<string, TableIndex<int>*>(tokens[1], &t));
			}
			else if (tokens[2]=="String"){
				TableIndex<KeyString> t((unsigned int)stoi(tokens[3]), (tokens[1]).c_str());
				str_tables.insert(make_pair<string, TableIndex<KeyString>*>(tokens[1], &t));
			}
			else 
				continue;
		}
		else if (tokens[0]=="I"){
			if (i_err_handler(tokens.size(), record_n, delimiter)) //size error
				continue;

		}
		else if (tokens[0]=="D"){
			if (d_err_handler(tokens.size())) //size error
				continue;
			if (int_tables.find(tokens[1]) != int_tables.end()) {
				if (int_tables.find(tokens[1])->second->delete_by_key(stoi(tokens[2])))
					continue;
			}
			else if (str_tables.find(tokens[1]) != str_tables.end()){
				if (str_tables.find(tokens[1])->second->delete_by_key(tokens[2]))
					continue;
			}
			else{

			}
		}
		else if (tokens[0]=="Scan"){
			if (s_err_handler(tokens.size())) //size error
				continue;
			if (int_tables.find(tokens[1]) != int_tables.end()) {
				int_tables.find(tokens[1])->second->scan_table();
			}
			else if (str_tables.find(tokens[1]) != str_tables.end()){
				str_tables.find(tokens[1])->second->scan_table();
			}
			else{

			}
		}
		else if (tokens[0]=="q"){
			if (q_err_handler(tokens.size())) //size error
				continue;
			if (tokens.size()==3){

			}
			else if (tokens.size()==4){

			}
		}
		else if (tokens[0]=="c"){
			if (c_err_handler(tokens.size())) //size error
				continue;

		}
		else if (tokens[0]=="p"){
			if (p_err_handler(tokens.size())) //size error
				continue;

		}
		else
			cout<<"Sorry! We do not support this kind of command!"<<endl;
		cout<<endl<<"STFU!Motherfucker!";
		
	}
	return 0;
}
