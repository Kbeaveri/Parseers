
#include <algorithm>
#include "Parser.h"

using namespace std;

int main() {
    ifstream fin("input.txt");
    if (!fin.is_open()) {
        cout << "NO";
    }
    string all_txt;
    string buffer;
    while (!fin.eof()) {
        getline(fin, buffer);
        all_txt += buffer+ " ";
    }
    Parser A(all_txt);
    A.start();
}