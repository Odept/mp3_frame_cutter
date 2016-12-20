CC = g++
CFLAGS  = -Wall -std=c++14
CFLAGS += -g3

LIBS = External/lib
LIB_MP3 = $(LIBS)/mp3.a

TARGET = mp3_cut
COMMANDS = commands

INCS = External/inc
DEPS = $(INCS)/mp3.h $(INCS)/tag.h $(INCS)/mpeg.h common.h
DEPS_CMDS = $(COMMANDS).cpp $(COMMANDS).h

# the first target is executed by default
default: $(TARGET)

$(TARGET): main.cpp $(DEPS) $(DEPS_CMDS) $(LIB_MP3) 
	@echo "# Generate" \"$(TARGET)\"
	$(CC) $(CFLAGS) -liconv -o $(TARGET) main.cpp $(COMMANDS).cpp $(LIB_MP3)

clean: 
	$(RM) *.o *~ $(TARGET)
	$(RM) -r $(TARGET).dSYM
