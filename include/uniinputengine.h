// Developer: Sreeraj
// GitHub: https://github.com/s-r-e-e-r-a-j

#ifndef UNIINPUT_H
#define UNIINPUT_H

#include <linux/input.h>
#include <linux/mutex.h>

struct uni_devset {
	struct input_dev *kbd;
	struct input_dev *mouse;
	struct input_dev *gamepad;
	struct input_dev *button;
	struct mutex lock;
};

extern struct uni_devset udevs;

int uni_register_devices(void);
void uni_unregister_devices(void);

int uni_emit_kbd_key(unsigned int code, int value);
int uni_emit_mouse_btn(unsigned int code, int value);
int uni_emit_mouse_rel(int dx, int dy, int wheel);
int uni_emit_gamepad_key(unsigned int code, int value);
int uni_emit_gamepad_abs(unsigned int axis, int val);
int uni_emit_button_key(unsigned int code, int value);

#endif
