SYS:=$(shell uname -s)
PWD:=$(shell pwd)

ifeq ($(SYS), Darwin)
BIN_DIR:="./bin/mac"
DEBUG_BIN_DIR:="./bin/mac-debug"
else
BIN_DIR:="./bin/linux"
DEBUG_BIN_DIR:="./bin/linux-debug"
endif
INSTALL_PREFIX="./wrapper"

### Require g++ >= 4.6
CXX:=$(shell which g++)
CC:=$(shell which gcc)
all: 
	if [ ! -d $(BIN_DIR) ]; then mkdir -pv $(BIN_DIR); fi && if [ ! -f $(BIN_DIR)/Makefile ]; then cd $(BIN_DIR) && cmake -DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_C_COMPILER=$(CC) -DCMAKE_INSTALL_PREFIX=$(INSTALL_PREFIX) ../../ && cd $(PWD); fi && cd $(BIN_DIR) && make
cmake: 
	if [ -d $(BIN_DIR) ]; then cd $(BIN_DIR) && cmake -DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_C_COMPILER=$(CC) -DCMAKE_INSTALL_PREFIX=$(INSTALL_PREFIX) ../../wrapper ; fi
debug: 
	if [ ! -d $(DEBUG_BIN_DIR) ]; then mkdir -pv $(DEBUG_BIN_DIR); fi && if [ ! -f $(DEBUG_BIN_DIR)/Makefile ]; then cd $(DEBUG_BIN_DIR) && cmake -DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_C_COMPILER=$(CC) -DBUILD_TYPE="DEBUG" ../../ && cd $(PWD); fi && cd $(DEBUG_BIN_DIR) && make
install: 
	if [ ! -d $(BIN_DIR) ]; then mkdir -pv $(BIN_DIR); fi && if [ ! -f $(BIN_DIR)/Makefile ]; then cd $(BIN_DIR) && cmake -DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_C_COMPILER=$(CC) -DCMAKE_INSTALL_PREFIX=$(INSTALL_PREFIX) ../../ && cd $(PWD); fi && cd $(BIN_DIR) && make install
