¥CC = gcc
CXX = g++
ECHO = echo
RM = rm -f

CFLAGS = -Wall -ggdb -funroll-loops -pg
CXXFLAGS = -Wall -ggdb -funroll-loops
LDFLAGS = -lncurses

BIN = main
OBJS = main.o binheap.o dice.o dungeoncreate.o dungeondisplay.o dungeonload.o dungeonsave.o globalvars.o monster.o monster_reader.o monsters.o mapobject.o objectfactory.o path.o parser.o pathfinding.o room.o stairs.o turn.o terminalcontrol.o utilities.o

all: $(BIN) etags

$(BIN): $(OBJS)
	@$(ECHO) Linking $@
	@$(CXX) $^ -o $@ $(LDFLAGS)

-include $(OBJS:.o=.d)

%.o: %.c
	@$(ECHO) Compiling $<
	@$(CC) $(CFLAGS) -MMD -MF $*.d -c $<

%.o: %.cpp
	@$(ECHO) Compiling $<
	@$(CXX) $(CXXFLAGS) -MMD -MF $*.d -c $<

.PHONY: all clean clobber etags

clean:
	@$(ECHO) Removing all generated files
	@$(RM) *.o *~ $(BIN) *.d TAGS core vgcore.* gmon.out

clobber: clean
	@$(ECHO) Removing backup files
	@$(RM) *~ \#* *pgm

etags:
	@$(ECHO) Updating TAGS
	@etags *.[ch]
