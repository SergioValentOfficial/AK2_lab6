obj-m += hello1.o
obj-m += hello2.o

# щоб можна було писати #include "hello1.h" (без ./ і без шляхів)
ccflags-y += -I$(src)
