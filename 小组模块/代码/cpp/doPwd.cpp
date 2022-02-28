#include <iostream>
#include <fstream>
#include <cstring>
#include "allbinary.h"
using namespace std;

void doPwd(int argc, char* argv[]) {
	bool judge_help = false;
	for (int i = 1; i < argc; i++) { //ÅÐ¶Ï×Ö·ûµÄ´æÔÚÐÔ
		if (strcmp(argv[i], "--help") == 0)
			judge_help = true;
	}
	if (judge_help) {
		ifstream fin("pwd--help.txt");
		char p;
		int i = 0;
		while (fin.get(p)) {
			gTerm.strout[i] = p;
			i++;
		}
		fin.close();
	}
	else {
		strcpy(gTerm.strout, gTerm.wdir);
		strcat(gTerm.strout, "\n");
	}
}
