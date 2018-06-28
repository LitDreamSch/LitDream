/*
* CoolQ Demo for VC++
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //应用AppID等信息，请正确填写，否则酷Q可能无法加载
#include "fstream"
#include "ctime"
#include "../../lib/LDAccount/LDAccount.h"
using namespace std;

int ac = -1; //AuthCode 调用酷Q的方法时需要用到
bool enabled = false;


// 读取大彩文件
// bool
// true -> 成功读取
// false -> 读取失败
string big_lottery_str = "LDAccount\\setfile\\biglottery.txt";
string READ_FAILED = "[X] 读取文件失败，失败代码：LDLottery";
bool bought[32] = {0};// true -> 没有被购买
int money = 0; // 奖池
int times = 0; // 期
string open_time = "";// 开奖日期，一天一次
struct buy_peo{
	string QQNum;
	int buy_num;
} buy_peos[20] = {"",0};
int buy_peos_pop = 0;

bool read_big_lottery(){
	ifstream lottery_in(big_lottery_str);
	string tmp = "";
	int tmp_int = 0;
	buy_peos_pop = 0;

	if(lottery_in.is_open()){
		while(getline(lottery_in,tmp)){
			if(tmp.find("remain=") == 0){// 剩余的彩票购买情况

				tmp = tmp.substr(tmp.find('=') + 1);//1,2,3
				while(tmp.size()){
					tmp_int = atoi(tmp.substr(0,tmp.find(',')).c_str());
					tmp = tmp.substr(tmp.find(',') + 1);
					bought[tmp_int] = true;
				}

			}
			if(tmp.find("money=") == 0)// 奖池
				money = atoi(tmp.substr(tmp.find('=') + 1).c_str());

			if(tmp.find("times=") == 0){// 期数
				times = atoi(tmp.substr(tmp.find('=') + 1).c_str());
			}

			if(tmp.find("opentime") == 0){// 开奖时间
				open_time = tmp.substr(tmp.find('=') + 1).c_str();
			}

			if(tmp.find("&") == 0){// 购买彩票的人
				buy_peos[buy_peos_pop].QQNum = tmp.substr(1,tmp.find('=') - 1);
				buy_peos[buy_peos_pop].buy_num = atoi(tmp.substr(tmp.find('=') + 1).c_str());

				buy_peos_pop ++;
			}
		}

		lottery_in.close();
		return true;
	}
	lottery_in.close();
	return false;
}






// 保存大彩文件
// bool
// true -> 正常保存
// false -> 保存失败
bool save_big_lottery(){
	ofstream lottery_out(big_lottery_str);
	if(lottery_out.is_open()){

		// 保存剩余彩票
		lottery_out << "remain=";
		for(int i = 1;i <= 30;i++)
			if(bought[i]) lottery_out << to_string(i) << ',';
		lottery_out << endl;

		// 保存奖池
		lottery_out << "money=" << to_string(money) << endl;

		// 保存期数
		lottery_out << "times=" << times << endl;

		// 保存开奖日期
		lottery_out << "opentime=" << open_time << endl;

		// 保存购买信息
		for(int i = 0;i < buy_peos_pop;i++)
			lottery_out << '&' << buy_peos[i].QQNum << '=' << to_string(buy_peos[i].buy_num) << endl;


		lottery_out.close();
		return true;
	}


	lottery_out.close();
	return false;

}


// 大彩购买函数
// string
// 返回的为要发送的文本
// int64_t fromQQ  ->  来自的QQ
// int buy_num     ->  购买的数字
// int fee         ->  票价
string buy_big_lottery_menu(int64_t fromQQ,int buy_num,int fee, LDAccount act ){
	bool read_ok = read_big_lottery();
	if(!read_ok) return READ_FAILED;
	string BOUGHT = "[X] 今天已经买过了，不可以再买了";
	string BOUGHT_BY_OTHERS = "[X] 这个已经被别人买过了，换一个数字吧";
	string TOO_POOR = "[X] 你太穷了，买不起的(" + to_string(fee) + " Dcoin)";
	string SAVE_FAILED = "[X] 保存失败，检查函数(buy_big_loter_menu)";

	if(act.buy_big_lottery_time == to_string(times)) // 今天已经买过了
		return BOUGHT;

	if(!bought[buy_num]) // 如果已经被购买
		return BOUGHT_BY_OTHERS;

	if(act.Dcoin < fee) // 太穷了
		return TOO_POOR;

	bought[buy_num] = false;
	act.Dcoin -= fee;
	act.buy_big_lottery_time = to_string(times);
	money += fee;
	buy_peos[buy_peos_pop].QQNum = to_string(fromQQ);
	buy_peos[buy_peos_pop].buy_num = buy_num;


	buy_peos_pop ++;

	bool ok = save_big_lottery();
	if(!ok) return SAVE_FAILED;
	act.save();

	string SUCCESS = "[√] 成功获得一张LDLoter！";
	return SUCCESS;

}

// 大彩开奖函数
// 返回值为应该发送的文本

string big_lottery_open(LDAccount act){
	bool read_ok = read_big_lottery();
	if(!read_ok) return READ_FAILED;
	string OPENED_TODAY = "[X] 今天已经开过咯，请等明天吧";
	string HEAD = "[!!!] 大家期待的 Loter 启动仪式开始了！那么本次究竟有没有人会命中呢？！\n[...] 号码生成中......\n";

	if(open_time == format_time()) return OPENED_TODAY;

	int number = get_randint(1,30);
	// int number = 19;

	open_time = format_time();

	HEAD += "[...] 本次的 Loter 号码为 : " + to_string(number) + '\n';
	if(bought[number]){//没有人买过
		HEAD += "[...] 很抱歉没有人命中本次 Loter，请再接再厉";
		for(int i = 1;i <= 30; i++ )
			bought[i] = 1;
		times ++;
		buy_peos_pop = 0;
		save_big_lottery();

		return HEAD;
	}

	for(int i = 0;i < buy_peos_pop;i ++){

		if(buy_peos[i].buy_num == number){
			LDAccount act(atoi(buy_peos[i].QQNum.c_str()));

			HEAD += "[!!!] 恭喜 " + act.Name + " 命中本次 Loter !!\n";
			HEAD += "[!!!] 获得 Dcoin : " + to_string(money) + " !!\n";
			HEAD += "[!!!] Dcoin : " + to_string(act.Dcoin) + " -> " + to_string(act.Dcoin + money);

			for(int i = 1;i <= 30; i++ )
				bought[i] = 1;
			times ++;
			buy_peos_pop = 0;
			money = 30;
			save_big_lottery();
			act.save();
			return HEAD;
		}

	}
}


// 大彩查看函数
// string
// 返回值为应该发送的文本
string get_big_lottery_menu(){
	bool read_ok = read_big_lottery();
	if(!read_ok) return READ_FAILED;

	string menu_msg = "*****!!LDLoter!!*****\n";
	                // * 01 02 03 04 05 06 *
	                // * 07 08 09 10 11 12 *
	                // * 13 14 15 16 17 18 *
	                // * 19 20 21 22 23 24 *
	                // * 25 26 27 28 29 30 *
	                // *********************
	                // * Total Dcoin:
	                // *********************
	                // # Loter_bug*Num BUY *
	                // # Loter_open   Open *
	                // *********************

  int current_num = 0;
	for(int line = 0;line < 5;line ++){
		menu_msg += "* ";
		for(int i = 1; i <= 6;i ++){
			current_num = line * 6 + i;
			if(bought[current_num]){// 没有被买过，则打印
				if(current_num / 10 == 0){ // 只有个位数的时候加上一个0
					menu_msg += '0';
				}
				menu_msg += to_string(current_num);
				menu_msg += ' ';
			}else{// 被买过了，留空
				menu_msg += "     ";
			}

		}
		menu_msg += "\n";
	}

	menu_msg += "*********************\n";
	menu_msg += "* Total Dcoin: " + to_string(money) + '\n';
	menu_msg += "*********************\n";
	menu_msg += "# loter_buy*Num BUY \n";
	menu_msg += "# loter_open   Open \n";
	menu_msg += "*********************";

	return menu_msg;
}
/*
* 返回应用的ApiVer、Appid，打包后将不会调用
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/*
* 接收应用AuthCode，酷Q读取应用信息后，如果接受该应用，将会调用这个函数并传递AuthCode。
* 不要在本函数处理其他任何代码，以免发生异常情况。如需执行初始化代码请在Startup事件中执行（Type=1001）。
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 酷Q启动
* 无论本应用是否被启用，本函数都会在酷Q启动后执行一次，请在这里执行应用初始化代码。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventStartup, 0)() {

	return 0;
}


/*
* Type=1002 酷Q退出
* 无论本应用是否被启用，本函数都会在酷Q退出前执行一次，请在这里执行插件关闭代码。
* 本函数调用完毕后，酷Q将很快关闭，请不要再通过线程等方式执行其他代码。
*/
CQEVENT(int32_t, __eventExit, 0)() {

	return 0;
}

