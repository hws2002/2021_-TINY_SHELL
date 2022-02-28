#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <string.h>
#include "allbinary.h"
#define MAXLINE 100000 
#define MAXFILE 100000 
#define GREEN "\e[92;1m"
#define RED "\e[91;1m"
#define BLUE "\e[94;1m"
#define RESET "\e[0m"
using namespace std;
Terminal gTerm = { "", "", "", "/", "", "" };

//����������

char ** getPrompt(char **prompt);
bool checking_valid(int cmdsize,char **prompt);
int  splitToken(char *command, char *argv[], int cmdSize);
char *getToken(char *arr);
int get_strout(int argc,char *argv[]);
void delete_memory(char ** prompt,char * command);

int Command(int argc, char *argv[]); 
void doDiff(int argc, char * argv[]); //����ȡ
void doGrep(int argc, char * argv[]); //��ȡ
void doTee(int argc, char * argv[]); //��ȡ
void doCat(int argc, char * argv[]); //��ȡ
void doCp(int argc, char * argv[]); //����ȡ
void doCd(int argc, char * argv[]); //����ȡ
void doPwd(int argc, char * argv[]); //����ȡ
void doEcho(int argc, char * argv[]); //����ȡ

//������
int main(int argc, char *argv[]){ 
    int cmdSize;
    char *command;
    //prompt����
    cout<<"Machine Name:"; cin>>gTerm.mach; cin.ignore();
    cout<<"Root Directory:"; cin>>gTerm.root; cin.ignore(); 
    cout<<"Login:"; cin>>gTerm.user; cin.ignore();
    char ** prompt;
    while (1) {
        prompt = getPrompt(prompt);
        cout<< GREEN <<prompt[0]<<prompt[1]<<prompt[2]<<RESET<<prompt[3]<<BLUE<<prompt[4]<<RESET<<prompt[5];
        // ���� command ������ gTerm.strin
        cin.getline(gTerm.strin,MAXLINE);
        cin.clear();
        //�������ֻ�лس����ո�����������
        cmdSize = strlen(gTerm.strin);
        bool valid = checking_valid(cmdSize,prompt);
        if(!valid) continue;
        //��token
        command = new char[cmdSize * sizeof(char)+1];
        strcpy(command, gTerm.strin);
        argc = 0;
        argc = splitToken(command, argv, cmdSize);
		memset(gTerm.strin,0,sizeof(char)*MAXFILE);//ģ��3��Ӧ
        //�����׼���
        int left_argc = get_strout(argc,argv);
        //�ͷ��ڴ�
        delete_memory(prompt,command);
        if (!left_argc) break;
    }
    return 0;
}

//����������
bool checking_valid(int cmdsize,char **prompt){
        int m =0;
        while(m<cmdsize){
            if (gTerm.strin[m] != '\0' && gTerm.strin[m] != ' '&& gTerm.strin[m] != '\n')
                break;
            m++;
        }
        if(m==cmdsize){
            for(int k=0; k<6;k++) delete (prompt[k]);
            delete [] prompt;
            memset(gTerm.strin,0,sizeof(char)*MAXFILE);
            return false;
        } else return true;
}
//Ϊ��ʹ��cdָ��֮��ܷ���ظ�����ʾ������������getPrompt����
char ** getPrompt(char ** prompt) { 
    prompt = new char*[6];
    prompt[0] = new char[strlen(gTerm.user) * sizeof(char)+1];
    strcpy(prompt[0],gTerm.user);
    prompt[1] = new char[1* sizeof(char)+1];
    strcpy(prompt[1],"@");
    prompt[2] = new char[strlen(gTerm.mach) * sizeof(char)+1];
    strcpy(prompt[2],gTerm.mach);
    prompt[3] = new char[1* sizeof(char)+1];
    strcpy(prompt[3],":");
    prompt[4] = new char[strlen(gTerm.wdir) * sizeof(char)+1];
    strcpy(prompt[4],gTerm.wdir);
    prompt[5] = new char[1* sizeof(char)+1];
    strcpy(prompt[5],"$");
    return prompt;
}

