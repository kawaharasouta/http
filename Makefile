OBJS=httpget.o
SRCS=$(OBJS:%.o=%.c)
#
# CFLAGS
#
# Solaris
#CFLANGS= -g
# Linux
CFLAGS=-g -DX86
#
# LDLIBS
#
# Solaris
#LDLIBS= -xildoff -lsocket -lnsl -lgen
# Linux
LDLIBS=
#
TARGET=httpget
$(TARGET): $(OBJS)
  $(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)
lint:
  lint $(CFLAGS)jj $(SRCS)


