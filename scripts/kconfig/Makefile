# We need this, in case the user has it in its environment
CC		= gcc
STRIP		= strip
IDIR		= /usr/include/ncursesw
LIBS		= ncursesw tinfo
DEFS		= CURSES_LOC="<curses.h>"

CFLAGS		= $(addprefix -I, $(IDIR))
CFLAGS		+= $(addprefix -D, $(DEFS))
CFLAGS		+= $(addprefix -D, $(DEFS))

LDFLAGS		:= $(addprefix -l, $(LIBS))

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

conf: conf.o zconf.tab.o
	@$(CC)  $^ -o $@
	@echo "   LD     $@"

mconf: $(mconf-objs)
	@$(CC)  $^ -o $@ $(LDFLAGS)
	@echo "   LD     $@"

zconf.tab.o: zconf.lex.c

zconf.tab.c: zconf.y
	@bison -o $@ -t -l $< && echo "   YACC   $@"

zconf.lex.c: zconf.l
	@flex -o $@ -L $< && echo "   LEX    $@"

%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@ && echo "   CC     $@"

all: conf mconf

clean:
	@rm -f $(objs) $(target) $(clean-files)
