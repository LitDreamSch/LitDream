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
#include <vector>

#define MAX_NUM 2147483646
#define LDQQ 2830770517

using namespace std;

int ac = -1; //AuthCode 调用酷Q的方法时需要用到
bool enabled = false;



// 猜拳
// return
// 	string 返回应该发送的字符串
// parameter
// 	string item
//     O  1
//     Y  2
//     B   3
string morra(string item,LDAccount act){

	LDAccount LD(LDQQ);
	string ENTER_ERROR = "[X] 猜拳的正确格式是 morra*石头/剪刀/布 哟，连这个还要小梦教你啊";
	string TOO_POOR = "[X] " + act.Name + " ,不是我说，你也太穷了，怎么玩啊";
	string LD_TOO_POOR = "[X] 小梦已经太穷了，玩不起游戏了";

	vector<string> item_name;
	item_name.push_back("1");
	item_name.push_back("石头");
	item_name.push_back("剪刀");
	item_name.push_back("布");

	int n = 0;
	if(item == "石头")
		n = 1;
	else if(item == "剪刀")
		n = 2;
	else if(item == "布")
		n = 3;
	if(n == 0) return ENTER_ERROR;

	if(act.Dcoin < 2) return TOO_POOR;

	// 胜利表，0为平局，1为获胜，-1为输
	int vince[4][4] = {
		{0,0,0,0},
		{0,0,1,-1},
		{0,-1,0,1},
		{0,1,-1,0}
	};

	int ld = get_randint(1,3*10);
	_sleep(998); // 防止两个随机数时间过短
	if(ld <= 3) ld = 2;
	if(ld > 3 && ld <= 7) ld = 1;
	if(ld > 7 && ld <= 10) ld = 3;
	if(ld > 10 && ld <=12) ld = 2;
	if(ld > 12 && ld <= 17) ld = 3;
	if(ld > 17 && ld <= 21) ld = 1;
	if(ld > 21 && ld <= 23) ld = 3;
	if(ld > 23 && ld <= 28) ld = 2;
	if(ld > 28) ld = 1;

	

	string RESULT = "[...] Little Dream's Morra!\n[...] " + act.Name + " -> " + item_name[n] + " : " + item_name[ld] + " <- 小梦\n";

	if(vince[n][ld] == 1){
		int get_dcoin = get_randint(1,5000) % 3 + 1;
		if(get_dcoin > LD.Dcoin)// 小梦玩不起了
			return LD_TOO_POOR;
		
		RESULT += "[!!!] " + act.Name + "获得了Morra的胜利！\n[!!!] 从小梦那里得到了 " + to_string(get_dcoin) + " Dcoin!\n[...] Dcoin:" + to_string(act.Dcoin) + " -> " + to_string(act.Dcoin + get_dcoin);
		RESULT += "\n[...] 小梦的Dcoin: " + to_string(LD.Dcoin) + " -> " + to_string(LD.Dcoin - get_dcoin);
		act.Dcoin += get_dcoin;
		act.save();

		LD.Dcoin -= get_dcoin;
		LD.save();
	}
	if(vince[n][ld] == -1){
		int lose_dcoin = get_randint(1,5000) % 3 + 1;
		RESULT += "[...] " + act.Name + "输掉了Morra\n[!!!] 被小梦拿走了 " + to_string(lose_dcoin) + " Dcoin!\n[...] Dcoin:" + to_string(act.Dcoin) + " -> " + to_string(act.Dcoin - lose_dcoin);
		RESULT += "\n[...] 小梦的Dcoin: " + to_string(LD.Dcoin) + " -> " + to_string(LD.Dcoin + lose_dcoin);
		act.Dcoin -= lose_dcoin;
		act.save();
		LD.Dcoin += lose_dcoin;
		LD.save();
	}
	if(!vince[n][ld])
		RESULT += "[...] " + act.Name + "在本局Morra中和小梦打了个平手，一点都不精彩";

	return RESULT;

}

