sinclude .config
sinclude .config.prf
CC              ?= $(CROSS_COMPILE)gcc
STRIP           ?= $(CROSS_COMPILE)strip
AR              ?= $(CROSS_COMPILE)ar
LD              ?= $(CROSS_COMPILE)ld

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
	@$(CC) -shared $(build_objs)  -o $(so_name) $(LIB)
	@echo "LD $(so_name)"
	@mv $(so_name) $(build)

PHONY += ar
ar: default
	@$(AR) -crs $(ar_name) $(build_objs)
	@echo "AR $(ar_name)"
	@mv $(ar_name) $(build)

%.o:%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)
	@echo "CC $<"

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
