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
using namespace std;

int ac = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
bool enabled = false;


// ��ȡ����ļ�
// bool
// true -> �ɹ���ȡ
// false -> ��ȡʧ��
string big_lottery_str = "LDAccount\\setfile\\biglottery.txt";
string READ_FAILED = "[X] ��ȡ�ļ�ʧ�ܣ�ʧ�ܴ��룺LDLottery";
bool bought[32] = {0};// true -> û�б�����
int money = 0; // ����
int times = 0; // ��
string open_time = "";// �������ڣ�һ��һ��
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
			if(tmp.find("remain=") == 0){// ʣ��Ĳ�Ʊ�������

				tmp = tmp.substr(tmp.find('=') + 1);//1,2,3
				while(tmp.size()){
					tmp_int = atoi(tmp.substr(0,tmp.find(',')).c_str());
					tmp = tmp.substr(tmp.find(',') + 1);
					bought[tmp_int] = true;
				}

			}
			if(tmp.find("money=") == 0)// ����
				money = atoi(tmp.substr(tmp.find('=') + 1).c_str());

			if(tmp.find("times=") == 0){// ����
				times = atoi(tmp.substr(tmp.find('=') + 1).c_str());
			}

			if(tmp.find("opentime") == 0){// ����ʱ��
				open_time = tmp.substr(tmp.find('=') + 1).c_str();
			}

			if(tmp.find("&") == 0){// �����Ʊ����
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






// �������ļ�
// bool
// true -> ��������
// false -> ����ʧ��
bool save_big_lottery(){
	ofstream lottery_out(big_lottery_str);
	if(lottery_out.is_open()){

		// ����ʣ���Ʊ
		lottery_out << "remain=";
		for(int i = 1;i <= 30;i++)
			if(bought[i]) lottery_out << to_string(i) << ',';
		lottery_out << endl;

		// ���潱��
		lottery_out << "money=" << to_string(money) << endl;

		// ��������
		lottery_out << "times=" << times << endl;

		// ���濪������
		lottery_out << "opentime=" << open_time << endl;

		// ���湺����Ϣ
		for(int i = 0;i < buy_peos_pop;i++)
			lottery_out << '&' << buy_peos[i].QQNum << '=' << to_string(buy_peos[i].buy_num) << endl;


		lottery_out.close();
		return true;
	}


	lottery_out.close();
	return false;

}


// ��ʹ�����
// string
// ���ص�ΪҪ���͵��ı�
// int64_t fromQQ  ->  ���Ե�QQ
// int buy_num     ->  ���������
// int fee         ->  Ʊ��
string buy_big_lottery_menu(int64_t fromQQ,int buy_num,int fee, LDAccount act ){
	bool read_ok = read_big_lottery();
	if(!read_ok) return READ_FAILED;
	string BOUGHT = "[X] �����Ѿ�����ˣ�������������";
	string BOUGHT_BY_OTHERS = "[X] ����Ѿ�����������ˣ���һ�����ְ�";
	string TOO_POOR = "[X] ��̫���ˣ������(" + to_string(fee) + " Dcoin)";
	string SAVE_FAILED = "[X] ����ʧ�ܣ���麯��(buy_big_loter_menu)";

	if(act.buy_big_lottery_time == to_string(times)) // �����Ѿ������
		return BOUGHT;

	if(!bought[buy_num]) // ����Ѿ�������
		return BOUGHT_BY_OTHERS;

	if(act.Dcoin < fee) // ̫����
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

	string SUCCESS = "[��] �ɹ����һ��LDLoter��";
	return SUCCESS;

}

// ��ʿ�������
// ����ֵΪӦ�÷��͵��ı�

string big_lottery_open(LDAccount act){
	bool read_ok = read_big_lottery();
	if(!read_ok) return READ_FAILED;
	string OPENED_TODAY = "[X] �����Ѿ�����������������";
	string HEAD = "[!!!] ����ڴ��� Loter ������ʽ��ʼ�ˣ���ô���ξ�����û���˻������أ���\n[...] ����������......\n";

	if(open_time == format_time()) return OPENED_TODAY;

	int number = get_randint(1,30);
	// int number = 19;

	open_time = format_time();

	HEAD += "[...] ���ε� Loter ����Ϊ : " + to_string(number) + '\n';
	if(bought[number]){//û�������
		HEAD += "[...] �ܱ�Ǹû�������б��� Loter�����ٽ�����";
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

			HEAD += "[!!!] ��ϲ " + act.Name + " ���б��� Loter !!\n";
			HEAD += "[!!!] ��� Dcoin : " + to_string(money) + " !!\n";
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


// ��ʲ鿴����
// string
// ����ֵΪӦ�÷��͵��ı�
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
			if(bought[current_num]){// û�б���������ӡ
				if(current_num / 10 == 0){ // ֻ�и�λ����ʱ�����һ��0
					menu_msg += '0';
				}
				menu_msg += to_string(current_num);
				menu_msg += ' ';
			}else{// ������ˣ�����
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
	string remsg = "";

	LDAccount act(fromQQ);

	CQ_sendGroupMsg(ac,fromGroup,"ok");

	// �鿴������˵�
	if(message.find("loter_menu") == 0){
		if(!act.is_loaded())
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());

		remsg = get_big_lottery_menu();
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������

	}
	// �����ʺ���
	if(message.find("loter_buy") == 0){
		if(!act.is_loaded())
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());

		int num = atoi(message.substr(message.find('*') + 1).c_str());
		if(num > 30 || num <= 0){
			remsg = "[X] Loter �ķ�ΧΪ 1 ~ 30 ";
			CQ_sendGroupMsg(ac,fromGroup,remsg.data());
			return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������

		}else{
			remsg = buy_big_lottery_menu(fromQQ,num,10,act);
			CQ_sendGroupMsg(ac,fromGroup,remsg.data());
			return EVENT_BLOCK; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
		}
	}

	// ��ʿ�Ʊ
	if(message.find("loter_open") == 0){
		if(!act.is_loaded())
			CQ_sendGroupMsg(ac,fromGroup,no_account.data());



		remsg = big_lottery_open(act);
		CQ_sendGroupMsg(ac,fromGroup,remsg.data());
		return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
	}


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
