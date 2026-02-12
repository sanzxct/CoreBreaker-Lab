CXX = clang++
CXXFLAGS = -std=c++20 -Wall -O3
LDFLAGS = -framework Hypervisor -framework Foundation
SIGN_IDENTITY = "CoreBreaker-Dev"
ENTITLEMENTS = Entitlements.plist
TARGET = bin/CoreBreaker
SRC = src/main.cpp

all: clean build sign

build:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

sign:
	codesign --force --entitlements $(ENTITLEMENTS) --sign $(SIGN_IDENTITY) $(TARGET)
	@echo "[+] Binary signed with Hypervisor entitlement."

clean:
	rm -rf bin/*