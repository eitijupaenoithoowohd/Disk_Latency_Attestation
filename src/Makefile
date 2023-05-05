# How to install dependencies and build:
#
# GCC
# sudo apt -y install build-essential clang-10
#
# Libraries
# sudo apt -y install libgoogle-perftools-dev
#
# Inatall stat and test tools
# sudo apt-get install cloc valgrind clang-tools
#
# make release # or
# make debug # or
# make prod[uction]
#
# Perf tool:
# sudo apt-get install linux-tools-common linux-tools-generic linux-tools-`uname -r`
# make perf # or
# make test
#

# Define our suffix list for quick compilation
.SUFFIXES:                    # Delete the default suffixes
.SUFFIXES: .c .o .h .d .o.d   # Define our suffix list

#
# Compiler flags
#

CFLAGS += -pipe -std=c11
CFLAGS += -fbuiltin

# To pass #define inside a code:
# make DEFINES=-DWRITE_CSV=false memtest
CFLAGS += $(DEFINES)

# libc lib for static
#LDFLAGS += -lc

EXE = disk_delay_of_writes

# Если задан define PRODUCTION, то активируется режим, в которм
# некоторые функции, существующие только для дополнительных исследований, не будут
# выполняться. По-умолчанию режим активируется только для сборки production.
# Чтобы активировать в отладочных режимах, исключив проверки и отключив счётчики
# достаточно указать PRODUCTION= как переменную для make. Например:
PRODUCTION ?= PRODUCTION

TCMALLOC = -ltcmalloc_minimal -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free
STATIC = -static
STRIP = -s
# Флаги дополнительных проверок. Must have!
WFLAGS += -Wall -Wextra -Wpedantic -Wshadow
WFLAGS += -Wconversion -Wsign-conversion -Winit-self -Wunreachable-code -Wformat-y2k
WFLAGS += -Wformat-nonliteral -Wformat-security -Wmissing-include-dirs
WFLAGS += -Wswitch-default -Wtrigraphs -Wstrict-overflow=5
WFLAGS += -Wfloat-equal -Wundef -Wshadow
WFLAGS += -Wbad-function-cast -Wcast-qual -Wcast-align
WFLAGS += -Wwrite-strings
WFLAGS += -Winline
# Если не clang, то значит эти опции для gcc
ifneq ($(CC), clang)
WFLAGS += -Wlogical-op
endif

# Arguments for tests
ARGS = /tmp /tmp/report.csv

# Config settings:
# The --no-print-directory option of make tells make not to print the message about entering and leaving the working directory.
MAKEFLAGS += --no-print-directory
CONFIG += ordered

#
# Print of variables
#
# If you want to find out the value of a makefile variable, just:
#make print-VARIABLE
# and it will return:
#VARIABLE = the_value_of_the_variable
#
print-% : ; @echo $* = $($*)

#
# Project files
#
SRCS = $(wildcard *.c)
HDRS = $(wildcard *.h)
# Exclude a file
OBJS = $(SRCS:.c=.o)
PREPROC = $(SRCS:.c=.i) # Preproc files http://www.viva64.com/en/t/0076/
PREPROC += $(SRCS:.c=.i.h)
# Asm
ASM = $(SRCS:.c=.asm)

#
# Sanitize build settings
#
STZDIR = sanitize
STZEXE = $(STZDIR)/$(EXE)
STZOBJS = $(addprefix $(STZDIR)/, $(OBJS))
STZDEP = $(STZOBJS:.o=.d)
STZLIBPATH = $(foreach d,$(LIBPATH),-L$d/debug)
STZDYNLIB = $(foreach d,$(LIBPATH),-Wl,-rpath,$d/debug)
STZCFLAGS += -fsanitize=address -g -O0 -DDEBUG
ifdef STATIC
STZCFLAGS += -static-libasan
endif

#
# Unittest build settings
#

