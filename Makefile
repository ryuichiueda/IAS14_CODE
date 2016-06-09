CXX = g++
TARGET = main
CXXFLAGS = -Wall -O3 --std=c++11
LDFLAGS = -lm
SRCS := $(wildcard *.cc)
OBJS := $(SRCS:.cc=.o) #SRCSの各ファイルのサフィックスの.cppを.oに変換

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) 

clean:
	rm -f $(TARGET) $(OBJS)

install:
	cp robot.sh /etc/init.d/
	chmod +x /etc/init.d/robot.sh
	update-rc.d robot.sh defaults

uninstall:
	update-rc.d robot.sh remove
	rm /etc/init.d/robot.sh
