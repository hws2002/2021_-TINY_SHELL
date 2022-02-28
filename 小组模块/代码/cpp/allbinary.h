#pragma once
struct Terminal {
	char user[100]; // 用户名
	char mach[100]; // 计算机名
	char root[100]; // 根目录
	char wdir[100]; // 工作目录
	char strin[100000]; // 重定向标准输入
	char strout[100000]; // 重定向标准输出
}; 

extern Terminal gTerm;