// 猜数字游戏进入函数
// RETURN:
//	string -> 返回应该发送的文本
// PARAMETER:
//	LDAccount act -> 账户类
//	int64_t start_num -> 数字上限
string start_guess_num(LDAccount act,int64_t start_num){
	string IN_GAME = "[X] 已经进入过猜数字游戏了，小梦不允许半途而废\n[...] guess*num 猜数字\n[...] guess*exit 退出猜数字";
	string START_GAME = "[√] 开始猜数字游戏咯！范围 [1,";
	string START_NUM_OUT_RANGE = "[X] 启动数字不对哟，启动数字要大于等于1，小于等于2147483646"; 
	

	if(start_num <= 0 || start_num > MAX_NUM) // 启动数字不正确
		return START_NUM_OUT_RANGE;

	if(act.guess_num != "-1") // 正在游戏之中
		return IN_GAME;

	int num = get_randint(1,start_num);
	// l:r:num:step
	START_GAME += to_string(start_num) + "] ,小梦就让你先开始吧\n[...] guess*num 猜数字";
	act.guess_num = "1:" + to_string(start_num) + ":" + to_string(num) + ":0";
	act.save();
	return START_GAME;
}

// 猜数字
// RETURN:
//	string -> 返回应该发送的文本
// PARAMETER:
//	LDAccount act -> 账户类
//	int number -> 猜的数字
//	int64_t fromGroup -> 群号
string guess_num(LDAccount act,int64_t number,int64_t fromGroup){
	string OUT_GAME = "[X] 还没开始游戏呢，先输入 guess*start*start_num 启动游戏";
	string OUT_RANGE = "[X] 猜数的范围错啦，范围是 [";
	string TOO_POOR = "[X] " +act.Name+ "啊，你太穷了，玩不起的";
	string LD_TOO_POOR = "[X] 小梦已经没有钱了 T T";
	LDAccount LD(LDQQ);

	if(LD.Dcoin < 2) // 小梦太穷了
		return LD_TOO_POOR;

	if(act.Dcoin < 2)//太穷了
		return TOO_POOR;

	// l:r:num:step
	int64_t l,r,num,step;
	string s = act.guess_num;


	// 取出数据
	l = atoi(s.substr(0,s.find(':')).c_str());
	s = s.substr(s.find(':') + 1);
	r = atoi(s.substr(0,s.find(':')).c_str());
	s = s.substr(s.find(':') + 1);
	num = atoi(s.substr(0,s.find(':')).c_str());
	s = s.substr(s.find(':') + 1);
	step = atoi(s.substr(0,s.find(':')).c_str());

	if(act.guess_num == "-1") // 没有开始游戏
		return OUT_GAME;


	if(number > r || number < l){ // 超过范围
		OUT_RANGE = OUT_RANGE + to_string(l) + ',' + to_string(r) + "]";
		return OUT_RANGE;
	}

	if(number == num){ // 玩家猜中
		string SUCCESS = "[*****************]\n[!!!] " + act.Name + " 猜中了数字 " + to_string(num) + " !获得了本轮猜数字的胜利!\n[!!!] 获得Dcoin: " + to_string(step * 4) + "\n[!!!] Dcoin: " + to_string(act.Dcoin) + " -> " + to_string(act.Dcoin + step * 4); 
		act.Dcoin += step * 4;
		act.guess_num = "-1";
		act.save();
		return SUCCESS;
	}

	

	// 如果玩家没有猜中
	string REMSG = "[***] GUESSNUM\n[...] " +act.Name+ " VS 小梦\n " + act.Name + " 没有命中数字\n[...] Dcoin: " + to_string(act.Dcoin) + " -> " + to_string(act.Dcoin - 2) + "\n[...] 区间变化 ";
	act.Dcoin -= 2;
	
	if(number < num){
		l = number + 1;
		REMSG += '[' + to_string(number + 1) + ',' + to_string(r) + ']';
		act.guess_num = to_string(number + 1) + ':' + to_string(r) + ':' + to_string(num) + ':' + to_string(step + 1);
	}else{
		r = number - 1;
		REMSG += '[' + to_string(l) + ',' + to_string(number - 1) + ']';
		act.guess_num = to_string(l) + ':' + to_string(number - 1) + ':' + to_string(num) + ':' + to_string(step + 1);
	}

	CQ_sendGroupMsg(ac,fromGroup,REMSG.data());
	act.save();

	_sleep(1000);


	int64_t LDGuess = (l + r) / 2;// 小梦猜数字最优决策
	if(LDGuess == num){ // 小梦猜中
		string SUCCESS = "[%%%%%%%%%%%%%%%%%]\n[...] " + LD.Name + " 猜中了数字 " + to_string(num) + " !获得了本轮猜数字的胜利!\n[...] 获得Dcoin: " + to_string(step * 4) + "\n[!!!] Dcoin: " + to_string(LD.Dcoin) + " -> " + to_string(LD.Dcoin + step * 4); 
		LD.Dcoin += step * 4;
		act.guess_num = "-1";
		LD.save();
		act.save();
		return SUCCESS;
	}

	// 小梦没有猜中
	string LD_REMSG = "[***] GUESSNUM\n[...] " +act.Name+ " VS 小梦\n[...] " + LD.Name + " 没有命中数字(" + to_string(LDGuess) + ")\n[...] Dcoin: " + to_string(LD.Dcoin) + " -> " + to_string(LD.Dcoin - 2) + "\n[...] 区间变化 ";

	if(LDGuess < num){
		l = LDGuess + 1;
		LD_REMSG += '[' + to_string(LDGuess + 1) + ',' + to_string(r) + ']';
		act.guess_num = to_string(LDGuess + 1) + ':' + to_string(r) + ':' + to_string(num) + ':' + to_string(step + 1);
	}else{
		r = LDGuess - 1;
		LD_REMSG += '[' + to_string(l) + ',' + to_string(LDGuess - 1) + ']';
		act.guess_num = to_string(l) + ':' + to_string(LDGuess - 1) + ':' + to_string(num) + ':' + to_string(step + 1);
	}
	LD.Dcoin -= 2;
	LD.save();
	act.save();

	return LD_REMSG;

}

