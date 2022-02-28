#include <iostream>
#include <fstream>
#include <cstring>
#include "allbinary.h"
using namespace std;

void doCd(int argc, char* argv[]) {
	bool judge_help = false;
	for (int i = 1; i < argc; i++) { //判断字符的存在性
		if (strcmp(argv[i], "--help") == 0)
			judge_help = true;
	}
	if (judge_help) {
		ifstream fin("cd--help.txt");
		char p;
		int i = 0;
		while (fin.get(p)) {
			gTerm.strout[i] = p;
			i++;
		}
		fin.close();
	}
	else {
		if (argv[1][0] == '/') {
			char store1[10][10] = { 0 };
				int c = 0;
				int d = 0;
				int len1 = strlen(argv[1]);
				for (int i = 0; i < len1; i++) {
					if (argv[1][i] == '/') {
						d = 0;
						c++;
					}				
					store1[c][d] = argv[1][i];
					d++;
				}
			if (strcmp(store1[c], "/..") != 0){
				strcpy(gTerm.wdir, argv[1]);
				} 
			else
			{
				char newstr[100] = { 0 };
				for (int m = 0; m < c - 1; m++) {
					strcat(newstr, store1[m]);
				}
				strcpy(gTerm.wdir, newstr);
				if (strcmp(gTerm.wdir, "") == 0){ 
					strcat(gTerm.wdir, "/"); 
				}
			}
		}
		else if (strcmp(argv[1], ".") == 0) {
			return;
		}
		else if (strcmp(argv[1], "..") == 0) { //上一层
			if (strcmp("/", gTerm.wdir) == 0) { //工作目录和根目录在同一层
				cerr << "it is the root file!" << endl;
				return;
			}
			else {
				char store[10][10] = { 0 };
				int a = 0;
				int b = 0;
				int len = strlen(gTerm.wdir);
				for (int i = 0; i < len; i++) {
					if (gTerm.wdir[i] == '/') {
						b = 0;
						a++;
					}				
					store[a][b] = gTerm.wdir[i];
					b++;
				}
				char newstr[100] = { 0 };
				for (int c = 0; c < a; c++) {
					strcat(newstr, store[c]);
				}
				strcpy(gTerm.wdir, newstr);
			}
			if (strcmp(gTerm.wdir, "") == 0)
				strcat(gTerm.wdir, "/");
		}
		else {
			if (strcmp(gTerm.root, "/") != 0) 
			{
				strcat(gTerm.wdir, argv[1]);
			}
			else
				strcat(gTerm.wdir, argv[1]);
		}
	}
}
