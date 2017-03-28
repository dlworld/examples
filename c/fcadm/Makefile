

TARGET	:= fcadm

HEADERS	:= $(wildcard *.h)
SOURCES	:= $(wildcard *.c)


all : $(TARGET)


$(TARGET) : Makefile $(HEADERS) $(SOURCES)
	gcc -g $(SOURCES) -o $@

clean : 
	-@rm -rfv $(TARGET)


