// Developer: Sreeraj
// GitHub: https://github.com/s-r-e-e-r-a-j

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/mutex.h>
#include <linux/errno.h>
#include "../include/uniinputengine.h"

struct uni_devset udevs;

static int set_kbd_caps(struct input_dev *dev) {
	unsigned int i;
	for (i = 0; i < KEY_MAX; ++i)
		input_set_capability(dev, EV_KEY, i);
	input_set_capability(dev, EV_REP, REP_DELAY);
	input_set_capability(dev, EV_REP, REP_PERIOD);
	return 0;
}

static int set_mouse_caps(struct input_dev *dev) {
	input_set_capability(dev, EV_KEY, BTN_LEFT);
	input_set_capability(dev, EV_KEY, BTN_RIGHT);
	input_set_capability(dev, EV_KEY, BTN_MIDDLE);
	input_set_capability(dev, EV_KEY, BTN_SIDE);
	input_set_capability(dev, EV_KEY, BTN_EXTRA);
	input_set_capability(dev, EV_KEY, BTN_FORWARD);
	input_set_capability(dev, EV_KEY, BTN_BACK);
	input_set_capability(dev, EV_REL, REL_X);
	input_set_capability(dev, EV_REL, REL_Y);
	input_set_capability(dev, EV_REL, REL_WHEEL);
	input_set_capability(dev, EV_REL, REL_HWHEEL);
	return 0;
}

static int set_gamepad_caps(struct input_dev *dev) {
	input_set_capability(dev, EV_KEY, BTN_SOUTH);
	input_set_capability(dev, EV_KEY, BTN_EAST);
	input_set_capability(dev, EV_KEY, BTN_NORTH);
	input_set_capability(dev, EV_KEY, BTN_WEST);
	input_set_capability(dev, EV_KEY, BTN_TL);
	input_set_capability(dev, EV_KEY, BTN_TR);
	input_set_capability(dev, EV_KEY, BTN_TL2);
	input_set_capability(dev, EV_KEY, BTN_TR2);
	input_set_capability(dev, EV_KEY, BTN_THUMBL);
	input_set_capability(dev, EV_KEY, BTN_THUMBR);
	input_set_capability(dev, EV_KEY, BTN_SELECT);
	input_set_capability(dev, EV_KEY, BTN_START);
	input_set_capability(dev, EV_KEY, BTN_MODE);
	input_set_capability(dev, EV_KEY, BTN_TRIGGER);
	input_set_capability(dev, EV_ABS, ABS_X);
	input_set_capability(dev, EV_ABS, ABS_Y);
	input_set_capability(dev, EV_ABS, ABS_Z);
	input_set_capability(dev, EV_ABS, ABS_RX);
	input_set_capability(dev, EV_ABS, ABS_RY);
	input_set_capability(dev, EV_ABS, ABS_RZ);
	input_set_capability(dev, EV_ABS, ABS_HAT0X);
	input_set_capability(dev, EV_ABS, ABS_HAT0Y);
	input_set_abs_params(dev, ABS_X, -32768, 32767, 0, 0);
	input_set_abs_params(dev, ABS_Y, -32768, 32767, 0, 0);
	input_set_abs_params(dev, ABS_RX, -32768, 32767, 0, 0);
	input_set_abs_params(dev, ABS_RY, -32768, 32767, 0, 0);
	input_set_abs_params(dev, ABS_Z, 0, 255, 0, 0);
	input_set_abs_params(dev, ABS_RZ, 0, 255, 0, 0);
	input_set_abs_params(dev, ABS_HAT0X, -1, 1, 0, 0);
	input_set_abs_params(dev, ABS_HAT0Y, -1, 1, 0, 0);
	return 0;
}

static int set_button_caps(struct input_dev *dev) {
	unsigned int i;
	for (i = BTN_0; i <= BTN_9; ++i)
		input_set_capability(dev, EV_KEY, i);
	return 0;
}

int uni_emit_kbd_key(unsigned int code, int value) {
	int err = -EINVAL;
	if (code >= KEY_MAX)
		return -EINVAL;
	mutex_lock(&udevs.lock);
	if (!udevs.kbd)
		goto out;
	input_report_key(udevs.kbd, code, value);
	input_sync(udevs.kbd);
	err = 0;
out:
	mutex_unlock(&udevs.lock);
	return err;
}
EXPORT_SYMBOL(uni_emit_kbd_key);

int uni_emit_mouse_btn(unsigned int code, int value) {
	int err = -EINVAL;
	switch (code) {
	case BTN_LEFT:
	case BTN_RIGHT:
	case BTN_MIDDLE:
	case BTN_SIDE:
	case BTN_EXTRA:
	case BTN_FORWARD:
	case BTN_BACK:
		break;
	default:
		return -EINVAL;
	}
	mutex_lock(&udevs.lock);
	if (!udevs.mouse)
		goto out;
	input_report_key(udevs.mouse, code, value);
	input_sync(udevs.mouse);
	err = 0;
out:
	mutex_unlock(&udevs.lock);
	return err;
}
EXPORT_SYMBOL(uni_emit_mouse_btn);

int uni_emit_mouse_rel(int dx, int dy, int wheel) {
	int err = -EINVAL;
	mutex_lock(&udevs.lock);
	if (!udevs.mouse)
		goto out;
	if (dx)
		input_report_rel(udevs.mouse, REL_X, dx);
	if (dy)
		input_report_rel(udevs.mouse, REL_Y, dy);
	if (wheel)
		input_report_rel(udevs.mouse, REL_WHEEL, wheel);
	input_sync(udevs.mouse);
	err = 0;
out:
	mutex_unlock(&udevs.lock);
	return err;
}
EXPORT_SYMBOL(uni_emit_mouse_rel);

