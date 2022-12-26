#include <vector>
#include <deque>
#include "Analizator.h"
#include <fstream>

using namespace std;
ofstream fout("output.txt");
class Parser
{
public:
	string all_txt;
	Analizator A;
	pair <string, string> now;
	int space;
	Parser(string all_txt) {
		this->all_txt = all_txt;
		A.Add(all_txt);
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
		now = Next_Lic();
		if (now.first == "PROGRAM") {
			now = Next_Lic();
			TreeAdd(space, "Program");
			check();
			if (now.second == "ID") {
				now = Next_Lic();
				space += 5;
				TreeAdd(space, now.second, now.first);
				check();
			}
			else {
				cout << "Program name error";
				exit(-1);
			}
		}
			Descr();
			OP();
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
				Simple_ID();
			}
			else {
				break;
			}
		}
	}
	void ID() {
		if (now.second == "ID") {
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
			now = Next_Lic();
			check();
			Expr();
		}
		else {
			cout << "OPErator eroor" << endl;
			exit(-1);
		}
	}
	void Expr() {
		SimpleExp();
		while (true) {
			if (now.first == "+" || now.first == "-")
			{
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
			now = Next_Lic();
			check();
			return;
		}
		else if (now.first == "(") {
			now = Next_Lic();
			check();
			Expr();
			if (now.first == ")") {
				now = Next_Lic();
				return;
			}
			else {
				cout << "simple exp error" << endl;
				exit(-1);
			}
		}
		else if (now.second == "IntNum") {
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
		bool flag = false;
		if (now.first == "FOR") {
			now = Next_Lic();
			check();
			Simple_ID();
		}
		else {
			return;
		}
		if (now.first == "TO") {
			now = Next_Lic();
			check();
			Expr();
		}
		else {
			cout << "for_op error" << endl;
			exit(-1);
		}
		if (now.first == "DO") {
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
			if (now.second == "ID") {}
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


	pair <string, string> Next_Lic()
	{
		if (A.is_empty()) {
			return A.Licsema();
		}
		return pair<string, string>("null", "null");
	}
};


