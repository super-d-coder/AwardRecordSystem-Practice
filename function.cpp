/**
 * @file function.cpp
 * @brief 奖项管理系统核心功能实现文件
 * @details 包含文件序列化/反序列化、菜单交互、奖项增删查等核心逻辑，
 *          实现内存数据（vector<Award>）与文件（awards.txt）的持久化交互
 * @author （可填写你的名字）
 * @date 2025-11-23
 */

#include "function.h"
#include <iostream>
#include <limits>

using namespace std;

/**
 * @brief 序列化：将内存中的奖项数据写入文件（CSV格式）
 * @param awards 待序列化的奖项容器（const引用避免拷贝，保证只读）
 * @param data_file 目标文件路径（const保证路径不可修改）
 * @note 1. 采用ios::out | ios::trunc模式打开文件，覆盖原有内容；
 *       2. CSV格式：奖项名称,获奖时间,奖项类型（每行一个奖项）；
 *       3. 流操作失败时通过cerr输出错误（无缓冲，优先提示）
 */
void writeAwards(const vector<Award>& awards, const string data_file) {
	// 构造输出文件流，绑定目标文件，写模式+截断原有内容
	ofstream out_file(data_file, ios::out | ios::trunc);

	// 检测文件流状态：打开失败则输出错误并返回
	if (!out_file) {
		cerr << "错误：文件awards.txt无法打开" << endl; // cerr无缓冲，直接输出错误
		return;
	}

	// 遍历奖项容器，逐行写入CSV格式数据（const&避免拷贝，保证只读）
	for (const Award& a : awards) {
		out_file << a.getName() << "," << a.getTime() << "," << a.getType() << endl;
	}

	// 显式关闭文件：刷新缓冲区+释放文件句柄（RAII也会自动关闭，显式更规范）
	out_file.close();
}

/**
 * @brief 反序列化：从CSV文件读取奖项数据到内存容器
 * @param awards 存储读取结果的奖项容器（非const引用，修改原容器）
 * @param data_file 源文件路径（const保证路径不可修改）
 * @note 1. 采用ios::in模式打开文件，文件不存在时仅提示，不报错；
 *       2. 解析CSV行时跳过格式错误的行（逗号数量不足）；
 *       3. getline读取整行，避免空格截断奖项名称
 */
void readAwards(vector<Award>& awards, const string data_file) {
	// 构造输入文件流，绑定目标文件，读模式
	ifstream in_file(data_file, ios::in);

	// 文件不存在/打开失败：提示无历史数据，直接返回
	if (!in_file) {
		cout << "提示：无历史数据，awards.txt不存在" << endl;
		return;
	}

	string line; // 存储文件中读取的单行CSV数据
	// 逐行读取文件：流有效时继续（未到文件尾+无读写错误）
	while (getline(in_file, line)) {
		// 查找第一个逗号位置（分隔奖项名称和时间）
		size_t step1 = line.find(',');
		// 从第一个逗号后查找第二个逗号（分隔时间和类型）
		size_t step2 = line.find(',', step1 + 1);
		// 格式校验：逗号数量不足则跳过该行，输出提示
		if (step1 == string::npos || step2 == string::npos) {
			cout << "该行数据有误" << endl;
			continue;
		}
		// 截取奖项名称：从开头到第一个逗号
		string name = line.substr(0, step1);
		// 截取获奖时间：第一个逗号后到第二个逗号
		string time = line.substr(step1 + 1, step2-step1-1);
		// 截取奖项类型：第二个逗号后到行尾
		string type = line.substr(step2 + 1);

		// 就地构造Award对象并插入容器（避免临时对象拷贝，效率更高）
		awards.emplace_back(name, time, type);
	}

	// 显式关闭文件：释放文件句柄
	in_file.close();
}

/**
 * @brief 奖项系统主菜单逻辑
 * @details 1. 程序启动时先读取文件数据到内存；
 *          2. 展示初始奖项列表；
 *          3. 循环接收用户输入，处理菜单选择；
 *          4. 每次操作后保存数据到文件，退出时终止循环
 * @note 1. 输入验证仅允许数字0-3，非数字输入会清空缓冲区并提示；
 *       2. 每次循环结束后调用saveAwards()，保证数据实时持久化
 */
