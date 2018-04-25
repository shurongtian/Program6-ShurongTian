# CS3377.502 Program6
# Shurong Tian
# sxt151030
# sxt151030@utdallas.edu


CXX = g++
CXXFLAGS = -Wall -g
CPPFLAGS = -I/scratch/perkins/include/
LDFLAGS = -L/scratch/perkins/lib
LDLIBS = -lcdk -lncurses

EXECFILE = program6
OBJS = program6.o

all: $(EXECFILE)

clean: 
	rm -f $(OBJS) $(EXECFILE) *.P *~ \#* 

$(OBJS): program6.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c program6.cc

$(EXECFILE): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)


backup:
	@make clean
	@mkdir -p ~/backups: chmod 700 ~/backups
	@$(eval CURDIRNAME := $(shell basename "'pwd'"))
	@$(eval MKBKUPNAME := ~/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H:%M:%S').tar.gz)
	@echo
	@echo Writing Backup file to: $(MKBKUPNAME)
	@echo
	@-tar zcfv $(MKBKUPNAME) ../$(CURDIRNAME) 2> /dev/null
	@chmod 600 $(MKBKUPNAME)
	@echo
	@echo Done!
