//头文件
#include <iostream> //string
using namespace std;
#include <vector>  //vector
#include <iomanip> //setw()、setprecision()
#include <fstream> //ifstream、close()
#include <cmath>   //pow()

//结构体
//概率区间值
struct Prob_inter_value // interval
{
    double min; //最小值
    double max; //最大值
};

//每字符信息字典  字符串/字符,频率,概率-概率区间值
struct Dict
{
    string ch;
    //字符串/字符  考虑到<LITERAL>和<RESET>字面值，不能用char数据类型的'L'、'S'代替，会与普通字符冲突
    int freq;                                 //频率
    double prob;                              //概率 小数一律统一用double数据类型
    struct Prob_inter_value prob_inter_value; //概率区间值
};

// VLC表
struct Vlc_table
{
    vector<struct Dict> dicts; //字符信息字典
    int ch_freq_sum;           //字符频率和  注意：是所有字符的频率和    为了计算每字符概率
};

//全局变量
//传参实在太多  用全局变量
//对象  3个
vector<string> literal_table; //字面值表
//用以记录新出现的字符    新出现的字符按照ASCII编码方式编码
//无扩展ASCII码共127个字符，需7位二进制数表示，则每一字面值用8位二进制数区分、表示
//将字面值表的二进制编码与算术编码拼接得最终编码    两者以“1111 1111”区分
//包括<LITERAL>、<RESET>    可定义其表示为“1000 0000”、“1000 0001”
//第一个八位位组串标明后续有多少八位位组串表示字符总数，第二个至相应的个数表示字符总数
//字符总数标明解码在何时停止
struct Vlc_table vlc_table;         // VLC表实例
struct Prob_inter_value curr_inter; //当前概率区间值
string code;                        //编码

vector<string> literal_table1; //解码字面值表
struct Vlc_table vlc_table1;   //解码 VLC表实例
string code1;                  //解码

//函数声明
//过程函数
void initialize();                                                    //初始化
void read_text();                                                     //读文本
void encode(char ch);                                                 //编码
void new_ch_handle(char ch);                                          //新字符处理
void old_ch_handle(char ch);                                          //旧字符处理
void encode_result();                                                 //编码结果
void decode();                                                        //解码
void decode_literal_table(string literal_code);                       //解码字面值表
void decode_algorithm_code(string algorithm_code, double &mid_value); //解码算术编码

//辅助函数
void show_object(); //显示对象

//主函数
int main()
{
    initialize();    //初始化
    read_text();     //读文本->编码->旧新字符处理
    encode_result(); //编码结果

    show_object(); //显示对象
    cout << endl;
    cout << "【编码结果】" << endl;
    cout << code << endl; //编码结果

    //依据解码验证编码是否正确
    decode(); //解码    解码字面值表，解码算术编码

    return 0;
}

//函数定义
//过程函数
//初始化
void initialize()
{
    //字面值表
    string literal = "<LITERAL>"; //只有"<LITERAL>"一个字符
    literal_table.push_back(literal);

    // VLC表
    struct Dict dict;
    dict.ch = "<LITERAL>";           //只有"<LITERAL>"一个字符
    dict.freq = 0;                   //频率为0
    dict.prob = 1.0;                 //概率为100%
    dict.prob_inter_value.min = 0.0; //概率区间值为0.0-1.0
    dict.prob_inter_value.max = 1.0;

    vlc_table.dicts.push_back(dict); //加入VLC表
    vlc_table.ch_freq_sum = 0;       //字符频率和为0

    //当前概率区间值
    curr_inter.min = 0.0; //初始当前概率区间值为0
    curr_inter.max = 0.0;

    return;
}

//读文本
void read_text()
{
    //打开文件
    string file_name = "./content.txt"; //文件名

    ifstream in_file(file_name, ios::in); //文件输入流类	使用流类的构造函数打开文件	只读，文件不存在出错，文本模式
    if (!in_file)
    {
        cerr << "The 'content.txt' doesn't exist" << endl; //标准错误输出
        exit(EXIT_FAILURE);                                //可用
    }

    //读取文件  注意：只读一次，即在读取时就进行编码
    char ch; //每字符

    while (1)
    {
        in_file >> ch; // ifstream类内部对 >> 输出流运算符做重载	会忽略换行符

        if (in_file.eof()) //文件末尾退出
        {
            break;
        }

        //处理必须放在判断文件结尾后，否则会多处理文本前一个数据
        encode(ch); //编码
    }

    in_file.close(); //关闭文件

    return;
}

