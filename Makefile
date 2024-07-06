restart: wbar
	-killall -9 wbar 2> /dev/null
	./wbar &

CPPFLAGS += -Wall -Wextra -Werror -ggdb
LDFLAGS  += -lm

wbar: xdg-output-unstable-v1-protocol.h idle-inhibit-unstable-v1-client-protocol.h
wbar: xdg-output-unstable-v1-protocol.o idle-inhibit-unstable-v1-client-protocol.o

WBAR_LIBS += wayland-client
WBAR_LIBS += wayland-cursor

CPPFLAGS += $(shell pkg-config --cflags $(WBAR_LIBS))
LDFLAGS  += $(shell pkg-config --libs   $(WBAR_LIBS))

wbar: main.cpp
	g++ -o $@ $(CPPFLAGS) $^ $(LDFLAGS)

WAYLAND_PROTOCOLS=$(shell pkg-config --variable=pkgdatadir wayland-protocols)
WAYLAND_SCANNER=$(shell pkg-config --variable=wayland_scanner wayland-scanner)

xdg-output-unstable-v1-protocol.h:
	$(WAYLAND_SCANNER) client-header $(WAYLAND_PROTOCOLS)/unstable/xdg-output/xdg-output-unstable-v1.xml $@
xdg-output-unstable-v1-protocol.c:
	$(WAYLAND_SCANNER) private-code $(WAYLAND_PROTOCOLS)/unstable/xdg-output/xdg-output-unstable-v1.xml $@
xdg-output-unstable-v1-protocol.o: xdg-output-unstable-v1-protocol.h

idle-inhibit-unstable-v1-client-protocol.h:
	$(WAYLAND_SCANNER) client-header $(WAYLAND_PROTOCOLS)/unstable/idle-inhibit/idle-inhibit-unstable-v1.xml $@
idle-inhibit-unstable-v1-client-protocol.c:
	$(WAYLAND_SCANNER) private-code $(WAYLAND_PROTOCOLS)/unstable/idle-inhibit/idle-inhibit-unstable-v1.xml $@
idle-inhibit-unstable-v1-client-protocol.o: idle-inhibit-unstable-v1-client-protocol.h

