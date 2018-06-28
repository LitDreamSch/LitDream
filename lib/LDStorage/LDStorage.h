#ifndef LDSTORAGE_H
#define LDSTORAGE_H
#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace std;

class LDStorage{
private:
  map<string,vector<string>>
  string QQNum;

public:

  // 初始化函数
  LDStorage();


  // 读取函数
  // return
  //  void
  // parameter
  //  string QQ -> 加载的QQ
  void load(string QQ);

};













#endif
