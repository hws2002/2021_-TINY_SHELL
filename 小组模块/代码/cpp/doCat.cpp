#include <iostream>
#include <fstream>
#include <cstring>
#include "allbinary.h"
using namespace std;
char ch[100000][5000] = { 0 };
 //行数（对-n而言）


void doCat(int argc, char* argv[]) {
	int ha = 0;
	bool judge_help = false;
	bool judge_n = false;
	bool judge_b = false;
	bool judge_s = false;
	bool judge_E = false;
	for (int i = 1; i < argc; i++) { //判断字符的存在性
		if (strcmp(argv[i], "--help") == 0)
			judge_help = true;
		if (strcmp(argv[i], "-n") == 0)
			judge_n = true;
		if (strcmp(argv[i], "-b") == 0)
			judge_b = true;
		if (strcmp(argv[i], "-s") == 0)
			judge_s = true;
		if (strcmp(argv[i], "-E") == 0)
			judge_E = true;
	}
	if (judge_help) { //--help的情况(注意：是否用数组下标“1”判断根据复合指令的情况可能会有调整)，似乎认为part3.exe也是一个argv了
		ifstream fin("cat--help.txt");
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
			ifstream fin;
			char filename[100];
			char tempfile[100];
			strcpy(tempfile, gTerm.root);
			strcat(tempfile, gTerm.wdir);
			strcat(tempfile, "/");
			strcat(tempfile, "temp.txt"); //组合出路径
			if (strcmp(argv[i], "-") == 0) { //创建一个新的文件存入strin的内容再输出到strout，这样可能保险一点
				ofstream file;
    			file.open(tempfile);
    			file << gTerm.strin;
    			file.close();
			}
			if (strcmp(argv[i], "-n") != 0 && strcmp(argv[i], "-b") != 0 && strcmp(argv[i], "-s") != 0 && strcmp(argv[i], "-E") != 0) { //不是-才处理
				if (strcmp(argv[i], "-") == 0) {
					strcpy(filename, tempfile);
				}
				else {
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
				}
				fin.open(filename);
				if (fin == NULL) {
					cerr << "cat: " << argv[i] << ": No such file or directory" << endl;
					return; //是否直接跳出？
				}
				else {
					char hang[9];
					for (int i = 0; i < 8; i++) { //行数数组
						hang[i] = ' ';
					}
					hang[8] = '\0';
					int h = 0; //对整体而言

					while (!fin.eof()) {
						ha++;
						h++;
						fin.getline(ch[h - 1], 1000, '\n'); //读取一行

						if (judge_n) { //n输出行数，先只考虑2位行数
							if (judge_s && (ch[h - 2][0] == '\n' || ch[h - 2][0] == '\0' || ch[h - 2][0] == '$') && (ch[h - 1][0] == '\n' || ch[h - 1][0] == '\0' || ch[h - 1][0] == '$')) {//这里-2有问题
								ha--;
							}
							else {
								/*if (ha / 10 != 0) {
									hang[4] = char(ha / 10 + 48);
									hang[5] = char(ha + 48 - 10 * (ha / 10));
								}
								else hang[5] = char(ha + 48);*/

								int h0, h1, h2, h3, h4, h5; //没有更好的想法，只能拆开一个一个输
								if (ha >= 1000000) { //超出范围
									cerr << "there are too many rows!" << endl;
									return;
								}
								else if (ha >= 100000 && ha < 1000000) {
									h0 = ha / 100000;
									hang[0] = char(h0 + 48);
									h1 = (ha - 100000 * h0) / 10000;
									hang[1] = char(h1 + 48);
									h2 = (ha - 100000 * h0 - 10000 * h1) / 1000;
									hang[2] = char(h2 + 48);
									h3 = (ha - 100000 * h0 - 10000 * h1 - 1000 * h2) / 100;
									hang[3] = char(h3 + 48);
									h4 = (ha - 100000 * h0 - 10000 * h1 - 1000 * h2 - h3 * 100) / 10;
									hang[4] = char(h4 + 48);
									h5 = ha - 100000 * h0 - 10000 * h1 - 1000 * h2 - h3 * 100 - h4 * 10;
									hang[5] = char(h5 + 48);
								}
								else if (ha >= 10000 && ha < 100000) {
									h1 = ha / 10000;
									hang[1] = char(h1 + 48);
									h2 = (ha - 10000 * h1) / 1000;
									hang[2] = char(h2 + 48);
									h3 = (ha - 10000 * h1 - 1000 * h2) / 100;
									hang[3] = char(h3 + 48);
									h4 = (ha - 10000 * h1 - 1000 * h2 - h3 * 100) / 10;
									hang[4] = char(h4 + 48);
									h5 = ha - 10000 * h1 - 1000 * h2 - h3 * 100 - h4 * 10;
									hang[5] = char(h5 + 48);
								}
								else if (ha >= 1000 && ha < 10000) {
									h2 = ha / 1000;
									hang[2] = char(h2 + 48);
									h3 = (ha - 1000 * h2) / 100;
									hang[3] = char(h3 + 48);
									h4 = (ha - 1000 * h2 - h3 * 100) / 10;
									hang[4] = char(h4 + 48);
									h5 = ha - 1000 * h2 - h3 * 100 - h4 * 10;
									hang[5] = char(h5 + 48);
								}
								else if (ha >= 100 && ha < 1000) {
									h3 = ha / 100;
									hang[3] = char(h3 + 48);
									h4 = (ha - h3 * 100) / 10;
									hang[4] = char(h4 + 48);
									h5 = ha - h3 * 100 - h4 * 10;
									hang[5] = char(h5 + 48);
								}
								else if (ha >= 10 && ha < 100) {
									h4 = ha / 10;
									hang[4] = char(h4 + 48);
									h5 = ha - h4 * 10;
									hang[5] = char(h5 + 48);
								}
								else if (ha >= 1 && ha < 10) {
									hang[5] = char(ha + 48);
								}

								if (judge_b) { //实现b
									if (ch[h - 1][0] != '\n' && ch[h - 1][0] != '\0') {
										strcat(gTerm.strout, hang);
									}
									else {
										ha--;
									}
								}
								else strcat(gTerm.strout, hang);
							}
						}
						else if (!judge_n && judge_b) { //单独实现b
							if (judge_s && (ch[h - 2][0] == '\n' || ch[h - 2][0] == '\0' || ch[h - 2][0] == '$') && (ch[h - 1][0] == '\n' || ch[h - 1][0] == '\0' || ch[h - 1][0] == '$')) {//这里-2有问题
								ha--;
							}
							else {
								/*if (ha / 10 != 0) {
									hang[4] = char(ha / 10 + 48);
									hang[5] = char(ha + 48 - 10 * (ha / 10));
								}
								else hang[5] = char(ha + 48);*/

								int h0, h1, h2, h3, h4, h5; //暂时没有更好的想法，只能拆开一个一个输
								if (ha >= 1000000) { //超出范围
									cerr << "there are too many rows!" << endl;
									return;
								}
								else if (ha >= 100000 && ha < 1000000) {
									h0 = ha / 100000;
									hang[0] = char(h0 + 48);
									h1 = (ha - 100000 * h0) / 10000;
									hang[1] = char(h1 + 48);
									h2 = (ha - 100000 * h0 - 10000 * h1) / 1000;
									hang[2] = char(h2 + 48);
									h3 = (ha - 100000 * h0 - 10000 * h1 - 1000 * h2) / 100;
									hang[3] = char(h3 + 48);
									h4 = (ha - 100000 * h0 - 10000 * h1 - 1000 * h2 - h3 * 100) / 10;
									hang[4] = char(h4 + 48);
									h5 = ha - 100000 * h0 - 10000 * h1 - 1000 * h2 - h3 * 100 - h4 * 10;
									hang[5] = char(h5 + 48);
								}
								else if (ha >= 10000 && ha < 100000) {
									h1 = ha / 10000;
									hang[1] = char(h1 + 48);
									h2 = (ha - 10000 * h1) / 1000;
									hang[2] = char(h2 + 48);
									h3 = (ha - 10000 * h1 - 1000 * h2) / 100;
									hang[3] = char(h3 + 48);
									h4 = (ha - 10000 * h1 - 1000 * h2 - h3 * 100) / 10;
									hang[4] = char(h4 + 48);
									h5 = ha - 10000 * h1 - 1000 * h2 - h3 * 100 - h4 * 10;
									hang[5] = char(h5 + 48);
								}
								else if (ha >= 1000 && ha < 10000) {
									h2 = ha / 1000;
									hang[2] = char(h2 + 48);
									h3 = (ha - 1000 * h2) / 100;
									hang[3] = char(h3 + 48);
									h4 = (ha - 1000 * h2 - h3 * 100) / 10;
									hang[4] = char(h4 + 48);
									h5 = ha - 1000 * h2 - h3 * 100 - h4 * 10;
									hang[5] = char(h5 + 48);
								}
								else if (ha >= 100 && ha < 1000) {
									h3 = ha / 100;
									hang[3] = char(h3 + 48);
									h4 = (ha - h3 * 100) / 10;
									hang[4] = char(h4 + 48);
									h5 = ha - h3 * 100 - h4 * 10;
									hang[5] = char(h5 + 48);
								}
								else if (ha >= 10 && ha < 100) {
									h4 = ha / 10;
									hang[4] = char(h4 + 48);
									h5 = ha - h4 * 10;
									hang[5] = char(h5 + 48);
								}
								else if (ha >= 1 && ha < 10) {
									hang[5] = char(ha + 48);
								}

								if (ch[h - 1][0] != '\n' && ch[h - 1][0] != '\0') {
									strcat(gTerm.strout, hang);
								}
								else {
									ha--;
								}
							}
						}
						bool judge__n = false;

						if (judge_E) { //有E
							for (int j = 0; j < 50; j++) {
								if (ch[h - 1][j] == '\n') {
									ch[h - 1][j] = '$';
									ch[h - 1][j + 1] = '\n';
									break;
								}
								else if (ch[h - 1][j] == '\0') {
									ch[h - 1][j] = '$';
									ch[h - 1][j + 1] = '\0';
									break;
								}
							}
						}
						if (judge_s) {
							if ((ch[h - 2][0] == '\n' || ch[h - 2][0] == '\0' || ch[h - 2][0] == '$') && (ch[h - 1][0] == '\n' || ch[h - 1][0] == '\0' || ch[h - 1][0] == '$')) {

							}
							else {
								strcat(gTerm.strout, ch[h - 1]);
								strcat(gTerm.strout, "\n");
							}
						}
						else {
							strcat(gTerm.strout, ch[h - 1]);
							strcat(gTerm.strout, "\n");
						}
					}
					fin.close();
				}
			}
			remove(tempfile);
		}
	}
}
