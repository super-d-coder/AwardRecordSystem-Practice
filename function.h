#pragma once

#ifndef FUNCTION_H  // 头文件保护（必须加，避免重复包含）
#define FUNCTION_H

#include <string>
#include <vector>
#include <fstream>

using namespace std;

//奖项类
class Award {
private:
	string name;
	string time;
	string type;
public:
	Award() = default;//默认构造函数，和写空函数一样，只是新语法
	//有参构造函数
	Award(string n,string t,string ty):name(move(n)),time(move(t)),type(move(ty)){}//std::move 的核心意思是：标记对象为 “可转移资源”，让编译器通过 “移动构造” 直接窃取其内部资源，避免拷贝开销
	string getName() const { return name; }
	string getTime() const { return time; }
	string getType() const { return type; }
};


//奖项管理系统类
class AwardSystem {
private:
	vector<Award> awards;//存储所有奖项的变量
	const string data_file = "awards.csv"; // 持久化文件路径（CSV格式）
public:
	const string getFile() { return data_file; }
	void mainMenu();//展示菜单，进入主循环
	void insertAward();//插入新的奖项
	void delAward();//根据编号删除奖项
	void showAwards();//展示现有的奖项
	void saveAwards() const;//保存结果到文件
};

void writeAwards(const vector<Award>& awards, const string data_file);// 序列化：内存→文件
void readAwards(vector<Award>& awards, const string data_file);// 反序列化：文件→内存

#endif  // FUNCTION_H