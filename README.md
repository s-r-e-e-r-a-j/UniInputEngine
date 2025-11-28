# UniInputEngine

**UniInputEngine** is a **Linux kernel driver** that creates **virtual input devices**, including a **keyboard**, **mouse**, **gamepad**, and **button device**.  
It uses the **Linux input subsystem** to generate input events directly from the **kernel**, allowing software on Linux to receive **synthetic input events** as if they came from **real hardware**.

## features 
- Auto-loads at boot using `/etc/modules-load.d/uniinputengine.conf`.

- **Creates multiple virtual input devices**
  - Virtual keyboard  
  - Virtual mouse  
  - Virtual gamepad  
  - Virtual button device  

- **Kernel-level input event generation (directly from the Linux kernel)**
  - Emits key events  
  - Emits button events  
  - Emits mouse button events   - Emits relative mouse movement (X, Y, wheel)  
  - Emits gamepad button and axis events  

- **Detected as real hardware**
  - Applications read events through `/dev/input/eventX`  
  - Behaves exactly like physical input devices  

- **Full keyboard support**
  - Registers all Linux `KEY_` codes

- **Virtual Keyboard Support**
  - Supports all keys available in the Linux kernel (`KEY_0` … `KEY_MAX`)
  - Supports key repeat events (`REP_DELAY`, `REP_PERIOD`)

- **Virtual Mouse Support**
  - Buttons: `BTN_LEFT`, `BTN_RIGHT`, `BTN_MIDDLE`, `BTN_SIDE`, `BTN_EXTRA`, `BTN_FORWARD`, `BTN_BACK`
  - Relative movement: `REL_X`, `REL_Y`
  - Scroll wheel: `REL_WHEEL`, `REL_HWHEEL`

- **Virtual Gamepad Support**
  - Buttons: `BTN_SOUTH`, `BTN_EAST`, `BTN_NORTH`, `BTN_WEST`, `BTN_TL`, `BTN_TR`, `BTN_TL2`, `BTN_TR2`, `BTN_THUMBL`, `BTN_THUMBR`, `BTN_SELECT`, `BTN_START`, `BTN_MODE`, `BTN_TRIGGER`
  - Analog axes: `ABS_X`, `ABS_Y`, `ABS_Z`, `ABS_RX`, `ABS_RY`, `ABS_RZ`
  - Hat switch: `ABS_HAT0X`, `ABS_HAT0Y`

- **Virtual Button Device Support**
  - Buttons: `BTN_0` … `BTN_9`

- Works on most Linux distributions with proper kernel headers.
