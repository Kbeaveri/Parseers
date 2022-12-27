#include <vector>
#include <deque>
#include "Analizator.h"
#include <fstream>
#include <map>

using namespace std;
class Semantik {
public:
	string id_begin;
	string id_end;
	void Set_begin(string id) {
		id_begin = id;
	}
	void Set_end(string id) {
		id_end = id;
	}
	void Poliz_descr() {

	}
};

class Parser
{
public:
	string all_txt;
	Analizator A;
	int space;
	pair <string, string> now;
	ofstream fout;
	Parser(string all_txt) {
		this->all_txt = all_txt;
		A.Add(all_txt);
		fout.open("output.txt");
	}

	void TreeAdd(int space, string name)
	{
		for (int i = 0; i < space; ++i)
		{
			fout << " ";
		}
		fout << name << "\n";
	}
	void TreeAdd(int space, string name, string name2)
	{

		for (int i = 0; i < space; ++i)
		{
			fout << " ";
		}
		fout << name << " --> " << name2 << "\n";

	}
	void start()
	{
		space = 3;
		TreeAdd(space, "Begin");
		space += 3;
		now = Next_Lic();
		if (now.first== "PROGRAM") {
			TreeAdd(space, "Program");
			space += 5;
			now = Next_Lic();
			check();
			if (now.second == "ID") {
				TreeAdd(space, now.second, now.first);
				now = Next_Lic();
				space += 5;
				check();
			}
			else {
				cout << "Program name error";
				exit(-1);
			}
		}
		space = 3;
		TreeAdd(space, "Description");
		Descr();
		space = 3;
		TreeAdd(space, "Operator");
		OP();
		space = 3;
		TreeAdd(space, "END");
		End();
		cout << "NO ERROR,good game" << endl;
	}
	void Descr() {
		while (true) {
			if (now.first == "INTEGER") {
				INTEGER();
			}
			else {
				break;
			}
		}
	}
	void INTEGER() {
		space = 6;
		if (now.first == "INTEGER") {
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			check();
			ID();
		}
	}
	void OP() {
		while (true) {
			if (now.first == "FOR") {
				FOR_OP();
			}
			else if (now.second == "ID") {
				TreeAdd(space+7, "OPER");
				Simple_ID();
			}
			else {
				break;
			}
		}
	}
	void ID() {
		if (now.second == "ID") {
			space = 24;
			TreeAdd(space,"VarList");
			space = 30;
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			if (now.first == ",") {
				now = Next_Lic();
				check();
				ID();
			}
		}
		else {
			cout << "ID eroor" << endl;
			exit(-1);
		}
	}
	void Simple_ID() {
		if (now.second == "ID") {
			space += 10;
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			check();
			Operators();
		}
		else {
			cout << "simple ID eroor" << endl;
			exit(-1);
		}
	}
	void Operators() {
		if (now.first == "=") {
			space += 5;
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			check();
			Expr();
		}
		else {
			cout << "Operator eroor" << endl;
			exit(-1);
		}
	}
	void Expr() {
		SimpleExp();
		while (true) {
			if (now.first == "+" || now.first == "-")
			{
				space += 5;
				TreeAdd(space, now.second, now.first);
				now = Next_Lic();
				check();
				Expr();
			}
			else {
				break;
			}
		}
	}
	void SimpleExp() {
		bool flag = false;
		if (now.second == "ID") {
			space += 5;
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			check();
			return;
		}
		else if (now.first == "(") {
			space += 5;
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			check();
			Expr();
			if (now.first == ")") {
				space += 5;
				TreeAdd(space, now.second, now.first);
				now = Next_Lic();
				return;
			}
			else {
				cout << "simple exp error" << endl;
				exit(-1);
			}
		}
		else if (now.second == "IntNum") {
			space += 5;
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			check();
			return;
		}
		else {
			cout << "simple exp error" << endl;
			exit(-1);
		}
	}
	void FOR_OP() {
		space = 10;
		TreeAdd(space, "OPER");
		if (now.first == "FOR") {
			space = 15;
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			check();
			Simple_ID();
		}
		else {
			return;
		}
		if (now.first == "TO") {
			space = 15;
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			space += 6;
			check();
			Expr();
		}
		else {
			cout << "for_op error" << endl;
			exit(-1);
		}
		if (now.first == "DO") {
			space = 15;
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			check();
			Simple_ID();
		}
		else {
			error();
			exit(-1);
		}
	}
	void End() {
		if (now.first == "END") {
			now = Next_Lic();
			if (now.second == "ID") {
				space += 10;
				TreeAdd(space, now.second, now.first);
			}
			else {
				cout << "Od";
			}
		}
		else {
			cout << "Dayte end";
		}
	}
	void error() {
		cout << "Error" << endl;
	}

	void OOO_Kurva() {

	}
	void check() {
		if (now.second == "null" && now.first == "null") {
			cout << "LOL Error";
			exit(-1);
		}
	}


	pair <string,string> Next_Lic()
	{
		if (A.is_empty()) {
			return A.Licsema();
		}
	}
};


