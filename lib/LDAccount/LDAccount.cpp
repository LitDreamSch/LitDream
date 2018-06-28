#include "stdafx.h"
#include "LDAccount.h"
#include "string"
#include "fstream"
#include "ctime"
using namespace std;

void get_star_content(string s,string &_head, string &_value){
  string head = "";
  string value = "";
  int i = 1;
  while(s[i] != '*')
	  i++;
  i++;
  for(i;s[i] != '*';i ++)
    head += s[i];

  i ++ ;

  for(i;i < s.size();i ++)
    value += s[i];

  _head = head;
  _value = value;

}

string& replace_all(string& str,const string& old_value,const string& new_value){
  while(true){
		string::size_type pos(0);
		if((pos=str.find(old_value))!=string::npos)
    	str.replace(pos,old_value.length(),new_value);
    else break;
  }
	return str;
}

string format_time(){
	time_t t = time(0);
	char temp[64] = {'\0'};
  string s;
	strftime(temp,sizeof(temp),"%Y%m%d",localtime(&t));
  s = temp;
	return s;
}


int get_randint(int64_t a,int64_t b){
	srand(time(NULL));
	return rand() % (b - a + 1) + a;
}


string get_level_name(LDAccount act){
	// 打开文件
	string level_name_file_str = "LDAccount\\setfile\\signlevel.txt";
	ifstream level_name(level_name_file_str);

	string each_level = "", key = "", value = "";
	string current_level = "1.域真空的基态.";
	bool find = false; // 如果最后没有找到合适的话就表示为最高等级

	if(level_name.is_open()){
		// each_level = "2.XXXXXX=5"
		while(getline(level_name,each_level)){

			key = "";value = "";
			int i = 0;
			for(i = 0;each_level[i] != '='; i++) // 获得key
				key += each_level[i];
			i++;
			for(i; i < each_level.size(); i++) // 获得value
				value += each_level[i];

			if(act.SignNum < atoi(value.c_str())) {level_name.close();return current_level;}
			current_level = key;

		}
	}

	level_name.close();
	return current_level;

}

string account_rep(LDAccount act){
	string menu_msg = "";
	string temp = "";
	ifstream model("LDAccount\\" + act.QQNum + "\\menumodel.txt");

	if(model.is_open()){
		while(getline(model,temp)) menu_msg += temp;
	}else{ // 如果不存在model文件的话就使用默认样式
		menu_msg = "+-----------\n| 昵称 : " + act.Name +
				   "\n| 等级 : "     + act.SignLevel + "( " + to_string(act.SignNum) + " )" +
				   "\n| Dcoin : "     + to_string(act.Dcoin) +
				   "\n+-----------"  +
				   "\n| 教了小梦 :" + to_string(act.WordsDir) + " 词." +
				   "\n+-----------";
	}

	// 生成系统时间标准字符串
	time_t t = time(0);
	char _temp[64] = {'\0'};
	string s;
	strftime(_temp,sizeof(_temp),"%Y%m%d%H%M%S",localtime(&t));
	s = _temp;
	// YYYYMMDDHHmmSS
	string year = s.substr(0,4);
	string month = s.substr(4,2);
	string day = s.substr(6,2);
	string hour = s.substr(8,2);
	string minute = s.substr(10,2);
	string second = s.substr(12,2);

	replace_all(menu_msg,"&amp;Name&amp;",act.Name);
	replace_all(menu_msg,"&amp;Dcoin&amp;",to_string(act.Dcoin));
	replace_all(menu_msg,"&amp;SignNum&amp;",to_string(act.SignNum));
	replace_all(menu_msg,"&amp;SignLevel&amp;",act.SignLevel);
	replace_all(menu_msg,"&amp;WordsDir&amp;",to_string(act.WordsDir));
	replace_all(menu_msg,"&amp;Hour&amp;",hour);
	replace_all(menu_msg,"&amp;Minute&amp;",minute);
	replace_all(menu_msg,"&amp;Second&amp;",second);
	replace_all(menu_msg,"&amp;Day&amp;",day);
	replace_all(menu_msg,"&amp;Year&amp;",year);
	replace_all(menu_msg,"&amp;Month&amp;",month);
	replace_all(menu_msg,"\\n","\n");

	return menu_msg;
}


LDAccount::LDAccount(int64_t QQNum){
  ifstream act_in("LDAccount\\" + to_string(QQNum) + "\\info.txt");


	string temp = "";
	string key = "",value = ""; // key=value
  this -> QQNum = to_string(QQNum);
	if(act_in.is_open()){
		while(getline(act_in,temp)){

			key = "";value = "";
			int i = 0;
			for(i = 0;temp[i] != '='; i++) // 获得key
				key += temp[i];
			i++;
			for(i; i < temp.size(); i++) // 获得value
				value += temp[i];
			if(key.size() != 0 && value.size() != 0){
        // cout << key << " " << value << endl;
				if(key == "Name") this -> Name = value;
				if(key == "birday") this ->Birday = value;
				if(key == "Dcoin") this -> Dcoin = atoi(value.c_str());
				if(key == "SignNum") this -> SignNum = atoi(value.c_str());
				if(key == "SignLevel") this -> SignLevel = value;
				if(key == "wordsdir") this -> WordsDir = atoi(value.c_str());
				if(key == "signtime") this -> SignTime = value;
				if(key == "buy_big_lottery_time") this -> buy_big_lottery_time = value;
				if(key == "guess_num") this -> guess_num = value;
			}
		}

		act_in.close();
	}

  act_in.close();
}

