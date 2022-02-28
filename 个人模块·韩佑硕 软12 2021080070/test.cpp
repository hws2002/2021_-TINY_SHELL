#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <string.h>
#include <unistd.h>
#define MAXLINE 10240
#define MAXFILE 10240
#define GREEN "\e[92;1m"
#define RED "\e[91;1m"
#define BLUE "\e[94;1m"
#define RESET "\e[0m"
using namespace std;

//结构及全局变量定义
struct Terminal {
    char user[MAXLINE]; //用户名
    char mach[MAXLINE]; //计算机名
    char root[MAXLINE]; //根目录
    char wdir[MAXLINE]; //工作目录
    char strin[MAXFILE]; //重定向标准输入
    char strout[MAXFILE]; //重定向标准输出
}; Terminal gTerm;
ifstream testing("test.txt");

//函数声明区

bool checking_valid(int cmdsize);
int  splitToken(char *command, char *argv[], int cmdSize);
char *getToken(char *arr);
int get_strout(int argc,char *argv[]);
void delete_memory(int argc, char * argv[],char * command);

int Command(int argc, char *argv[]); 
void doDiff(int argc, char * argv[]); //不读取
void doGrep(int argc, char * argv[]); //读取
void doTee(int argc, char * argv[]); //读取
void doCat(int argc, char * argv[]); //读取
void doCp(int argc, char * argv[]); //不读取
void doCd(int argc, char * argv[]); //不读取
void doPwd(int argc, char * argv[]); //不读取
void doEcho(int argc, char * argv[]); //不读取
//主函数
int main(int argc, char *argv[]){ 
    int cmdSize;
    char *command;
    //prompt生成
    cout<<"Machine Name:"; cin>>gTerm.mach; cin.ignore();
    cout<<"Root Directory:"; cin>>gTerm.root; cin.ignore(); strcpy(gTerm.wdir,gTerm.root);
    cout<<"Login:"; cin>>gTerm.user; cin.ignore();
    cout<<endl;
    while (testing) {
        testing.getline(gTerm.strin,MAXLINE);
        sleep(1);
        cout<<"输入 : "<<gTerm.strin<<"\n";
        sleep(1);
        //如果输入只有回车，空格，则重新输入
        cmdSize = strlen(gTerm.strin);
        bool valid = checking_valid(cmdSize);
        if(!valid) continue;
        //拆token
        command = new char[cmdSize * sizeof(char)+1];
        strcpy(command, gTerm.strin);
        argc = 0;
        argc = splitToken(command, argv, cmdSize);
        //输出标准输出
        int left_argc = get_strout(argc,argv);
        //释放内存
        delete_memory(argc,argv,command);
        if (!left_argc) break;
        cout<<endl;
    }
    testing.close();
    return 0;
}

//函数定义区
bool checking_valid(int cmdsize){
        int m =0;
        while(m<cmdsize){
            if (gTerm.strin[m] != '\0' && gTerm.strin[m] != ' '&& gTerm.strin[m] != '\n')
                break;
            m++;
        }
        if(m==cmdsize){
            memset(gTerm.strin,0,sizeof(char)*MAXFILE);
            return false;
        } else return true;
}

// 把标准输入里的内容拆成若干个token，存储到argv，把token个数存储到argc 
int splitToken(char *command, char *argv[],int cmdSize){
    int argc = 0;
    int tokenSize = 0;
    //算argc（token 个数）,把strin拆成token存储到argv里
    for (argc = 0; argc<cmdSize; argc++){
        // 删除空格
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
        // 如果删除空格之后是‘\0’ 则表明没有了token，-》 argv[argc]要填NULL,return argc
        if(command[0] == '\0'){
            return argc;
        }
        //把token储存到 argv里(通过getToken函数)
        //这里又用复制本是因为strtok_r函数会改变接受的字符串，而command
        char commandcp[cmdSize];
        strcpy(commandcp,command);
        argv[argc] = getToken(commandcp);
        //把这次的token的长度也记上 （用处 ： 为求下一个token，更新command）
        tokenSize = strlen(argv[argc]);
        // 因为token被储存到argv里，command要缩短（除去一个token)
        strcpy(command, command+tokenSize);
        // cmdSize也要缩短
        cmdSize -= (tokenSize-1);
    }
    return argc;
}

//用strtok函数拆token
char *getToken(char *arr){
    int tokenSize;
    char *argv;
    char *token;
    token = strtok_r(arr," |",&arr);
    tokenSize = strlen(token);
    argv = new char[tokenSize * sizeof(char)+1]; //为了节省memory，申请了内存，以后要释放
    strcpy(argv, token);
    return argv;
}

