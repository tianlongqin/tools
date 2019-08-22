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

src := $(foreach dir,$(dirs),$(wildcard $(dir)/*.c))
obj := $(patsubst %.c, %.o, $(src))

LIB = -lpthread
CFLAGS = -fPIC

build = ./build
build_objs = $(build)/*.o

so_name = libtool.so
ar_name = libtool.a

INCLUDE = $(addprefix -I, $(dirs))

default: $(obj)
	@mkdir -p build && mv $^ $(build)

so: default
	$(CC) -shared $(build_objs)  -o $(so_name) $(LIB)
	mv $(so_name) $(build)

ar: default
	$(AR) -crs $(ar_name) $(build_objs)
	mv $(ar_name) $(build)

menuconfig:
	./scripts/mconf Config.in

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

clean:
	rm -rf $(build)