#
# Project files
#
UNITSRCS = basic_trend.c
# Exclude a file
UNITOBJS = $(UNITSRCS:.c=.o)

UNITDIR = unittests
UNITEXE = $(UNITDIR)/$(EXE)
UNITPATH = $(addprefix $(UNITDIR)/, $(UNITOBJS))
UNITDEP = $(UNITPATH:.o=.d)
UNITLIBPATH = $(foreach d,$(LIBPATH),-L$d/debug)
UNITDYNLIB = $(foreach d,$(LIBPATH),-Wl,-rpath,$d/debug)
UNITCFLAGS += -DUNITTEST -g -O0 -DDEBUG
ifdef STATIC
UNITCFLAGS += -static-libasan
endif

#
# Debug build settings
#
DBGDIR = debug
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGDEP = $(DBGOBJS:.o=.d)
DBGHDR = $(DBGOBJS:.o=.o.d)
DBGLIBPATH = $(foreach d,$(LIBPATH),-L$d/debug)
DBGDYNLIB = $(foreach d,$(LIBPATH),-Wl,-rpath,$d/debug)
DBGCFLAGS += -g -ggdb -ggdb1 -ggdb2 -ggdb3 -O0 -DDEBUG
# Активация профайлера Gprof. Некорректно работает с Valgrind.
# Лучше использовать Callgrind - формат графа вызовов поддерживается
# средствами визуализации, например, kcachegrind.
#DBGCFLAGS += -pg

#
# Production build settings
#
PRODDIR = production
PRODEXE = $(PRODDIR)/$(EXE)
PRODOBJS = $(addprefix $(PRODDIR)/, $(OBJS))
PRODDEP = $(PRODOBJS:.o=.d)
PRODLIBPATH = $(foreach d,$(LIBPATH),-L$d/release)
PRODDYNLIB = $(foreach d,$(LIBPATH),-Wl,-rpath,$d/release)
PRODCFLAGS = -O3 -funroll-loops -DNDEBUG -D$(PRODUCTION)
PRODCFLAGS += -march=native
# Если статическая сборка, то добавить флаги
ifdef STATIC
PRODLDFLAGS += -lm -lc
endif
ifneq ($(CC), clang)
PRODCFLAGS += -flto
endif

