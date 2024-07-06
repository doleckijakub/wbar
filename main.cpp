#include <stdint.h>
#include <stdio.h>
#define eprintf(format, ...) fprintf(stderr, format, ##__VA_ARGS__)
#include <string.h>
#include <stdlib.h>

#include <wayland-client.h>
#include <wayland-client-protocol.h>
#include "xdg-output-unstable-v1-protocol.h"
#include "idle-inhibit-unstable-v1-client-protocol.h"

struct Client {
    struct wl_display *wl_display;
    struct wl_registry *wl_registry;
    struct zxdg_output_manager_v1 *xdg_output_manager;
    struct zwp_idle_inhibit_manager_v1 *idle_inhibit_manager;
};

void die(const char *why) {
    eprintf("error: %s, aborting\n", why);
    exit(1);
}

int main() {
    Client client = { };

    client.wl_display = wl_display_connect(NULL);
    if (!client.wl_display) die("failed to connect to the wayland display");

    client.wl_registry = wl_display_get_registry(client.wl_display);
    if (!client.wl_display) die("failed to access the wayland registry");

    {
        const struct wl_registry_listener registry_listener = {
            .global = [](void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
                Client &client = *(Client *) data;

                if (strcmp(interface, zxdg_output_manager_v1_interface.name) == 0) {
                    if (version < ZXDG_OUTPUT_V1_NAME_SINCE_VERSION) die("wl_registry version too old");
                    client.xdg_output_manager = (zxdg_output_manager_v1 *) wl_registry_bind(registry, name, &zxdg_output_manager_v1_interface, ZXDG_OUTPUT_V1_NAME_SINCE_VERSION);
                } else if (strcmp(interface, zwp_idle_inhibit_manager_v1_interface.name) == 0) {
                    client.idle_inhibit_manager = (zwp_idle_inhibit_manager_v1 *) wl_registry_bind(registry, name, &zwp_idle_inhibit_manager_v1_interface, 1);
                }
            },
            .global_remove = [](void *data, struct wl_registry *registry, uint32_t name) {
                // nop
                (void) data;
                (void) registry;
                (void) name;
            },
        };

        wl_registry_add_listener(client.wl_registry, &registry_listener, &client);
        wl_display_roundtrip(client.wl_display);
    }

    if (!client.xdg_output_manager) die("failed to bind to the xdg output manager");
    if (!client.idle_inhibit_manager) die("failed to bind to the idle inhibit manager");

    printf("Hello, World!");
}