//编码
void encode(char ch)
{
    //判断是不是新的字符
    int old_ch_flag = 0; //旧字符标记  0无1有

    vector<struct Dict>::iterator it = vlc_table.dicts.begin();
    for (it; it != vlc_table.dicts.end(); it++)
    {
        if (it->ch[0] == ch) //不是 注意：字符信息字典中的ch是string数据类型
        {
            old_ch_handle(ch); //旧字符处理
            old_ch_flag = 1;

            break;
        }
    }

    if (old_ch_flag == 0) //是
    {
        new_ch_handle(ch); //新字符处理
    }

    return;
}

//新字符处理
void new_ch_handle(char ch)
{
    // 1.将新字符添加到字面值表
    string literal = ""; //先定义后赋值
    literal = literal + ch;
    literal_table.push_back(literal);

    // 1.将<LITERAL>加入输出流————更新当前概率区间值，字符频率+1
    //查找<LITERAL>对应的概率区间值
    vector<struct Dict>::iterator it = vlc_table.dicts.begin();
    for (it; it != vlc_table.dicts.end(); it++)
    {
        if (it->ch == "<LITERAL>")
        {
            it->freq = it->freq + 1; //出现频率+1

            //更新当前概率区间值
            curr_inter.min = it->prob_inter_value.min;
            curr_inter.max = it->prob_inter_value.max;

            break;
        }
    }

    // 2.更新VLC表
    vlc_table.ch_freq_sum = vlc_table.ch_freq_sum + 1 + 1; //更新VLC表字符频率和    <LITERAL>+字符

    //将新符号添加到VLC表
    struct Dict dict;
    dict.ch = ch;
    dict.freq = 1;

    vlc_table.dicts.push_back(dict);

    //按序更新所有字符概率、概率区间值 重点：按序，在当前已更新的概率区间下再细分
    double porb_value = curr_inter.min;                         //概率值，在当前已更新的概率区间移动   分配给每字符的概率区间最小值
    double curr_inter_domain = curr_inter.max - curr_inter.min; //当前已更新的概率区间差/域 以用来细分概率区间值
    double prop = 0.0;                                          //依字符概率在当前已更新的概率区间求所占比

    it = vlc_table.dicts.begin();
    for (it; it != vlc_table.dicts.end(); it++)
    {
        it->prob = (1.0 * it->freq) / vlc_table.ch_freq_sum; //依据频率计算概率    注意整型计算转换为浮点型

        it->prob_inter_value.min = porb_value;                      //概率区间最小值是移动到的概率值
        prop = it->prob * curr_inter_domain;                        //依字符概率在当前已更新的概率区间的所占比   是*不是/
        it->prob_inter_value.max = it->prob_inter_value.min + prop; //概率区间最大值是偏移的概率占比

        porb_value = it->prob_inter_value.max; //概率值移动为当前字符的概率区间最大值
    }

    return;
}

//旧字符处理
void old_ch_handle(char ch)
{
    // 1.查VLC表，更新当前概率区间值，字符频率+1
    vector<struct Dict>::iterator it = vlc_table.dicts.begin();
    for (it; it != vlc_table.dicts.end(); it++)
    {
        if (it->ch[0] == ch)
        {
            it->freq = it->freq + 1; //字符频率+1

            curr_inter.min = it->prob_inter_value.min;
            curr_inter.max = it->prob_inter_value.max;

            break;
        }
    }

    // 2.更新VLC表
    vlc_table.ch_freq_sum = vlc_table.ch_freq_sum + 1; // VLC表字符频率和+1

    //按序更新所有字符概率、概率区间值 重点：按序，在当前已更新的概率区间下再细分
    double porb_value = curr_inter.min;                         //概率值，在当前已更新的概率区间移动   分配给每字符的概率区间最小值
    double curr_inter_domain = curr_inter.max - curr_inter.min; //当前已更新的概率区间差/域 以用来细分概率区间值
    double prop = 0.0;                                          //依字符概率在当前已更新的概率区间求所占比

    it = vlc_table.dicts.begin();
    for (it; it != vlc_table.dicts.end(); it++)
    {
        it->prob = (1.0 * it->freq) / vlc_table.ch_freq_sum; //依据频率计算概率    注意整型计算转换为浮点型

        it->prob_inter_value.min = porb_value;                      //概率区间最小值是移动到的概率值
        prop = it->prob * curr_inter_domain;                        //依字符概率在当前已更新的概率区间的所占比   是*不是/
        it->prob_inter_value.max = it->prob_inter_value.min + prop; //概率区间最大值是偏移的概率占比

        porb_value = it->prob_inter_value.max; //概率值移动为当前字符的概率区间最大值
    }

    return;
}

