VPATH = ../include

INCLUDE_FLAG = -I../include
LIBRARY_FLAG =
DEBUG_FLAG = -g -Wall

SOURCE_FILES = $(wildcard *.cpp)
OBJ_FILES = $(patsubst %.cpp, %.o, $(SOURCE_FILES))
FINAL_TARGETS = GaussianShape

CC = g++


.PHONY: all
all: $(FINAL_TARGETS)


$(FINAL_TARGETS): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LIBRARY_FLAG)


%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(INCLUDE_FLAG) $< > $@.$$$$; \
	sed 's,\(.*\)\.o[:blank:]*:,\1.o $@: ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


%.o: %.cpp
	$(CC) -c $< $(INCLUDE_FLAG) $(DEBUG_FLAG)


.PHONY: clean
clean:
	rm -f *.o *.d *.d.* $(FINAL_TARGETS)


include $(SOURCE_FILES:.cpp=.d)