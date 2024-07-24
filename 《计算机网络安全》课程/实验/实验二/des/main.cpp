//预处理指令——————————
//自定义头文件
#include "des.h"
#include "workMode.h"

//函数声明——————————
void compareStrBit(string str1, string str2); //比较两字符串不同的位数

//主函数——————————
int main()
{
    const string plainText("yezhening computer network security"); //明文
    const string key("91002705");                                  //密钥

    WorkMode *workMode = new WorkMode(); //工作模式对象

    workMode->cbc(plainText, key); //密文分组链接（CBC）模式
    workMode->cfb(plainText, key); //密文反馈（CFB）模式

    delete workMode;

    // string str1 = "0111100101100101011110100110100001100101011011100110100101101110";
    // string str2 = "0100001000010100101001000100111000010110110001101101000100110111";
    // compareStrBit(str1, str2);

    // str1 = "0110011100100000011000110110111101101101011100000111010101110100";
    // str2 = "1110011100100000011000110110111101101101011100000111010101110100";
    // compareStrBit(str1, str2);

    // str1 = "0110010101110010001000000110111001100101011101000111011101101111";
    // str2 = "0110010101110010001000000110111001100101011101000111011101101111";
    // compareStrBit(str1, str2);

    // str1 = "0111001001101011001000000111001101100101011000110111010101110010";
    // str2 = "0111001001101011001000000111001101100101011000110111010101110010";
    // compareStrBit(str1, str2);

    // str1 = "0110100101110100011110010011010100110101001101010011010100110101";
    // str2 = "0110100101110100011110010011010100110101001101010011010100110101";
    // compareStrBit(str1, str2);

    // str1 = "0111100101100101011110100110100001100101011011100110100101101110011001110010000001100011011011110110110101110000011101010111010001100101011100100010000001101110011001010111010001110111011011110111001001101011001000000111001101100101011000110111010101110010011010010111010001111001";
    // str2 = "111110010110010101111010011010000110010101101110011010010110111011100111001000000110001101101111011011010111000001110101011101000110010101";
    // compareStrBit(str1, str2);

    // DES *des = new DES(); //数据加密标准对象
    // //提示：
    // //复杂类型/自定义类型可能占用空间大，堆区比栈区大，需要在堆区手动管理
    // //手动管理：使用new创建对象，使用delete释放对象->搭配使用

    // const string plainText("yezhenin"); //明文
    // const string key("91002705");       //密钥
    // string cipherTextASCII("");         //密文的ASCII码
    // //提示：
    // //密文的ASCII码没有必要转换为字符，因为：
    // // 1.转换的字符无意义
    // // 2.转换的字符可能为非打印字符
    // string plainTextASCII(""); //明文的ASCII码

    // des->encrypt(plainText, key, cipherTextASCII); //加密
    // //参数：明文（已知），密钥（已知），密文的ASCII码（未知）

    // des->decrypt(cipherTextASCII, key, plainTextASCII); //解密
    // //参数：密文的ASCII码（已知），密钥（已知），明文的ASCII码（未知）

    // //字符转ASCII码转换器网址：https://www.qqxiuzi.cn/bianma/ascii.htm
    // // ASCII码转字符转换器网址：https://www.asciim.cn/m/tools/convert_ascii_to_string.html

    // //雪崩效应测试
    // //提示：
    // //由设计接口：测试明文变化需要考虑到字节层面，传递明文参数；测试密文变化需要考虑到位层面，传递密文的ASCII码参数
    // //因为加密和解密过程相似，所以变化密文进行测试

    // string cipherTextCo = "0000110101111001101000001011100001101001111000010100000001011011"; //密文正确码
    // string plainTextCo = "0111100101100101011110100110100001100101011011100110100101101110";  //明文正确码

    // //第一组
    // string cipherTextEr = cipherTextCo;           //密文错误码为密文正确码
    // cipherTextEr[0] = 1;                          //修改1位：0->1
    // string plainTextEr("");                       //明文错误码
    // des->decrypt(cipherTextEr, key, plainTextEr); //解密
    // //参数：密文错误码（已知），密钥（已知），明文错误码（未知）
    // compareStrBit(plainTextCo, plainTextEr); //比较两字符串不同的位数

    // //第二组
    // cipherTextEr = cipherTextCo;                  //密文错误码为密文正确码
    // cipherTextEr[0] = 1;                          //修改1位：0->1
    // cipherTextEr[63] = 0;                         //修改1位：1->0
    // plainTextEr = "";                             //明文错误码
    // des->decrypt(cipherTextEr, key, plainTextEr); //解密
    // //参数：密文错误码（已知），密钥（已知），明文错误码（未知）
    // compareStrBit(plainTextCo, plainTextEr); //较两字符串不同的位数

    // //第三组
    // cipherTextEr = cipherTextCo;                  //密文错误码为密文正确码
    // cipherTextEr[0] = 1;                          //修改1位：0->1
    // cipherTextEr[1] = 1;                          //修改1位：0->1
    // cipherTextEr[63] = 0;                         //修改1位：1->0
    // plainTextEr = "";                             //明文错误码
    // des->decrypt(cipherTextEr, key, plainTextEr); //解密
    // //参数：密文错误码（已知），密钥（已知），明文错误码（未知）
    // compareStrBit(plainTextCo, plainTextEr); //较两字符串不同的位数

    // delete des;

    return 0;
}

//比较两字符串不同的位数
void compareStrBit(string str1, string str2)
{
    int diffBitCount = 0; //不同位计数
    for (int i = 0; i < str1.size(); ++i)
    {
        if (str1[i] != str2[i])
        {
            ++diffBitCount;
        }
    }

    cout << "不同位数：\t" << diffBitCount << endl;
    cout << endl;

    return;
}