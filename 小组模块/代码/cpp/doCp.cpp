#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include "allbinary.h"

using namespace std;

void doCp(int argc, char* argv[]) {
	char filewords[100000] = { 0 };
	bool judge_help = false;
	for (int i = 1; i < argc; i++) { //判断字符的存在性
		if (strcmp(argv[i], "-n") == 0)
			return;
		if (strcmp(argv[i], "--help") == 0)
			judge_help = true;
	}
	if (judge_help) { //--help的情况(注意：是否用数组下标“1”判断根据复合指令的情况可能会有调整)，似乎认为part3.exe也是一个argv了
		ifstream fin;
		fin.open("cp--help.txt");
		char p;
		int i = 0;
		while (fin.get(p)) {
			gTerm.strout[i] = p;
			i++;
		}
		fin.close();
	}
	else {
		bool firstfile = true;
		int filenum = 0;
		for (int j = 1; j < argc; j++) {
			if (strcmp(argv[j], "-n") != 0) filenum++;
		}
		if (filenum > 2) {
			cerr << "too many files" << endl;
			return; //linux可以复制到目录，如果有空的话可以试一试
		}
		for (int i = 1; i < argc; i++) {
			ifstream fin;
			if (strcmp(argv[i], "-n") != 0) {
				if (firstfile == true) {
					if (strcmp(argv[i], "-") == 0) {
						strcat(filewords, gTerm.strin);
					}
					else {
						char filename[100];
						if (argv[i][0] != '/') {
						strcpy(filename, gTerm.root);
						strcat(filename, gTerm.wdir);
						strcat(filename, "/");
						strcat(filename, argv[i]); //组合出路径
						}
						else {
							strcpy(filename, gTerm.root);
							strcat(filename, argv[i]);
						}
						fin.open(filename);
						if (fin == NULL) {
							cerr << "cp: " << argv[i] << ": No such file or directory" << endl;
							return; //直接退出？
						}
						else {
							char ch[50];
							while (!fin.eof()) {
								memset(ch, 0, sizeof(ch));
								fin.getline(ch, 1000, '\0');
								strcat(filewords, ch); //将第一个文件的内容储存到数组中
							}
							fin.close();
						}
					}
				}
				else {
					if (strcmp(argv[i], "-") == 0) {
						strcpy(gTerm.strout, filewords);
					}
					else {
						ofstream finn;
						char filename[100];
						if (argv[i][0] != '/') {
						strcpy(filename, gTerm.root);
						strcat(filename, gTerm.wdir);
						strcat(filename, "/");
						strcat(filename, argv[i]); //组合出路径
						}
						else {
							strcpy(filename, gTerm.root);
							strcat(filename, argv[i]);
						}
						finn.open(filename);
						if (finn != NULL) {
							finn << filewords;
							finn.close();
						}
					}
				}
				firstfile = false;
			}
		}
	}
}
