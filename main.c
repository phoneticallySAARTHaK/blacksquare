#include <stdio.h>
#include <string.h>
#include <wayland-client.h>

struct wl_compositor *compositor;
struct wl_shm *shm;
struct wl_shell *shell;

void registry_global_handler(
		void *data,
		struct wl_registry *registry,
		uint32_t name,
		const char *interface,
		uint32_t version
		)
{
	if (strcmp(interface, "wl_compositor") == 0)
	{
		compositor = wl_registry_bind(registry,
				name,
				&wl_compositor_interface,
				3);
	}
	else if (strcmp(interface, "wl_shm") == 0)
	{
		shm = wl_registry_bind(registry, name,
				&wl_shm_interface, 1);
	}
	else if (strcmp(interface, "wl_shell") == 0)
	{
		shell = wl_registry_bind(registry, name,
				&wl_shell_interface, 1);
	}
	printf("interface: '%s', version: %u, name: %u\n",
			interface, version, name);
}

void registry_global_remove_handler(
		void *data,
		struct wl_registry *registry,
		uint32_t name
		)
{
	printf("Removed: %u\n", name);
}
const struct wl_registry_listener registry_listner = {
		.global = registry_global_handler,
		.global_remove = registry_global_remove_handler
	};

int main(void)
{
	struct wl_display *display = wl_display_connect(NULL);
	if (display)
		printf("Connected\n");
	else 
	{
		printf("Not connected\n");
		return 1;
	}
	struct wl_registry *registry = wl_display_get_registry(display);

	wl_registry_add_listener(registry, &registry_listner, NULL);

	wl_display_roundtrip(display);

	if (compositor && shm && shell)
	{
		printf("Got 'em all\n");
	}
	else
	{
		printf("Some required globals unavailable\n");
		return 1;
	}

	while (1)
	{
		wl_display_dispatch(display);
	}
	wl_display_disconnect(display);
}
