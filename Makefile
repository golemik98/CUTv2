# SECTION SHELL
RM := rm -rf

define print_cc
	$(if $(Q), @echo "[CC]        $(1)")
endef

define print_bin
	$(if $(Q), @echo "[BIN]       $(1)")
endef

define print_rm
    $(if $(Q), @echo "[RM]        $(1)")
endef

define print
    $(if $(Q), @echo "$(1)")
endef

# ARGS (Quiet OR Verbose), type make V=1 to enable verbose mode
ifeq ("$(origin V)", "command line")
	Q :=
else
	Q := @
endif


# SECTION PROJECT
SDIR := ./src
IDIR := ./include
ADIR := ./app
TDIR := ./test

SRC := $(wildcard $(SDIR)/*.c) #foo.c
ASRC := $(SRC) $(wildcard $(ADIR)/*.c)
TSRC := $(SRC) $(wildcard $(TDIR)/*.c)

H_INC := $(foreach h, $(IDIR), -I$h)

APP_BIN := ./main.out
TEST_BIN := ./test.out

# lista obiektów potrzebna żeby wygenerować source
SOBJ := $(SRC:%.c=%.o)
AOBJ := $(ASRC:%.c=%.o)
TOBJ := $(TSRC:%.c=%.o)
OBJ := $(SOBJ) $(AOBJ) $(TOBJ)

# SECTION COMPILER
COMPILER ?= gcc

C_STD := -std=c99
C_OPT := -O3
C_WARNS :=

ifeq ($(COMPILER),clang)
	C_WARNS += -Weverything -Wno-padded
else ifneq (, $(filter $(COMPILER), cc gcc))
	C_WARNS += -Wall -Wextra 
endif

ifeq ("$(origin DEBUG)", "command line") # eg. make test DEBUG=1 
	GGDB := -ggdb3 -gdwarf-4
	C_OPT := -O0
else
	GGDB :=
endif

C_FLAGS := $(C_STD) $(C_WARNS) $(C_OPT) $(GGDB)

# SECTION TARGET
PHONY:all
all:
	@$(MAKE) app --no-print-directory

app: $(APP_BIN)
.PHONY:app
$(APP_BIN): $(AOBJ)
	$(call print_bin,$(APP_BIN))
	$(Q)$(COMPILER) $(C_FLAGS) $(H_INC) $(AOBJ) -o $(APP_BIN)

test: $(TEST_BIN)
.PHONY:test 
$(TEST_BIN): $(TOBJ)
	$(call print_bin,$(TEST_BIN))
	$(Q)$(COMPILER) $(C_FLAGS) $(H_INC) $(TOBJ) -o $(TEST_BIN)

%.o:%.c
	$(call print_bin,$<)
	$(Q)$(COMPILER) $(C_FLAGS) -c $< $(H_INC) -o $@ 

.PHONY:memcheck
memcheck: test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 $(TEST_BIN)

.PHONY:clean 
clean:
	$(call print_rm,$(APP_BIN))
	$(Q)$(RM) $(APP_BIN)
	$(call print_rm,$(TEST_BIN))
	$(Q)$(RM) $(TEST_BIN)
	$(call print_rm,$(OBJ))
	$(Q)$(RM) $(OBJ)

.PHONY:regression
regression:
	$(call print,Regression has been started:)
	$(call print,cleaning:)
	$(Q)$(MAKE) clean --no-print-directory
	$(call print,building app:)
	$(Q)$(MAKE) app --no-print-directory
	$(call print,cleaning:)
	$(Q)$(MAKE) clean --no-print-directory
	$(call print,building tests:)
	$(Q)$(MAKE) test --no-print-directory
	$(call print,running tests:)
	$(call print,tests PASSED)
	$(Q)./$(TEST_BIN)
	$(call print,valgrind memcheck:)
	$(Q)$(MAKE) memcheck --no-print-directory
	$(call print,memcheck PASSED)
	$(call print,Regression PASSED)

.PHONY: help
help:
	@echo "Makefile"
	@echo -e
	@echo "Targets:"
	@echo "    all               - build app and test"
	@echo "    app               - build only app"
	@echo "    test              - build only test"
	@echo "    regression        - build and run all tests"
	@echo "    memcheck          - build test and run them using valgrind"
	@echo -e
	@echo "Makefile supports Verbose mode when V=1 (make all V=1)"
	@echo "Makefile supports Debug mode when DEBUG=1 (make all DEBUG=1)"
	@echo "To change default compiler (gcc) change CC variable (i.e export CC=clang)"