//头文件————————————————————
#include <iostream>

//命名空间————————————————————
using namespace std;

//类————————————————————
//凯撒密码
class CaesarCode
{
public:
    string plaintext;  //明文
    int secretKey;     //密钥
    string ciphertext; //密文

    void encryptionProcess(); //加密过程
    void decryptionProcess(); //解密过程
};

//凯撒密码的加密过程
void CaesarCode::encryptionProcess()
{
    int ascii = 0;                  //字符的ASCII码
    for (char ch : this->plaintext) //对明文的每个字符
    {
        ascii = (int)ch;          //字符转ASCII码
        ascii += this->secretKey; //加密   可取模
        ch = (char)ascii;         // ASCII码转字符

        this->ciphertext += ch; //存储到密文
    }

    return;
}

//凯撒密码的解密过程
void CaesarCode::decryptionProcess()
{
    this->plaintext = ""; //清空明文

    //解密
    int ascii = 0;                   //字符的ASCII码
    for (char ch : this->ciphertext) //对密文的每个字符
    {
        ascii = (int)ch;          //字符转ASCII码
        ascii -= this->secretKey; //解密   可取模
        ch = (char)ascii;         // ASCII码转字符

        this->plaintext += ch; //存储到明文
    }

    return;
}

//主函数————————————————————
int main()
{
    CaesarCode caesarCode; //凯撒密码

    caesarCode.plaintext = "abc"; //初始化明文
    caesarCode.secretKey = 3;     //初始化密钥
    caesarCode.ciphertext = "";   //初始化密文

    caesarCode.encryptionProcess(); //加密过程

    cout << "明文：" << caesarCode.plaintext << endl;
    cout << "密钥：" << caesarCode.secretKey << endl;
    cout << "密文：" << caesarCode.ciphertext << endl;

    caesarCode.decryptionProcess(); //解密过程

    cout << "密文：" << caesarCode.ciphertext << endl;
    cout << "密钥：" << caesarCode.secretKey << endl;
    cout << "明文：" << caesarCode.plaintext << endl;

    return 0;
}