// �ѱ�׼����������ݲ�����ɸ�token���洢��argv����token�����洢��argc 
int splitToken(char *command, char *argv[],int cmdSize){
    int argc = 0;
    int tokenSize = 0;
    //��argc��token ������,��strin���token�洢��argv��
    for (argc = 0; argc<cmdSize; argc++){
        // ɾ���ո�
        while (command[0] == ' '){
            strcpy(command, command+1);
            while(command[0] == '|'){
                argv[argc] = new char(sizeof(char)*1+1);
                strcpy(argv[argc],"|");
                strcpy(command, command+1);
                cmdSize --;
                argc ++;
            }
        }
        while(command[0] == '|'){
            argv[argc] = new char(sizeof(char)*1+1);
            strcpy(argv[argc],"|");
            strcpy(command, command+1);
            cmdSize --;
            argc ++;
            while(command[0] == ' '){
                strcpy(command,command+1);
            }            
        }
        // ���ɾ���ո�֮���ǡ�\0�� �����û����token��-�� argv[argc]Ҫ��NULL,return argc
        if(command[0] == '\0'){
            return argc;
        }
        //��token���浽 argv��(ͨ��getToken����)
        //�������ø��Ʊ�����Ϊstrtok_r������ı���ܵ��ַ�������command
        char commandcp[cmdSize];
        strcpy(commandcp,command);
        argv[argc] = getToken(commandcp);
        //����ε�token�ĳ���Ҳ���� ���ô� �� Ϊ����һ��token������command��
        tokenSize = strlen(argv[argc]);
        // ��Ϊtoken�����浽argv�commandҪ���̣���ȥһ��token)
        strcpy(command, command+tokenSize);
        // cmdSizeҲҪ����
        cmdSize -= (tokenSize-1);
    }
    return argc;
}

//��strtok������token
char *getToken(char *arr){
    int tokenSize;
    char *argv;
    char *token;
    token = strtok(arr," |");
    tokenSize = strlen(token);
    argv = new char[tokenSize * sizeof(char)+1]; //Ϊ�˽�ʡmemory���������ڴ棬�Ժ�Ҫ�ͷ�
    strcpy(argv, token);
    return argv;
}

int get_strout(int argc, char* argv[]){ 
    //�������exit�����terminal
    if(!strcasecmp(argv[0],"exit")) { //exit����
		for(int k=0;k<argc;k++){//exit | ~ �������ֱ�ӽ���������ִ�к���ָ��
			delete argv[k];
        	return 0;
		}
    }
    //�ж��Ƿ��������ָ��
    int i=0;
    while(i<argc && strcasecmp(argv[i],"|")){ 
        i++;
    }
    if (i==argc) {//������ | 
         //�����߲����� | ָ���ֱ�����strout
        int switcher = Command(i,argv);
        switch(switcher){
            case 0:
                cout<<gTerm.strout;
                break;
            case 1:
                cerr<<"command '"<<argv[0]<<"' not found\n";
                break;            
        }
		for(int k=0; k<argc; k++){
			delete argv[k];
		}
        memset(gTerm.strout,0,sizeof(char) * MAXFILE);
    } else { // ~|~ �� ~| ����ʽ
		if(argc == i+1){//��~|�������
			//��ִ��~|֮ǰ��ָ��
			if(strcasecmp(argv[0],"cd")) { //��Ϊ��ʵ��cdָ��������ڸ���ָ���м�Ļ������ᱻִ��,����ָ��Ļ���ִ�У����Ұ�ǰ��ı�׼�����������һ��ָ��ı�׼����
				int switcher = Command(i,argv);
				switch(switcher){
					case 0:
						memset(gTerm.strin,0,sizeof(char) * MAXFILE);
						strcpy(gTerm.strin,gTerm.strout);
						memset(gTerm.strout,0,sizeof(char) * MAXFILE);
						break;
					case 1:
						cerr<<"command '"<<argv[0]<<"' not found\n";
						break;
				}    
			}
			//�ͷŶ�̬����
			for(int k=0; k<argc; k++){
				delete argv[k];
			}
			//��������
			char new_strin[MAXFILE];
			char * new_command;
			bool right_command = false;
			int new_cmdSize = strlen(new_strin);
			while(!right_command){
				cout<<"pipe> ";cin.getline(new_strin,MAXFILE);
				cin.clear();
				new_cmdSize = strlen(new_strin);
				int n =0;
				while(n<new_cmdSize){
					if(new_strin[n]!='\0' && new_strin[n]!=' ' && new_strin[n]!='\n')
						break;
						n++;
					}
					if(n==new_cmdSize){
						memset(new_strin,0,sizeof(char)*MAXFILE);
						continue;
					} else right_command = true;
				} 
			new_command = new char[new_cmdSize * sizeof(char)+1];
			strcpy(new_command, new_strin);
			argc = splitToken(new_command,argv,new_cmdSize);
			argc = get_strout(argc,argv);
            delete [] new_command;				
		} else {//~|~
			if(!strcasecmp(argv[i+1],"|")){ //���� ~ || ~ ���������ִ��|| ��ߵ�ָ�ֱ������쳣���û���������
				cerr<<"parse error near '|'\n";
				for(int k=0; k<argc; k++){
					delete argv[k];
				}
				return argc;
			}
			else if(!strcasecmp(argv[0],"cd")) { //��Ϊ��ʵ��cdָ��������ڸ���ָ���м�Ļ������ᱻִ��
				for(int k=0; k<=i; k++){
					delete argv[k];
				}
				argc -= i+1;
				argv += i+1;
				argc = get_strout(argc,argv);
			} else { //����ָ��Ļ���ִ�У����Ұ�ǰ��ı�׼�����������һ��ָ��ı�׼����
				int switcher = Command(i,argv);
				switch(switcher){
					case 0:
						memset(gTerm.strin,0,sizeof(char) * MAXFILE);
						strcpy(gTerm.strin,gTerm.strout);
						memset(gTerm.strout,0,sizeof(char) * MAXFILE);
						for(int k=0; k<=i; k++){
							delete argv[k];
						}
						argc -= i+1;
						argv += i+1;
						argc = get_strout(argc,argv);
						break;
					case 1:
						cerr<<"command '"<<argv[0]<<"' not found\n";
						for(int k=0; k<=i; k++){
							delete argv[k];
						}
						argc -= i+1;
						argv += i+1;
						argc = get_strout(argc,argv);
						break;
				}    
			}
		}
    }
    return argc;
}

