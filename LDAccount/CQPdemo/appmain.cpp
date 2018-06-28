/*
* CoolQ Demo for VC++
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����
#include "fstream"
#include "ctime"
#include "../../lib/LDAccount/LDAccount.h"
#include <vector>

#define MAX_NUM 2147483646
#define LDQQ 2830770517

using namespace std;

int ac = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
bool enabled = false;



// ��ȭ
// return
// 	string ����Ӧ�÷��͵��ַ���
// parameter
// 	string item
//     O  1
//     Y  2
//     B   3
string morra(string item,LDAccount act){

	LDAccount LD(LDQQ);
	string ENTER_ERROR = "[X] ��ȭ����ȷ��ʽ�� morra*ʯͷ/����/�� Ӵ���������ҪС�ν��㰡";
	string TOO_POOR = "[X] " + act.Name + " ,������˵����Ҳ̫���ˣ���ô�氡";
	string LD_TOO_POOR = "[X] С���Ѿ�̫���ˣ��治����Ϸ��";

	vector<string> item_name;
	item_name.push_back("1");
	item_name.push_back("ʯͷ");
	item_name.push_back("����");
	item_name.push_back("��");

	int n = 0;
	if(item == "ʯͷ")
		n = 1;
	else if(item == "����")
		n = 2;
	else if(item == "��")
		n = 3;
	if(n == 0) return ENTER_ERROR;

	if(act.Dcoin < 2) return TOO_POOR;

	// ʤ����0Ϊƽ�֣�1Ϊ��ʤ��-1Ϊ��
	int vince[4][4] = {
		{0,0,0,0},
		{0,0,1,-1},
		{0,-1,0,1},
		{0,1,-1,0}
	};

	int ld = get_randint(1,3*10);
	_sleep(998); // ��ֹ���������ʱ�����
	if(ld <= 3) ld = 2;
	if(ld > 3 && ld <= 7) ld = 1;
	if(ld > 7 && ld <= 10) ld = 3;
	if(ld > 10 && ld <=12) ld = 2;
	if(ld > 12 && ld <= 17) ld = 3;
	if(ld > 17 && ld <= 21) ld = 1;
	if(ld > 21 && ld <= 23) ld = 3;
	if(ld > 23 && ld <= 28) ld = 2;
	if(ld > 28) ld = 1;

	

	string RESULT = "[...] Little Dream's Morra!\n[...] " + act.Name + " -> " + item_name[n] + " : " + item_name[ld] + " <- С��\n";

	if(vince[n][ld] == 1){
		int get_dcoin = get_randint(1,5000) % 3 + 1;
		if(get_dcoin > LD.Dcoin)// С���治����
			return LD_TOO_POOR;
		
		RESULT += "[!!!] " + act.Name + "�����Morra��ʤ����\n[!!!] ��С������õ��� " + to_string(get_dcoin) + " Dcoin!\n[...] Dcoin:" + to_string(act.Dcoin) + " -> " + to_string(act.Dcoin + get_dcoin);
		RESULT += "\n[...] С�ε�Dcoin: " + to_string(LD.Dcoin) + " -> " + to_string(LD.Dcoin - get_dcoin);
		act.Dcoin += get_dcoin;
		act.save();

		LD.Dcoin -= get_dcoin;
		LD.save();
	}
	if(vince[n][ld] == -1){
		int lose_dcoin = get_randint(1,5000) % 3 + 1;
		RESULT += "[...] " + act.Name + "�����Morra\n[!!!] ��С�������� " + to_string(lose_dcoin) + " Dcoin!\n[...] Dcoin:" + to_string(act.Dcoin) + " -> " + to_string(act.Dcoin - lose_dcoin);
		RESULT += "\n[...] С�ε�Dcoin: " + to_string(LD.Dcoin) + " -> " + to_string(LD.Dcoin + lose_dcoin);
		act.Dcoin -= lose_dcoin;
		act.save();
		LD.Dcoin += lose_dcoin;
		LD.save();
	}
	if(!vince[n][ld])
		RESULT += "[...] " + act.Name + "�ڱ���Morra�к�С�δ��˸�ƽ�֣�һ�㶼������";

	return RESULT;

}

// ��������Ϸ���뺯��
// RETURN:
//	string -> ����Ӧ�÷��͵��ı�
// PARAMETER:
//	LDAccount act -> �˻���
//	int64_t start_num -> ��������
string start_guess_num(LDAccount act,int64_t start_num){
	string IN_GAME = "[X] �Ѿ��������������Ϸ�ˣ�С�β������;����\n[...] guess*num ������\n[...] guess*exit �˳�������";
	string START_GAME = "[��] ��ʼ��������Ϸ������Χ [1,";
	string START_NUM_OUT_RANGE = "[X] �������ֲ���Ӵ����������Ҫ���ڵ���1��С�ڵ���2147483646"; 
	

	if(start_num <= 0 || start_num > MAX_NUM) // �������ֲ���ȷ
		return START_NUM_OUT_RANGE;

	if(act.guess_num != "-1") // ������Ϸ֮��
		return IN_GAME;

	int num = get_randint(1,start_num);
	// l:r:num:step
	START_GAME += to_string(start_num) + "] ,С�ξ������ȿ�ʼ��\n[...] guess*num ������";
	act.guess_num = "1:" + to_string(start_num) + ":" + to_string(num) + ":0";
	act.save();
	return START_GAME;
}

// ������
// RETURN:
//	string -> ����Ӧ�÷��͵��ı�
// PARAMETER:
//	LDAccount act -> �˻���
//	int number -> �µ�����
//	int64_t fromGroup -> Ⱥ��
string guess_num(LDAccount act,int64_t number,int64_t fromGroup){
	string OUT_GAME = "[X] ��û��ʼ��Ϸ�أ������� guess*start*start_num ������Ϸ";
	string OUT_RANGE = "[X] �����ķ�Χ��������Χ�� [";
	string TOO_POOR = "[X] " +act.Name+ "������̫���ˣ��治���";
	string LD_TOO_POOR = "[X] С���Ѿ�û��Ǯ�� T T";
	LDAccount LD(LDQQ);

	if(LD.Dcoin < 2) // С��̫����
		return LD_TOO_POOR;

	if(act.Dcoin < 2)//̫����
		return TOO_POOR;

	// l:r:num:step
	int64_t l,r,num,step;
	string s = act.guess_num;


	// ȡ������
	l = atoi(s.substr(0,s.find(':')).c_str());
	s = s.substr(s.find(':') + 1);
	r = atoi(s.substr(0,s.find(':')).c_str());
	s = s.substr(s.find(':') + 1);
	num = atoi(s.substr(0,s.find(':')).c_str());
	s = s.substr(s.find(':') + 1);
	step = atoi(s.substr(0,s.find(':')).c_str());

	if(act.guess_num == "-1") // û�п�ʼ��Ϸ
		return OUT_GAME;


	if(number > r || number < l){ // ������Χ
		OUT_RANGE = OUT_RANGE + to_string(l) + ',' + to_string(r) + "]";
		return OUT_RANGE;
	}

	if(number == num){ // ��Ҳ���
		string SUCCESS = "[*****************]\n[!!!] " + act.Name + " ���������� " + to_string(num) + " !����˱��ֲ����ֵ�ʤ��!\n[!!!] ���Dcoin: " + to_string(step * 4) + "\n[!!!] Dcoin: " + to_string(act.Dcoin) + " -> " + to_string(act.Dcoin + step * 4); 
		act.Dcoin += step * 4;
		act.guess_num = "-1";
		act.save();
		return SUCCESS;
	}

	

	// ������û�в���
	string REMSG = "[***] GUESSNUM\n[...] " +act.Name+ " VS С��\n " + act.Name + " û����������\n[...] Dcoin: " + to_string(act.Dcoin) + " -> " + to_string(act.Dcoin - 2) + "\n[...] ����仯 ";
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


	int64_t LDGuess = (l + r) / 2;// С�β��������ž���
	if(LDGuess == num){ // С�β���
		string SUCCESS = "[%%%%%%%%%%%%%%%%%]\n[...] " + LD.Name + " ���������� " + to_string(num) + " !����˱��ֲ����ֵ�ʤ��!\n[...] ���Dcoin: " + to_string(step * 4) + "\n[!!!] Dcoin: " + to_string(LD.Dcoin) + " -> " + to_string(LD.Dcoin + step * 4); 
		LD.Dcoin += step * 4;
		act.guess_num = "-1";
		LD.save();
		act.save();
		return SUCCESS;
	}

	// С��û�в���
	string LD_REMSG = "[***] GUESSNUM\n[...] " +act.Name+ " VS С��\n[...] " + LD.Name + " û����������(" + to_string(LDGuess) + ")\n[...] Dcoin: " + to_string(LD.Dcoin) + " -> " + to_string(LD.Dcoin - 2) + "\n[...] ����仯 ";

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

// �˳���������Ϸ
// RETURN:
//	string -> ����Ӧ�÷��͵��ı�
// PARAMETER:
//	LDAccount act -> �˻���

string out_guess_num(LDAccount act){
	string OUT_GAME = "[X] ��û��ʼ��Ϸ�أ������� guess*start ������Ϸ";
	act.guess_num = "-1";
	act.save();
	return "[��] �ɹ��˳���Ϸ";
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
	string no_account = "[X]С������û������˻�,���� account_reg ��ע��һ���˻�";

	LDAccount act(fromQQ);
	string remsg = "";


	if(message == "account"){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
		}


		remsg = act.print_account();
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());

		return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������

	}

	//�˻�ע��
	if(message.find("account_reg") == 0){
		act.save();
		remsg = "[��] ע��ɹ������� account �鿴�˻�";
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������

	}

	//�˻��޸�
	//account_set*name*Ҫ�޸ĵ��ǳ�
	if(message.find("account_set") == 0){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
		}
		string key,value;
		get_star_content(message,key,value);
		remsg = act.act_set(key,value);
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������

	}

	// ǩ��
	if(message.find("sign") == 0){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
		}
		remsg = act.sign();
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
	}

	// �Զ���account�˵���ʽ
	if(message.find("account_model") == 0){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
		}
		string model = message.substr(message.find('*') + 1);
		remsg = act.set_account_model(model);
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK;
	}

	
	// ��ȭ
	if(message.find("morra") == 0){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
		}
		remsg = morra(message.substr(message.find('*') + 1),act);
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK;
	}

	// �鿴С���˻�
	if(message.find("account*LD") == 0){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
		}
		LDAccount LD(LDQQ);
		remsg =  "//==========\n";
		remsg += "\\\\ С��\n" ;
		remsg += "// Dcoin: " + to_string(LD.Dcoin) + " \n";
		remsg += "\\\\==========";
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK;
	}

	// ��ʼ������
	if(message.find("guess*start") == 0){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
		}
		message = message.substr(message.find('*') + 1);
		remsg = start_guess_num(act,atoi(message.substr(message.find('*') + 1).c_str()));
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK;
	}

	// �������˳�
	if(message.find("guess*exit") == 0){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
		}
		remsg = out_guess_num(act);
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK;
	}

	// ��������Ϸ
	if(message.find("guess*") == 0){
		if(!act.is_loaded()){
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());
			return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
		}
		remsg = guess_num(act,atoi(message.substr(message.find('*') + 1).c_str()),fromGroup);
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK;
	}

	
	
	act.clean();
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
CQEVENT(int32_t, __menuA, 0)() {
	MessageBoxA(NULL, "����menuA�����������봰�ڣ����߽�������������", "" ,0);
	return 0;
}

CQEVENT(int32_t, __menuB, 0)() {
	MessageBoxA(NULL, "����menuB�����������봰�ڣ����߽�������������", "" ,0);
	return 0;
}
