#pragma once
#ifndef LDHOME_H
#define LDHOME_H
#include <iostream>
#include <string>
#include <map>
#ifndef LDACCOUNT_H
#define LDACCOUNT_H
#include "../LDAccount/LDAccount.h"
#endif
using namespace std;

class LDHome{

private:
    string name;            // 房屋名称
    map<string,int> furnitures;  // 家具列表
    map<string,int> rooms;       // 房屋
    string QQNum;           // QQ号码
    int area;               // 占地面积
    int elected;           // 是否已经通电

public:
  // 初始化函数
  // int fromQQ -> QQ号码
  LDHome(int fromQQ);

  // 读取home信息函数
  // int fromQQ -> QQ号码
  // bool
  // true -> 读取成功
  // false -> 读取失败
  bool load(int fromQQ);

  // 清空home信息
  void clean();

  // 是否载入home
  // bool
  // true -> 载入
  // false -> 未载入
  bool is_loaded();

  // 更改房屋名称
  // bool
  // true -> 成功改名
  // false -> 改名失败
  // string new_name -> 新的名称
  // home*rename*NEW_NAME
  bool rename(string new_name);

  // 获得房屋的名称
  // string
  // 返回的为房屋的名称
  // 如果未载入信息则会返回空字符串
  string get_name();


  // 查看某个家具是否安置
  // bool
  // true -> 存在
  // false -> 不存在
  // string furn_name -> 家具名称
  bool has_furn(string furn_name);

  // 添加某个家具
  // void
  // string furn_name -> 家具名称
  // int level -> 等级
  void add_furn(string furn_name,int level);

  // 移除某个家具
  // void
  // string furn_name -> 家具名称
  void del_furn(string furn_name);

  // 查看某个房间是否建立
  // bool
  // true -> 存在
  // false -> 不存在
  // string room_name -> 房间名称
  bool has_room(string room_name);

  // 建造房间
  // void
  // string room_name -> 房间名称
  void add_room(string room_name,int level);

  // 拆除房间
  // void
  // string room_name -> 房间名称
  void del_room(string room_name);

  // 获得房屋面积
  // int
  // 返回占地面积
  int get_area();

  // 扩大房屋占地面积
  // string
  // 返回应该发送的文本
  // int add_area -> 要增加的面积
  // LDAccount act -> 账户
  string expand(int add_area,LDAccount act);

  // 安装电力系统
  // string
  // 返回应该发送的文本
  // string install_elect();

  // 保存Home
  void save();

  // 打印Home
  // string
  // 返回应该发送的文本
  string print();
};

#endif
