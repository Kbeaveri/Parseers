#include <vector>
#include <deque>
#include "Analizator.h"
#include <fstream>
#include <map>

using namespace std;

class Parser
{
public:
	string all_txt;
	Analizator A;
	string id_begin;
	string id_end;
	Token now;
	int space;
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
		if (now.name == "PROGRAM") {
			TreeAdd(space, "Program");
			space += 5;
			now = Next_Lic();
			check();
			if (now.KW == "ID") {
				TreeAdd(space, now.KW, now.name);
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
			if (now.name == "INTEGER") {
				INTEGER();
			}
			else {
				break;
			}
		}
	}
	void INTEGER() {
		space = 6;
		if (now.name == "INTEGER") {
			TreeAdd(space, now.KW, now.name);
			now = Next_Lic();
			check();
			ID();
		}
	}
	void OP() {
		while (true) {
			if (now.name == "FOR") {
				FOR_OP();
			}
			else if (now.KW == "ID") {
				TreeAdd(space+7, "OPER");
				Simple_ID();
			}
			else {
				break;
			}
		}
	}
	void ID() {
		if (now.KW == "ID") {
			space = 24;
			TreeAdd(space,"VarList");
			space = 30;
			TreeAdd(space, now.KW, now.name);
			now = Next_Lic();
			if (now.name == ",") {
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
		if (now.KW == "ID") {
			space += 10;
			TreeAdd(space, now.KW, now.name);
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
		if (now.name == "=") {
			space += 5;
			TreeAdd(space, now.KW, now.name);
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
			if (now.name == "+" || now.name == "-")
			{
				space += 5;
				TreeAdd(space, now.KW, now.name);
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
		if (now.KW == "ID") {
			space += 5;
			TreeAdd(space, now.KW, now.name);
			now = Next_Lic();
			check();
			return;
		}
		else if (now.name == "(") {
			space += 5;
			TreeAdd(space, now.KW, now.name);
			now = Next_Lic();
			check();
			Expr();
			if (now.name == ")") {
				space += 5;
				TreeAdd(space, now.KW, now.name);
				now = Next_Lic();
				return;
			}
			else {
				cout << "simple exp error" << endl;
				exit(-1);
			}
		}
		else if (now.KW == "IntNum") {
			space += 5;
			TreeAdd(space, now.KW, now.name);
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
		if (now.name == "FOR") {
			space = 15;
			TreeAdd(space, now.KW, now.name);
			now = Next_Lic();
			check();
			Simple_ID();
		}
		else {
			return;
		}
		if (now.name == "TO") {
			space = 15;
			TreeAdd(space, now.KW, now.name);
			now = Next_Lic();
			space += 6;
			check();
			Expr();
		}
		else {
			cout << "for_op error" << endl;
			exit(-1);
		}
		if (now.name == "DO") {
			space = 15;
			TreeAdd(space, now.KW, now.name);
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
		if (now.name == "END") {
			now = Next_Lic();
			if (now.KW == "ID") {
				space += 10;
				TreeAdd(space, now.KW, now.name);
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
		if (now.KW == "null" && now.name == "null") {
			cout << "LOL Error";
			exit(-1);
		}
	}


	Token Next_Lic()
	{
		if (A.is_empty()) {
			return A.Licsema();
		}
	}
};


