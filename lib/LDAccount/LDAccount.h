#pragma once
#ifndef LDAccount_H
#define LDAccount_H
#include <iostream>
using namespace std;

// 取星号key,value的内容
// *study*key*value
// string s 分析的字符串
// string &_head 保存的head
// string &_value 保存的value
void get_star_content(string s,string &_head, string &_value);

// 替换string中的所有内容为另外一个内容
// string&
// 返回替换过后的string
// string& str -> 原字符串
// const string& old_value -> 要被替换的内容
// const string& new_value -> 要替换的内容
string& replace_all(string& str,const string& old_value,const string& new_value);


// 时间格式化
// string
// 返回时间文本，为YYYYMMDD
string format_time();

// 获得随机数函数
// int
// 返回需求的随机数
// int a -> 下限
// int b -> 上限
int get_randint(int64_t a,int64_t b);




class LDAccount{

public:
  // member
  string Name;
  string Birday ;// 19990119
  int Dcoin;
  int SignNum;
  string SignLevel;
  int WordsDir;
  string SignTime;
  string buy_big_lottery_time;
  string QQNum;
  string guess_num;
 

  // 账户初始化函数，接受一个QQ号
  LDAccount(int64_t QQNum);

  // 读取指定账户
  // bool
  //     true -> 读取成功
  //     false -> 读取失败
  // int QQNum  ->  发送者QQ
  bool read(int64_t QQNum);

  // 账户打印函数
  // string
  // 返回需求的文本
  string print_account();

  // 清空函数
  void clean();

  // 保存账户
  // bool
  //        true -> 保存成功
  //        false-> 保存失败
  bool save();

  // 修改账户设置
  // string
  // 返回应该发送的文本
  // string key,     -> 修改的项目
  // string value    -> 修改为的值
  string act_set(string key,string value);


  // 修改account菜单样式
  // string
  // 返回应该发送的文本
  // string model    ->  修改的样本
  string set_account_model(string model);

  // 修改Dcoin
  // int
  // 返回Dcoin
  // int num -> 要修改的数量
  int dcoin(int num);

  // 购买loter
  // string
  // 返回应该发送的文本
  // int fee -> loter价格
  string buy_loter(int fee);

  // 签到
  // string
  // 返回应该返回的文本
  string sign();

  // 是否载入账户
  // bool
  // true -> 正常载入
  // false -> 暂时没有载入账户

  bool is_loaded();
};

// 账户菜单的变量替换函数
// string
// 返回的为菜单
// int64_t fromQQ  ->  发送者QQ
// 替换变量表
// - &Name&        -> 昵称
// - &Dcoin&       -> Dcoin
// - &SignNum&     -> 签到次数
// - &SignLevel&   -> 签到等级
// - &WordsDir&    -> 教小梦的词数量
// - &Hour&        -> 时
// - &Minute&      -> 分
// - &Second&      -> 秒
// - &Year&        -> 年
// - &Month&       -> 月
// - &Day&         -> 日
// - \n            -> 回车
string account_rep(LDAccount act);



// 读取签到等级设置文件
// string
// 返回当前签到次数对应的等级
// int64_t fromQQ -> QQ号码
string get_level_name(LDAccount act);






#endif
