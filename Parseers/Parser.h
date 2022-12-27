#include <vector>
#include <deque>
#include "Analizator.h"
#include <fstream>
#include <map>
#include <iterator>
#include <stack>

using namespace std;
class Semantik {
public:
	string id_begin;
	string id_end;
	ofstream poliz;
	ofstream semant;
	map <string,int> determinate;
	vector <pair<string,string>> expr;
	Semantik() {
		poliz.open("Poliz.txt");
		semant.open("semant.txt");
	}
	void Set_begin(string id) {
		id_begin = id;
	}
	void Set_end(string id) {
		id_end = id;
	}
	void Descr(string id) {
		determinate[id]++;
	}
	void Check_descr(int kol, int nomer) {
		semant << "Проверка Descr" << endl;
		map <string, int> ::iterator it = determinate.begin();
		for (it; it != determinate.end(); it++) {
			if (it->second > 1) {
				semant << "Переменная (" << it->first << ") объявлена" << it->second << " на pos : " << kol << "  " << nomer << endl;
			}
		}
	}
	void Check_id(string id,int kol,int nomer) {
		if (determinate[id] == 0) {
			semant << "Неиницилизированная переменная " << id << " на pos: " << kol << "  "<< nomer << endl;
		}

	}
	void Check_end() {
		semant << "Проверка Begin end" << endl;
		if (id_begin == id_end) {
			semant << "end_id == begin_id" << endl;
		}
		else {
			semant << "end_id != begin_id" << endl<< "end_id = "<< id_end << "      " << "begin id = "<< id_begin;
		}
	}
	void Deycstra() {
		stack <string> lol;
		for (int i = 0; i < expr.size(); i++) {
			cout << expr[i].first << " ";
		}
		cout << endl;
		int i = 0;
		while (i < expr.size()) {
			if (expr[i].second == "IntNum" || expr[i].second == "ID") {
				poliz << expr[i].first << " ";
				i++;
				continue;
			}
			if (expr[i].first == "+" || expr[i].first == "-") {
				while (!lol.empty() && lol.top() != "="&& lol.top() != "(") {
					poliz << lol.top() << " ";
					lol.pop();
				}
				lol.push(expr[i].first);
				i++;
				continue;
			}
			if (expr[i].first == "=") {
				lol.push("=");
				i++;
				continue;
			}
			if (expr[i].first == "(") {
				lol.push("(");
				i++;
				continue;
			}
			if (expr[i].first == ")") {
				while (!lol.empty() && lol.top() != "(") {
					poliz << lol.top() << " ";
					lol.pop();
				}
				lol.pop();
				i++;
				continue;
			}
		}
		while (!lol.empty()) {
			poliz << lol.top() << " ";
			lol.pop();
		}
		expr.clear();
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
	Semantik sm;
	int kol, nomer = 1;
	int kolvo = 0;
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
			nomer++;
			check();
			if (now.second == "ID") {
				sm.id_begin = now.first;
				sm.poliz << "PROGRAM  " << now.first << endl;
				TreeAdd(space, now.second, now.first);
				now = Next_Lic();
				space += 5;
				kol++;
				nomer = 1;
				check();
			}
			else {
				cout << "Program name error строка " << kol << "лекскма " << nomer;
				exit(-1);
			}
		}
		space = 3;
		TreeAdd(space, "Description");
		Descr();
		sm.Check_descr(kol,nomer);
		space = 3;
		TreeAdd(space, "Operator");
		sm.semant << "Проверка OP" << endl;
		while (now.first == "FOR" || now.second == "ID") {
			OP();
			sm.poliz << endl;
			kol++;
			nomer = 1;
		}
		space = 3;
		TreeAdd(space, "END");
		End();
		sm.Check_end();
		A.hash.fout();
		cout << "NO Syntax ERROR,good game" << endl;
	}
	void Descr() {
		while (true) {
			if (now.first == "INTEGER") {
				INTEGER();
				sm.poliz << kolvo << "INTEGER DECL " << endl;
				kolvo = 0;
			}
			else {
				break;
			}
			kol++;
			nomer = 1;
		}
	}
	void INTEGER() {
		space = 6;
		if (now.first == "INTEGER") {
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			nomer++;
			check();
			ID();
		}
	}
	void ID() {
		kolvo++;
		if (now.second == "ID") {
			space = 24;
			TreeAdd(space, "VarList");
			space = 30;
			sm.poliz << now.first << " ";
			sm.Descr(now.first);
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			nomer++;
			if (now.first == ",") {
				now = Next_Lic();
				nomer++;
				check();
				ID();
			}
		}
		else {
			cout << "ID eroor" << kol << nomer << endl;
			exit(-1);
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
				break;
			}
			else {
				break;
			}
		}
	}
	void Simple_ID() {
		if (now.second == "ID") {
			sm.Check_id(now.first, kol, nomer);
			sm.expr.push_back(make_pair(now.first, now.second));
			space += 10;
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			nomer++;
			check();
			Operators();
			sm.Deycstra();
		}
		else {
			cout << "simple ID eroor pos : " << kol << " " << nomer << endl;
			exit(-1);
		}
	}
	void Operators() {
		if (now.first == "=") {
			space += 5;
			sm.expr.push_back(make_pair(now.first, now.second));
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			nomer++;
			check();
			Expr();
		}
		else {
			cout << "Operator eroor pos : " << kol << " " << nomer << endl;
			exit(-1);
		}
	}
	void Expr() {
		SimpleExp();
		while (true) {
			if (now.first == "+" || now.first == "-")
			{
				sm.expr.push_back(make_pair(now.first,now.second));
				space += 5;
				TreeAdd(space, now.second, now.first);
				now = Next_Lic();
				nomer++;
				check();
				Expr();
			}
			else {
				break;
			}
		}
	}
	void SimpleExp() {
		if (now.second == "ID") {
			sm.expr.push_back(make_pair(now.first, now.second));
			sm.Check_id(now.first,kol, nomer);
			space += 5;
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			nomer++;
			check();
			return;
		}
		else if (now.first == "(") {
			sm.expr.push_back(make_pair(now.first, now.second));
			space += 5;
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			nomer++;
			check();
			Expr();
			if (now.first == ")") {
				sm.expr.push_back(make_pair(now.first, now.second));
				space += 5;
				TreeAdd(space, now.second, now.first);
				now = Next_Lic();
				nomer++;
				return;
			}
			else {
				cout << "simple exp errorpos : " << kol << " " << nomer << endl;
				exit(-1);
			}
		}
		else if (now.second == "IntNum") {
			sm.expr.push_back(make_pair(now.first, now.second));
			space += 5;
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			nomer++;
			check();
			return;
		}
		else {
			cout << "simple exp error pos : " << kol << " " << nomer << endl;
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
			nomer++;
			check();
			Simple_ID();
			sm.Deycstra();
			sm.poliz << " m1  DEFL ";
		}
		else {
			return;
		}
		if (now.first == "TO") {
			space = 15;
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			nomer++;
			space += 6;
			check();
			Expr();
			sm.Deycstra();
			sm.poliz << " m2 BF ";
		}
		else {
			cout << "for_op error" << endl;
			exit(-1);
		}
		if (now.first == "DO") {
			space = 15;
			TreeAdd(space, now.second, now.first);
			now = Next_Lic();
			nomer++;
			check();
			OP();
			sm.Deycstra();
			sm.poliz << " m1 BRL " << endl;
		}
		else {
			error();
			exit(-1);
		}
		OP();
	}
	void End() {
		if (now.first == "END") {
			now = Next_Lic();
			if (now.second == "ID") {
				space += 10;
				sm.id_end = now.first;
				TreeAdd(space, now.second, now.first);
				sm.poliz <<endl <<"END  " << now.first << endl;
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