int get_strout(int argc, char* argv[]){ 
    //如果输入exit则结束terminal
    if(!strcasecmp(argv[0],"exit")) { //exit功能
        return 0;
    }
    //判断是否包含符合指令
    int i=0;
    while(i<argc && strcasecmp(argv[i],"|")){ // ~ ||~이런식으로 넣으면 ,여기에 argv[0] = "|"조건에서 걸림 i=0, argc 는 1이상, 이때는 그냥 parse error 만 출력하고, 설령 뒤에 올바른 명령어가 있더라도, 실행하지 않음, 단, 앞에 명령어들은 실행
        i++;
    }
    if (i==argc) {//"~|"形式 或者 不包含 | 
        if(i==0){ //”~|“形式，重新收输入，拆token放到argv，再次执行get_strout
                char new_strin[MAXFILE];
                char * new_command;
                bool right_command = false;
                int new_cmdSize = strlen(new_strin);
                while(!right_command){
                    sleep(1);
                    cout<<"输出 ： pipe> "; testing.getline(new_strin,MAXFILE);
                    cout<<"再次输入 : "<<new_strin<<"\n";
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
                if(!strcasecmp(argv[0],"exit")){
                    for(int i=0; i<argc; i++){
                        delete (argv[i]);
                    }
                    delete [] new_command;
                    return 0;
                }
                delete [] new_command;
                argc = get_strout(argc,argv);
        } else {//如果后边不包含 | 指令，则直接输出strout
            int switcher = Command(i,argv);
            switch(switcher){
                case 0:
                    cout<<"输出 : "<<gTerm.strout;
                    break;
                case 1:
                    cerr<<"输出 : command '"<<argv[0]<<"' not found\n";
                    break;
            }
        }
        memset(gTerm.strout,0,sizeof(char) * MAXFILE);
    } else { // ~|~ 的形式
        if(i==0){ //出现 ~ || ~ 的情况，不执行|| 后边的指令，直接输出异常，用户重新输入
            cerr<<"parse error near '|'\n";
            return argc;
        }
        else if(!strcasecmp(argv[i+1],"cd")) { //因为真实的cd指令如果放在复合指令中间的话，不会被执行
            argv += i+1;
            argc -= i+1;
            argc = get_strout(argc,argv);
        } else { //其他指令的话就执行，并且把前面的标准输出拷贝到下一个指令的标准输入
            int switcher = Command(i,argv);
            switch(switcher){
                case 0:
                    memset(gTerm.strin,0,sizeof(char) * MAXFILE);
                    strcpy(gTerm.strin,gTerm.strout);
                    memset(gTerm.strout,0,sizeof(char) * MAXFILE);
                    argv += i+1;
                    argc -= i+1;
                    break;
                case 1:
                    cerr<<"输出 : command '"<<argv[0]<<"' not found\n";
                    argv += i+1;
                    argc -= i+1;
                    break;
            }
            argc = get_strout(argc,argv);
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

void delete_memory(int argc, char * argv[],char * command){
    memset(gTerm.strin,0,sizeof(char)*MAXFILE);
    cin.clear();   
    for(int i=0; i<argc; i++){
        delete argv[i];
    }
    delete []command;
}

void doEcho(int argc, char * argv[]){ //echo 指令实现
    char empty[2] = {' '};
    char endl[2] = {'\n'};
    if(argv[1] == NULL) { //只输入 1）echo 或者 2）echo加空格时
        strncpy(gTerm.strout,endl,1); 
    } else { 
        if(!strncmp(argv[1],"-n",2)){ //如果选了 -n option
            if (argv[2] != NULL) { //且后边不是空格，则存储后边的字符串，并不加换行
                for(int i=2; i<argc; i++){
                strncat(gTerm.strout,empty,1);
                strcat(gTerm.strout,argv[i]);
                }
                strcpy(gTerm.strout,gTerm.strout+1);
            }
        }
        else { //没选-n option
            if(!strcasecmp(argv[1],"--help")){ //输入 --help 时
                ifstream help_file("echo--help.txt");
                int length = 0;
                while(help_file){
                    help_file.getline(gTerm.strout + length,10240);
                    strcat(gTerm.strout,"\n");
                    length = strlen(gTerm.strout);
                }
                gTerm.strout[strlen(gTerm.strout)-1] = '\0';
                help_file.close();
            } else { //输入 其他字符串时，存储到gTerm.strout 中，并加换行
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

void doDiff(int argc, char * argv[]){
    strcpy(gTerm.strout,"diff 指令被执行\n");}
void doGrep(int argc, char * argv[]){
    strcpy(gTerm.strout,"grep 指令被执行\n");}
void doTee(int argc, char * argv[]){
    strcpy(gTerm.strout,"tee 指令被执行\n");}
void doCat(int argc, char * argv[]){
    strcpy(gTerm.strout,"cat 指令被执行\n");}
void doCp(int argc, char * argv[]){
    strcpy(gTerm.strout,"cp 指令被执行\n");}
void doCd(int argc, char * argv[]){
    strcpy(gTerm.strout,"cd 指令被执行\n");}
void doPwd(int argc, char * argv[]){
    strcpy(gTerm.strout,"pwd 指令被执行\n");}