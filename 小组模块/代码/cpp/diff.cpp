#include <iostream> 
#include <fstream>
#include <string.h>
#include <algorithm>
#include <vector>
#include "allbinary.h"
using namespace std;

void doDiff(int argc, char *argv[])     //调用参数，一些选项，两个文件名 
{
	int m = 1, n = 1;
	string str[argc], Root = gTerm.root, Wdir = gTerm.wdir;			//转化为string类，方便操作(其实是本人知识有限)  
	bool mood[8];
	for(int i = 0; i < 8; i++)
		mood[i] = 0;
	for(int i = 0; i < argc; i++)
		str[i] = argv[i];
	/*
	记录diff指令的选项，依次为 
	--help：显示帮助。
	-b：不检查空格字符的不同。
	-B：不检查空白行。
	-i：不检查大小写的不同。
	-q：仅显示有无差异，不显示详细的信息。无差异时不显示，有差异时显示“File [文件 1] and [文件 2] differ”。
	-w：忽略全部的空格字符。
	-I[字符串]：若两个文件在某几行有所不同，但这几行同时都包选项中指定字符串，则不显示这两个文件的差异。
	*/
	if(str[0] == "diff")
	{
		for(int i = 1; i < argc; i++)
			if(str[i] == "--help") mood[1] = 1;				//已完成 
		for(int i = 1; i < argc - 2; i++)
			if(str[i] == "-b") mood[2] = 1;					//已完成 
		for(int i = 1; i < argc - 2; i++)
			if(str[i] == "-B") mood[3] = 1;					//已完成 
		for(int i = 1; i < argc - 2; i++)
			if(str[i] == "-i") mood[4] = 1;					//已完成 
		for(int i = 1; i < argc - 2; i++)
			if(str[i] == "-q") mood[5] = 1;					//已完成 
		for(int i = 1; i < argc - 2; i++)
			if(str[i] == "-w") mood[6] = 1;					//已完成 
		for(int i = 1; i < argc - 2; i++)
			if(str[i].substr(0,2) == "-I")					//貌似完成了，不是很确定 
			{
				mood[7] = 1;
				str[0] = str[i].erase(0, 2);				//特定字符串记为str[0]
				str[i] = "-I" + str[i];						//虽然很笨，但我只会这样了 
			}
		for(int i = 1; i < argc - 2; i++)					//刚好mood[0]用来检查输入的指令是不是合法的 
		{ 
			if(str[i] == "--help") mood[0] = 1;
			if(str[i] == "-b") mood[0] = 1;
			if(str[i] == "-B") mood[0] = 1;
			if(str[i] == "-i") mood[0] = 1;
			if(str[i] == "-q") mood[0] = 1;
			if(str[i] == "-w") mood[0] = 1;
			if(str[i].substr(0, 2) == "-I") mood[0] = 1;
			if(mood[0]) mood[0] = 0;
			else											//用cerr输出错误，说明文档里这样要求的，就这样写了
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
		vector<string> str1, str2, share, shared;		//shared用于存储最长公共子序列 
		str1.push_back("empty");				//不想使用下表为零的字符串(因为下面的程序写好了，懒得改) 
		str2.push_back("empty");
		shared.push_back("empty");
		share.push_back("empty");
		//文件名处理 
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
		int end = 0;										//记录strin的下标，用于比较strin中的两个文件 
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
			fin.open(str[argc-2].c_str());						//这个文件名还没改，因为改了就打不开了，没定义root 
			if(!fin)											//改好了，不过完全不放心，呜呜呜 
			{
				cerr << "diff:[Error]There is no such file or directory\n" ;
				return;
			}	
			for(int i = 1; !fin.eof(); i++)
			{
				string temp;
				getline(fin, temp);							//将文件1的每一行存入str1,同时得到m=行数+1 
				if(mood[3] && temp.length() == 0)			//-B不记录空行 
				continue;
				str1.push_back(temp);
				m++;
			}
			fin.close();
		}
		if(str[argc-1].compare(str[argc-1].length()-1,1,"-") == 0)				//不太理解strin(流汗黄豆) 
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
			fin.open(str[argc - 1].c_str());					//这个文件名也没换，原因同上 
			if(!fin)											//换了，同上不确定 
			{
				cerr << "diff:[Error]There is no such file or directory\n";
				return;
			}
			for(int i = 1; !fin.eof(); i++)
			{
				string temp;
				getline(fin, temp);							//将文件2的每一行存入str2，同时得到n=行数+1
				if(mood[3] && temp.length() == 0)			//-B不记录空行 
				continue;
				str2.push_back(temp);
				n++;
			}
			fin.close();
		}
		str[argc - 2] = argv[argc - 2];
		str[argc - 1] = argv[argc - 1];						//把文件名改了回去，方便后面-q输出时使用 
		vector<string> nstr1(str1), nstr2(str2);
		/*这里定义两个字符串数组，用于根据功能选项进行去除空格字符、转化为小写等操作后再行比较 
		  这样可以在输出时仍输出原文件中的内容*/
		if(mood[2])							//-b将多个空格替换为一个空格 
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
		if(mood[4])							//-i不考虑大小写(全转化为小写再diff) 
		{
			for(int i = 1; i < m; i++)
			transform(nstr1[i].begin(), nstr1[i].end(), nstr1[i].begin(), ::tolower);
			for(int i = 1; i < n; i++)
			transform(nstr2[i].begin(), nstr2[i].end(), nstr2[i].begin(), ::tolower);
		}
		if(mood[6])							//-w忽略全部的空格字符
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
		if(mood[7])							//不检查特定字符串,将含有特定字符(str[0])的行用str[0]替换 
		{
			for(int i = 1; i < m; i++)
				if(nstr1[i].find(str[0]) != string::npos)
					nstr1[i] = str[0];
			for(int i = 1; i < n; i++)
				if(nstr2[i].find(str[0]) != string::npos)
					nstr2[i] = str[0];
		}
		//LCS最长公共子序列 
		//初始化数组：dp记录公共子序列长度；f用于回溯找到公共子序列 
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
		if(mood[5])												//-q仅显示有无差异 
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
		int c = 0;												//用于记录strout的下标 
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
			shared.push_back(share[l0]);				//将公共子序列顺序排列(之前的share是倒序的) 
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