int Command(int argc, char *argv[]){
    if(strcasecmp(argv[0],"diff") == 0) {doDiff(argc,argv); return 0;}
    else if(strcasecmp(argv[0],"grep")==0) {doGrep(argc,argv); return 0;}
    else if(strcasecmp(argv[0],"tee")==0) {doTee(argc,argv);return 0;}
    else if(strcasecmp(argv[0],"cat")==0) {doCat(argc,argv);return 0;}
    else if(strcasecmp(argv[0],"cp")==0) {doCp(argc,argv);return 0;}
    else if(strcasecmp(argv[0],"cd")==0) {doCd(argc,argv);return 0;}
    else if(strcasecmp(argv[0],"pwd")==0) {doPwd(argc,argv);return 0;}
    else if(strcasecmp(argv[0],"echo")==0) {doEcho(argc,argv);return 0;}
    else if(strcasecmp(argv[0],"|")==0) return 2;
    else return 1;
}

void delete_memory(char ** prompt,char * command){
    for(int k=0; k<6;k++) delete prompt[k];
    delete [] prompt;
    memset(gTerm.strin,0,sizeof(char)*MAXFILE);
    cin.clear();   
    delete []command;
}

void doEcho(int argc, char * argv[]){ //echo ָ��ʵ��
    char empty[2] = {' '};
    char endl[2] = {'\n'};
    if(argv[1] == NULL) { //ֻ���� 1��echo ���� 2��echo�ӿո�ʱ
        strncpy(gTerm.strout,endl,1); 
    } else { 
        if(!strncmp(argv[1],"-n",2)){ //���ѡ�� -n option
            if (argv[2] != NULL) { //�Һ�߲��ǿո���洢��ߵ��ַ����������ӻ���
                for(int i=2; i<argc; i++){
                strncat(gTerm.strout,empty,1);
                strcat(gTerm.strout,argv[i]);
                }
                strcpy(gTerm.strout,gTerm.strout+1);
            }
        }
        else { //ûѡ-n option
            if(!strcasecmp(argv[1],"--help")){ //���� --help ʱ
                ifstream help_file("echo--help.txt");
                int length = 0;
                while(help_file){
                    help_file.getline(gTerm.strout + length,10240);
                    strcat(gTerm.strout,"\n");
                    length = strlen(gTerm.strout);
                }
                gTerm.strout[strlen(gTerm.strout)-1] = '\0';
                help_file.close();
            } else { //���� �����ַ���ʱ���洢��gTerm.strout �У����ӻ���
                for(int i=1; i<argc; i++){
                strncat(gTerm.strout,empty,1);
                strcat(gTerm.strout,argv[i]);        
                }
                strcpy(gTerm.strout,gTerm.strout+1);
                strncat(gTerm.strout,endl,1);           
            }
        }
    }
}
