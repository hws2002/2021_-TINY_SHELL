#include <iostream> 
#include <fstream>
#include <string.h>
#include <algorithm>
#include <vector>
#include "allbinary.h"
using namespace std;

void doDiff(int argc, char *argv[])     //���ò�����һЩѡ������ļ��� 
{
	int m = 1, n = 1;
	string str[argc], Root = gTerm.root, Wdir = gTerm.wdir;			//ת��Ϊstring�࣬�������(��ʵ�Ǳ���֪ʶ����)  
	bool mood[8];
	for(int i = 0; i < 8; i++)
		mood[i] = 0;
	for(int i = 0; i < argc; i++)
		str[i] = argv[i];
	/*
	��¼diffָ���ѡ�����Ϊ 
	--help����ʾ������
	-b�������ո��ַ��Ĳ�ͬ��
	-B�������հ��С�
	-i��������Сд�Ĳ�ͬ��
	-q������ʾ���޲��죬����ʾ��ϸ����Ϣ���޲���ʱ����ʾ���в���ʱ��ʾ��File [�ļ� 1] and [�ļ� 2] differ����
	-w������ȫ���Ŀո��ַ���
	-I[�ַ���]���������ļ���ĳ����������ͬ�����⼸��ͬʱ����ѡ����ָ���ַ���������ʾ�������ļ��Ĳ��졣
	*/
	if(str[0] == "diff")
	{
		for(int i = 1; i < argc; i++)
			if(str[i] == "--help") mood[1] = 1;				//����� 
		for(int i = 1; i < argc - 2; i++)
			if(str[i] == "-b") mood[2] = 1;					//����� 
		for(int i = 1; i < argc - 2; i++)
			if(str[i] == "-B") mood[3] = 1;					//����� 
		for(int i = 1; i < argc - 2; i++)
			if(str[i] == "-i") mood[4] = 1;					//����� 
		for(int i = 1; i < argc - 2; i++)
			if(str[i] == "-q") mood[5] = 1;					//����� 
		for(int i = 1; i < argc - 2; i++)
			if(str[i] == "-w") mood[6] = 1;					//����� 
		for(int i = 1; i < argc - 2; i++)
			if(str[i].substr(0,2) == "-I")					//ò������ˣ����Ǻ�ȷ�� 
			{
				mood[7] = 1;
				str[0] = str[i].erase(0, 2);				//�ض��ַ�����Ϊstr[0]
				str[i] = "-I" + str[i];						//��Ȼ�ܱ�������ֻ�������� 
			}
		for(int i = 1; i < argc - 2; i++)					//�պ�mood[0]������������ָ���ǲ��ǺϷ��� 
		{ 
			if(str[i] == "--help") mood[0] = 1;
			if(str[i] == "-b") mood[0] = 1;
			if(str[i] == "-B") mood[0] = 1;
			if(str[i] == "-i") mood[0] = 1;
			if(str[i] == "-q") mood[0] = 1;
			if(str[i] == "-w") mood[0] = 1;
			if(str[i].substr(0, 2) == "-I") mood[0] = 1;
			if(mood[0]) mood[0] = 0;
			else											//��cerr�������˵���ĵ�������Ҫ��ģ�������д��
			{
				cerr << "diff:[Error] " << str[i] << " is unrecognizable\n";
				return; 
			}
		}
		//diff--help 
		if(argc == 2)
		{
		if(mood[1])
		{
			string temp, help;
			ifstream fin;
			fin.open("Diff--help.txt");
			while(!fin.eof())
			{
				getline(fin, temp);
				help = help + temp + "\n";
			}
			fin.close();
			for(int i = 0; i < help.length()-1; i++)
			gTerm.strout[i] = help[i]; 
			return;
		}
		else
		{
			cerr << "diff:[Error] " << str[1] << " is unrecognizable\n";
			return;
		}
	  	}
		vector<string> str1, str2, share, shared;		//shared���ڴ洢����������� 
		str1.push_back("empty");				//����ʹ���±�Ϊ����ַ���(��Ϊ����ĳ���д���ˣ����ø�) 
		str2.push_back("empty");
		shared.push_back("empty");
		share.push_back("empty");
		//�ļ������� 
		if(str[argc-2].compare(0, 1, "/") == 0)
		str[argc-2] = Root + str[argc-2];
		else
		{
			if(Wdir.compare(Wdir.length()-1, 1, "/") == 0)
			str[argc-2] = Root + Wdir + str[argc-2];
			else
			str[argc-2] = Root + Wdir + "/" + str[argc-2];
		}
		if(str[argc-1].compare(0, 1, "/") == 0)
		str[argc-1] = Root + str[argc-1];
		else
		{
			if(Wdir.compare(Wdir.length()-1, 1, "/") == 0)
			str[argc-1] = Root + Wdir + str[argc-1];
			else
			str[argc-1] = Root + Wdir + "/" + str[argc-1];
		}
		int end = 0;										//��¼strin���±꣬���ڱȽ�strin�е������ļ� 
		if(str[argc-2].compare(str[argc-2].length()-1,1,"-") == 0)
		{
			int i = 0;
			string temp;
			while(gTerm.strin[i] != '\0')
			{
				if(gTerm.strin[i] == '\n')
				str1.push_back("");
				for(i; gTerm.strin[i] != '\n'; i++)
				temp += gTerm.strin[i];
				str1.push_back(temp);
				i++;
			}
			m = str1.size();
			end = i + 1;
		}
		else
		{
			ifstream fin;
			fin.open(str[argc-2].c_str());						//����ļ�����û�ģ���Ϊ���˾ʹ򲻿��ˣ�û����root 
			if(!fin)											//�ĺ��ˣ�������ȫ�����ģ������� 
			{
				cerr << "diff:[Error]There is no such file or directory\n" ;
				return;
			}	
			for(int i = 1; !fin.eof(); i++)
			{
				string temp;
				getline(fin, temp);							//���ļ�1��ÿһ�д���str1,ͬʱ�õ�m=����+1 
				if(mood[3] && temp.length() == 0)			//-B����¼���� 
				continue;
				str1.push_back(temp);
				m++;
			}
			fin.close();
		}
		if(str[argc-1].compare(str[argc-1].length()-1,1,"-") == 0)				//��̫���strin(�����ƶ�) 
		{
			int i = end;
			string temp;
			while(gTerm.strin[i] != '\0')
			{
				if(gTerm.strin[i] == '\n')
				str2.push_back("");
				for(i; gTerm.strin[i] != '\n'; i++)
				temp += gTerm.strin[i];
				str2.push_back(temp);
				i++;
			}
			n = str2.size();
		}
		else
		{
			ifstream fin;
			fin.open(str[argc - 1].c_str());					//����ļ���Ҳû����ԭ��ͬ�� 
			if(!fin)											//���ˣ�ͬ�ϲ�ȷ�� 
			{
				cerr << "diff:[Error]There is no such file or directory\n";
				return;
			}
			for(int i = 1; !fin.eof(); i++)
			{
				string temp;
				getline(fin, temp);							//���ļ�2��ÿһ�д���str2��ͬʱ�õ�n=����+1
				if(mood[3] && temp.length() == 0)			//-B����¼���� 
				continue;
				str2.push_back(temp);
				n++;
			}
			fin.close();
		}
		str[argc - 2] = argv[argc - 2];
		str[argc - 1] = argv[argc - 1];						//���ļ������˻�ȥ���������-q���ʱʹ�� 
		vector<string> nstr1(str1), nstr2(str2);
		/*���ﶨ�������ַ������飬���ڸ��ݹ���ѡ�����ȥ���ո��ַ���ת��ΪСд�Ȳ��������бȽ� 
		  �������������ʱ�����ԭ�ļ��е�����*/
		if(mood[2])							//-b������ո��滻Ϊһ���ո� 
		{
			for(int i = 1; i < m; i++)
			{
				int j = 0;
				while((j = nstr1[i].find("  ", j)) != string::npos)
				nstr1[i].erase(j, 1); 
			}
			for(int i = 1; i < n; i++)
			{
				int j = 0;
				while((j = nstr2[i].find("  ", j)) != string::npos)
				nstr2[i].erase(j, 1);
			}
		} 
		if(mood[4])							//-i�����Ǵ�Сд(ȫת��ΪСд��diff) 
		{
			for(int i = 1; i < m; i++)
			transform(nstr1[i].begin(), nstr1[i].end(), nstr1[i].begin(), ::tolower);
			for(int i = 1; i < n; i++)
			transform(nstr2[i].begin(), nstr2[i].end(), nstr2[i].begin(), ::tolower);
		}
		if(mood[6])							//-w����ȫ���Ŀո��ַ�
		{
			for(int i = 1; i < m; i++)
			{
				int j = 0;
				while((j = nstr1[i].find(" ", j)) != string::npos)
				nstr1[i].erase(j, 1);
			}
			for(int i = 1; i < n; i++)
			{
				int j = 0;
				while((j = nstr2[i].find(" ", j)) != string::npos)
				nstr2[i].erase(j, 1);
			}
		}
		if(mood[7])							//������ض��ַ���,�������ض��ַ�(str[0])������str[0]�滻 
		{
			for(int i = 1; i < m; i++)
				if(nstr1[i].find(str[0]) != string::npos)
					nstr1[i] = str[0];
			for(int i = 1; i < n; i++)
				if(nstr2[i].find(str[0]) != string::npos)
					nstr2[i] = str[0];
		}
		//LCS����������� 
		//��ʼ�����飺dp��¼���������г��ȣ�f���ڻ����ҵ����������� 
		int dp[m][n], f[m][n];
		dp[0][0] = 0;
		for(int i = 1; i < m; i++)
			dp[i][0] = 0;
		for(int i = 1; i < n; i++)
			dp[0][i] = 0;
		for(int i = 1; i < m; i++)
			for(int j = 1; j < n; j++)
			{
				
				if(nstr1[i] == nstr2[j])
				{
					dp[i][j] = dp[i - 1][j - 1] + 1;
					f[i][j] = 1;
				}	
				else if(dp[i][j-1]<=dp[i-1][j])
					 {
					  	dp[i][j] = dp[i-1][j];
						f[i][j] = 2; 	
					 }
					else
					  {
					   	 dp[i][j] = dp[i][j-1];
						 f[i][j] = 3;		 
					  }
			}
		int l1 = m - 1, l2 = n - 1, l0 = dp[m - 1][n - 1];
		if(mood[5])												//-q����ʾ���޲��� 
		{
			if((l1 == l2) && (l2 == l0))
			return;
			else
			{
				string diff = "File "+str[argc-2]+" and File "+str[argc-1]+" differ";
				for(int i = 0; i < diff.length(); i++)
				gTerm.strout[i] = diff[i];
				gTerm.strout[diff.length()] = '\n'; 
				return;
			}
		}
		int c = 0;												//���ڼ�¼strout���±� 
		if((l1 == l2) && (l2 == l0))
		{
			string temp = "There is no difference!";
			for(int i = 0; i < temp.length(); i++)
			gTerm.strout[c++] = temp[i];
			gTerm.strout[c++] = '\n'; 
			return;
		}
		if(l0 == 0)
		{
			if(m == 1)
			{
				gTerm.strout[c++] = '0';
				gTerm.strout[c++] = 'a';
				gTerm.strout[c++] = '1';
				gTerm.strout[c++] = '\n';
			}
			else if(n == 1)
			{
				gTerm.strout[c++] = '1';
				gTerm.strout[c++] = 'd';
				gTerm.strout[c++] = '0';
				gTerm.strout[c++] = '\n';
			}
			else
			{
				gTerm.strout[c++] = '1';
				gTerm.strout[c++] = 'c';
				gTerm.strout[c++] = '1';
				gTerm.strout[c++] = '\n';
			}
			for(int i = 1; i < m; i++)
			{
				gTerm.strout[c++] = '<';
				for(int j = 0; j < str1[i].length(); j++)
				gTerm.strout[c++] = str1[i][j];
				gTerm.strout[c++] = '\n';
			}
			if(m > 1 && n > 1)
			{
				gTerm.strout[c++] = '-';
				gTerm.strout[c++] = '-';
				gTerm.strout[c++] = '-';
				gTerm.strout[c++] = '\n';
			}
			for(int i = 1; i < n; i++)
			{
				gTerm.strout[c++] = '>';
				for(int j = 0; j < str2[i].length(); j++)
				gTerm.strout[c++] = str2[i][j];
				gTerm.strout[c++] = '\n';
			}
			return;
 		}
		while(l1 > 0 && l2 > 0)
		{
			if(f[l1][l2] == 1)
			{
				share.push_back(nstr1[l1]);
				l1--;
				l2--;
			}
			else if(f[l1][l2] == 2) l1--;
			else if(f[l1][l2] == 3) l2--;
		}
		for(l0; l0 > 0; l0--)
			shared.push_back(share[l0]);				//������������˳������(֮ǰ��share�ǵ����) 
		l0 = dp[m-1][n-1];
		int a = 1, b = 1;
		for(int i = 1; i <= l0; i++)
		{
			if(nstr1[a] == shared[i])
			{
				if(nstr2[b] != shared[i])
				{
					int x = 0, y = 0, temp = a - 1;
					for(int z = temp; z >= 1; z = z / 10)
						x++;
					y = x;
					while(x > 0)
					{
						gTerm.strout[c + x - 1] = '0' + temp % 10;
						temp = temp / 10;
						x--;
					}
					c = c + y;
					y = 0;
					if(a==1) gTerm.strout[c++] = '0';
					gTerm.strout[c++] = 'a';
					temp = b;
					for(int z = temp; z >= 1; z = z / 10)
						x++;
					y = x;
					while(x > 0)
					{
						gTerm.strout[c + x - 1] = '0' + temp % 10;
						temp = temp / 10;
						x--;
					}
					c = c + y;
					y = 0;
					gTerm.strout[c++] = '\n';
					for(b; nstr2[b] != shared[i]; b++)
					{
						gTerm.strout[c++] = '>';
						for(int j = 0; j < str2[b].length(); j++)
						gTerm.strout[c++] = str2[b][j];
						gTerm.strout[c++] = '\n';
						a++;
					}
				}
				else
				{
					a++;
					b++;
				}
			}
			else if(nstr2[b] == shared[i])
				 {
				 	int x = 0, y = 0, temp = a;
					for(int z = temp; z >= 1; z = z / 10)
						x++;
					y = x;
					while(x > 0)
					{
						gTerm.strout[c + x - 1] = '0' + temp % 10;
						temp = temp / 10;
						x--;
					}
					c = c + y;
					y = 0;
				 	gTerm.strout[c++] = 'd';
					temp = b - 1;
					for(int z = temp; z >= 1; z = z / 10)
						x++;
					y = x;
					while(x > 0)
					{
						gTerm.strout[c + x - 1] = '0' + temp % 10;
						temp = temp / 10;
						x--;
					}
					c = c + y;
					y = 0;
					if(b==1) gTerm.strout[c++] = '0';
				 	gTerm.strout[c++] = '\n';
				 	for(a; nstr1[a] != shared[i]; a++)
				 	{
				 		gTerm.strout[c++] = '<';
						for(int j = 0; j < str1[a].length(); j++)
						gTerm.strout[c++] = str1[a][j];
						gTerm.strout[c++] = '\n';
						a++;
						b++;
					}
				 }
				 else if(nstr2[b] != shared[i])
				 {
				 	int x = 0, y = 0, temp = a;
					for(int z = temp; z >= 1; z = z / 10)
						x++;
					y = x;
					while(x > 0)
					{
						gTerm.strout[c + x - 1] = '0' + temp % 10;
						temp = temp / 10;
						x--;
					}
					c = c + y;
					y = 0;
				 	gTerm.strout[c++] = 'c';
				 	temp = b;
					for(int z = temp; z >= 1; z = z / 10)
						x++;
					y = x;
					while(x > 0)
					{
						gTerm.strout[c + x - 1] = '0' + temp % 10;
						temp = temp / 10;
						x--;
					}
					c = c + y;
					y = 0;
				 	gTerm.strout[c++] = '\n';
				 	for(a; nstr1[a] != shared[i]; a++)
				 	{
				 		gTerm.strout[c++] = '<';
						for(int j = 0; j < str1[a].length(); j++)
						gTerm.strout[c++] = str1[a][j];
						gTerm.strout[c++] = '\n';
					}
				 	gTerm.strout[c++] = '-';
					gTerm.strout[c++] = '-';
					gTerm.strout[c++] = '-';
					gTerm.strout[c++] = '\n';
				 	for(b; nstr2[b] != shared[i]; b++)
				 	{
				 		gTerm.strout[c++] = '>';
						for(int j = 0; j < str2[b].length(); j++)
						gTerm.strout[c++] = str2[b][j];
						gTerm.strout[c++] = '\n';
					}
				 	a++;
				 	b++;
				 }
		}
		if((a <= m - 1) && (b <= n - 1))
		{
			int x = 0, y = 0, temp = a;
			for(int z = temp; z >= 1; z = z / 10)
				x++;
			y = x;
			while(x > 0)
			{
				gTerm.strout[c + x - 1] = '0' + temp % 10;
				temp = temp / 10;
				x--;
			}
			c = c + y;
			y = 0;
			gTerm.strout[c++] = 'c';
			temp = b;
			for(int z = temp; z >= 1; z = z / 10)
			x++;
			y = x;
			while(x > 0)
			{
				gTerm.strout[c + x - 1] = '0' + temp % 10;
				temp = temp / 10;
				x--;
			}
			c = c + y;
			y = 0;
		 	gTerm.strout[c++] = '\n';
			for(a; a <= m - 1; a++)
			{
		 		gTerm.strout[c++] = '<';
				for(int i = 0; i < str1[a].length(); i++)
				gTerm.strout[c++] = str1[a][i];
				gTerm.strout[c++] = '\n';
			}
			gTerm.strout[c++] = '-';
			gTerm.strout[c++] = '-';
			gTerm.strout[c++] = '-';
			gTerm.strout[c++] = '\n';
			for(b; b <= n - 1; b++)
			{
				gTerm.strout[c++] = '>';
				for(int i = 0; i < str2[b].length(); i++)
				gTerm.strout[c++] = str2[b][i];
				gTerm.strout[c++] = '\n';
			}
		}
		else
		{
			if(a <= m - 1)
			{
				int x = 0, y = 0, temp = a;
				for(int z = temp; z >= 1; z = z / 10)
					x++;
				y = x;
				while(x > 0)
				{
					gTerm.strout[c + x - 1] = '0' + temp % 10;
					temp = temp / 10;
					x--;
				}
				c = c + y;
				y = 0;
				gTerm.strout[c++] = 'd';
				temp = n - 1;
				for(int z = temp; z >= 1; z = z / 10)
					x++;
				y = x;
				while(x > 0)
				{
					gTerm.strout[c + x - 1] = '0' + temp % 10;
					temp = temp / 10;
					x--;
				}
				c = c + y;
				y = 0;
				gTerm.strout[c++] = '\n';
				for(a; a <= m - 1; a++)
				{
		 			gTerm.strout[c++] = '<';
					for(int i = 0; i < str1[a].length(); i++)
					gTerm.strout[c++] = str1[a][i];
					gTerm.strout[c++] = '\n';
				}
			}
			if(b <= n - 1)
			{
				int x = 0, y = 0, temp = m - 1;
				for(int z = temp; z >= 1; z = z / 10)
					x++;
				y = x;
				while(x > 0)
				{
					gTerm.strout[c + x - 1] = '0' + temp % 10;
					temp = temp / 10;
					x--;
				}
				c = c + y;
				y = 0;
				gTerm.strout[c++] = 'a';
				temp = b;
				for(int z = temp; z >= 1; z = z / 10)
				x++;
				y = x;
				while(x > 0)
				{
					gTerm.strout[c + x - 1] = '0' + temp % 10;
					temp = temp / 10;
					x--;
				}
				c = c + y;
				y = 0;
				gTerm.strout[c++] = '\n';
				for(b; b <= n - 1; b++)
				{
					gTerm.strout[c++] = '>';
					for(int i = 0; i < str2[b].length(); i++)
					gTerm.strout[c++] = str2[b][i];
					gTerm.strout[c++] = '\n';
				}
			}
		}
		return;
	}
}
