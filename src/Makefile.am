bin_PROGRAMS = esco

if ENABLE_BRAINFUCK
BRAINFUCK_SRC = bfmachine.cpp bfmemory.cpp bfpreprocessor.cpp spoonpreprocessor.cpp ookpreprocessor.cpp hodorpreprocessor.cpp xdpreprocessor.cpp
else
BRAINFUCK_SRC =
endif

if ENABLE_HQ9
HQ9_SRC = hqpreprocessor.cpp hqmachine.cpp
else
HQ9_SRC =
endif

if ENABLE_WHITESPACE
WHITESPACE_SRC = wspreprocessor.cpp wsmachine.cpp wsmemory.cpp wsnumber.cpp
else
WHITESPACE_SRC =
endif

if ENABLE_BEFUNGE
BEFUNGE_SRC = b93machine.cpp b93preprocessor.cpp
else
BEFUNGE_SRC =
endif

if ENABLE_BYTER
BYTER_SRC = bytermachine.cpp byterpreprocessor.cpp
else
BYTER_SRC =
endif

esco_SOURCES = $(BRAINFUCK_SRC) $(HQ9_SRC) $(WHITESPACE_SRC) $(BEFUNGE_SRC) $(BYTER_SRC) machine.cpp envtouch.cpp strerror.cpp escoconf.cpp machinecreater.cpp esco.cpp termenv.cpp compositeenv.cpp blankreader.cpp filereader.cpp sourcereader.cpp interface.cpp consoleface.cpp

INCLUDES = -I../include

test: all
	@echo;\
	for file in $$( ls ../$(SCRIPT_DIR) ); do \
		echo "Testing $$file";\
		md5sum=$$( esco -f ../$(SCRIPT_DIR)/$$file -t $$file -o esco_tmp > /dev/null; md5sum esco_tmp| awk '{print $$1}' );\
		rm esco_tmp;\
		tcmd5sum=$(cmd5sum);\
		if [ "$$file" = "hq+" ]; then \
			tcmd5sum=$(hqmd5sum);\
		fi;\
		if [ "$$md5sum" != "$$tcmd5sum" ]; then \
			echo "Error: md5sums not matching!!! ($$md5sum)";\
			exit 1;\
		else\
			echo "$$file testing OK!";\
		fi;\
	done;


