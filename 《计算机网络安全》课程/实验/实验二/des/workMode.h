#ifndef DES_WORKMODE_H_
#define DES_WORKMODE_H_

#include <iostream>
#include <vector>

using std::string;
using std::vector;

//工作模式类
class WorkMode
{
public:
    void cbc(const string &plainText, const string &key); //密文分组链接（CBC）模式
    void cfb(const string &plainText, const string &key); //密文反馈（CFB）模式

private:
    void plainTextGP(const string &plainText, vector<string> &plainTextGroups); //明文分组和填充
};

#endif