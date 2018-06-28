/*
* CoolQ Demo for VC++
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //应用AppID等信息，请正确填写，否则酷Q可能无法加载
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <ctime>
#include "..\..\lib\LDAccount\LDAccount.h"
using namespace std;

// #define DEBUG

int ac = -1; //AuthCode 调用酷Q的方法时需要用到
bool enabled = false;

map <string,vector<string>> dict_root;
vector<string> dict_path[30000];
int pop = 0;
string dict_add = "Dict.json"; // 字典相对位置
bool dict_readed = false;



// 取字符串中的“”的内容
string get_content(string s){
  string ret = "";
  for(int i = s.find('\"') + 1;i <= s.size();i ++){
    if(s[i] == '\"') break;
    ret += s[i];
  }

  return ret;
}

// 统计现在词典的关键字数量以及回答数量
// int &key_cnt -> 返回现在有的关键字数量
// int &ans_cnt -> 返回现在有的回答数量
void dict_cnt(int &key_cnt,int &ans_cnt){
  map<string,vector<string>>::iterator it;
  it = dict_root.begin();
  int key = dict_root.size(),ans = 0;

  while(it != dict_root.end()){
    ans += it -> second.size();
    it ++ ;
  }
  key_cnt = key;
  ans_cnt = ans;
}

// 保存现在的字典文件
// 如果保存成功则返回true,否则为false

bool save_dict(){
  ofstream dict_out("Dict.json");
  bool ok = false;
  map<string,vector<string>>::iterator it;
  it = dict_root.begin();

  if(dict_out.is_open()){
    ok = true;
    dict_out << "{" << endl;

    while(it != dict_root.end()){
      if(it -> second.size() <= 0) {it++;continue;} // 如果该关键字已经不存在回复项，则进行删除
      dict_out << "\t\"" << it -> first << "\": [" << endl;// 开始一个新的关键字

      for(int i = 0;i < it -> second.size();i ++){
        dict_out << "\t\t\"" << it -> second[i] << "\"";
        if(i != it -> second.size() - 1)// 最后一个输出不包含逗号
          dict_out << ",";
        dict_out << endl;
      }

      dict_out << "\t]";

      if(it != dict_root.end()) // 最后一个输出不包含逗号
        dict_out << ",";
      dict_out << endl;
      it ++;
    }

    dict_out << "}" << endl;
  }


  dict_out.close();

  if(ok) return true;
  else return false;
}

// 小梦学习函数
// string _head  关键字
// string _value 回复语句
void LDstudy(string _head,string _value,int64_t fromGroup,int64_t fromQQ){
  LDAccount act(fromQQ);
  if(!act.is_loaded()) {CQ_sendGroupMsg(ac,fromGroup,"[X]小梦这里没有你的账户,输入 account_reg 来注册一个账户");return;}

  int key1 = 0,ans1 = 0;
  int key2 = 0,ans2 = 0;
  // 先保存现在词典的key 和 ans 在key1 和 ans1 之中
  dict_cnt(key1,ans1);

	// 加入dict_root
	if(dict_root.find(_head) == dict_root.end())
    dict_root.insert(make_pair(_head,dict_path[pop ++]));

  // 检测该回复方式是否已经存在
  bool ans_existed = false;
  for(int i = 0; i < dict_root[_head].size(); i++){
    if(dict_root[_head][i] == _value){
      ans_existed = true;
      break;
    }
  }

  string return_msg = "";

  if(!ans_existed) //如果该回复方式不存在，则加入字典之中
    dict_root[_head].push_back(_value);
  else{
    return_msg += "[X]小梦早就知道怎么回答这个问题了！";
    CQ_sendGroupMsg(ac,fromGroup,return_msg.data());
    return;
  }


  bool write_ok = save_dict();// 保存字典

  if(!write_ok){ // 保存失败，可能是文件被占用
    CQ_sendGroupMsg(ac,fromGroup,"[X]小梦的数据库似乎出了什么问题，这次什么都没有学会[sad");
    return;
  }

  //比较之前的数据
  dict_cnt(key2,ans2);

  //增加添加者的贡献词数
  act.WordsDir += 1;
  act.save();
  act.clean();

  return_msg += "[Y]小梦学会了新的词语！\n[...]关键词数:";
  return_msg += to_string(key1);
  return_msg += " -> ";
  return_msg += to_string(key2);
  return_msg += "\n";
  return_msg += "[...]回答句数:";
  return_msg += to_string(ans1);
  return_msg += " -> ";
  return_msg += to_string(ans2);
  return_msg += "\n";
  return_msg += "[...]对 ";
  return_msg += _head;
  return_msg += " 的回答方式有 ";
  return_msg += to_string(dict_root[_head].size());
  return_msg += " 种\n";
  return_msg += "[...] " + act.Name + " 一共教了小梦 " + to_string(act.WordsDir) + " 个词！";

  CQ_sendGroupMsg(ac,fromGroup,return_msg.data());


}



// 小梦词库删除函数
// string
//     返回的为要发送的文本
// string _head    -> 关键字
// string _value   -> 要删除的语句
string LDelete(string _head,string _value){
  int key1 = 0,ans1 = 0;
  int key2 = 0,ans2 = 0;
  // 先保存现在词典的key 和 ans 在key1 和 ans1 之中
  dict_cnt(key1,ans1);
  string failed = "[X]小梦根本就不知道这个问题怎么回答啦！";

  // 查看字典中有无对应的关键字
  if(dict_root.find(_head) == dict_root.end()){// 没有这个关键字，回复failed
    return failed;
  }

  // 检测该回复方式是否存在
  bool ans_existed = false;
  for(int i = 0; i < dict_root[_head].size(); i++){
    if(dict_root[_head][i] == _value){
      ans_existed = true;
      // 删除元素
      dict_root[_head].erase(dict_root[_head].begin() + i);
      if(dict_root[_head].size() == 0){ // 该关键字已经没有回复项
        map<string,vector<string>>::iterator it;
        it = dict_root.begin();

        while(it != dict_root.end()){
          if(it -> first == _head){
            dict_root.erase(it);
            break;
          }
          it++;
        }

      }

      break;
    }
  }

  string return_msg = "";

  if(!ans_existed){ //如果该回复方式不存在，则返回failed
    return failed;
  }
  else{ // 该回复方式存在，进行删除

    dict_cnt(key2,ans2);

    return_msg += "[√]删除成功！小梦已经把这个的回复方式遗忘了\n[...]关键词数:";
    return_msg += to_string(key1);
    return_msg += " -> ";
    return_msg += to_string(key2);
    return_msg += "\n";
    return_msg += "[...]回答句数:";
    return_msg += to_string(ans1);
    return_msg += " -> ";
    return_msg += to_string(ans2);
    return_msg += "\n";
    return_msg += "[...]对 ";
    return_msg += _head;
    return_msg += " 的回答方式有 ";
    return_msg += to_string(dict_root[_head].size());
    return_msg += " 种";

    bool write_ok = save_dict();// 保存字典
    return return_msg;
  }

  return failed;

}


// 读取小梦的字典函数
void read_dict_json(){
  ifstream dict_in(dict_add);
  string buffer, buffer_content,current_head;
  int cnt = 0;
  while(getline(dict_in,buffer)){
      // 当读入的buffer中含有{}时，忽略不计
      // 当读入的buffer中含有[ 时，申请一个新的vector储存分支
      if(buffer.find('[') != std::string::npos){

        string head_content = get_content(buffer);
        current_head = head_content;
        dict_root.insert(make_pair(head_content,dict_path[pop ++]));

      }else if(buffer.find(']') != std::string::npos) {
        continue; // 一个支路读取完毕
      }
      else if(buffer.find('{') == std::string::npos && buffer.find('}') == std::string::npos){ // 正常信息读取
        buffer_content = get_content(buffer);
        dict_root[current_head].push_back(buffer_content);
      }
  }

  dict_in.close();
}

// 小梦聊天函数
// bool
//     true  -> 小梦成功回复
//     false -> 小梦拒绝回复
// int64_t fromGroup -> QQ群号码
// string message    -> 接收到的信息
// int retpro        -> 小梦回复的概率
bool LDSpeak(int64_t fromGroup,string message,int retpro,int same_ret){


	// 小梦一定概率以相同的语句回复别人
	srand(time(NULL));
	int same_pro = rand() % 100;
	// CQ_sendGroupMsg(ac,fromGroup,to_string(same_pro).data());

	if(same_pro <= same_ret){
		CQ_sendGroupMsg(ac,fromGroup,message.data());
		return true;
	}

	// 进行小梦的概率回复
	srand(time(NULL));
	int return_pro = rand() % 100;

	if(return_pro <= (100 - retpro))
		return false;

	vector <string> proret; // proret中存放着匹配的关键字

	map<string,vector<string>>::iterator it;
	it = dict_root.begin();

	while(it != dict_root.end()){ // 统计关键字
		if(message.find(it -> first) != std::string::npos && it -> second.size() > 0){// 匹配到该关键字
			proret.push_back(it -> first);
		}
		it ++ ;
	}


	if(proret.size() != 0){ // 当匹配到了关键字的时候，进行回复
		srand(time(NULL));
		int which_key_word = rand() % proret.size();
		srand(time(NULL));
		int which_to_say = rand() % dict_root[proret[which_key_word]].size();

		CQ_sendGroupMsg(ac,fromGroup,dict_root[proret[which_key_word]][which_to_say].data());
		return true;
	}
  return false;
}


// 小梦回复分散函数
// int64_t fromGroup -> QQ群号码
// int64_t fromQQ    -> QQ号码
// string message    -> 接受消息
void LDisperse(int64_t fromGroup,int64_t fromQQ,string message){
  string _head,_value;
  string return_msg;

  // 小梦学习
  // *study*key*value

  if(message.find("*study") == 0){

    get_star_content(message,_head,_value);
    LDstudy(_head,_value,fromGroup,fromQQ);
    return; //关于返回值说明, 见“_eventPrivateMsg”函数
  }

  if(message.find("*delete") == 0){
    get_star_content(message,_head,_value);

//    string debug_str = "";
//    debug_str += _head + " " + _value;
//    CQ_sendGroupMsg(ac,fromGroup,debug_msg.data());


	  return_msg = LDelete(_head,_value);
    CQ_sendGroupMsg(ac,fromGroup,return_msg.data());
    return;
  }

  // 小梦回复
  bool success_to_speak = LDSpeak(fromGroup,message,50,3);// 调用小梦聊天函数，注意小梦的聊天函数应该最后一个调用
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
  if(!dict_readed){ // 防止小梦的字典被多次读取
  	read_dict_json();
  	dict_readed = true;
  }

  LDisperse(fromGroup,fromQQ,message);


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
