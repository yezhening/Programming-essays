// 文件Bus.java
//  这个Java的抽象类（C++中也成为抽象类）是对某趟公交车的抽象和模拟。
#include <vector>
#include <random>
#include <chrono>

class Bus
{
public:
    Bus() : spanBounds(SPANS + 1, 0), period(std::nan("")) {}
    static const int SPANS = 10000; // 把概率密度函数的定义域（这里是[0,period]）分割成的份数，每一份的大小不一定相等，但落入每一份的概率相等。
    static const int MULTIPLE = 50; // 为了获得更好的精度，预处理时可以把定义域按照这个比例再细化。
protected:
    std::vector<double> spanBounds; // 定义域被分割成为等概率的小区间，spanBounds是所有小区间的边界。这个结构中的数据需要constructTimeSpans()方法来填充。
    double period;                  // 发车周期存在这个私有变量中。
public:
    virtual void setPeriod() = 0;                 // 设置发车周期，使对象成员变量period获取值。必须由用户给出，必须在constructTimeSpans()方法中显式调用一次。
    virtual double getDensityValue(double x) = 0; // 获取概率密度函数在某点的值。必须由用户给出。用抽象方法强制二次开发用户实现。
    void constructTimeSpans()                     // 由概率密度函数构造数据结构，辅助快速返回等公交车一次的随机时长。
    {
        setPeriod();
        // 在这里添加你的代码

        // 1. 求（累积）概率分布函数
        // 1.1 求每一区间的概率和总概率
        std::vector<double> span_probability_vec; // 区间概率向量。约定左边界点存储的概率是每一区间的概率
        double total_probability = 0.0;           // 总概率

        for (int i = 0; i < SPANS * MULTIPLE - 1; ++i) // 对每一区间求概率。注意考虑 i + 1不能越界
        {
            double interval = this->period / (SPANS * MULTIPLE);    // 每一区间的间隔
            double left_x = i * interval;                           // 区间左边界点
            double right_x = (i + 1) * interval;                    // 区间右边界点
            double left_y = getDensityValue(left_x);                // 区间左边界点的概率密度值
            double right_y = getDensityValue(right_x);              // 区间右边界点的概率密度值
            double probability = (left_y + right_y) * interval / 2; // 梯形法（（上底 + 下底）× 高 ÷ 2）求面积，概率密度函数的区间面积（积分）是区间概率分布

            span_probability_vec.push_back(probability); // 记录每一区间的概率。约定左边界点存储的概率是每一区间的概率
            total_probability += probability;            // 记录总概率
        }

        // 1.2 求每一区间的标准化概率
        for (double &p : span_probability_vec)
        {
            p /= total_probability; //  将每一区间概率除以总概率进行标准化，确保总概率为1
        }

        // 1.3 求每一区间的（累积）概率分布函数（CDF）
        std::vector<double> span_cumulative_probabilitiy_vec; // 区间累积概率向量。约定左边界点存储的累积概率作为每一区间的累积概率
        double total_cumulative_probability = 0.0;            // 总累积概率

        for (const double p : span_probability_vec) // 对每一区间概率
        {
            total_cumulative_probability += p;                                        // 求当前总累积概率
            span_cumulative_probabilitiy_vec.push_back(total_cumulative_probability); // 当前总累积概率就是区间的累积概率。约定左边界点存储的累积概率作为每一区间的累积概率
        }

        // 2. 求等车时间（使用概率分布函数的反变换抽样）
        unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()); // 为随机数引擎提供种子，用当前时间作种子
        std::default_random_engine generator(seed);                                                                 // 使用默认引擎
        std::uniform_real_distribution<double> distribution(0.0, 1.0);                                              // 生成范围在 [0.0, 1.0) 之间的均匀分布的 double 数（这是概率分布函数的反变换抽样原理需要的）
        // 注意：随机数预设定需要放在循环外，保证循环内生成的随机数的随机种子一样
        for (int i = 0; i < SPANS; ++i) // 遍历 spanBound 的区间数，在每一区间填入等待时间
        {

            double random_probability = distribution(generator); // 生成随机概率
            while (random_probability == 0.000000)               // 需要(0.0, 1.0) 均匀分布的随机数（这是概率分布函数的反变换抽样原理需要的），生成直到不是0的数，注意判断使用小数点后六位的0
            {
                random_probability = distribution(generator);
            }

            for (int j = 0; j < span_cumulative_probabilitiy_vec.size(); ++j) // 遍历每个区间的累积概率，查找满足的累积概率
            {
                if (random_probability <= span_cumulative_probabilitiy_vec[j]) // 如果随机概率 <= 某个区间的累积概率，认为在该区间位置 j 可以等到车（由概率分布函数的反变换抽样原理）
                {
                    double interval = this->period / (SPANS * MULTIPLE); // 每一区间的间隔
                    this->spanBounds[i] = j * interval;                  // 横坐标上，位置数 * 间隔就是（等待）时间（由概率分布函数的反变换抽样原理），约定 spanBound 左边界存储等待时间作为区间的等待时间

                    break; // 构造了当前区间 i 位置的等待时间，退出查找满足的概率，继续处理下一区间
                }
            }
        }
    }
    double getOnceWaitingSpan() // 获取一次等车的时间，这是个符合概率密度函数的随机数，需要依赖数组double[] spanBounds获取：
    {
        // std::random_device rd;
        // std::mt19937 gen(rd()); // rd() 可能由于系统的实现或硬件的限制导致同一个 .exe 获取到相同的种子，导致获取相同的随机数

        unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()); // 为随机数引擎提供种子
        // now()现在时间，time_since_epoch()自1970.1.1时间，count()计数值可能是纳秒取决于系统实现
        // unsigned int：count()返回的是一个持续时间（duration），其类型可以是任何整数类型，包括可能是带符号整数类型，生成器的种子通常要求是无符号整数。
        std::default_random_engine generator(seed);                    // 使用默认引擎
        std::uniform_real_distribution<double> distribution(0.0, 1.0); // 生成范围在 [0.0, 1.0) 之间的均匀分布的 double 数（Java Math.random() 范围在 [0.0, 1.0)，使用均匀分布）
        // double randomNumber = distribution(generator);                 // 生成随机数
        // double randomNumber = rand() / RAND_MAX;                 // srand()设置种子后，另一种获取[0.0, 1.0) 之间的均匀分布的 int 随机数方式

        int spanPosition = static_cast<int>(distribution(generator) * SPANS);
        double leftBound = spanBounds[spanPosition];
        double rightBound = spanBounds[spanPosition + 1];
        return leftBound + distribution(generator) * (rightBound - leftBound);
    }
};