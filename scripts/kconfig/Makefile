# We need this, in case the user has it in its environment
CC		= gcc
check-lxdialog  = ./lxdialog/check-lxdialog.sh
LIBS		= $(shell $(check-lxdialog) -ldflags)
CFLAGS		= $(shell $(check-lxdialog) -ccflags)
check_ncursesw	= $(shell $(check-lxdialog) -check $(CC) $(CFLAGS) )

# ===========================================================================
# Shared Makefile for the various kconfig executables:
# conf:	  Used for defconfig, oldconfig and related targets
# nconf:  Used for the nconfig target.
#         Utilizes ncurses
# mconf:  Used for the menuconfig target
#         Utilizes the lxdialog package
# qconf:  Used for the xconfig target
#         Based on Qt which needs to be installed to compile it
# gconf:  Used for the gconfig target
#         Based on GTK+ which needs to be installed to compile it
# object files used by all kconfig flavours

lxdialog	:= lxdialog/checklist.o lxdialog/util.o lxdialog/inputbox.o
lxdialog	+= lxdialog/textbox.o lxdialog/yesno.o lxdialog/menubox.o

conf-objs	:= conf.o  zconf.tab.o
mconf-objs	:= mconf.o zconf.tab.o $(lxdialog)

target		+= zconf.lex.c
clean-files	:= zconf.tab.c zconf.lex.c zconf.hash.c $(conf-objs) $(mconf-objs)
target = conf mconf

check = $(wildcard $(addsuffix /$(1),$(subst :, ,$(PATH))))

all: check conf mconf

PHONY = check all clean
check:
ifneq ($(check_ncursesw),)
	@echo $(check_ncursesw)
endif
conf: conf.o zconf.tab.o
	@$(CC) $^ -o $@
	@echo "   LD     $@"

mconf: $(mconf-objs)
	@$(CC) $^ -o $@ $(LIBS)
	@echo "   LD     $@"

zconf.tab.o: zconf.lex.c

zconf.tab.c: zconf.y
ifneq ($(call check,bison),)
	@bison -o $@ -t -l $< && echo "   YACC   $@"
else
	@echo "Not find bison, please install bison!"
endif

zconf.lex.c: zconf.l
ifneq ($(call check,flex),)
	@flex -o $@ -L $< && echo "   LEX    $@"
else
	@echo "Not find flex, please install flex!"
endif

%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@ && echo "   CC     $@"


clean:
	@rm -f $(objs) $(target) $(clean-files)

.PHONY:$(PHONY)
