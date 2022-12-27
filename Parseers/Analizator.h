#include <iostream>
#include <string>
#include <vector>
#include "HashTable.h"

using namespace std;
class Token {
public:
    string name;
    string KW;
    bool determined = false;
    Token();
    Token(string name, string KW) {
        this->name = name;
        this->KW = KW;
    }
    void Set(string name, string KW) {
        this->name = name;
        this->KW = KW;
    }
    const string&  operator ()() {
        return this->name;
    }
    friend bool operator==(const Token & left, const Token & right);
};
bool operator ==(const Token& left, const Token& right) {
    return left.name == right.name;
}
class Analizator {
public:
    vector <string> KW = { "PROGRAM","END","INTEGER","FOR","TO","DO" };
    vector <string> SEP = { "|n","\t" };
    vector <string> Raz = {")","(", "=" , "+" };
    string all_txt;
    string buf;
    Token now;
    HashTable <Token> hash;
    int num = 0;
    Analizator() {
        this->all_txt = "";
    }
    Analizator(string all_txt) {
        this->all_txt = all_txt;
    }
    void Add(string a) {
        this->all_txt = a;
    }
    string Go_next() {
        int i = num;
        string buf;
        while (all_txt[i] != ' ' && all_txt.size() > i) {
            buf += all_txt[i];
            i++;
        }
        num = i + 1;
        return buf;
    };
    int DKA() {
        int state = 1;
        buf = Go_next();
        bool isnum = false;
        if (isdigit(buf[0])) {
            state = 2;
        }
        else if (isalpha(buf[0])) {
            state = 3;
        }
        else {
            return 4;
        }
        switch (state)
        {
        case 2:
            for (int i = 0; i < buf.size(); i++) {
                if (!isdigit(buf[i])) {
                    return 4;
                }
            }
            return 2;
        case 3:
            for (int i = 0; i < buf.size(); i++) {
                if (!isalpha(buf[i])) {
                    return 4;
                }
            }
            return 3;
        }
    }
    Token Licsema() {
        int a = DKA();
        if (a == 2) {
            now.Set(buf, "IntNum");
            hash.Add(now);
            return now;
        }
        if (a == 3) {
            if (find(KW.begin(), KW.end(), buf) != KW.end()) {
                now.Set(buf, "KEY WORLD");
                hash.Add(now);
                return now;
            }
            else {
                now.Set(buf, "ID");
                hash.Add(now);
                return now;
            }
        }
        if (a == 4) {
            if (find(Raz.begin(), Raz.end(), buf) != Raz.end()) {
                now.Set(buf, "OPERATOR");
                hash.Add(now);
                return now;
            }
            if (buf == "\n") {
                now.Set(buf, "SEP");
                hash.Add(now);
                return now;
            }
            else {
                now.Set(buf, "ERROR");
                hash.Add(now);
                return now;
            }
        }

    }
    bool is_empty() {
        if (num < all_txt.size()) {
            return true;
        }
        else {
            return false;
        }
    }
};