//编码结果
void encode_result()
{
    // //字符总数编码
    // int ch_sum = vlc_table.ch_freq_sum; //字符总数   包括<LITERAL>字符数
    // string ch_sum_code;                 //字符总数编码

    // //可扩展
    // if (ch_sum < 256) //表明后面有几字节表示字符长度
    // {
    //     ch_sum_code = "00000001";
    // }

    // //十进制转二进制
    // int remainder;         //余数
    // char ch_remainder;     //字符类型的余数
    // string binary;         // ASCII码的二进制编码 除二取余法时未倒序的
    // string reverse_binary; // ASCII码的二进制编码 除二取余法法后倒序的
    // int zero_count;        //不足8位组需要补0的个数

    // while (1) //取二进制码    除二取余法
    // {
    //     if (ch_sum == 0) //整数部分为0退出
    //     {
    //         break;
    //     };

    //     remainder = ch_sum % 2;         //取余数
    //     ch_remainder = remainder + '0'; //字符类型的余数
    //     binary = binary + ch_remainder; //二进制编码

    //     ch_sum = ch_sum / 2; //除2
    // }

    // ch_remainder = ch_sum + '0'; //当整数部分/2==0时，还可能有剩下的数为0或者1，编为二进制码存
    // binary = binary + ch_remainder;

    // remainder = binary.size() % 8;
    // if (remainder != 0) //不是8的整数倍   在末尾补0再倒序
    // {
    //     zero_count = 8 - remainder;

    //     for (int i = 0; i < zero_count; i++)
    //     {
    //         binary = binary + '0';
    //     }
    // }

    // for (int i = binary.size() - 1; i >= 0; i--) //除二取余法法后倒序的二进制编码 转换为数组下标-1
    // {
    //     reverse_binary = reverse_binary + binary[i];
    // }

    //字面值编码
    string literal_code; //字面值编码

    vector<string>::iterator it = literal_table.begin();
    for (it; it != literal_table.end(); it++)
    {
        int ascii; //字面值的ASCII码

        string binary;         // ASCII码的二进制编码 除二取余法时未倒序的
        int remainder;         //余数
        char ch_remainder;     //字符类型的余数
        string reverse_binary; // ASCII码的二进制编码 除二取余法法后倒序的
        //不截取最高位为0的编码 让每字面值以固定8位二进制编码传
        //正常ASCII字符都编码为8位

        if (*it == "<LITERAL>") //特殊字符
        {
            literal_code = literal_code + "10000000";
        }
        else //其他字符
        {
            ascii = int((*it)[0]); // char强制类型转换为int    取ASCII码

            while (1) //取二进制码    除二取余法
            {
                if (ascii == 0) //整数部分为0退出
                {
                    break;
                };

                remainder = ascii % 2;          //取余数
                ch_remainder = remainder + '0'; //字符类型的余数
                binary = binary + ch_remainder; //二进制编码

                ascii = ascii / 2; //除2
            }

            ch_remainder = ascii + '0'; //当整数部分/2==0时，还可能有剩下的数为0或者1，编为二进制码存
            binary = binary + ch_remainder;

            for (int i = binary.size() - 1; i >= 0; i--) //除二取余法法后倒序的二进制编码 转换为数组下标-1
            {
                reverse_binary = reverse_binary + binary[i];
            }

            literal_code = literal_code + reverse_binary; //字面值编码
        }
    }

    //算术编码
    string algo_code;
    double mid_value = (curr_inter.max - curr_inter.min) / 2; //取当前概率区间值中的中值以代表区间
    //使用表示二进制位数最少的十进制数需运算判断，不用

    //算术编码中，每读一位字符，十进制结果的小数位数+1
    //以使得增加1位小数表示的性能比用8位表示的好    1位小数的数字0-9最多用3位表示
    //十进制结果转二进制编码    1.二进制转十进制    2.将小数每1位十进制数转换为二进制
    // 2需控制小数位数，否则可能出现字符表示的编码数更优情况 字符数*8<小数位数*3
    //选用1 1的性能优越性？大概
    //自定义    取编码数=字符数*8/2时为有效位数 需要寻求平衡

    //解码时看该值依次落在哪个区间即可

    //十进制小数转二进制数
    while (1)
    {
        if (mid_value == 0) //小数值为0时结束
        {
            break;
        }

        if (algo_code.size() == vlc_table.ch_freq_sum * 8 / 2) //小数循环乘不尽时，以字符数*8/2时为有效位数
        {
            break;
        }

        mid_value = mid_value * 2;
        if (mid_value < 1) //编码为0
        {
            algo_code = algo_code + '0';
        }
        else // 1
        {
            algo_code = algo_code + '1';
            mid_value = mid_value - 1; //取小数部分
        }
    }

    //拼接字符编码+字面值编码+算术编码
    // code = code + ch_sum_code;
    code = code + literal_code;
    code = code + "11111111";
    code = code + algo_code;

    return;
}

