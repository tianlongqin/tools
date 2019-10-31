CFLAGS= -DCURSES_LOC="<curses.h>"

INCLUDE=-I/usr/include/ncursesw
LIBS= -lncursesw -ltinfo

lxdialog := lxdialog/checklist.o lxdialog/util.o lxdialog/inputbox.o
lxdialog += lxdialog/textbox.o lxdialog/yesno.o lxdialog/menubox.o

zconf_c = zconf.tab.c zconf.lex.c zconf.hash.c
objs := mconf.o zconf.tab.o $(lxdialog)

target=mconf

mconf: $(objs)
	@gcc -o $@ $^ $(LIBS)

$(zconf_c):
	@cp zconf.tab.c_shipped zconf.tab.c
	@cp zconf.hash.c_shipped zconf.hash.c
	@cp zconf.lex.c_shipped zconf.lex.c

%.o:%.c
	@gcc $(CFLAGS) -c $< -o $@ $(INCLUDE)
clean:
	@rm -rf $(objs) $(target) $(zconf_c)
