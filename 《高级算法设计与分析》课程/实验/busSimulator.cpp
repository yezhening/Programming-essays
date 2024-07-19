// 文件BusSimulator.java
#include <limits>
#include <iostream>
#include <string>

#include "bus.hpp"

// class BusEven10;
// class BusTriangle10;
// class BusParabola5;
// 若使用 new 需要类的完整定义，前置声明不满足

class BusEven10 : public Bus
{
public:
    BusEven10()
    {
        constructTimeSpans();
    }
    void setPeriod() override
    {
        this->period = 10.0;
    }
    double getDensityValue(double x) override
    {
        return 0.1; // 概率密度函数 f(x)=0.1
    }
};
class BusTriangle10 : public Bus
{
public:
    BusTriangle10()
    {
        constructTimeSpans();
    }
    void setPeriod() override
    {
        this->period = 10.0;
    }
    double getDensityValue(double x) override
    {
        return 0.2 * x; // 概率密度函数 f(x) = 0.2x
    }
};
class BusParabola5 : public Bus
{
public:
    BusParabola5()
    {
        constructTimeSpans();
    }
    void setPeriod() override
    {
        this->period = 5.0;
    }
    double getDensityValue(double x) override
    {
        return 3.0 / 125 * x * x; // 概率密度函数 f(x) = 3/125*x^2
    }
};
class BusSimulator
{
public:
    // 该方法实现等待多路目标公交车平均时长的模拟：
    static double getAverageWaitingTime(int totalCount, std::vector<Bus *> buses)
    {
        double totalWaitingTime = 0.0;
        for (int i = 0; i < totalCount; i++)
        {
            double minWaitingTime = std::numeric_limits<double>::infinity();
            for (Bus *bus : buses)
            {
                double waitingTime = bus->getOnceWaitingSpan();
                if (waitingTime < minWaitingTime)
                    minWaitingTime = waitingTime;
            }
            totalWaitingTime += minWaitingTime;
        }
        double averageWaitingTime = totalWaitingTime / totalCount;
        return averageWaitingTime;
    }
    static void testBusEven10(int total)
    {
        BusEven10 *busEven10 = new BusEven10();
        double averageWaitingTime = getAverageWaitingTime(total, std::vector<Bus *>{busEven10});
        std::cout << averageWaitingTime << std::endl;
    }
    static void testBusTriangle10(int total)
    {
        BusTriangle10 *busTriangle10 = new BusTriangle10();
        double averageWaitingTime = getAverageWaitingTime(total, std::vector<Bus *>{busTriangle10});
        std::cout << averageWaitingTime << std::endl;
    }
    static void testBusParabola5(int total)
    {
        BusParabola5 *busParabola5 = new BusParabola5();
        double averageWaitingTime = getAverageWaitingTime(total, std::vector<Bus *>{busParabola5});
        std::cout << averageWaitingTime << std::endl;
    }
    static void testBusEven10AndEven10(int total)
    {
        BusEven10 *busEven10A = new BusEven10();
        BusEven10 *busEven10B = new BusEven10();
        double averageWaitingTime = getAverageWaitingTime(total, std::vector<Bus *>{busEven10A, busEven10B});
        std::cout << averageWaitingTime << std::endl;
    }
    static void testBusEven10AndTriangle10(int total)
    {
        BusEven10 *busEven10 = new BusEven10();
        BusTriangle10 *busTriangle10 = new BusTriangle10();
        double averageWaitingTime = getAverageWaitingTime(total, std::vector<Bus *>{busEven10, busTriangle10});
        std::cout << averageWaitingTime << std::endl;
    }
    static void testBusEven10AndTriangle10AndParabola5(int total)
    {
        BusEven10 *busEven10 = new BusEven10();
        BusTriangle10 *busTriangle10 = new BusTriangle10();
        BusParabola5 *busParabola5 = new BusParabola5();
        double averageWaitingTime = getAverageWaitingTime(total, std::vector<Bus *>{busEven10, busTriangle10, busParabola5});
        std::cout << averageWaitingTime << std::endl;
    }
    static void main(std::vector<std::string> args)
    {
        int total = 1000000;
        std::cout << "testBusEven10:" << std::endl;
        testBusEven10(total);

        std::cout << "\r\n testBusTriangle10:" << std::endl;
        testBusTriangle10(total);

        std::cout << "\r\n testBusParabola5:" << std::endl;
        testBusParabola5(total);

        std::cout << "\r\n testBusEven10AndEven10:" << std::endl;
        testBusEven10AndEven10(total);

        std::cout << "\r\n testBusEven10AndTriangle10:" << std::endl;
        testBusEven10AndTriangle10(total);

        std::cout << "\r\n testBusEven10AndTriangle10AndParabola5:" << std::endl;
        testBusEven10AndTriangle10AndParabola5(total);
    }
};

int main()
{
    BusSimulator::main(std::vector<std::string>{""});
    getchar();

    return 0;
}