//解码
void decode()
{
    //区分字面值编码和算术编码
    string literal_code;   //字面值编码
    string algorithm_code; //算术编码
    int delim_loc;         // 分隔码首次出现的位置

    delim_loc = code.find("11111111");    //查找分隔码首次出现的位置  是数组下标
    literal_code = code.substr(0, 24);    //从0开始24个字符的子串
    algorithm_code = code.substr(24 + 8); //从24+8开始到末尾的子串

    double mid_value; //解码的概率区间值

    decode_literal_table(literal_code);               //解码字面值表
    decode_algorithm_code(algorithm_code, mid_value); //解码算术编码

    //综合解码
    //初始化解码VLC表
    struct Dict dict;
    dict.ch = "<LITERAL>";           //只有"<LITERAL>"一个字符
    dict.freq = 0;                   //频率为0
    dict.prob = 1.0;                 //概率为100%
    dict.prob_inter_value.min = 0.0; //概率区间值为0.0-1.0
    dict.prob_inter_value.max = 1.0;

    vlc_table1.dicts.push_back(dict); //加入解码VLC表
    vlc_table1.ch_freq_sum = 0;       //字符频率和为0

    //判断概率区间值所在区间，取字符
    int literal_loc = 0; //定位读到的解码字面值表位置
    //在出现<LITERAL>时，表明有新字符，从解码字面值表中获取新字符
    //解码字面值表中字符是按照新字符出现顺序排列的

    vector<struct Dict>::iterator it = vlc_table1.dicts.begin();
    for (it; it != vlc_table1.dicts.end(); it++)
    {
        if ((mid_value >= it->prob_inter_value.min) && (mid_value <= it->prob_inter_value.max))
        {
            // code1 = code1 + it->ch;
            if (it->ch == "<LITERAL>") //表明有新字符，应从解码字面值表中获取字符
            {
            }
        }
    }

    cout << code1;

    return;
}

//解码字面值表
void decode_literal_table(string literal_code)
{
    string binary;  //二进制编码
    int ascii;      //十进制ASCII码
    int temp_num;   //二进制转十进制时二进制码为1得的十进制值
    string literal; //字面值

    for (int i = 0; i < literal_code.size(); i = i + 8) //循环取    每组位置   每八位一组
    {
        binary = literal_code.substr(i, 8);

        if (binary == "10000000") //特殊字符
        {
            literal = "<LITERAL>";
            literal_table1.push_back(literal); //加入解码字面值表
        }
        else //其他字符
        {
            //二进制转十进制
            for (int j = 0; j < binary.size(); j++)
            {
                if (binary[j] == '1') //二进制码为1  取数
                {
                    temp_num = pow(2, binary.size() - j - 1); //考虑j和2次方的关系  0是2的7次方
                    ascii = ascii + temp_num;
                }
            }

            literal = ascii;                   // C++中，只要把ASCII码赋值给字符变量，就可以转换为对应的字符
            literal_table1.push_back(literal); //加入解码字面值表

            ascii = 0; //置位
        }
    }

    return;
}

//解码算术编码
void decode_algorithm_code(string algorithm_code, double &mid_value)
{
    //二进制转十进制小数
    double temp_num; //二进制转十进制小数时值为1的1位转换值
    int power = -1;  //转换时2的幂值

    for (int i = 0; i < algorithm_code.size(); i++)
    {
        if (algorithm_code[i] == '1')
        {
            temp_num = pow(2, power); //第0位是2的-1次方    不用字符型乘，因为是1直接取原数即可
            mid_value = mid_value + temp_num;
        }

        power = power - 1;
    }

    return;
}

//辅助函数
//显示对象
void show_object()
{
    //字面值表
    cout << "【字面值表】" << endl;
    vector<string>::iterator it = literal_table.begin();
    for (it; it != literal_table.end(); it++)
    {
        cout << *it << endl;
    }
    cout << endl;

    // VLC表
    cout << "【VLC表】" << endl;
    cout << "字符       频率    概率    概率区间值" << endl;

    cout << left; //设置左对齐

    vector<struct Dict>::iterator it1 = vlc_table.dicts.begin();
    for (it1; it1 != vlc_table.dicts.end(); it1++)
    {
        cout << setw(11) << it1->ch; //设置宽度
        cout << setw(7) << it1->freq;
        cout << setw(8) << fixed << setprecision(2) << it1->prob; //设置浮点数有效位数->设置小数精度

        cout.unsetf(ios::fixed); //取消设置小数精度
        cout.precision(15);      //默认输出6位有效位数  double有效位为15，能保证15位无问题  不足15位不输出
        cout << it1->prob_inter_value.min << "----" << it1->prob_inter_value.max << endl;
    }

    cout << "字符频率和：" << vlc_table.ch_freq_sum << endl;
    cout << endl;

    //当前概率区间值
    cout << "【当前概率区间值】" << endl;
    cout << curr_inter.min << "----" << curr_inter.max << endl;

    return;
}