void AwardSystem::mainMenu()
{
	// 程序启动：从文件读取历史奖项数据到内存容器
	readAwards(awards, data_file);

	// 展示初始加载的奖项列表
	showAwards();

	int choice = 0;// 用户菜单选择（0-3）
	bool running = true;// 系统运行标志（true=继续，false=退出）
	while (running) {
		cout << endl;//操作后换行，更加美观
		cout << "输入一个正整数" << endl;
		cout << "0表示展示当前奖项库\n1表示插入新的奖项\n2表示删除现有的奖项\n3表示退出" << endl;

		// ========== 输入验证：仅允许数字输入 ==========
		// cin >> choice失败（非数字输入）时，进入循环修正
		while (!(cin >> choice)) {
			// 重置cin错误状态（清除failbit标志）
			cin.clear();
			// 清空输入缓冲区：删除所有字符直到换行符（清除非数字错误输入）
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			// 提示用户重新输入
			cout << "❌ 输入错啦！只能输数字0-3，请重新输入：";
		}

		// 菜单选择分支处理
		switch (choice) {
		case 0:
			showAwards();// 展示所有奖项
			break;
		case 1:
			insertAward();// 插入新奖项
			break;
		case 2:
			delAward();// 删除现有奖项（待实现）
			break;
		case 3:
			running = false;// 终止循环，退出系统
			break;
		default:
			cout << "输入数字不合法，请重试！" << endl;
		}
	}
	//操作后保存数据：保证内存数据同步到文件
	saveAwards();
}

/**
 * @brief 插入新奖项到内存容器
 * @note 1. 先清空cin缓冲区残留的换行符（避免getline跳过输入）；
 *       2. getline读取整行输入，支持带空格的奖项名称；
 *       3. 空值校验：名称/时间/类型不能为空，否则提示错误并返回；
 *       4. 插入成功后数据会在mainMenu中自动保存到文件
 */
void AwardSystem::insertAward()
{
	string name;   // 奖项名称
	string time;   // 获奖时间
	string type;   // 奖项类型
	cin.clear(); // 重置cin错误状态（清除之前输入验证的错误标志）
	// 清空cin缓冲区：删除所有字符直到换行符（清除菜单选择残留的换行符）
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "-----接下来输入新奖项-----" << endl;
	cout << "奖项名称:";
	getline(cin, name); // 读取奖项名称（支持空格）
	cout << "获奖时间:";
	getline(cin, time); // 读取获奖时间（支持空格）
	cout << "奖项类型:";
	getline(cin, type); // 读取奖项类型（支持空格）
	// 空值校验：任一字段为空则提示错误并返回
	if (name.empty() || time.empty() || type.empty()) {
		cerr << "错误：输入有误" << endl;
		return;
	}
	// 就地构造Award对象并插入容器（内存层面添加奖项）
	awards.emplace_back(name, time, type);
}


/**
 * @brief 删除现有奖项（已实现按索引删除）
 * @details 1. 支持按奖项编号（索引）删除；
 *          2. 校验输入合法性（非数字/越界/空容器）；
 *          3. 删除后自动同步文件数据（mainMenu中调用saveAwards）
 * @todo 1. 扩展支持按奖项名称模糊删除（可选）；
 */
void AwardSystem::delAward()
{
	// 空容器判断
	if (awards.empty()) {
		cerr << "错误：当前奖项库为空，无奖项可删除" << endl;
		return;
	}

	cout << "请输入要删除奖项的编号:";
	int index = 0;//读取用户要删除的编号

	// 非数字输入验证（和mainMenu保持一致）
	while (!(cin >> index)) {
		cin.clear(); // 重置错误状态
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清空错误输入
		cerr << "❌ 输入错误！只能输入数字，请重新输入：";
	}

	//检测删除的奖项是否存在,若不存在重新读取
	if (index <= 0 || index > awards.size()) {
		cerr << "错误：要删除的奖项不存在" << endl;
		return;
	}

	auto it = awards.begin();//创建迭代器寻找要删除的元素
	it = it + index - 1;//指向要删除的目标

	if (it >= awards.end()) { // 双重校验，避免极端情况越界
		cerr << "错误：要删除的奖项不存在" << endl;
		return;
	}

	it = awards.erase(it);//删除指定目标
	cout << "成功删除编号为" << index << "的奖项，删除该奖项后表变为：" << endl;
	showAwards();
}



/**
 * @brief 展示内存中所有奖项
 * @note 遍历奖项容器，按CSV格式输出每个奖项的属性（名称,时间,类型）
 */
void AwardSystem::showAwards()
{
	cout << "-----------奖项清单------------" << endl;

	// 遍历容器：const&避免拷贝，保证只读
	int num = 1;//定义奖项编号，方便计数
	for (const Award& a : awards) {
		cout << num << ' ';
		cout << a.getName() << "\t" << a.getTime() << "\t" << a.getType() << endl;
		num++;
	}
}

/**
 * @brief 保存内存奖项数据到文件（封装writeAwards）
 * @details 调用全局函数writeAwards，将类内awards容器写入data_file指定的文件
 * @note const成员函数：保证不修改类内成员变量，符合const正确性
 */
void AwardSystem::saveAwards() const
{
	writeAwards(awards, data_file);
}