QDVERSION=2.3.7

if IS_MACOSX
CONFIGURE_ARGS=--prefix=$(abs_builddir)/../i/QD/ --host=$(host)
else
CONFIGURE_ARGS=--prefix=$(abs_builddir)/../i/QD/ --host=$(host) --disable-shared
endif

EXTRACT_STAMP=.qd_extract_done
PATCH_STAMP=.qd_patch_done
CONFIGURE_STAMP=.qd_configure_done
BUILD_STAMP=.qd_build_done
INSTALL_STAMP=.qd_install_done
INSTALL_INTERNAL_STAMP=.qd_install_internal_done
CHECK_STAMP=.qd_check_done

all: extract config build check install_internal

work/$(EXTRACT_STAMP):
	rm -rf work ; mkdir -p work/internal
	cd $(abs_builddir)/work/internal; tar xvfz ../../download/qd-$(QDVERSION).tar.gz
	touch $(abs_builddir)/work/$(EXTRACT_STAMP)

work/$(PATCH_STAMP): work/$(EXTRACT_STAMP)
	touch $(abs_builddir)/work/$(PATCH_STAMP)

work/$(CONFIGURE_STAMP): work/$(PATCH_STAMP)
	cd $(abs_builddir)/work/internal/qd-$(QDVERSION) ; env "CC=$(CC)" "CXX=$(CXX)" "CPPFLAGS=$(CPPFLAGS)" "F77=$(F77)" "LDFLAGS=$(LDFLAGS)" "NM=$(NM)" "RANLIB=$(RANLIB)" ./configure $(CONFIGURE_ARGS) --enable-fortran=no
	touch $(abs_builddir)/work/$(CONFIGURE_STAMP)

work/$(BUILD_STAMP): work/$(CONFIGURE_STAMP)
	cd $(abs_builddir)/work/internal/qd-$(QDVERSION) ; ${MAKE}
	touch $(abs_builddir)/work/$(BUILD_STAMP)

work/$(CHECK_STAMP): work/$(BUILD_STAMP)
#	cd $(abs_builddir)/work/internal/qd-$(QDVERSION) ; ${MAKE} check
	touch $(abs_builddir)/work/$(CHECK_STAMP)

work/$(INSTALL_INTERNAL_STAMP): work/$(CHECK_STAMP)
	cd $(abs_builddir)/work/internal/qd-$(QDVERSION) ; ${MAKE} install
	touch $(abs_builddir)/work/$(INSTALL_INTERNAL_STAMP)

extract: work/$(EXTRACT_STAMP)

patch: work/$(PATCH_STAMP)

config: work/$(CONFIGURE_STAMP)

build: work/$(BUILD_STAMP)

check: work/$(CHECK_STAMP)

install_internal: work/$(INSTALL_INTERNAL_STAMP)

install: work/$(INSTALL_STAMP)

distdir:

clean:
	rm -rf work
distclean:
	rm -rf work
