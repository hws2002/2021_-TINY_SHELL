#pragma once
struct Terminal {
	char user[100]; // �û���
	char mach[100]; // �������
	char root[100]; // ��Ŀ¼
	char wdir[100]; // ����Ŀ¼
	char strin[100000]; // �ض����׼����
	char strout[100000]; // �ض����׼���
}; 

extern Terminal gTerm;
