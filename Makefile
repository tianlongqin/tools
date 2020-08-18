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
obj-$(CONFIG_HASHMAP)		+= hash_map
obj-$(CONFIG_RING)		+= ring

zlog-$(CONFIG_ZLOG)		+= libzlog.a
3rdpart 			+= $(zlog-y)

src = $(foreach dir, $(obj-y), $(wildcard $(dir)/*.c))
obj = $(patsubst %.c, %.o, $(src)) config.o

build = ./build
build_objs = $(build)/*.o
3rdpart_lib += $(addprefix $(build)/, $(3rdpart))

LIB += -lpthread
LIB += -Wl,--whole-archive,$(3rdpart_lib),--no-whole-archive

CFLAGS = -fPIC -DCONFIG_AUTOCONF
INCLUDE = $(addprefix -I, $(obj-y)) -I$(TOPDIR) -I$(TOPDIR)/include

so_name = libtool.so
ar_name = libtool.a

define create_config
	@echo "#include <stdio.h>"	> $@
	@echo "void config(void) {" >> $@
	@cat .config |tr -s '\n'|grep -v "#"| awk '{print "printf(\"" $$1 "\");"}' >> $@
	@echo "}" >> $@
endef

PHONY += default
default: $(obj) sysconfig $(3rdpart)
	@mkdir -p build && mv $(obj) $(build)

$(3rdpart): $(3rdpart_lib)

$(3rdpart_lib):
	@mkdir -p build
	@$(MAKE) -C 3rdpart $(3rdpart)

$(obj): $(CONFIG)

$(CONFIG)::
	$(call create_config)

PHONY += all
all: $(so_name) $(ar_name)

PHONY += $(so_name)
$(so_name): default
	@$(CC) -shared $(build_objs)  -o $(so_name) $(LIB)
	@echo "LD $(so_name)"
	@mv $(so_name) $(build)

ar_script       = ar.mri
define cmd_combine_static_lib
	@$(if $(wildcard $@),@$(RM) $@)
	@$(if $(wildcard $(ar_script)),@$(RM) $(ar_script))
	@echo "CREATE $@" > $(ar_script)
	@$(foreach lib, $(filter %.a, $^), echo "ADDLIB $(lib)" >> $(ar_script);)
	@echo "SAVE" >> $(ar_script)
	@echo "END" >> $(ar_script)
	@$(AR) -M < $(ar_script) && echo "AR $@"
	@$(RM) $(ar_script)
endef

PHONY += $(ar_name)
$(ar_name): default $(3rdpart_lib) build/lib.a
	$(call cmd_combine_static_lib)
	@mv $@ $(build)

build/lib.a:
	@$(AR) -crs lib.a $(build_objs)
	@mv lib.a $(build)

$(obj):%.o:%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)
	@echo "CC $<"

PHONY += menuconfig
menuconfig: $(mconf)
	@$< Kconfig
	@mkdir -p include/config include/generated
	@$(conf) --syncconfig Kconfig

PHONY += menu
menu: $(conf)
	@$< Kconfig
	@mkdir -p include/config include/generated
	@$< --syncconfig Kconfig

$(conf) $(mconf):
	$(MAKE) -C $(CURDIR)/scripts/kconfig all

sysconfig: $(conf) $(KCONFIG_CONFIG)
	@mkdir -p include/config include/generated
	@$< --syncconfig Kconfig

PHONY += clean
clean:
	@rm -rf $(build) $(obj) $(CONFIG)
	@$(MAKE) -C 3rdpart $@

PHONY += mrproper
mrproper:
	@rm -rf $(build) .config include/config include/generated
	@$(MAKE) -C $(CURDIR)/scripts/kconfig clean
	@$(MAKE) -C 3rdpart $@

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
