#include <iostream>
#include <string>
#include <limits>
using namespace std;

//奖项类
class Award {
private:
	string name;
	string time;
	string type;
};


//奖项管理系统类
class AwardSystem {
private:
	//存储所有奖项的变量
public:
	void mainMenu();//展示菜单，进入主循环
	void insertAward();//插入新的奖项
	void delAward();//删除奖项
	void showAwards();//展示现有的奖项
};



int main() {
	int a;
	AwardSystem sys;
	sys.mainMenu();
	return 0;
}

void AwardSystem::mainMenu()
{
	int choice = 0;//用户选择
	bool running = true;//是否继续运行系统
	while (running) {
		cout << "-------------------------------------------" << endl;
		cout << "输入一个正整数" << endl;
		cout << "0表示展示当前奖项库\n1表示插入新的奖项\n2表示删除现有的奖项\n3表示退出" << endl;
		cin >> choice;

		// ========== 以下是输入验证核心代码（逐行解释）ai完成 ==========
			// 第一步：检查输入是不是数字（cin >> choice 失败=非数字）
			while (!(cin >> choice)) {
				// 1. 清空cin的错误状态（把“读不懂”的状态重置）
				cin.clear();
				// 2. 删掉缓冲区里的错内容（比如用户输的a、@，全清掉）
				// numeric_limits<streamsize>::max() 就是“清空所有”的意思
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				// 3. 提示用户重新输入
				cout << "❌ 输入错啦！只能输数字0-3，请重新输入：";
			}
		// ========== 验证代码结束 ===

		switch (choice) {
		case 0:
			showAwards();
			break;
		case 1:
			insertAward();
			break;
		case 2:
			delAward();
			break;
		case 3:
			running = false;//退出系统
			break;
		default:
			cout << "输入数字不合法，请重试！" << endl;
		}
	}
}

void AwardSystem::insertAward()
{
}

void AwardSystem::delAward()
{
}

void AwardSystem::showAwards()
{
}
