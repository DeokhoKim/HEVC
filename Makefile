
AR:=ar
PWD:=$(shell pwd)

LIB_NAME=libhevc
MAJOR_V=0
MINOR_V=0
VERSION=$(MAJOR_V).$(MINOR_V)

ROOT=$(PWD)
CXXFLAGS:= -g -fPIC -Wall -Werror -std=c++0x -I$(ROOT)
LDFLAGS:=-Llib -lhevc -Wl,-rpath=$(ROOT)/lib

LIB_OBJ= \
	common/annexE.o \
	common/bitstream.o \
	common/bytestream.o \
	common/NAL.o \
	common/PTL.o \
	common/syntax_element.o \
	common/VPS.o \

TEST_OBJ= \
	test/test_stream.o \

all: mkdir library

mkdir:
	mkdir -p lib bin

library: $(LIB_OBJ)
	$(AR) rcs lib/libhevc.a $(LIB_OBJ)
	$(CXX) $(CXXFLAGS) -shared -Wl,-soname,$(LIB_NAME).so.$(MAJOR_V) -o lib/$(LIB_NAME).so.$(VERSION) $(LIB_OBJ)
	ln -sf $(ROOT)/lib/$(LIB_NAME).so.$(VERSION) $(ROOT)/lib/$(LIB_NAME).so.$(MAJOR_V)
	ln -sf $(ROOT)/lib/$(LIB_NAME).so.$(VERSION) $(ROOT)/lib/$(LIB_NAME).so

common/%.o: common/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: library $(TEST_OBJ)

test/%.o: test/%.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(@:.o=) $<

clean:
	rm -f $(LIB_OBJ)
	rm -f $(TEST_OBJ)
	rm -f lib/$(LIB_NAME).a
	rm -f lib/$(LIB_NAME).so.$(VERSION)
	rm -f lib/$(LIB_NAME).so.$(MAJOR_V)
	rm -f lib/$(LIB_NAME).so


	
