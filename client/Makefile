INCLUDEDIRS = \

LIBDIRS = \

LIBS = \

CXX = g++

CPPFLAGS = -Wall -O -std=c++11 -g $(INCLUDEDIRS)
LDFLAGS =  $(LIBDIRS) $(LIBS)

bin = ccdc

t1 = main
t2 = ccdc
t3 = msg_queue
t4 = parser

obj = $(t1).o $(t2).o $(t3).o $(t4).o

all: $(bin)
$(bin): $(obj)
		$(CXX) $(CPPFLAGS) $(obj) $(LDFLAGS) -o $@

clean:
		rm -f $(bin) *.o
