//头文件
#include <iostream> //string
using namespace std;
#include <vector>  //vector
#include <iomanip> //setw()、setprecision()
#include <fstream> //ifstream、close()
#include <cmath>   //pow()

//结构体
//概率区间值
//需要维护区间最小值和最大值两个变量
//方案
//使用一个结构体封装两个变量
struct Prob_inter_value // interval
{
    double min; //最小值
    double max; //最大值
};

//每字符信息字典
//需要维护字符的相应信息
//方案
//使用结构体
//字符相应信息有：字符串 / 字符，频率，概率，概率区间值
//字符串 / 字符：考虑到“<LITERAL>”和“<RESET>”字面值，不能用“char”数据类型的 'L'、'S'代替，会与普通字符冲突
//频率：用以计算其概率 - 概率：是小数，小数一律用“double”数据类型表示
//考虑算术编码对小数有效位数/精度的要求，使用“double”而不是“float”数据类型的精度更大，编解码能够相对更准确
//概率区间值：算术编码中各字符所占的概率区间值是动态变化的，需进行记录
struct Dict
{
    string ch;
    int freq;
    double prob;
    struct Prob_inter_value prob_inter_value;
};

//可变长编码（VLC）表
//需要维护所有字符的相应信息
//方案
//使用结构体
//所有字符的相应信息有：字符信息字典，字符频率和
//字符频率和：是所有字符的频率和，用以计算每字符概率
struct Vlc_table
{
    vector<struct Dict> dicts;
    int ch_freq_sum;
};

//全局变量
//在具体编程实现中，自适应算术编码需关注/维护字面值表、可变长编码（VLC）表、当前概率区间值等内容/变量
//在使用函数封装各功能以提高程序复用性时，需将该些变量作为参数传递，造成程序繁杂
//将其作为全局变量
vector<string> literal_table; //字面值表
//字面值
//方案1
//记录新出现的字符，字符按照ASCII编码方式编码
//无扩展ASCII码共127个字符，需7位二进制数表示，则每一字面值用8位二进制数区分、表示
//将字面值表编码与算术编码拼接得最终编码 - 约定二进制编码与算术编码两者以“1111 1111”区分
//约定 "<LITERAL>"、"<RESET> " 分别表示为“1000 0000”、“1000 0001”
//约定字面值的二进制编码以每8位一组，方便解码
//方案2
//为了简化编程
//假定解码端已知字面值表
//即在传输算术编码前先传输字面值表编码
//解码得字面值表
//即在编程时使用同一全局变量
struct Prob_inter_value curr_inter;  //当前概率区间值
struct Vlc_table vlc_table;          // VLC表实例
string code;                         //编码
struct Prob_inter_value curr_inter1; //解码当前概率区间值
struct Vlc_table vlc_table1;         //解码VLC表实例
string code1;                        //解码

//函数声明
//过程函数
void init();                  //初始化
void read_text();             //读文本
void encode(char ch);         //编码
void new_ch_handle(char ch);  //新字符处理
void old_ch_handle(char ch);  //旧字符处理
void encode_result();         //编码结果
void init1();                 //初始化1
void decode();                //解码
void new_ch_handle1(char ch); //解码新字符处理

//辅助函数
void show_object();       //显示对象
void update_vlc_table();  //更新VLC表
void update_vlc_table1(); //更新解码VLC表
void show_object1();      //显示解码对象

//主函数
int main()
{
    init();          //初始化
    read_text();     //读文本->编码->新旧字符处理
    encode_result(); //编码结果

    show_object(); //显示对象
    cout << endl;
    cout << "【编码结果】" << endl;
    cout << code << endl; //编码结果

    //依据解码验证编码是否正确
    init1();  //初始化1
    decode(); //解码

    cout << endl;
    show_object1(); //显示解码对象
    cout << endl;
    cout << "【解码结果】" << endl;
    cout << code1 << endl; //编码结果

    return 0;
}

