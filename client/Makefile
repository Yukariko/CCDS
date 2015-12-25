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
t5 = eio
t6 = nbd

obj = $(t1).o $(t2).o $(t3).o $(t4).o $(t5).o $(t6).o

all: $(bin)
$(bin): $(obj)
		$(CXX) $(CPPFLAGS) $(obj) $(LDFLAGS) -o $@

clean:
		rm -f $(bin) *.o