/*
* Type=1003 应用已被启用
* 当应用被启用后，将收到此事件。
* 如果酷Q载入时应用已被启用，则在_eventStartup(Type=1001,酷Q启动)被调用后，本函数也将被调用一次。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	return 0;
}


/*
* Type=1004 应用将被停用
* 当应用被停用前，将收到此事件。
* 如果酷Q载入时应用已被停用，则本函数*不会*被调用。
* 无论本应用是否被启用，酷Q关闭前本函数都*不会*被调用。
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}


/*
* Type=21 私聊消息
* subType 子类型，11/来自好友 1/来自在线状态 2/来自群 3/来自讨论组
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t msgId, int64_t fromQQ, const char *msg, int32_t font) {

	//如果要回复消息，请调用酷Q方法发送，并且这里 return EVENT_BLOCK - 截断本条消息，不再继续处理  注意：应用优先级设置为"最高"(10000)时，不得使用本返回值
	//如果不回复消息，交由之后的应用/过滤器处理，这里 return EVENT_IGNORE - 忽略本条消息
	return EVENT_IGNORE;
}


/*
* Type=2 群消息
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t msgId, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
	string message = msg;
	string no_account = "[X]小梦这里没有你的账户,输入 account_reg 来注册一个账户";
	string remsg = "";

	LDAccount act(fromQQ);

	CQ_sendGroupMsg(ac,fromGroup,"ok");

	// 查看大彩主菜单
	if(message.find("loter_menu") == 0){
		if(!act.is_loaded())
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());

		remsg = get_big_lottery_menu();
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数

	}
	// 购买大彩函数
	if(message.find("loter_buy") == 0){
		if(!act.is_loaded())
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());

		int num = atoi(message.substr(message.find('*') + 1).c_str());
		if(num > 30 || num <= 0){
			remsg = "[X] Loter 的范围为 1 ~ 30 ";
			CQ_sendGroupMsg(ac,fromGroup,remsg.data());
			return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数

		}else{
			remsg = buy_big_lottery_menu(fromQQ,num,10,act);
			CQ_sendGroupMsg(ac,fromGroup,remsg.data());
			return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数
		}
	}

	// 大彩开票
	if(message.find("loter_open") == 0){
		if(!act.is_loaded())
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());



		remsg = big_lottery_open(act);
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
	}


	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=4 讨论组消息
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t msgId, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=101 群事件-管理员变动
* subType 子类型，1/被取消管理员 2/被设置管理员
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=102 群事件-群成员减少
* subType 子类型，1/群员离开 2/群员被踢 3/自己(即登录号)被踢
* fromQQ 操作者QQ(仅subType为2、3时存在)
* beingOperateQQ 被操作QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=103 群事件-群成员增加
* subType 子类型，1/管理员已同意 2/管理员邀请
* fromQQ 操作者QQ(即管理员QQ)
* beingOperateQQ 被操作QQ(即加群的QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=201 好友事件-好友已添加
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=301 请求-好友添加
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}


/*
* Type=302 请求-群添加
* subType 子类型，1/他人申请入群 2/自己(即登录号)受邀入群
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}

	return EVENT_IGNORE; //关于返回值说明, 见“_eventPrivateMsg”函数
}

/*
* 菜单，可在 .json 文件中设置菜单数目、函数名
* 如果不使用菜单，请在 .json 及此处删除无用菜单
*/
CQEVENT(int32_t, __menuA, 0)() {
	MessageBoxA(NULL, "这是menuA，在这里载入窗口，或者进行其他工作。", "" ,0);
	return 0;
}

CQEVENT(int32_t, __menuB, 0)() {
	MessageBoxA(NULL, "这是menuB，在这里载入窗口，或者进行其他工作。", "" ,0);
	return 0;
}
