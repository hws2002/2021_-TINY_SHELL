#include <iostream> //�ǵ��޸�ʹ�ú���Ҳ�ܲ���
#include <fstream>
#include <cstring>
#include "allbinary.h"
using namespace std;

void doTee(int argc, char* argv[]) {
	bool judge_help = false;//��help�Ͳ����ļ�
	bool judge_a = false;//�ж�a������λ�þ���
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--help") == 0)
			judge_help = true;
		if (strcmp(argv[i], "-a") == 0)
			judge_a = true;
	}
	if (judge_help) { //--help�����(ע�⣺�Ƿ��������±ꡰ1���жϸ��ݸ���ָ���������ܻ��е���)���ƺ���Ϊpart3.exeҲ��һ��argv��
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
			if (strcmp(argv[i], "-a") != 0) //����-a�Ŵ���
			{
				if (!judge_a) { //û��-a�����ɵ��ļ�ֱ�Ӹ���
					if (strcmp(argv[i], "-") != 0) {
						char filename[100];
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
						strcat(filename, argv[i]); //��ϳ�·��
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
