#include "workMode.h"
#include "des.h"

#include <sstream>
#include <bitset>

using std::bitset;
using std::cout;
using std::endl;
using std::to_string;

//函数声明——————————

//密文分组链接（CBC）模式
//参数：明文，密钥
void WorkMode::cbc(const string &plainText, const string &key)
{
    cout << "密文分组链接（CBC）模式——————————" << endl;

    vector<string> plainTextGroups;                //所有明文分组
    this->plainTextGP(plainText, plainTextGroups); //明文分组和填充

    DES *des = new DES(); //数据加密标准对象

    //加密
    cout << "加密——————————" << endl;
    string plainTextGroupASCII("");
    string iv(64, '0');  //初始向量
    string groupXor(""); //分组和初始向量的异或
    //注意：xor是关键字
    string cipherTextGroupASCII(""); //密文分组的ASCII码
    vector<string> cipherTextGroups; //所有密文分组

    for (string &plainTextGroup : plainTextGroups) //对每个明文分组
    {
        cout << "明文：\t" << plainTextGroup << endl;

        //取明文分组的ASCII码
        for (const char &ch : plainTextGroup)
        {
            plainTextGroupASCII += bitset<8>(ch).to_string(); // 1字节字符转8位字符串    字符->bitset对象->字符串
        }
        cout << "明文的ASCII码：\t" << plainTextGroupASCII << endl;

        //明文分组的ASCII码和初始向量异或
        for (int i = 0; i < iv.size(); i++)
        {
            if (plainTextGroupASCII[i] == iv[i]) //相同为0
            {
                groupXor += "0";
            }
            else //不同为1
            {
                groupXor += "1";
            }
        }

        //对异或加密
        des->encrypt(groupXor, key, cipherTextGroupASCII);

        cout << "密文的ASCII码：\t" << cipherTextGroupASCII << endl;
        cout << endl;

        cipherTextGroups.push_back(cipherTextGroupASCII); //存储密文分组

        plainTextGroupASCII = "";  //清空明文分组的ASCII码
        iv = cipherTextGroupASCII; //更新初始向量
        groupXor = "";             //清空异或
    }

    //解密
    cout << "解密——————————" << endl;
    string deVal("");          //解密值
    iv.assign(64, '0');        //初始向量
    string plainTextASCII(""); //明文的ASCII码

    for (string &cipherTextGroup : cipherTextGroups) //对每个密文分组
    {
        des->decrypt(cipherTextGroup, key, deVal); //解密

        //解密值和初始向量异或
        for (int i = 0; i < iv.size(); i++)
        {
            if (deVal[i] == iv[i]) //相同为0
            {
                plainTextASCII += "0";
            }
            else //不同为1
            {
                plainTextASCII += "1";
            }
        }
        cout << "明文的ASCII码：\t" << plainTextASCII << endl;

        //更新
        deVal = "";
        iv = cipherTextGroup;
        plainTextASCII = "";
    }
    cout << endl;

    delete des;

    return;
}

//明文分组和填充
//参数：明文，所有明文分组
void WorkMode::plainTextGP(const string &plainText, vector<string> &plainTextGroups)
{
    string plainText2(plainText); //明文2

    //使用PKCS#7填充
    int remainder = plainText2.size() % 8; //余数   3
    int padNum = 8 - remainder;            //填充数  5
    string padVal = to_string(padNum);     //填充值  5

    if (padNum != 0)
    {
        for (int i = 0; i < padNum; i++)
        {
            plainText2 += padVal;
        }
    }
    else //==0
    {
        for (int i = 0; i < 64; i++)
        {
            plainText2 += "64";
        }
    }

    //分组 每8个字符为1组
    string plainTextGroup(""); //一个明文分组
    for (int i = 0; i < plainText2.size(); i += 8)
    {
        plainTextGroup = plainText2.substr(i, 8);
        plainTextGroups.push_back(plainTextGroup);
    }

    return;
}

//密文反馈（CFB）模式
void WorkMode::cfb(const string &plainText, const string &key)
{
    cout << "密文反馈（CFB）模式——————————" << endl;

    DES *des = new DES(); //数据加密标准对象

    //加密
    cout << "加密——————————" << endl;

    //取明文分组的ASCII码
    string plainTextASCII(""); //明文的ASCII码
    for (const char &ch : plainText)
    {
        plainTextASCII += bitset<8>(ch).to_string(); // 1字节字符转8位字符串    字符->bitset对象->字符串
    }
    cout << "明文的ASCII码：\t" << plainTextASCII << endl;

    string iv(64, '0');         //初始向量
    string enVal("");           //加密值
    string selVal("");          //选择值
    string plainTextSe("");     //明文选择
    string cipherTextASCII(""); //密文的ASCII码
    string cipherTextSe("");    //密文的S位
    string left("");            //更新左

    for (int i = 0; i < plainTextASCII.size();) //对ASCII码流
    {
        des->encrypt(iv, key, enVal); //加密

        selVal = enVal.substr(0, 8); //约定选择8位

        plainTextSe = plainTextASCII.substr(i, 8); //明文选择8位

        //明文流和约定选择值异或
        for (int j = 0; j < selVal.size(); j++) // 8个
        {
            if (plainTextSe[j] == iv[j]) //相同为0
            {
                cipherTextSe += "0";
            }
            else //不同为1
            {
                cipherTextSe += "1";
            }
        }

        //更新
        cipherTextASCII += cipherTextSe;
        left = iv.substr(8, 56);  //取右边的56位
        iv = left + cipherTextSe; //更新初始向量
        enVal = "";
        i += 8; //更新明文数据流
        cipherTextSe = "";
    }

    cout << "密文的ASCII码：\t" << cipherTextASCII << endl;
    cout << endl;

    //解密
    cout << "解密——————————" << endl;
    iv.assign(64, '0');  //初始向量
    enVal = "";          //加密值
    selVal = "";         //选择值
    cipherTextSe = "";   //密文选择
    plainTextSe = "";    //明文选择
    plainTextASCII = ""; //明文的ASCII码
    left = "";           //更新左

    for (int i = 0; i < cipherTextASCII.size();) //对ASCII码流
    {
        des->encrypt(iv, key, enVal); //加密

        selVal = enVal.substr(0, 8); //约定选择8位

        cipherTextSe = cipherTextASCII.substr(i, 8); //密文选择8位

        //密文流和约定选择值异或
        for (int j = 0; j < selVal.size(); j++) // 8个
        {
            if (cipherTextSe[j] == iv[j]) //相同为0
            {
                plainTextSe += "0";
            }
            else //不同为1
            {
                plainTextSe += "1";
            }
        }

        //更新
        plainTextASCII += plainTextSe;
        left = iv.substr(8, 56);  //取右边的56位
        iv = left + cipherTextSe; //更新初始向量
        enVal = "";
        i += 8; //更新密文数据流
        plainTextSe = "";
    }

    cout << "明文的ASCII码：\t" << plainTextASCII << endl;
    cout << endl;

    delete des;

    return;
}
