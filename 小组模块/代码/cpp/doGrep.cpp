#include<iostream>
#include<stdlib.h>
#include<cstring>
#include<string>
#include<fstream>
#include"allbinary.h"
using namespace std;




struct Linemeaning{//一个包含文件号，行号，匹配起点，终点的结构体 
	int linenum;
	int start;
	int end;
	int filenum;
};
void copy(char *str1,char *str2){//类strcpy函数 
    int len=strlen(str2);
    cout<<len;
    for(int i=0;i<len;i++){
        printf("!");
        str1[i]=str2[i];
    }
}
int docut(char A[],char *Xing2[])//在模式串具有*通配符时，对其以*为分界切割字串，便于进一步处理 
{
	char Temp[105];
	int Tempcnt = 0, cnt1 = 0;
    int len=strlen(A);
    bool flag=false;
	for (int i = 0; i < len; i++)
	{
		if (A[i] == '*')
		{
            A[i]='\0';
			flag=false;
		}
        else if(!flag){
            flag=true;
            Xing2[++cnt1]=A+i;
        }
		//Temp[Tempcnt++] = A[i];
        
	}
	
	return cnt1;
}
int judge(int id, int left,int cnt1,char *Xing2[],char B[])//在模式串具有*时，判断前cn1-1个子串匹配后最靠左的终点 
{
	if (left > strlen(B) - 1) return -1;
	if (id == cnt1) return left;
	int point = 0;
	for (int i = left; i < strlen(B); i++)
	{
		if (Xing2[id][point] != B[i]&&Xing2[id][point]!='.')
		{
			i=i-point;
			point = 0;
			continue;
		}
		point++;
		if (point == strlen(Xing2[id]))
		{
			int f = judge(id + 1, i + 1,cnt1,Xing2,B);
			return f;
		}
	}
	return -1;
}
Linemeaning docompare(char A[],char B[],int u,int v){//比对两个字符串，若成功匹配，则记录行号，文件号 
	int p=0;
	Linemeaning V={0};
	int kl=strlen(A);
	//cout<<kl<<endl;
	//cout<<A<<endl;
	for(int i=0;i<strlen(A);i++){
		//cout<<1<<endl;
		if(A[i]==42){
			//cout<<1<<endl;
			p++;
		}
	}
	//cout<<p<<endl;
	if(p==0){
	for(int o=0;o<strlen(A);o++){
		if(A[o]=='.'){
			continue;
		}
		else{
	for(int i=0;i<strlen(B);i++){
		if(B[i]==A[o]){
			bool Q=true;
			bool P=false;
			int m=i;
			int q=i;
			for(int j=o+1;j<strlen(A);j++){
				if(A[j]!=42&&A[j]!=46){
					q++;
					if(A[j]!=B[q]){
						cout<<1<<endl;
						Q=false;
						break;
					}
				}
				if(A[j]==46){
					q++;
					continue;
				}
				
			}
			if(Q==true){
				//标记该行和起止字符
				V.linenum=u;
				V.start=m-o;
				V.end=q; 
				V.filenum=v;
				break;
			}
		}	
	}
	break;
	}
	}
	return V;
	}
	else{
		char *Xing2[105]={0};
		char D[100]={0};
		strcpy(D,A);
		int cnt1=docut(D,Xing2);
		//cout<<1<<endl;
		if(A[0]=='*'&&A[strlen(A)-1]=='*'){
			//cout<<judge(1,0,cnt1+1,Xing2,B)<<endl;
			if(judge(1,0,cnt1+1,Xing2,B)!=-1){
				V.filenum=v;
				V.linenum=u;
				V.start=0;
				V.end=strlen(B)-1;
			}
		}
		else if(A[0]=='*'&&A[strlen(A)-1]!='*'){
			if(p==1){
				int x=0;
				int y=strlen(Xing2[1]);
				for(int e=y-1;y>=0;y--){
						if(Xing2[1][e]=='.'){
							continue;
					}
				for(int i=strlen(B)-1;i>=0;i--){
					bool Q=true;
					if(Xing2[1][e]==B[i]){
						for(int j=i-1;j>i-y;j--){
							if(Xing2[1][e-i+j]=='.'){
								continue;
							}
							if(Xing2[1][e-i+j]!=B[j]){
								Q=false;
								break;
							}
						}
						if(Q==true){
							x=i+y-e;
							V.filenum=v;
							V.linenum=u;
							V.start=0;
							V.end=x-1;
							break;
						}
					}
					}
					break;
				}
					}
			else{
			if(judge(1,0,cnt1,Xing2,B)!=-1){
				int left=judge(1,0,cnt1,Xing2,B);
				int x=0;
				int y=strlen(Xing2[cnt1]);
				for(int e=y-1;y>=0;y--){
						if(Xing2[cnt1][e]=='.'){
							continue;
					}
				for(int i=strlen(B)-1;i>=left;i--){
					bool Q=true;
					if(Xing2[cnt1][e]==B[i]){
						for(int j=i-1;j>i-y;j--){
							if(Xing2[cnt1][e-i+j]=='.'){
								continue;
							}
							if(Xing2[cnt1][e-i+j]!=B[j]){
								Q=false;
								break;
							}
						}
						if(Q==true){
							x=i+y-e;
							V.filenum=v;
							V.linenum=u;
							V.start=0;
							V.end=x-1;
							break;
						}
					}
					}
					break;
					}
				}
					
			}
		}
		else if(A[0]!='*'&&A[strlen(A)-1]=='*'){
			if(judge(1,0,cnt1+1,Xing2,B)!=-1){
				V=docompare(Xing2[1],B,u,v);
				V.start=V.start-strlen(Xing2[1])+1;
				V.end=strlen(B)-1;
			}
			}
		else{
			if(judge(1,0,cnt1,Xing2,B)!=-1){
				int left=judge(1,0,cnt1,Xing2,B);
				int x=0;
				int y=strlen(Xing2[cnt1]);
				for(int e=y-1;y>=0;y--){
						if(Xing2[cnt1][e]=='.'){
							continue;
					}
				for(int i=strlen(B)-1;i>=left;i--){
					bool Q=true;
					if(Xing2[cnt1][e]==B[i]){
						for(int j=i-1;j>i-y;j--){
							if(Xing2[cnt1][e-i+j]=='.'){
								continue;
							}
							if(Xing2[cnt1][e-i+j]!=B[j]){
								Q=false;
								break;
							}
						}
						if(Q==true){
							x=i+y-e;
							V=docompare(Xing2[1],B,u,v);
							V.start=V.start-strlen(Xing2[1])+1;
							V.end=x-1;
							break;
						}
					}
					}
					break;
				}
			}
		}
		return V;
	}
	return V;
}


