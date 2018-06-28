/*
* CoolQ Demo for VC++
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����
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

int ac = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
bool enabled = false;

map <string,vector<string>> dict_root;
vector<string> dict_path[30000];
int pop = 0;
string dict_add = "Dict.json"; // �ֵ����λ��
bool dict_readed = false;



// ȡ�ַ����еġ���������
string get_content(string s){
  string ret = "";
  for(int i = s.find('\"') + 1;i <= s.size();i ++){
    if(s[i] == '\"') break;
    ret += s[i];
  }

  return ret;
}

// ͳ�����ڴʵ�Ĺؼ��������Լ��ش�����
// int &key_cnt -> ���������еĹؼ�������
// int &ans_cnt -> ���������еĻش�����
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

// �������ڵ��ֵ��ļ�
// �������ɹ��򷵻�true,����Ϊfalse

bool save_dict(){
  ofstream dict_out("Dict.json");
  bool ok = false;
  map<string,vector<string>>::iterator it;
  it = dict_root.begin();

  if(dict_out.is_open()){
    ok = true;
    dict_out << "{" << endl;

    while(it != dict_root.end()){
      if(it -> second.size() <= 0) {it++;continue;} // ����ùؼ����Ѿ������ڻظ�������ɾ��
      dict_out << "\t\"" << it -> first << "\": [" << endl;// ��ʼһ���µĹؼ���

      for(int i = 0;i < it -> second.size();i ++){
        dict_out << "\t\t\"" << it -> second[i] << "\"";
        if(i != it -> second.size() - 1)// ���һ���������������
          dict_out << ",";
        dict_out << endl;
      }

      dict_out << "\t]";

      if(it != dict_root.end()) // ���һ���������������
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

// С��ѧϰ����
// string _head  �ؼ���
// string _value �ظ����
void LDstudy(string _head,string _value,int64_t fromGroup,int64_t fromQQ){
  LDAccount act(fromQQ);
  if(!act.is_loaded()) {CQ_sendGroupMsg(ac,fromGroup,"[X]С������û������˻�,���� account_reg ��ע��һ���˻�");return;}

  int key1 = 0,ans1 = 0;
  int key2 = 0,ans2 = 0;
  // �ȱ������ڴʵ��key �� ans ��key1 �� ans1 ֮��
  dict_cnt(key1,ans1);

	// ����dict_root
	if(dict_root.find(_head) == dict_root.end())
    dict_root.insert(make_pair(_head,dict_path[pop ++]));

  // ���ûظ���ʽ�Ƿ��Ѿ�����
  bool ans_existed = false;
  for(int i = 0; i < dict_root[_head].size(); i++){
    if(dict_root[_head][i] == _value){
      ans_existed = true;
      break;
    }
  }

  string return_msg = "";

  if(!ans_existed) //����ûظ���ʽ�����ڣ�������ֵ�֮��
    dict_root[_head].push_back(_value);
  else{
    return_msg += "[X]С�����֪����ô�ش���������ˣ�";
    CQ_sendGroupMsg(ac,fromGroup,return_msg.data());
    return;
  }


  bool write_ok = save_dict();// �����ֵ�

  if(!write_ok){ // ����ʧ�ܣ��������ļ���ռ��
    CQ_sendGroupMsg(ac,fromGroup,"[X]С�ε����ݿ��ƺ�����ʲô���⣬���ʲô��û��ѧ��[sad");
    return;
  }

  //�Ƚ�֮ǰ������
  dict_cnt(key2,ans2);

  //��������ߵĹ��״���
  act.WordsDir += 1;
  act.save();
  act.clean();

  return_msg += "[Y]С��ѧ�����µĴ��\n[...]�ؼ�����:";
  return_msg += to_string(key1);
  return_msg += " -> ";
  return_msg += to_string(key2);
  return_msg += "\n";
  return_msg += "[...]�ش����:";
  return_msg += to_string(ans1);
  return_msg += " -> ";
  return_msg += to_string(ans2);
  return_msg += "\n";
  return_msg += "[...]�� ";
  return_msg += _head;
  return_msg += " �Ļش�ʽ�� ";
  return_msg += to_string(dict_root[_head].size());
  return_msg += " ��\n";
  return_msg += "[...] " + act.Name + " һ������С�� " + to_string(act.WordsDir) + " ���ʣ�";

  CQ_sendGroupMsg(ac,fromGroup,return_msg.data());


}



// С�δʿ�ɾ������
// string
//     ���ص�ΪҪ���͵��ı�
// string _head    -> �ؼ���
// string _value   -> Ҫɾ�������
string LDelete(string _head,string _value){
  int key1 = 0,ans1 = 0;
  int key2 = 0,ans2 = 0;
  // �ȱ������ڴʵ��key �� ans ��key1 �� ans1 ֮��
  dict_cnt(key1,ans1);
  string failed = "[X]С�θ����Ͳ�֪�����������ô�ش�����";

  // �鿴�ֵ������޶�Ӧ�Ĺؼ���
  if(dict_root.find(_head) == dict_root.end()){// û������ؼ��֣��ظ�failed
    return failed;
  }

  // ���ûظ���ʽ�Ƿ����
  bool ans_existed = false;
  for(int i = 0; i < dict_root[_head].size(); i++){
    if(dict_root[_head][i] == _value){
      ans_existed = true;
      // ɾ��Ԫ��
      dict_root[_head].erase(dict_root[_head].begin() + i);
      if(dict_root[_head].size() == 0){ // �ùؼ����Ѿ�û�лظ���
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

  if(!ans_existed){ //����ûظ���ʽ�����ڣ��򷵻�failed
    return failed;
  }
  else{ // �ûظ���ʽ���ڣ�����ɾ��

    dict_cnt(key2,ans2);

    return_msg += "[��]ɾ���ɹ���С���Ѿ�������Ļظ���ʽ������\n[...]�ؼ�����:";
    return_msg += to_string(key1);
    return_msg += " -> ";
    return_msg += to_string(key2);
    return_msg += "\n";
    return_msg += "[...]�ش����:";
    return_msg += to_string(ans1);
    return_msg += " -> ";
    return_msg += to_string(ans2);
    return_msg += "\n";
    return_msg += "[...]�� ";
    return_msg += _head;
    return_msg += " �Ļش�ʽ�� ";
    return_msg += to_string(dict_root[_head].size());
    return_msg += " ��";

    bool write_ok = save_dict();// �����ֵ�
    return return_msg;
  }

  return failed;

}


// ��ȡС�ε��ֵ亯��
void read_dict_json(){
  ifstream dict_in(dict_add);
  string buffer, buffer_content,current_head;
  int cnt = 0;
  while(getline(dict_in,buffer)){
      // �������buffer�к���{}ʱ�����Բ���
      // �������buffer�к���[ ʱ������һ���µ�vector�����֧
      if(buffer.find('[') != std::string::npos){

        string head_content = get_content(buffer);
        current_head = head_content;
        dict_root.insert(make_pair(head_content,dict_path[pop ++]));

      }else if(buffer.find(']') != std::string::npos) {
        continue; // һ��֧·��ȡ���
      }
      else if(buffer.find('{') == std::string::npos && buffer.find('}') == std::string::npos){ // ������Ϣ��ȡ
        buffer_content = get_content(buffer);
        dict_root[current_head].push_back(buffer_content);
      }
  }

  dict_in.close();
}

// С�����캯��
// bool
//     true  -> С�γɹ��ظ�
//     false -> С�ξܾ��ظ�
// int64_t fromGroup -> QQȺ����
// string message    -> ���յ�����Ϣ
// int retpro        -> С�λظ��ĸ���
bool LDSpeak(int64_t fromGroup,string message,int retpro,int same_ret){


	// С��һ����������ͬ�����ظ�����
	srand(time(NULL));
	int same_pro = rand() % 100;
	// CQ_sendGroupMsg(ac,fromGroup,to_string(same_pro).data());

	if(same_pro <= same_ret){
		CQ_sendGroupMsg(ac,fromGroup,message.data());
		return true;
	}

	// ����С�εĸ��ʻظ�
	srand(time(NULL));
	int return_pro = rand() % 100;

	if(return_pro <= (100 - retpro))
		return false;

	vector <string> proret; // proret�д����ƥ��Ĺؼ���

	map<string,vector<string>>::iterator it;
	it = dict_root.begin();

	while(it != dict_root.end()){ // ͳ�ƹؼ���
		if(message.find(it -> first) != std::string::npos && it -> second.size() > 0){// ƥ�䵽�ùؼ���
			proret.push_back(it -> first);
		}
		it ++ ;
	}


	if(proret.size() != 0){ // ��ƥ�䵽�˹ؼ��ֵ�ʱ�򣬽��лظ�
		srand(time(NULL));
		int which_key_word = rand() % proret.size();
		srand(time(NULL));
		int which_to_say = rand() % dict_root[proret[which_key_word]].size();

		CQ_sendGroupMsg(ac,fromGroup,dict_root[proret[which_key_word]][which_to_say].data());
		return true;
	}
  return false;
}


// С�λظ���ɢ����
// int64_t fromGroup -> QQȺ����
// int64_t fromQQ    -> QQ����
// string message    -> ������Ϣ
void LDisperse(int64_t fromGroup,int64_t fromQQ,string message){
  string _head,_value;
  string return_msg;

  // С��ѧϰ
  // *study*key*value

  if(message.find("*study") == 0){

    get_star_content(message,_head,_value);
    LDstudy(_head,_value,fromGroup,fromQQ);
    return; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
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

  // С�λظ�
  bool success_to_speak = LDSpeak(fromGroup,message,50,3);// ����С�����캯����ע��С�ε����캯��Ӧ�����һ������
}

/*
* ����Ӧ�õ�ApiVer��Appid������󽫲������
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/*
* ����Ӧ��AuthCode����Q��ȡӦ����Ϣ��������ܸ�Ӧ�ã���������������������AuthCode��
* ��Ҫ�ڱ��������������κδ��룬���ⷢ���쳣���������ִ�г�ʼ����������Startup�¼���ִ�У�Type=1001����
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 ��Q����
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q������ִ��һ�Σ���������ִ��Ӧ�ó�ʼ�����롣
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventStartup, 0)() {
	return 0;
}


/*
* Type=1002 ��Q�˳�
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q�˳�ǰִ��һ�Σ���������ִ�в���رմ��롣
* ������������Ϻ󣬿�Q���ܿ�رգ��벻Ҫ��ͨ���̵߳ȷ�ʽִ���������롣
*/
CQEVENT(int32_t, __eventExit, 0)() {

	return 0;
}

