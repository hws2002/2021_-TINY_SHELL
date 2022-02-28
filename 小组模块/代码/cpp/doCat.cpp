#include <iostream>
#include <fstream>
#include <cstring>
#include "allbinary.h"
using namespace std;
char ch[100000][5000] = { 0 };
 //��������-n���ԣ�


void doCat(int argc, char* argv[]) {
	int ha = 0;
	bool judge_help = false;
	bool judge_n = false;
	bool judge_b = false;
	bool judge_s = false;
	bool judge_E = false;
	for (int i = 1; i < argc; i++) { //�ж��ַ��Ĵ�����
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
	if (judge_help) { //--help�����(ע�⣺�Ƿ��������±ꡰ1���жϸ��ݸ���ָ���������ܻ��е���)���ƺ���Ϊpart3.exeҲ��һ��argv��
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
			strcat(tempfile, "temp.txt"); //��ϳ�·��
			if (strcmp(argv[i], "-") == 0) { //����һ���µ��ļ�����strin�������������strout���������ܱ���һ��
				ofstream file;
    			file.open(tempfile);
    			file << gTerm.strin;
    			file.close();
			}
			if (strcmp(argv[i], "-n") != 0 && strcmp(argv[i], "-b") != 0 && strcmp(argv[i], "-s") != 0 && strcmp(argv[i], "-E") != 0) { //����-�Ŵ���
				if (strcmp(argv[i], "-") == 0) {
					strcpy(filename, tempfile);
				}
				else {
					if (argv[i][0] != '/') {
						strcpy(filename, gTerm.root);
						strcat(filename, gTerm.wdir);
						strcat(filename, "/");
						strcat(filename, argv[i]); //��ϳ�·��
					}
					else {
						strcpy(filename, gTerm.root);
						strcat(filename, argv[i]);
					}
				}
				fin.open(filename);
				if (fin == NULL) {
					cerr << "cat: " << argv[i] << ": No such file or directory" << endl;
					return; //�Ƿ�ֱ��������
				}
				else {
					char hang[9];
					for (int i = 0; i < 8; i++) { //��������
						hang[i] = ' ';
					}
					hang[8] = '\0';
					int h = 0; //���������

					while (!fin.eof()) {
						ha++;
						h++;
						fin.getline(ch[h - 1], 1000, '\n'); //��ȡһ��

						if (judge_n) { //n�����������ֻ����2λ����
							if (judge_s && (ch[h - 2][0] == '\n' || ch[h - 2][0] == '\0' || ch[h - 2][0] == '$') && (ch[h - 1][0] == '\n' || ch[h - 1][0] == '\0' || ch[h - 1][0] == '$')) {//����-2������
								ha--;
							}
							else {
								/*if (ha / 10 != 0) {
									hang[4] = char(ha / 10 + 48);
									hang[5] = char(ha + 48 - 10 * (ha / 10));
								}
								else hang[5] = char(ha + 48);*/

								int h0, h1, h2, h3, h4, h5; //û�и��õ��뷨��ֻ�ܲ�һ��һ����
								if (ha >= 1000000) { //������Χ
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

								if (judge_b) { //ʵ��b
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
						else if (!judge_n && judge_b) { //����ʵ��b
							if (judge_s && (ch[h - 2][0] == '\n' || ch[h - 2][0] == '\0' || ch[h - 2][0] == '$') && (ch[h - 1][0] == '\n' || ch[h - 1][0] == '\0' || ch[h - 1][0] == '$')) {//����-2������
								ha--;
							}
							else {
								/*if (ha / 10 != 0) {
									hang[4] = char(ha / 10 + 48);
									hang[5] = char(ha + 48 - 10 * (ha / 10));
								}
								else hang[5] = char(ha + 48);*/

								int h0, h1, h2, h3, h4, h5; //��ʱû�и��õ��뷨��ֻ�ܲ�һ��һ����
								if (ha >= 1000000) { //������Χ
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

						if (judge_E) { //��E
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
