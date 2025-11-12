.PHONY: clean run build
#target: dependences
#      command
CC = g++

CFLAGS = -D _DEBUG -D _DUMP -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat \
-Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy \
-Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow \
-Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn \
-Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef \
-Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers \
-Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector \
-fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla \
-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attrib$\
ute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

ifdef DEBUG
CFLAGS += -DLIST_DEBUG
endif

ifdef DUMP
CFLAGS += -DLIST_DUMP
endif

build: solver

main.o: main.cpp
	$(CC) main.cpp $(CFLAGS) -c -o main.o

list.o: list.cpp
	$(CC) list.cpp $(CFLAGS) -c -o list.o


solver: main.o list.o
	$(CC) $(CFLAGS) main.o list.o -o solver.exe

clean:
	rm *.o

main_test: main_test.o
	$(CC) $(CFLAGS) main_test.o -o main_test

run:
	.\solver

