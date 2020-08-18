3rdpart_dir = zlog-1.3.0
define build_zlog
	@echo "CHK	zlog"
	@if [ ! -d zlog-1.3.0 ]; then \
		tar xf zlog-1.3.0.tgz;	\
	fi
	@cd zlog-1.3.0 && make
	@cd zlog-1.3.0/src && install -m 0644 libzlog.a $(TOPDIR)/build/
	@cd zlog-1.3.0/src && install -m 0644 zlog.h $(TOPDIR)/include/
endef

PHONY   += libzlog.a
libzlog.a:
	$(call build_zlog)

PHONY   += clean
clean:

PHONY   += mrproper
mrproper: clean
	@rm -rf $(3rdpart_dir)

.PHONY: $(PHONY)
