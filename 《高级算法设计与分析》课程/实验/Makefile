.PHONY : all
all : busSimulator.exe

# 使用 bus.cpp 会由重定义链接错误
# 使用 bus.h 不合适
# 声明和定义都有用 bus.hpp
# 依赖可以不写 bus.hpp，但修改 bus.hpp 检测不到
busSimulator.exe : busSimulator.cpp bus.hpp
	g++ -o $@ $^

.PHONY : clean
clean :
	del *.exe