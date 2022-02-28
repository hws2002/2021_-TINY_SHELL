#include <iostream> //记得修改使得汉字也能操作
#include <fstream>
#include <cstring>
#include "allbinary.h"
using namespace std;

void doTee(int argc, char* argv[]) {
	bool judge_help = false;//有help就不管文件
	bool judge_a = false;//判断a，任意位置均可
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--help") == 0)
			judge_help = true;
		if (strcmp(argv[i], "-a") == 0)
			judge_a = true;
	}
	if (judge_help) { //--help的情况(注意：是否用数组下标“1”判断根据复合指令的情况可能会有调整)，似乎认为part3.exe也是一个argv了
		ifstream fin("tee--help.txt");
		char p;
		int i = 0;
		while (fin.get(p)) {
			gTerm.strout[i] = p;
			i++;
		}
		fin.close();
	}
	else {
		for (int i = 1; i < argc; i++) {
			if (strcmp(argv[i], "-a") != 0) //不是-a才处理
			{
				if (!judge_a) { //没有-a则生成的文件直接覆盖
					if (strcmp(argv[i], "-") != 0) {
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
						ofstream fout(filename);
						fout << gTerm.strin;
						fout.close();
					}
				}
				else {
					if (strcmp(argv[i], "-") != 0) {
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
						ofstream fout1(filename,ios::app);
						if (fout1 != NULL) {
							fout1 << gTerm.strin;
							fout1.close();
						}
					}
				}
			}
		}
		strcpy(gTerm.strout, gTerm.strin);
	}
}