#
# Release build settings
#
RELDIR = release
RELEXE = $(RELDIR)/$(EXE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELDEP = $(RELOBJS:.o=.d)
RELLIBPATH = $(foreach d,$(LIBPATH),-L$d/release)
RELDYNLIB = $(foreach d,$(LIBPATH),-Wl,-rpath,$d/release)
RELCFLAGS = -O3 -funroll-loops -DNDEBUG
RELCFLAGS += -march=native
# Если статическая сборка, то добавить флаги
ifdef STATIC
RELLDFLAGS += -lm -lc
endif
# Если не clang, то значит эти опции для gcc
ifneq ($(CC), clang)
RELWFLAGS += -Wsuggest-attribute=const -Wsuggest-attribute=pure -Wsuggest-attribute=noreturn -Wsuggest-attribute=format -Wmissing-format-attribute
RELCFLAGS += -flto
endif

# https://stackoverflow.com/questions/17834582/run-make-in-each-subdirectory
TOPTARGETS := all

# Default build
all: prep release

.PHONY: all clean debug prep release remake clang openmp one test sanitize

# Clang
clang: CC = clang
clang: all

#
# Sanitize rules
#
sanitize: $(STZEXE)
	@ASAN_OPTIONS=symbolize=1 ASAN_SYMBOLIZER_PATH=$(shell which llvm-symbolizer) $(STZEXE) $(ARGS)

$(STZEXE): $(STZOBJS)
	@$(CC) $(CFLAGS) $(STZCFLAGS) $(STZLIBPATH) $(STZDYNLIB) $(WFLAGS) -o $(STZEXE) $^ $(LDFLAGS)
	@echo "$@ linked."

-include $(STZDEP)

$(STZDIR)/%.o: %.c
	@mkdir -p $(STZDIR)
	@$(CC) -MM $(INCPATH) $(CFLAGS) $(STZCFLAGS) $(WFLAGS) $< | sed '1s/^/$$\(STZDIR\)\//' > $(@D)/$(*F).d
	@$(CC) -c $(INCPATH) $(CFLAGS) $(STZCFLAGS) $(WFLAGS) -o $@ $<
	@echo $<" compiled."

#
# Debug rules
#
debug: $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
	@$(CC) $(CFLAGS) $(DBGCFLAGS) $(DBGLIBPATH) $(DBGDYNLIB) $(WFLAGS) -o $(DBGEXE) $^ $(LDFLAGS)
	@echo "$@ linked."

-include $(DBGDEP)

$(DBGDIR)/%.o: %.c
	@mkdir -p $(DBGDIR)
	@$(CC) -MM $(INCPATH) $(CFLAGS) $(DBGCFLAGS) $(WFLAGS) $< | sed '1s/^/$$\(DBGDIR\)\//' > $(@D)/$(*F).d
	@$(CC) -c $(INCPATH) $(CFLAGS) $(DBGCFLAGS) $(WFLAGS) -o $@ $<
	@echo $<" compiled."

#
# Production rules
#
prod: production
production: $(PRODEXE)

$(PRODEXE): $(PRODOBJS)
	@$(CC) $(CFLAGS) $(WFLAGS) $(STATIC) $(STRIP) $(PRODLIBPATH) $(PRODDYNLIB) $(PRODLDFLAGS) $(TCMALLOC) -o $(PRODEXE) $^ $(LDFLAGS)
	@echo "$@ linked."

-include $(PRODDEP)

$(PRODDIR)/%.o: %.c
	@mkdir -p $(PRODDIR)
	@$(CC) -MM $(INCPATH) $(CFLAGS) $(PRODCFLAGS) $< | sed '1s/^/$$\(PRODDIR\)\//' > $(@D)/$(*F).d
	@$(CC) -c $(INCPATH) $(CFLAGS) $(PRODCFLAGS) -o $@ $<
	@echo $<" compiled."

#
# Release rules
#
release: $(RELEXE)

# Linking problem with "undefined reference to 'dlopen' "
# https://stackoverflow.com/a/11221504/7104681
$(RELEXE): $(RELOBJS)
	@$(CC) $(CFLAGS) $(WFLAGS) $(STATIC) $(STRIP) $(RELLIBPATH) $(RELDYNLIB) $(RELLDFLAGS) $(TCMALLOC) -o $(RELEXE) $^ $(LDFLAGS)
	@echo "$@ linked."

-include $(RELDEP)

$(RELDIR)/%.o: %.c
	@mkdir -p $(RELDIR)
	@$(CC) -MM $(INCPATH) $(CFLAGS) $(WFLAGS) $(RELWFLAGS) $(RELCFLAGS) $< | sed '1s/^/$$\(RELDIR\)\//' > $(@D)/$(*F).d
	@$(CC) -c $(INCPATH) $(CFLAGS) $(WFLAGS) $(RELWFLAGS) $(RELCFLAGS) -o $@ $<
	@echo $<" compiled."

#
# Unittesting
#

unittest: $(UNITEXE)

$(UNITEXE): $(UNITOBJS)
	$(CC) $(CFLAGS) $(UNITCFLAGS) $(UNITLIBPATH) $(UNITDYNLIB) $(WFLAGS) -o $(UNITEXE) $^ $(LDFLAGS)
	echo "$@ linked."

-include $(UNITDEP)

$(UNITDIR)/%.o: %.c
	mkdir -p $(UNITDIR)
	$(CC) -MM $(INCPATH) $(CFLAGS) $(UNITCFLAGS) $(WFLAGS) $< | sed '1s/^/$$\(UNITDIR\)\//' > $(@D)/$(*F).d
	$(CC) -c $(INCPATH) $(CFLAGS) $(UNITCFLAGS) $(WFLAGS) -o $@ $<
	echo $<" compiled."

# Optional preprocessor files
%.i:%.c clean-preproc
	@$(CC) -E -C -o $@ $(INCPATH) $(CFLAGS) $<
# C-C++ Beautifier
#	@bcpp -na $@ > $@.h
	@bcpp -na -s -i 4 $@ > $@.h
	@sed -i 's/[ \t]*\# [[:digit:]]\+ \".*//g' $@.h
#	@sed -i '/^ *$//d' $@.h

# Optional Assembler files
%.asm:%.c clean-asm
	@$(CC) -S -C $(INCPATH) $(CFLAGS) $(WFLAGS) $(RELWFLAGS) $(RELCFLAGS) $(RELLDFLAGS) -o $@ $(LDFLAGS) $(TCMALLOC) $<

#
# Other rules
#
prep:
#	@mkdir -p

remake: clean all

# Tests
test: sanitize clang-analyzer cachegrind callgrind massif cppcheck memtest perf

cppcheck:
	cppcheck --enable=all --platform=unix64 --std=c11 --std=posix -q --force --inconclusive .

memtest: debug
	valgrind -v --tool=memcheck --leak-check=full --leak-resolution=high --undef-value-errors=yes --show-reachable=yes --num-callers=20 $(DBGDIR)/$(EXE) $(ARGS)

cachegrind: debug
	valgrind --tool=cachegrind --branch-sim=yes $(DBGDIR)/$(EXE) $(ARGS)

callgrind: debug
	valgrind --tool=callgrind --dump-instr=yes --collect-jumps=yes $(DBGDIR)/$(EXE) $(ARGS)

helgrind: debug
	valgrind --tool=helgrind --read-var-info=yes --num-callers=20 $(DBGDIR)/$(EXE) $(ARGS)

massif: debug
	valgrind --tool=massif --stacks=yes --num-callers=20 $(DBGDIR)/$(EXE) $(ARGS)
	ms_print ./massif.out.*

clang-analyzer:
	# Run clang static analyzer and view analysis results in a web browser when the build command completes
	scan-build -V make debug

splint:
	splint -I /usr/include/x86_64-linux-gnu +posixlib $(SRCS)

doc:
	@doxygen Doxyfile

gource:
	gource --seconds-per-day 0.1 --auto-skip-seconds 1

#https://eax.me/c-cpp-profiling/
#https://perf.wiki.kernel.org/index.php/Main_Page
perf:
	sudo perf stat $(DBGDIR)/$(EXE) $(ARGS)

# Statistic code info and count of lines
stat: cloc
cloc:
	@cloc --exclude-dir=$(STZDIR),$(DBGDIR),$(PRODDIR),$(RELDIR),$(OMPDIR) ../

# Character | prevent threading with clean
clean: | clean-preproc clean-asm
	@rm -rf *.out.* doc $(STZDEP) $(DBGDEP) $(PRODDEP) $(RELDEP) \
		$(STZHDR) $(DBGHDR) $(PRODHDR) $(RELHDR) \
		$(DBGEXE) $(STZEXE) $(PRODEXE) $(RELEXE) \
		$(STZOBJS) $(DBGOBJS) $(PRODOBJS) $(RELOBJS)
	@test -d $(STZDIR) && rm -d $(STZDIR) || true
	@test -d $(DBGDIR) && rm -d $(DBGDIR) || true
	@test -d $(PRODDIR) && rm -d $(PRODDIR) || true
	@test -d $(RELDIR) && rm -d $(RELDIR) || true
	@echo $(EXE) cleared.

clean-preproc:
	@rm -rf $(PREPROC)

clean-asm:
	@rm -rf $(ASM)