int CI(char A[]){//字符串转整数 
	int k=0;
	int P[5]={0};
	for(int i=0;i<strlen(A);i++){
		P[i]=A[i]-48;
	}
	for(int i=0;i<strlen(A);i++){
		for(int j=0;j<strlen(A)-i-1;j++){
			P[i]=10*P[i];
		}
		k=k+P[i];
	}
	return k;
}
void IC(int k,char A[]){//整数转字符串 
	int B[6]={0};
	int p=0;
	for(int i=0;i>-1;i++){
		if(k==0){
			break;
		}
		else{
			B[i]=k%10;
			k=k/10;
			p++;
		}
	}
	for(int i=0;i<p;i++){
		A[p-1-i]=B[i]+48;
	}
	return;
}
void doGrep(int argc,char *argv[]){
	Linemeaning L[105][105]={0};
	int K[10]={0};
	int k=0;
	int Ap=0;
	int Bp=0;
	int Linecnt[105]={0};
	char endlp[2]={'\n'};
	int m=0;
	if(!strcmp(argv[1],"--help")){
		cerr<<"--help：显示帮助"<<endl<<"-c：计算符合样式的行数。"<<endl<<"-h：在显示符合样式的那一行之前，不标示该行所属的文件名称。"<<endl;
		cerr<<"-H：在显示符合样式的那一行之前，标示该行所属的文件名称。"<<endl<<"-i：忽略字符大小写的差别。"<<endl<<"-n：在显示符合样式的那一行之前，标示出该行的编号。"<<endl;
		cerr<<"-A[行数]：除了显示符合范本样式的那一列之外，同时显示该行之后的[行数]行内容。"<<endl<<"-B[行数]：除了显示符合样式的那一行之外，同时显示该行之前的[行数]内容。"<<endl;
		return; //帮助文档 
	}

	for(int i=1;i<argc;i++){
		if(strncmp(argv[i],"-",1)==0){
			if(strncmp(argv[i],"-h",2)==0){//完成 
				K[1]=1;
				k=i;
			}
			else if(strncmp(argv[i],"-H",2)==0){//完成 
				K[2]=1;
				k=i;
			}
			else if(strncmp(argv[i],"-i",2)==0){//完成 
				K[3]=1;
				k=i;
			}
			else if(strncmp(argv[i],"-n",2)==0){//完成 
				K[4]=1;
				k=i;
			}
			else if(strncmp(argv[i],"-c",2)==0){//完成 
				K[5]=1;
				k=i;
			}
			else if(strncmp(argv[i],"-A",2)==0){//
				K[6]=1;
				char A[10]={0};
				k=i;
				for(int j=0;j<strlen(argv[i])-2;j++){
					A[j]=argv[i][j+2];	
				}
				A[strlen(argv[i])-2]='\0';
				Ap=CI(A);
			}
			else if(strncmp(argv[i],"-B",2)==0){//
				K[7]=1;
				char B[10]={0};
				k=i;
				for(int j=0;j<strlen(argv[i])-2;j++){
					B[j]=argv[i][j+2];	
				}
				B[strlen(argv[i])-2]='\0';
				Bp=CI(B);
				}//录入七条不同的指令 
			else if(strcmp(argv[i],"-")==0){
				K[8]=1;
				m=i;//模式串或比对文件中包含标准输入 
			}
			else{
				cerr<<"不存在指令"<<argv[i]<<" "<<"请输入--help查询帮助"<<endl;
				return; 
			}
		}
	}
	
	if(m<k+1&&m!=0){
		cerr<<"不存在指令“-”请输入--help查询帮助"<<endl;
		return;
	}
	if(K[1]==1&&K[2]==1){
		cerr<<"出现相悖指令"<<endl;
		return;
	}
	
	if(K[8]==1&&m==k+1){
		argv[m]=gTerm.strin;
	}
	//录入模式串 
	int column=0;
	int Match[105][105]={0};
	int UnMatch[105][105]={0};
	for(int i=k+2;i<argc;i++){//录入文件 ，并得到要输出的行 
		ifstream fin;
		char *str[100]={0};
		char Road[100]={0};
		char str1[100]={0};
		if(i==m){//有标准输入作为文件名时 
			int Lenm=strlen(gTerm.strin);
			int cnt2=1;
			int read=0;
			for(int j=0;j<Lenm;j++){
				if(gTerm.strin[j]!='\n'){
					str1[read]=gTerm.strin[j];
					read++;
				}
				else{
					//cout<<1;
					str1[read]='\0';
					str[cnt2]=str1;
					if(K[3]==1){//忽略大小写 
						char IS1[100]={0};
						char IS2[100]={0};
						for(int w=0;w<read;w++){
							if(str[cnt2][w]<=90&&str[cnt2][w]>=65){
								IS2[w]=str[cnt2][w]+32;
							}
							else{
								IS2[w]=str[cnt2][w];
							}
						}
						for(int w=0;w<strlen(argv[k+1]);w++){
							if(argv[k+1][w]<=90&&argv[k+1][w]>=65){
								IS1[w]=argv[k+1][w]+32;
							}
							else{
								IS1[w]=argv[k+1][w];
							}
						}
						L[m-k-1][cnt2]=docompare(IS1,IS2,cnt2,m-k-1);
					}
					else{
						L[m-k-1][cnt2]=docompare(argv[k+1],str[cnt2],cnt2,m-k-1);
					}
					if(L[m-k-1][cnt2].filenum!=0){
						Match[m-k-1][L[m-k-1][cnt2].linenum]=1;
						if(UnMatch[m-k-1][L[m-k-1][cnt2].linenum]==1){
							UnMatch[m-k-1][L[m-k-1][cnt2].linenum]=0;
						}
						if(K[6]==1||K[7]==1){
							if(K[6]==1){
								for(int j=Ap;j>=1;j--){
									if(Match[i-k-1][L[i-k-1][cnt2].linenum-j]!=1){
										UnMatch[i-k-1][L[i-k-1][cnt2].linenum-j]=1;
									}
								}
							}
							if(K[7]==1){
								for(int j=1;j<=Bp;j++){
									if(Match[i-k-1][L[i-k-1][cnt2].linenum+j]!=1)
										UnMatch[i-k-1][L[i-k-1][cnt2].linenum+j]=1;
								}
							}				
						}
						column++;
					}
					cnt2++;
					read=0;
				}
			}
		continue;	
		}
		strcat(Road,gTerm.root);
		strcat(Road,gTerm.wdir);
		strcat(Road,"/");
		strcat(Road,argv[i]);
		fin.open(Road);
		if(fin==NULL){
			cerr<<"该目录下不存在名为"<<argv[i]<<"的文件"<<endl; 
			return;
		}
		
			int cnt=1;
			while(!fin.eof()){
				fin.getline(str1,1000,'\n');
				str[cnt]=str1;
				//cout<<str[cnt];
				if(K[3]==1){
					char IS1[100]={0};
					char IS2[100]={0};
					int ALen=strlen(str[cnt]);
					int BLen=strlen(argv[k+1]);
					for(int j=0;j<ALen;j++){
						if(str[cnt][j]<=90&&str[cnt][j]>=65){
						IS2[j]=str[cnt][j]+32;
						}
						else{
							IS2[j]=str[cnt][j];
						}		
					}
					for(int j=0;j<BLen;j++){
						if(argv[k+1][j]<=90&&argv[k+1][j]>=65){
							IS1[j]=argv[k+1][j]+32;
						}
						else{
							IS1[j]=argv[k+1][j];
						}
					}
					L[i-k-1][cnt]=docompare(IS1,IS2,cnt,i-k+1);
				}
				else{
					L[i-k-1][cnt]=docompare(argv[k+1],str[cnt],cnt,i-k+1);
				}
				if(L[i-k-1][cnt].filenum!=0){
					Match[i-k-1][L[i-k-1][cnt].linenum]=1;
					if(UnMatch[i-k-1][L[i-k-1][cnt].linenum]==1){
						UnMatch[i-k-1][L[i-k-1][cnt].linenum]=0;
					}
					if(K[6]==1||K[7]==1){
						if(K[6]==1){
							for(int j=Ap;j>=1;j--){
								if(Match[i-k-1][L[i-k-1][cnt].linenum-j]!=1){
									UnMatch[i-k-1][L[i-k-1][cnt].linenum-j]=1;
								}
							}
						}
						if(K[7]==1){
							for(int j=1;j<=Bp;j++){
								if(Match[i-k-1][L[i-k-1][cnt].linenum+j]!=1)
									UnMatch[i-k-1][L[i-k-1][cnt].linenum+j]=1;
							}
						}				
					}
					column++;
				}
				cnt++;
			}
			}
			//最多输入一行字符长度为9999 
			
		
	if(K[5]==1){//在标准输出之前打出行数 
		char P[20]="line number:";
		char COL[10]={'0'};
		IC(column,COL);
		COL[strlen(COL)]='\n';
		COL[strlen(COL)+1]='\0';
		strcat(gTerm.strout,P);
		strcat(gTerm.strout,COL);
	}

	for(int i=1;i<argc-k-1;i++){//对标准输出进行操作， 
		ifstream fin;
		char *str[100]={0};
		char str1[100]={0};
		if(i==m-k-1){
			int Lenm=strlen(gTerm.strin);
			int cnt2=1;
			int read=0;
			for(int j=0;j<Lenm;j++){
				if(gTerm.strin[j]!='\n'){
					str1[read]=gTerm.strin[j];
					read++;
				}
				else{
					str1[read]='\0';
					str[cnt2]=str1;
					if(Match[i][cnt2]==1){
						if(K[2]==1){
							if(K[4]==1){
								strcat(gTerm.strout,"strin");
								strcat(gTerm.strout,"--");
								char LNM[7]={0};
								IC(L[i][cnt2].linenum,LNM);
								for(int h=0;h<strlen(LNM);h++){
									strcat(gTerm.strout,"\e[92;1m");
									char tempt[2]={0};
									tempt[0]=LNM[h];
									tempt[1]='\0';
									strcat(gTerm.strout,tempt);
								}
								strcat(gTerm.strout,"\e[94;1m:");
								for(int h=0;h<strlen(str[cnt2]);h++){
									if(h<L[i][cnt2].start||h>L[i][cnt2].end){
										char Zifu[2]={0};
										Zifu[0]=str[cnt2][h];
										Zifu[1]='\0';
										strcat(gTerm.strout,"\e[0m");
										strcat(gTerm.strout,Zifu);
									}
											else{
												char ZifuM[2]={0};
												ZifuM[0]=str[cnt2][h];
												ZifuM[1]='\0';
												strcat(gTerm.strout,"\e[91;1m");
												strcat(gTerm.strout,ZifuM);
											}
										}
										strcat(gTerm.strout,endlp);
							}
							else{
								strcat(gTerm.strout,"strin");
								strcat(gTerm.strout,"--");
								for(int h=0;h<strlen(str[cnt2]);h++){
									if(h<L[i][cnt2].start||h>L[i][cnt2].end){
										char Zifu[2]={0};
										Zifu[0]=str[cnt2][h];
										Zifu[1]='\0';
										strcat(gTerm.strout,"\e[0m");
										strcat(gTerm.strout,Zifu);
									}
											else{
												char ZifuM[2]={0};
												ZifuM[0]=str[cnt2][h];
												ZifuM[1]='\0';
												strcat(gTerm.strout,"\e[91;1m");
												strcat(gTerm.strout,ZifuM);
											}
										}
										strcat(gTerm.strout,endlp);
								
							}
						}
						else{
							if(K[4]==1){
								char LNM[7]={0};
								IC(L[i][cnt2].linenum,LNM);
								for(int h=0;h<strlen(LNM);h++){
									strcat(gTerm.strout,"\e[92;1m");
									char tempt[2]={0};
									tempt[0]=LNM[h];
									tempt[1]='\0';
									strcat(gTerm.strout,tempt);
								}
								
								strcat(gTerm.strout,"\e[94;1m:");
								for(int h=0;h<strlen(str[cnt2]);h++){
									if(h<L[i][cnt2].start||h>L[i][cnt2].end){
										char Zifu[2]={0};
										Zifu[0]=str[cnt2][h];
										Zifu[1]='\0';
									strcat(gTerm.strout,"\e[0m");
										strcat(gTerm.strout,Zifu);
									}
											else{
												char ZifuM[2]={0};
												ZifuM[0]=str[cnt2][h];
												ZifuM[1]='\0';
												strcat(gTerm.strout,"\e[91;1m");
												strcat(gTerm.strout,ZifuM);
											}
										}
										strcat(gTerm.strout,endlp);
							}
							else{
								for(int h=0;h<strlen(str[cnt2]);h++){
									if(h<L[i][cnt2].start||h>L[i][cnt2].end){
										char Zifu[2]={0};
										Zifu[0]=str[cnt2][h];
										Zifu[1]='\0';
										strcat(gTerm.strout,"\e[0m");
										strcat(gTerm.strout,Zifu);
									}
											else{
												char ZifuM[2]={0};
												ZifuM[0]=str[cnt2][h];
												ZifuM[1]='\0';
												strcat(gTerm.strout,"\e[91;1m");
												strcat(gTerm.strout,ZifuM);
											}
										}
										strcat(gTerm.strout,endlp);
							}
						}
						
					}
					if(UnMatch[i][cnt2]==1){
						if(K[2]==1){
							if(K[4]==1){
								strcat(gTerm.strout,"strin");
								strcat(gTerm.strout,"--");
								char LNM[7]={0};
								IC(cnt2,LNM);
								for(int h=0;h<strlen(LNM);h++){
									strcat(gTerm.strout,"\e[92;1m");
									char tempt[2]={0};
									tempt[0]=LNM[h];
									tempt[1]='\0';
									strcat(gTerm.strout,tempt);
								}
								strcat(gTerm.strout,"\e[94;1m-");
								for(int h=0;h<strlen(str[cnt2]);h++){
									char Zifu[2]={0};
									Zifu[0]=str[cnt2][h];
									Zifu[1]='\0';
									strcat(gTerm.strout,"\e[0m");
									strcat(gTerm.strout,Zifu);
									}
								}
								strcat(gTerm.strout,endlp);
							
							if(K[4]!=1){
								strcat(gTerm.strout,"strin");
								strcat(gTerm.strout,"--");
								for(int h=0;h<strlen(str[cnt2]);h++){
									char Zifu[2]={0};
									Zifu[0]=str[cnt2][h];
									Zifu[1]='\0';
									strcat(gTerm.strout,"\e[0m");
									strcat(gTerm.strout,Zifu);
								}
								
									strcat(gTerm.strout,endlp);
								
							}
							}
						
						else{
							if(K[4]==1){
								char LNM[7]={0};
								IC(cnt2,LNM);
								for(int h=0;h<strlen(LNM);h++){
									strcat(gTerm.strout,"\e[92;1m");
									char tempt[2]={0};
									tempt[0]=LNM[h];
									tempt[1]='\0';
									strcat(gTerm.strout,tempt);
								}
								strcat(gTerm.strout,"\e[94;1m-");
								for(int h=0;h<strlen(str[cnt2]);h++){
										char Zifu[2]={0};
										Zifu[0]=str[cnt2][h];
										Zifu[1]='\0';
										strcat(gTerm.strout,"\e[0m");
										strcat(gTerm.strout,Zifu);
										}
										strcat(gTerm.strout,endlp);
							}
							else{
								for(int h=0;h<strlen(str[cnt2]);h++){
										char Zifu[2]={0};
										Zifu[0]=str[cnt2][h];
										Zifu[1]='\0';
										strcat(gTerm.strout,"\e[0m");
										strcat(gTerm.strout,Zifu);
									
										
										}
										strcat(gTerm.strout,endlp);
							}
						}
							
					}
					cnt2++;
					read=0;
				}
		}
		continue;
	}
		
		char Road[1000]={0};
		strcat(Road,gTerm.root);
		strcat(Road,gTerm.wdir);
		strcat(Road,"/");
		strcat(Road,argv[k+1+i]);
		fin.open(Road);
		int cnt=1;
		while(!fin.eof()){
			fin.getline(str1,10000,'\n');
			str[cnt]=str1;
			if(Match[i][cnt]==1){
				if(K[2]==1){
					if(K[4]==1){
						strcat(gTerm.strout,argv[k+1+i]);
						strcat(gTerm.strout,"--");
						char LNM[7]={0};
						IC(L[i][cnt].linenum,LNM);
						for(int h=0;h<strlen(LNM);h++){
							strcat(gTerm.strout,"\e[92;1m");
							char tempt[2]={0};
							tempt[0]=LNM[h];
							tempt[1]='\0';
							strcat(gTerm.strout,tempt);
						}
						strcat(gTerm.strout,"\e[94;1m:");
						for(int h=0;h<strlen(str[cnt]);h++){
							if(h<L[i][cnt].start||h>L[i][cnt].end){//不匹配 
								char Zifu[2]={0};
								Zifu[0]=str[cnt][h];
								Zifu[1]='\0';
								strcat(gTerm.strout,"\e[0m");
								strcat(gTerm.strout,Zifu);
							}
									else{//匹配 
										char ZifuM[2]={0};
										ZifuM[0]=str[cnt][h];
										ZifuM[1]='\0';
										strcat(gTerm.strout,"\e[91;1m");
										strcat(gTerm.strout,ZifuM);
									}
								}
								strcat(gTerm.strout,endlp);
					}
					else{
						strcat(gTerm.strout,argv[k+1+i]);
						strcat(gTerm.strout,"--");
						for(int h=0;h<strlen(str[cnt]);h++){
							if(h<L[i][cnt].start||h>L[i][cnt].end){
								char Zifu[2]={0};
								Zifu[0]=str[cnt][h];
							Zifu[1]='\0';
								strcat(gTerm.strout,"\e[0m");
								strcat(gTerm.strout,Zifu);
							}
									else{
										char ZifuM[2]={0};
										ZifuM[0]=str[cnt][h];
										ZifuM[1]='\0';
										strcat(gTerm.strout,"\e[91;1m");
										strcat(gTerm.strout,ZifuM);
									}
								}
								strcat(gTerm.strout,endlp);
						
					}
				}
				else{
					if(K[4]==1){
						char LNM[7]={0};
						IC(L[i][cnt].linenum,LNM);
						for(int h=0;h<strlen(LNM);h++){
							strcat(gTerm.strout,"\e[92;1m");
							char tempt[2]={0};
							tempt[0]=LNM[h];
							tempt[1]='\0';
							strcat(gTerm.strout,tempt);
						}
						strcat(gTerm.strout,"\e[94;1m:");
						for(int h=0;h<strlen(str[cnt]);h++){
							if(h<L[i][cnt].start||h>L[i][cnt].end){
								char Zifu[2]={0};
								Zifu[0]=str[cnt][h];
								Zifu[1]='\0';
							strcat(gTerm.strout,"\e[0m");
								strcat(gTerm.strout,Zifu);
							}
									else{
										char ZifuM[2]={0};
										ZifuM[0]=str[cnt][h];
										ZifuM[1]='\0';
										strcat(gTerm.strout,"\e[91;1m");
										strcat(gTerm.strout,ZifuM);
									}
								}
								strcat(gTerm.strout,endlp);
					}
					else{
						for(int h=0;h<strlen(str[cnt]);h++){
							if(h<L[i][cnt].start||h>L[i][cnt].end){ 
								char Zifu[2]={0};
								Zifu[0]=str[cnt][h];
								Zifu[1]='\0';
								strcat(gTerm.strout,"\e[0m");
								strcat(gTerm.strout,Zifu);
							}
									else{
										char ZifuM[2]={0};
										ZifuM[0]=str[cnt][h];
										ZifuM[1]='\0';
										strcat(gTerm.strout,"\e[91;1m");
										strcat(gTerm.strout,ZifuM);
									}
								}
								strcat(gTerm.strout,endlp);
					}
				}
				
			}
			if(UnMatch[i][cnt]==1){//不匹配行的输出 
				if(K[2]==1){
					if(K[4]==1){
						strcat(gTerm.strout,argv[k+1+i]);
						strcat(gTerm.strout,"--");
						char LNM[7]={0};
						IC(cnt,LNM);
						for(int h=0;h<strlen(LNM);h++){
							strcat(gTerm.strout,"\e[92;1m");
							char tempt[2]={0};
							tempt[0]=LNM[h];
							tempt[1]='\0';
							strcat(gTerm.strout,tempt);
						}
						strcat(gTerm.strout,"\e[94;1m-");
						for(int h=0;h<strlen(str[cnt]);h++){
							char Zifu[2]={0};
							Zifu[0]=str[cnt][h];
							Zifu[1]='\0';
							strcat(gTerm.strout,"\e[0m");
							strcat(gTerm.strout,Zifu);
							}
						}
						strcat(gTerm.strout,endlp);
					
					if(K[4]!=1){
						strcat(gTerm.strout,argv[k+1+i]);
						strcat(gTerm.strout,"--");
						for(int h=0;h<strlen(str[cnt]);h++){
							char Zifu[2]={0};
							Zifu[0]=str[cnt][h];
							Zifu[1]='\0';
							strcat(gTerm.strout,"\e[0m");
							strcat(gTerm.strout,Zifu);
						}
						
							strcat(gTerm.strout,endlp);
						
					}
					}
				
				else{
					if(K[4]==1){
						char LNM[7]={0};
						IC(cnt,LNM);
						for(int h=0;h<strlen(LNM);h++){
							strcat(gTerm.strout,"\e[92;1m");
							char tempt[2]={0};
							tempt[0]=LNM[h];
							tempt[1]='\0';
							strcat(gTerm.strout,tempt);
						}
						strcat(gTerm.strout,"\e[94;1m-");
						for(int h=0;h<strlen(str[cnt]);h++){
								char Zifu[2]={0};
								Zifu[0]=str[cnt][h];
								Zifu[1]='\0';
								strcat(gTerm.strout,"\e[0m");
								strcat(gTerm.strout,Zifu);
								}
								strcat(gTerm.strout,endlp);
					}
					else{
						for(int h=0;h<strlen(str[cnt]);h++){
								char Zifu[2]={0};
								Zifu[0]=str[cnt][h];
								Zifu[1]='\0';
								strcat(gTerm.strout,"\e[0m");
								strcat(gTerm.strout,Zifu);
							
								
								}
								strcat(gTerm.strout,endlp);
					}
				}
					
			}
			cnt++;		
		}
	}	
}
	

	//进行字符串比对


