# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  clox_config = debug

else ifeq ($(config),release)
  clox_config = release

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := clox

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

clox:
ifneq (,$(clox_config))
	@echo "==== Building clox ($(clox_config)) ===="
	@${MAKE} --no-print-directory -C . -f clox.make config=$(clox_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f clox.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   clox"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"