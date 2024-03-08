CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c++17 -lstdc++
MainModel = Model/MainModel.h
TEST_LIBS = -lgtest -lgtest_main -DQT_TESTLIB_LIB
QMAKE = qmake6
EXE_FILE = SmartCalc2_0

all: install

install:
	mkdir -p build
	cd Pro && $(QMAKE) && make && mv $(EXE_FILE) ../build
	$(MAKE) clean_assembly
    
uninstall:
	rm -rf build
    
dist: install
	tar -cf SmartCalc2_0.tar.gz build/
	rm -rf build
    
extract:
	tar -xvf SmartCalc2_0.tar.gz
    
dvi: open

open:
	lynx dvi/dvi.html

clean: clean_assembly
	rm -rf build SmartCalc2_0.tar.gz
	cd Tests && rm -rf test

clean_assembly: 
	cd Pro && \
	rm -rf .qmake.stash *.o Makefile moc* ui*

tests:
	cd Tests && \
	g++ $(CFLAGS) test.cpp ../Model/MainModel* ../Model/ModelCredit* -o test $(TEST_LIBS) && \
	./test && \
	rm -rf test

sanitize: clean
	cd Tests && \
	g++ $(CFLAGS) test.cpp ../Model/MainModel* ../Model/ModelCredit* -o test $(TEST_LIBS) -fsanitize=address && \
	./test && \
	rm -rf test

do_style:
	clang-format -style=Google -i Controller/*
	clang-format -style=Google -i Model/*
	clang-format -style=Google -i View/*cpp View/*h
	clang-format -style=Google -i Tests/*cpp

check_style:
	clang-format -style=Google -n Controller/*
	clang-format -style=Google -n Model/*
	clang-format -style=Google -n View/*cpp View/*h
	clang-format -style=Google -n Tests/*cpp