int uni_emit_gamepad_key(unsigned int code, int value) {
	int err = -EINVAL;
	if (code >= KEY_MAX)
		return -EINVAL;
	mutex_lock(&udevs.lock);
	if (!udevs.gamepad)
		goto out;
	input_report_key(udevs.gamepad, code, value);
	input_sync(udevs.gamepad);
	err = 0;
out:
	mutex_unlock(&udevs.lock);
	return err;
}
EXPORT_SYMBOL(uni_emit_gamepad_key);

int uni_emit_gamepad_abs(unsigned int axis, int val) {
	int err = -EINVAL;
	if (axis >= ABS_MAX)
		return -EINVAL;
	mutex_lock(&udevs.lock);
	if (!udevs.gamepad)
		goto out;
	input_report_abs(udevs.gamepad, axis, val);
	input_sync(udevs.gamepad);
	err = 0;
out:
	mutex_unlock(&udevs.lock);
	return err;
}
EXPORT_SYMBOL(uni_emit_gamepad_abs);

int uni_emit_button_key(unsigned int code, int value) {
	int err = -EINVAL;
	if (code < BTN_0 || code > BTN_9)
		return -EINVAL;
	mutex_lock(&udevs.lock);
	if (!udevs.button)
		goto out;
	input_report_key(udevs.button, code, value);
	input_sync(udevs.button);
	err = 0;
out:
	mutex_unlock(&udevs.lock);
	return err;
}
EXPORT_SYMBOL(uni_emit_button_key);

int uni_register_devices(void) {
	int err;
	mutex_init(&udevs.lock);
	udevs.kbd = input_allocate_device();
	if (!udevs.kbd)
		return -ENOMEM;
	udevs.kbd->name = "uniinputengine-virtual-keyboard";
	udevs.kbd->phys = "uniinputengine/input0";
	udevs.kbd->id.bustype = BUS_VIRTUAL;
	set_kbd_caps(udevs.kbd);
	err = input_register_device(udevs.kbd);
	if (err) {
		input_free_device(udevs.kbd);
		udevs.kbd = NULL;
		return err;
	}
	udevs.mouse = input_allocate_device();
	if (!udevs.mouse) {
		err = -ENOMEM;
		goto fail_mouse_alloc;
	}
	udevs.mouse->name = "uniinputengine-virtual-mouse";
	udevs.mouse->phys = "uniinputengine/input1";
	udevs.mouse->id.bustype = BUS_VIRTUAL;
	set_mouse_caps(udevs.mouse);
	err = input_register_device(udevs.mouse);
	if (err)
		goto fail_mouse_reg;
	udevs.gamepad = input_allocate_device();
	if (!udevs.gamepad) {
		err = -ENOMEM;
		goto fail_gamepad_alloc;
	}
	udevs.gamepad->name = "uniinputengine-virtual-gamepad";
	udevs.gamepad->phys = "uniinputengine/input2";
	udevs.gamepad->id.bustype = BUS_VIRTUAL;
	set_gamepad_caps(udevs.gamepad);
	err = input_register_device(udevs.gamepad);
	if (err)
		goto fail_gamepad_reg;
	udevs.button = input_allocate_device();
	if (!udevs.button) {
		err = -ENOMEM;
		goto fail_button_alloc;
	}
	udevs.button->name = "uniinputengine-virtual-button";
	udevs.button->phys = "uniinputengine/input3";
	udevs.button->id.bustype = BUS_VIRTUAL;
	set_button_caps(udevs.button);
	err = input_register_device(udevs.button);
	if (err)
		goto fail_button_reg;
	return 0;
fail_button_reg:
	input_free_device(udevs.button);
	udevs.button = NULL;
fail_button_alloc:
	input_unregister_device(udevs.gamepad);
	udevs.gamepad = NULL;
	return err;
fail_gamepad_reg:
	input_free_device(udevs.gamepad);
	udevs.gamepad = NULL;
fail_gamepad_alloc:
	input_unregister_device(udevs.mouse);
	udevs.mouse = NULL;
	return err;
fail_mouse_reg:
	input_free_device(udevs.mouse);
	udevs.mouse = NULL;
fail_mouse_alloc:
	input_unregister_device(udevs.kbd);
	udevs.kbd = NULL;
	return err;
}
EXPORT_SYMBOL(uni_register_devices);

void uni_unregister_devices(void) {
	mutex_lock(&udevs.lock);
	if (udevs.button) {
		input_unregister_device(udevs.button);
		udevs.button = NULL;
	}
	if (udevs.gamepad) {
		input_unregister_device(udevs.gamepad);
		udevs.gamepad = NULL;
	}
	if (udevs.mouse) {
		input_unregister_device(udevs.mouse);
		udevs.mouse = NULL;
	}
	if (udevs.kbd) {
		input_unregister_device(udevs.kbd);
		udevs.kbd = NULL;
	}
	mutex_unlock(&udevs.lock);
}
EXPORT_SYMBOL(uni_unregister_devices);

static int __init ui_init(void) {
	return uni_register_devices();
}

static void __exit ui_exit(void) {
	uni_unregister_devices();
}

module_init(ui_init);
module_exit(ui_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sreeraj S Kurup");
MODULE_DESCRIPTION("UniInput Engine: Virtual multi-device driver providing keyboard, mouse, gamepad, and button input support");