/*
* Type=1003 Ӧ���ѱ�����
* ��Ӧ�ñ����ú󣬽��յ����¼���
* �����Q����ʱӦ���ѱ����ã�����_eventStartup(Type=1001,��Q����)�����ú󣬱�����Ҳ��������һ�Ρ�
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	return 0;
}


/*
* Type=1004 Ӧ�ý���ͣ��
* ��Ӧ�ñ�ͣ��ǰ�����յ����¼���
* �����Q����ʱӦ���ѱ�ͣ�ã��򱾺���*����*�����á�
* ���۱�Ӧ���Ƿ����ã���Q�ر�ǰ��������*����*�����á�
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	return 0;
}


/*
* Type=21 ˽����Ϣ
* subType �����ͣ�11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/����������
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t msgId, int64_t fromQQ, const char *msg, int32_t font) {

	//���Ҫ�ظ���Ϣ������ÿ�Q�������ͣ��������� return EVENT_BLOCK - �ضϱ�����Ϣ�����ټ�������  ע�⣺Ӧ�����ȼ�����Ϊ"���"(10000)ʱ������ʹ�ñ�����ֵ
	//������ظ���Ϣ������֮���Ӧ��/�������������� return EVENT_IGNORE - ���Ա�����Ϣ
	return EVENT_IGNORE;
}



/*
* Type=2 Ⱥ��Ϣ
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t msgId, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
  string message = msg;
  if(!dict_readed){ // ��ֹС�ε��ֵ䱻��ζ�ȡ
  	read_dict_json();
  	dict_readed = true;
  }

  LDisperse(fromGroup,fromQQ,message);


  return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=4 ��������Ϣ
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t msgId, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=101 Ⱥ�¼�-����Ա�䶯
* subType �����ͣ�1/��ȡ������Ա 2/�����ù���Ա
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=102 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/ȺԱ�뿪 2/ȺԱ���� 3/�Լ�(����¼��)����
* fromQQ ������QQ(��subTypeΪ2��3ʱ����)
* beingOperateQQ ������QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=103 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/����Ա��ͬ�� 2/����Ա����
* fromQQ ������QQ(������ԱQQ)
* beingOperateQQ ������QQ(����Ⱥ��QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=201 �����¼�-���������
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=301 ����-�������
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=302 ����-Ⱥ���
* subType �����ͣ�1/����������Ⱥ 2/�Լ�(����¼��)������Ⱥ
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}

/*
* �˵������� .json �ļ������ò˵���Ŀ��������
* �����ʹ�ò˵������� .json ���˴�ɾ�����ò˵�
*/