// 退出猜数字游戏
// RETURN:
//	string -> 返回应该发送的文本
// PARAMETER:
//	LDAccount act -> 账户类

string out_guess_num(LDAccount act){
	string OUT_GAME = "[X] 还没开始游戏呢，先输入 guess*start 启动游戏";
	act.guess_num = "-1";
	act.save();
	return "[√] 成功退出游戏";
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

	LDAccount act(fromQQ);
	string remsg = "";


	if(message == "account"){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数
		}


		remsg = act.print_account();
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());

		return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数

	}

	//账户注册
	if(message.find("account_reg") == 0){
		act.save();
		remsg = "[√] 注册成功，输入 account 查看账户";
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数

	}

	//账户修改
	//account_set*name*要修改的昵称
	if(message.find("account_set") == 0){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数
		}
		string key,value;
		get_star_content(message,key,value);
		remsg = act.act_set(key,value);
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数

	}

	// 签到
	if(message.find("sign") == 0){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数
		}
		remsg = act.sign();
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数
	}

	// 自定义account菜单样式
	if(message.find("account_model") == 0){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数
		}
		string model = message.substr(message.find('*') + 1);
		remsg = act.set_account_model(model);
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK;
	}

	
	// 猜拳
	if(message.find("morra") == 0){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数
		}
		remsg = morra(message.substr(message.find('*') + 1),act);
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK;
	}

	// 查看小梦账户
	if(message.find("account*LD") == 0){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数
		}
		LDAccount LD(LDQQ);
		remsg =  "//==========\n";
		remsg += "\\\\ 小梦\n" ;
		remsg += "// Dcoin: " + to_string(LD.Dcoin) + " \n";
		remsg += "\\\\==========";
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK;
	}

	// 开始猜数字
	if(message.find("guess*start") == 0){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数
		}
		message = message.substr(message.find('*') + 1);
		remsg = start_guess_num(act,atoi(message.substr(message.find('*') + 1).c_str()));
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK;
	}

	// 猜数字退出
	if(message.find("guess*exit") == 0){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数
		}
		remsg = out_guess_num(act);
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK;
	}

	// 猜数字游戏
	if(message.find("guess*") == 0){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //关于返回值说明, 见“_eventPrivateMsg”函数
		}
		remsg = guess_num(act,atoi(message.substr(message.find('*') + 1).c_str()),fromGroup);
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK;
	}

	
	
	act.clean();
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
