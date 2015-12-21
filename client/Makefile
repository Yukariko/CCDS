INCLUDEDIRS = \

LIBDIRS = \

LIBS = \

CPPFLAGS = -Wall -O -std=c++0x -g -DESRI_UNIX $(INCLUDEDIRS) -fPIE
LDFLAGS =  $(LIBDIRS) $(LIBS)

bin = ccdc

t1 = main
t2 = ccdc
t3 = msg_queue

obj = $(t1).o $(t2).o $(t3).o $(t4).o $(t5).o $(t6).o

all: $(bin)
$(bin): $(obj)
		$(CXX) $(CPPFLAGS) $(obj) $(LDFLAGS) -o $@

clean:
		rm -f $(bin) *.o