//函数定义
//过程函数
//初始化
void init()
{
    //字面值表
    string literal = "<LITERAL>"; //只有"<LITERAL>"一个字符
    literal_table.push_back(literal);

    //当前概率区间值
    curr_inter.min = 0.0; //初始当前概率区间值为0
    curr_inter.max = 0.0;

    // VLC表
    struct Dict dict;
    dict.ch = "<LITERAL>";           //只有"<LITERAL>"一个字符
    dict.freq = 0;                   //频率为0
    dict.prob = 1.0;                 //概率为100%
    dict.prob_inter_value.min = 0.0; //概率区间值为0.0-1.0
    dict.prob_inter_value.max = 1.0;

    vlc_table.dicts.push_back(dict); //加入VLC表
    vlc_table.ch_freq_sum = 0;       //字符频率和为0

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

    //读取文件  注意：只读取一次文本，在读取时进行编码
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
    //判断是不是新字符
    int old_ch_flag = 0; //旧字符标记  0无1有

    vector<struct Dict>::iterator it;
    for (it = vlc_table.dicts.begin(); it != vlc_table.dicts.end(); it++)
    {
        if (it->ch[0] == ch) //不是 注意：字符信息字典中的字符是“string”数据类型
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
    string literal; //字面值    先定义后赋值

    literal = ch;
    literal_table.push_back(literal);

    // 2.将“<LITERAL>”字符加入输出流————更新当前概率区间值，字符频率+1
    //查找“<LITERAL>”字符对应的概率区间值
    vector<struct Dict>::iterator it;
    for (it = vlc_table.dicts.begin(); it != vlc_table.dicts.end(); it++)
    {
        if (it->ch == "<LITERAL>")
        {
            it->freq = it->freq + 1; //频率+1

            //更新当前概率区间值
            curr_inter.min = it->prob_inter_value.min;
            curr_inter.max = it->prob_inter_value.max;

            break;
        }
    }

    // 3.更新VLC表
    //将新符号添加到VLC表
    struct Dict dict;
    dict.ch = ch;
    dict.freq = 1;
    vlc_table.dicts.push_back(dict);

    vlc_table.ch_freq_sum = vlc_table.ch_freq_sum + 1 + 1; //更新VLC表字符频率和    “<LITERAL>”+新字符

    update_vlc_table(); //更新VLC表

    return;
}

//旧字符处理
void old_ch_handle(char ch)
{
    // 1.更新当前概率区间值，字符频率+1
    vector<struct Dict>::iterator it;
    for (it = vlc_table.dicts.begin(); it != vlc_table.dicts.end(); it++)
    {
        if (it->ch[0] == ch)
        {
            it->freq = it->freq + 1; //频率+1

            //更新当前概率区间值
            curr_inter.min = it->prob_inter_value.min;
            curr_inter.max = it->prob_inter_value.max;

            break;
        }
    }

    // 2.更新VLC表
    vlc_table.ch_freq_sum = vlc_table.ch_freq_sum + 1; // VLC表字符频率和+1

    update_vlc_table(); //更新VLC表

    return;
}

//编码结果
void encode_result()
{
    //算术编码
    double mid_value;

    //选取概率区间值代表当前概率区间
    //为最大程度”突破“熵、压缩数据，需在当前概率区间中选取能表示二进制位数最少的十进制数
    //最佳选取需进行一定的运算判断，增加编程难度、复杂度
    //方案
    //取当前概率区间值中的中值
    mid_value = (curr_inter.max - curr_inter.min) / 2;
    mid_value = curr_inter.min + mid_value; //注意：加上原值才落在相应区间

    //算术编码为什么可以压缩数据
    //在保留字符排列顺序的同时，对更高频/概率更大的字符，赋予更大的小数区间
    //按频率/概率划分区间：为了在最终目标区间内，找一个二进制最短的小数作为最终二进制编码
    //最终目标区间的范围越大，可容纳的小数精度就越低，最终的二进制编码越短
    //如在低精度的 [0.1, 0.2) 和 高精度的 [0.1111111111, 0.1111111112) 之间各找一个最短二进制编码比较，在 [0.1, 0.2) 中找到的的最短二进制编码更短

    //其他说法
    //每读一位字符，十进制结果的小数位数 + 1
    // 1位小数的数字0-9最多用4位二进制码表示，2的4次方 = 16 > 10 （对小数每1位的十进制数转换为二进制数）
    // 1个字符需8位二进制码表示
    // 8 > 4以使得性能更优

    //十进制小数转换二进制数的方式
    //方案1
    //将小数每1位的十进制数转换为二进制数
    //需控制小数位数，否则可能出现字符的二进制数更优情况：字符数×8 < 小数位数×4
    //方案2
    //二进制转十进制
    //选用二进制转十进制
    //当遇到循环乘不尽小数时，取二进制数 = 字符数×8÷2时为有效位数，该方式需要验证以寻求平衡

    //十进制小数转换二进制数
    while (1)
    {
        if (mid_value == 0) //小数值为0时结束
        {
            break;
        }

        if (code.size() == vlc_table.ch_freq_sum * 8 / 2) //小数循环乘不尽时，以字符数*8/2时为有效位数
        {
            break;
        }

        mid_value = mid_value * 2;
        if (mid_value < 1) //编码为0
        {
            code = code + '0';
        }
        else // 1
        {
            code = code + '1';
            mid_value = mid_value - 1; //取小数部分
        }
    }

    return;
}

//初始化1
void init1()
{
    //解码当前概率区间值
    curr_inter1.min = 0.0; //初始当前概率区间值为0
    curr_inter1.max = 0.0;

    //初始化解码VLC表
    struct Dict dict;
    dict.ch = "<LITERAL>";           //只有"<LITERAL>"一个字符
    dict.freq = 0;                   //频率为0
    dict.prob = 1.0;                 //概率为100%
    dict.prob_inter_value.min = 0.0; //概率区间值为0.0-1.0
    dict.prob_inter_value.max = 1.0;

    vlc_table1.dicts.push_back(dict); //加入解码VLC表
    vlc_table1.ch_freq_sum = 0;       //字符频率和为0

    return;
}

//解码
void decode()
{
    //取概率区间值的中值
    double mid_value; //概率区间值的中值

    mid_value = 0.0; //需要初始化

    //二进制转十进制小数
    double temp_num; //二进制转十进制小数时值为1的1位转换值
    int power;       //转换时2的幂值    初始为-1

    power = -1;

    for (int i = 0; i < code.size(); i++) //移动位数
    {
        if (code[i] == '1')
        {
            temp_num = pow(2, power); //第0位是2的-1次方    不用字符型乘，因为是1数直接取原数即可
            mid_value = mid_value + temp_num;
        }

        power = power - 1; //幂值减小
    }

    //解码何时停止
    //由于算术编码特性，当前概率区间值可”无限“落在”无限“的概率区间，可导致解码出”无限“字符
    //方案1
    //对已解码的字符数计数，当达到编码的字符总数时停止
    //编码时将字符总数编码传输，解码时先获取编码的字符总数
    //约定第一个8位位组串（二进制码）标明后续有多少个8位位组串表示字符总数，第二个至相应个数的8位位组串表示字符总数
    //方案2
    //为了简化编程
    //假定解码端已知编码的字符总数
    //即在传输算术编码前先传输字符总数编码，解码得字符总数
    //即在编程时使用同一全局变量

    int decode_ch_count;   //已解码的字符数计数
    int literal_table_loc; //定位读到的解码字面值表位置
    //在出现“<LITERAL>”字符时，表明有新字符，从解码字面值表中获取新字符
    //解码字面值表中字符是按照新字符出现顺序排列的
    string ch; //解码得的字符

    decode_ch_count = 0;
    literal_table_loc = 0;

    while (1)
    {
        if (decode_ch_count == vlc_table.ch_freq_sum) //对已解码的字符数计数，当达到编码的字符总数时停止    注意不是table1
        {
            break;
        }

        vector<struct Dict>::iterator it;
        for (it = vlc_table1.dicts.begin(); it != vlc_table1.dicts.end(); it++) //判断概率区间值所在区间，取字符
        {
            if ((mid_value >= it->prob_inter_value.min) && (mid_value <= it->prob_inter_value.max))
            {

                if (it->ch == "<LITERAL>") //表明有新字符，应从字面值表中获取字符
                {
                    // 1.更新当前概率区间值，“<LITERAL>”字符频率 + 1
                    curr_inter1.min = it->prob_inter_value.min;
                    curr_inter1.max = it->prob_inter_value.max;

                    it->freq = it->freq + 1;

                    // 2.从字面值表中获取字符
                    ch = literal_table.at(literal_table_loc);
                    if (ch == "<LITERAL>") //跳过特殊字符
                    {
                        literal_table_loc = literal_table_loc + 1;
                        ch = literal_table.at(literal_table_loc);
                    }
                    literal_table_loc = literal_table_loc + 1; //更新定位读到的解码字面值表位置

                    // 3.更新VLC表
                    //将新符号添加到VLC表
                    struct Dict dict;
                    dict.ch = ch;
                    dict.freq = 1;
                    vlc_table1.dicts.push_back(dict);

                    vlc_table1.ch_freq_sum = vlc_table1.ch_freq_sum + 1 + 1; //更新VLC表字符频率和    “<LITERAL>”+新字符

                    decode_ch_count = decode_ch_count + 2; //已解码字符数+2
                }
                else //旧字符
                {
                    // 1.更新当前概率区间值，字符频率+1
                    curr_inter1.min = it->prob_inter_value.min;
                    curr_inter1.max = it->prob_inter_value.max;

                    it->freq = it->freq + 1;

                    // 2.从VLC表中获取字符
                    ch = it->ch; //笨了 不是字面值表！

                    // 3.更新VLC表
                    vlc_table1.ch_freq_sum = vlc_table1.ch_freq_sum + 1; //字符频率和+1

                    decode_ch_count = decode_ch_count + 1; //已解码字符数+1
                }

                update_vlc_table1(); //更新解码VLC表

                code1 = code1 + ch; //加入解码

                break; //注意退出判断解码概率区间的循环
            }
        }
    }

    return;
}

//辅助函数
//显示对象
void show_object()
{
    //字面值表
    cout << "【字面值表】" << endl;

    vector<string>::iterator it;
    for (it = literal_table.begin(); it != literal_table.end(); it++)
    {
        cout << *it << endl;
    }
    cout << endl;

    //当前概率区间值
    cout << "【当前概率区间值】" << endl;
    cout << curr_inter.min << "----" << curr_inter.max << endl;
    cout << endl;

    // VLC表
    cout << "【VLC表】" << endl;
    cout << "字符       频率    概率    概率区间值" << endl;

    cout << left; //设置左对齐

    vector<struct Dict>::iterator it1;
    for (it1 = vlc_table.dicts.begin(); it1 != vlc_table.dicts.end(); it1++)
    {
        cout << setw(11) << it1->ch; //设置宽度
        cout << setw(7) << it1->freq;
        cout << setw(8) << fixed << setprecision(2) << it1->prob; //设置浮点数有效位数->设置小数精度

        cout.unsetf(ios::fixed); //取消设置小数精度
        cout.precision(15);      //默认输出6位有效位数  double有效位为15，能保证15位无问题  不足15位不输出
        cout << it1->prob_inter_value.min << "----" << it1->prob_inter_value.max << endl;
    }

    cout << "字符频率和：" << vlc_table.ch_freq_sum << endl;

    return;
}

//更新VLC表
void update_vlc_table()
{
    //更新可变长编码表（VLC表）
    //按序更新所有字符概率和概率区间值 - 重点：按序，在当前已更新的概率区间下再细分
    //方案
    //使用一概率区间值“指针”进行标记、移动、赋值操作
    double porb_value;        //概率值，在当前已更新的概率区间移动   分配给每字符的概率区间最小值
    double curr_inter_domain; //当前已更新的概率区间差/域 以用来细分概率区间值
    double prop;              //依字符概率在当前已更新的概率区间求所占比

    porb_value = curr_inter.min;
    curr_inter_domain = curr_inter.max - curr_inter.min;

    vector<struct Dict>::iterator it;
    for (it = vlc_table.dicts.begin(); it != vlc_table.dicts.end(); it++)
    {
        it->prob = (1.0 * it->freq) / vlc_table.ch_freq_sum; //依据频率计算概率    注意整型计算转换为浮点型

        it->prob_inter_value.min = porb_value;                      //概率区间最小值是移动到的概率值
        prop = it->prob * curr_inter_domain;                        //依字符概率在当前已更新的概率区间的所占比   是*不是/
        it->prob_inter_value.max = it->prob_inter_value.min + prop; //概率区间最大值是偏移的概率占比

        porb_value = it->prob_inter_value.max; //概率值移动为当前字符的概率区间最大值
    }

    return;
}

//更新解码VLC表
void update_vlc_table1()
{
    //更新可变长编码表（VLC表）
    //按序更新所有字符概率和概率区间值 - 重点：按序，在当前已更新的概率区间下再细分
    //方案
    //使用一概率区间值“指针”进行标记、移动、赋值操作
    double porb_value;         //概率值，在当前已更新的概率区间移动   分配给每字符的概率区间最小值
    double curr_inter1_domain; //当前已更新的概率区间差/域 以用来细分概率区间值
    double prop;               //依字符概率在当前已更新的概率区间求所占比

    porb_value = curr_inter1.min;
    curr_inter1_domain = curr_inter1.max - curr_inter1.min;

    vector<struct Dict>::iterator it;
    for (it = vlc_table1.dicts.begin(); it != vlc_table1.dicts.end(); it++)
    {
        it->prob = (1.0 * it->freq) / vlc_table1.ch_freq_sum; //依据频率计算概率    注意整型计算转换为浮点型

        it->prob_inter_value.min = porb_value;                      //概率区间最小值是移动到的概率值
        prop = it->prob * curr_inter1_domain;                       //依字符概率在当前已更新的概率区间的所占比   是*不是/
        it->prob_inter_value.max = it->prob_inter_value.min + prop; //概率区间最大值是偏移的概率占比

        porb_value = it->prob_inter_value.max; //概率值移动为当前字符的概率区间最大值
    }

    return;
}

//显示解码对象
void show_object1()
{
    //解码当前概率区间值
    cout << "【解码当前概率区间值】" << endl;
    cout << curr_inter1.min << "----" << curr_inter1.max << endl;
    cout << endl;

    // VLC表
    cout << "【解码VLC表】" << endl;
    cout << "字符       频率    概率    概率区间值" << endl;

    cout << left; //设置左对齐

    vector<struct Dict>::iterator it1;
    for (it1 = vlc_table1.dicts.begin(); it1 != vlc_table1.dicts.end(); it1++)
    {
        cout << setw(11) << it1->ch; //设置宽度
        cout << setw(7) << it1->freq;
        cout << setw(8) << fixed << setprecision(2) << it1->prob; //设置浮点数有效位数->设置小数精度

        cout.unsetf(ios::fixed); //取消设置小数精度
        cout.precision(15);      //默认输出6位有效位数  double有效位为15，能保证15位无问题  不足15位不输出
        cout << it1->prob_inter_value.min << "----" << it1->prob_inter_value.max << endl;
    }

    cout << "字符频率和：" << vlc_table1.ch_freq_sum << endl;
}