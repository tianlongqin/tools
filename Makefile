sinclude .config

KCONFIG_CONFIG  ?= .config
TOPDIR		= $(shell pwd)
export TOPDIR

mconf		= $(CURDIR)/scripts/kconfig/mconf
conf		= $(CURDIR)/scripts/kconfig/conf
config		= $(CURDIR)/.config
CONFIG		= $(CURDIR)/config.c

obj-$(CONFIG_DEBUG)		+= debug
obj-$(CONFIG_FILE)		+= file
obj-$(CONFIG_SEMAPHORE)		+= semaphore
obj-$(CONFIG_SHAREMEM)		+= share_mem
obj-$(CONFIG_THREADPOOL)	+= thread_pool
obj-$(CONFIG_SERIAL)		+= serial
obj-$(CONFIG_TIMER)		+= timer
obj-$(CONFIG_SIGNAL)		+= signal
obj-$(CONFIG_MATCH)		+= match_algo

src = $(foreach dir, $(obj-y), $(wildcard $(dir)/*.c))
obj = $(patsubst %.c, %.o, $(src)) config.o

LIB = -lpthread

CFLAGS = -fPIC
INCLUDE = $(addprefix -I, $(obj-y)) -I$(TOPDIR) -I$(TOPDIR)/include

build = ./build
build_objs = $(build)/*.o

so_name = libtool.so
ar_name = libtool.a

define create_config
	@echo "#include <stdio.h>"	> $@
	@echo "void config(void) {" >> $@
	@cat .config |grep -v "#"| awk '{print "printf(\"" $$1 "\");"}' >> $@
	@echo "}" >> $@
endef

PHONY += default
default: $(obj) sysconfig
	@mkdir -p build && mv $(obj) $(build)

$(obj): $(CONFIG)

$(CONFIG)::
	$(call create_config)

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

$(obj):%.o:%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)
	@echo "CC $<"

PHONY += menuconfig
menuconfig: $(mconf)
	@$< Kconfig
	@mkdir -p include/config include/generated
	@$(conf) --syncconfig Kconfig

$(mconf):
	$(MAKE) -C $(CURDIR)/scripts/kconfig all

sysconfig: $(conf) $(KCONFIG_CONFIG)
	@mkdir -p include/config include/generated
	@$< --syncconfig Kconfig

PHONY += clean
clean:
	@rm -rf $(build) $(obj) $(CONFIG)

PHONY += mrproper
mrproper:
	@rm -rf $(build) .config include/config include/generated
	@$(MAKE) -C $(CURDIR)/scripts/kconfig clean

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
