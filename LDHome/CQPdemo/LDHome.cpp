#include <iostream>
#include "stdafx.h"
#include <fstream>
#include <string>
#include <map>
#include "LDHome.h"
using namespace std;


LDHome::LDHome(int fromQQ){
  string home_str = "LDAccount\\" + to_string(fromQQ) + "\\home.txt";
  string buffer;
  string key,value;

  this -> QQNum = to_string(fromQQ);

  ifstream home_in(home_str);
  if(home_in.is_open()){

    while(getline(home_in,buffer)){
      key = buffer.substr(0,buffer.find('='));
      value = buffer.substr(buffer.find('=') + 1);

      // ��������
      if(key == "name") this -> name = value;
      if(key == "elected") this -> elected = atoi(value.c_str());
      if(key == "area") this -> area = atoi(value.c_str());

      // �Ҿ�¼��
      if(key.find('*') == 0){
        key = key.substr(1);
        furnitures[key] = atoi(value.c_str());
        continue;
      }

      // ����¼��
      if(key.find('&') == 0){
        key = key.substr(1);
        rooms[key] = atoi(value.c_str());
        continue;
      }
    }
  }else{ // �½��˺�
    this -> name = this -> QQNum + "'s home";
    this -> area = 50;
    this -> elected = 0;
    (*this).save();

  }

  home_in.close();

}



bool LDHome::load(int fromQQ){
  string home_str = "LDAccount\\" + to_string(fromQQ) + "\\home.txt";
  string buffer;
  string key,value;

  this -> QQNum = to_string(fromQQ);

  ifstream home_in(home_str);
  if(home_in.is_open()){

    while(getline(home_in,buffer)){
      key = buffer.substr(0,buffer.find('='));
      value = buffer.substr(buffer.find('=') + 1);

      // ��������
      if(key == "name") this -> name = value;
      if(key == "elected") this -> elected = atoi(value.c_str());
      if(key == "area") this -> area = atoi(value.c_str());

      // �Ҿ�¼��
      if(key.find('*') == 0){
        key = key.substr(1);
        furnitures[key] = atoi(value.c_str());
        continue;
      }

      // ����¼��
      if(key.find('&') == 0){
        key = key.substr(1);
        rooms[key] = atoi(value.c_str());
        continue;
      }


    }
    home_in.close();
    return false;

  }else{ // �½��˻�
    this -> name = "";
    this -> area = 50;
    this -> elected = 0;
    (*this).save();

  }

  home_in.close();
  return true;
}


void LDHome::clean(){
  this -> name = "";
}

bool LDHome::is_loaded(){
  if(this -> name == "") return false;
  return true;
}

bool LDHome::rename(string new_name){
  if((*this).is_loaded()){
    this -> name = new_name;
    return true;
  }
  return false;
}


string LDHome::get_name(){
  return this -> name;
}


bool LDHome::has_furn(string furn_name){
  int finded = this -> furnitures.count(furn_name);
  if(!finded) return false;
  else return true;
}

void LDHome::add_furn(string furn_name,int level){
  this -> furnitures[furn_name] = level;
}

void LDHome::del_furn(string furn_name){
  this -> furnitures.erase(furn_name);
}

bool LDHome::has_room(string room_name){
  int finded = this -> rooms.count(room_name);
  if(!finded) return false;
  else return true;
}

void LDHome::add_room(string room_name,int level){
  this -> rooms[room_name] = level;
}

void LDHome::del_room(string room_name){
  this -> rooms.erase(room_name);
}


int LDHome::get_area(){
  return this -> area;
}

void LDHome::save(){
  string home_str = "LDAccount\\" + this -> QQNum + "\\home.txt";

  ofstream home_out(home_str);

  // ���������Ϣ
  home_out << "name=" << this -> name << endl;
  home_out << "elected=" << this -> elected << endl;
  home_out << "area=" << this -> area << endl;

  // ����Ҿ�
  map <string ,int >::iterator it;
  it = this -> furnitures.begin();

  while(it != this -> furnitures.end()){
    home_out << '*' << it -> first << '=' << it -> second << endl;
    it ++;
  }

  // �������
  it = this -> rooms.begin();

  while(it != this -> rooms.end()){
    home_out << '&' << it -> first << '=' << it -> second << endl;
    it ++;
  }

  home_out.close();

}

string LDHome::expand(int add_area,LDAccount act){

  int fee = 0;
  for(int i = 1;i <= add_area;i++)
  fee += (this -> area + i) * 0.5;

  string POOR = "[X] ��̫���ˣ����Ҫ�� " + this -> name + " ���� " + to_string(add_area) + " ƽ��������Ҫ " + to_string(fee) + " Dcoin";
  string SUCCESS = "[��] " + act.Name + " �ɹ��� " + this -> name + " ���������\n[...] ���: " + to_string(this -> area) + " -> " + to_string(this -> area + add_area);





  if(fee > act.Dcoin)// ̫��
    return POOR;

  this -> area += add_area;

  act.Dcoin -= fee;

  act.save();

  (*this).save();

  return SUCCESS;
}


// string LDHome::install_elect(){
//   this ->
//
// }

string LDHome::print(){
  string menu_str = "";
  menu_str += "//============\n";
  menu_str += "|| " + this -> name + "\n";
  menu_str += "|| �����" + to_string(this -> area) + "\n";
  menu_str += "||============";
  return menu_str;
}





//
