ifdef OS
ifeq (,$(findstring windows,${OS}))
ifneq (,$(findstring Windows,${OS}))
WINDOWS = 1
endif
else
WINDOWS = 1
endif
endif

CXX = g++
LD = g++

ifdef WINDOWS
RM = del
else
RM = rm -f
endif

CXXFLAGS = -Wall -Wextra -g -O0

SRCS = example.cpp sxml.cpp
OBJS = $(addsuffix .o,$(basename ${SRCS}))

OUTPUT = example

.PHONY: clean

all: ${OUTPUT}

clean:
	${RM} ${OBJS} ${OUTPUT}

${OUTPUT}: ${OBJS}
	${LD} -o $@ ${LDFLAGS} ${OBJS} ${LIBS} ${EXTRA_LIBS}

%.o: %.cpp
	${CXX} -c ${CXXFLAGS} ${INCS} $< -o $@
