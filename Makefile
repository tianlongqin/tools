sinclude .config

CC              ?= $(CROSS_COMPILE)gcc
STRIP           ?= $(CROSS_COMPILE)strip
AR              ?= $(CROSS_COMPILE)ar
LD              ?= $(CROSS_COMPILE)ld

dirs :=
ifeq ($(CONFIG_DEBUG),y)
	dirs += debug
endif
ifeq ($(CONFIG_FILE),y)
	dirs += file
endif
ifeq ($(CONFIG_SEMAPHORE),y)
	dirs += semaphore
endif
ifeq ($(CONFIG_SHAREMEM),y)
	dirs += share_mem
endif
ifeq ($(CONFIG_THREADPOOL),y)
	dirs += thread_pool
endif

ifeq ($(CONFIG_SERIAL),y)
	dirs += serial
endif
src := $(foreach dir,$(dirs),$(wildcard $(dir)/*.c))
obj := $(patsubst %.c, %.o, $(src))

LIB = -lpthread
CFLAGS = -fPIC

build = ./build
build_objs = $(build)/*.o
INCLUDE = $(addprefix -I, $(dirs))

so_name = libtool.so
ar_name = libtool.a

PHONY += default
default: $(obj)
	@mkdir -p build && mv $^ $(build)

PHONY += all
all: so ar

PHONY += so
so: default
	$(CC) -shared $(build_objs)  -o $(so_name) $(LIB)
	mv $(so_name) $(build)

PHONY += ar
ar: default
	$(AR) -crs $(ar_name) $(build_objs)
	mv $(ar_name) $(build)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

PHONY += menuconfig
menuconfig:
	./scripts/mconf Config.in

PHONY += clean
clean:
	rm -rf $(build)

PHONY += mrproper
mrproper:
	rm -rf $(build) .config

PHONY += help
help:
	@echo 'Cleaning targets:'
	@echo '  clean          - Remove most generated files'
	@echo '  mrproper       - Remove all generated files + .config'
	@echo 'Building targets:'
	@echo '  all            - Build all'
	@echo '  so             - Build shared library'
	@echo '  ar             - Build static library'
	@echo 'Config targets:'
	@echo '  menuconfig     - Config file'



.PHONY: $(PHONY)