bool LDAccount::read(int64_t QQNum){
  ifstream act_in("LDAccount\\" + to_string(QQNum) + "\\info.txt");

	string temp = "";
	string key = "",value = ""; // key=value
  this -> QQNum = to_string(QQNum);
	if(act_in.is_open()){
		while(getline(act_in,temp)){

			key = "";value = "";
			int i = 0;
			for(i = 0;temp[i] != '='; i++) // 获得key
				key += temp[i];
			i++;
			for(i; i < temp.size(); i++) // 获得value
				value += temp[i];
			if(key.size() != 0 && value.size() != 0){
        // cout << key << " " << value << endl;
				if(key == "Name") this -> Name = value;
				if(key == "birday") this ->Birday = value;
				if(key == "Dcoin") this -> Dcoin = atoi(value.c_str());
				if(key == "SignNum") this -> SignNum = atoi(value.c_str());
				if(key == "SignLevel") this -> SignLevel = value;
				if(key == "wordsdir") this -> WordsDir = atoi(value.c_str());
				if(key == "signtime") this -> SignTime = value;
				if(key == "buy_big_lottery_time") this -> buy_big_lottery_time = value;
				if(key == "guess_num") this -> guess_num = value;
			}
		}

		act_in.close();
		return true;
	}

  act_in.close();
	return false;
}


string LDAccount::print_account(){
  string menu_msg = "";
	menu_msg = account_rep(*this);
	return menu_msg;
}

void LDAccount::clean(){
  this -> Name = "";
}

bool LDAccount::save(){
  ofstream info_out("LDAccount\\" + this -> QQNum + "\\info.txt");

	// 写入初始化数据
	if(info_out.is_open()){
		info_out << "Name=" << this -> Name << endl;
		info_out << "Birday=" << this -> Birday << endl;
		info_out << "Dcoin=" << this -> Dcoin << endl;
		info_out << "SignNum=" << this -> SignNum << endl;
		info_out << "SignLevel=" << this -> SignLevel << endl;
		info_out << "wordsdir=" << this -> WordsDir << endl;
		info_out << "signtime=" << this -> SignTime << endl;
		info_out << "buy_big_lottery_time=" << this -> buy_big_lottery_time << endl;
		info_out <<	"guess_num=" << this -> guess_num << endl;


		info_out.close();
		return true;
	}

	return false;
}

string LDAccount::act_set(string key,string value){
	string CANNOT_CAHNGE = "[X]该项目不可以进行修改的.";
	string TOO_LONG = "[X]修改的值太长了,我懒得往数据库里面写";
	string CHANGE_OK = "[√]修改成功!";
	// 修改昵称
	if(key == "name"){
		if(value.size() <= 20){
			this -> Name = value;
			(*this).save();
			return CHANGE_OK;
		}
		else return TOO_LONG;
	}

	if(key == "sex"){
		if(value.size() <= 20){

			(*this).save();
			return CHANGE_OK;
		}
		else return TOO_LONG;
	}
	return CANNOT_CAHNGE;
}


string LDAccount::set_account_model(string model){
  string FAILED = "[X] 因为某种原因修改样式失败了(失败函数:set_account_model)";
	string SUCCESS = "[√] 成功修改样式!";
	string model_str = "LDAccount\\" + this -> QQNum + "\\menumodel.txt";

	ofstream model_out(model_str);
	if(model_out.is_open()){
		model_out << model;
		model_out.close();
		return SUCCESS;
	}
	model_out.close();
	return FAILED;
}

string LDAccount::sign(){
  string SIGNED = "[X]今天已经签到过了，请不要再签到了，否则小梦要私吞你的Dcoin了！";
	string SIGN = "[√] " + this -> Name + " 签到成功!";
	string current_time = format_time();

	if(this -> SignTime == current_time){ // 已经签到过了
		int punishment = get_randint(0,10);
		SIGNED += "\n[...] " + this -> Name + " 被小梦强行夺走 " + to_string(punishment) + " Dcoin";
		this -> Dcoin -= punishment;
		(*this).save();
		return SIGNED;

	}else{
		int get_dcoin = get_randint(5,20);
		this -> Dcoin += get_dcoin;
		this -> SignTime = current_time;
		this -> SignNum += 1;

		SIGN += "\n[...] Dcoin: " + to_string(this -> Dcoin - get_dcoin) + " -> " + to_string(this -> Dcoin) + " (+" + to_string(get_dcoin) + ")";

		string updata_level = get_level_name(*this); // 查看是否升级
		if(updata_level != this -> SignLevel){
			SIGN += "\n[!!!] " + this -> Name + " 的等级提升了！\n[!!!] " + this -> SignLevel + " -> " + updata_level;
			this -> SignLevel = updata_level;
		}



		//为了增加签到的娱乐性，加入几个事件
		srand(time(NULL));
		int amuse = get_randint(0,100);
		string EVENT = "";

		// 不给Dcoin
		if(amuse <= 5){
			string EVENT = "\n[!!!] 不过因为小梦没钱了所以这次就不给 " + this -> Name + " Dcoin了.";
			EVENT += "\n[...] Dcoin: " + to_string(this -> Dcoin) + " -> " + to_string(this -> Dcoin - get_dcoin) + " (-" + to_string(get_dcoin) + ")";

			this -> Dcoin -= get_dcoin;
			SIGN += EVENT;
		}

		// 给双倍Dcoin
		if(amuse >5 && amuse <= 10){
			string EVENT = "\n[!!!] 小梦今天心情超级好，多给 " + this -> Name + " 两倍的Dcoin";
			this -> Dcoin += get_dcoin;
			EVENT += "\n[...] Dcoin: " + to_string(this -> Dcoin - get_dcoin) + " -> " + to_string(this -> Dcoin) + " (+" + to_string(get_dcoin) + ")";
		}


		(*this).save();
		return SIGN;

	}
}

bool LDAccount::is_loaded(){
  if(this -> Name == "") return false;
  else return true;
}
