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
  - Supports all keys available in the Linux kernel (from `KEY_0` to `KEY_MAX`)
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
  - Buttons: from `BTN_0` to `BTN_9`

- Works on most Linux distributions with proper kernel headers.

## Requirements

- Linux kernel 6.14 or newer  
- GCC 13 or newer, or the GCC version used to build your running kernel  
- Make  
- Linux kernel headers for the running kernel (usually preinstalled on Ubuntu)
> **Note:** Tested on Linux kernel 6.14.0-35-generic (Linux kernel 6.14) on Ubuntu 24.04.3 LTS

---
  
## Installation

1. **Clone the repository:**
```bash
git clone https://github.com/s-r-e-e-r-a-j/UniInputEngine.git
 ```
2. **Go to the UniInputEngine directory:**
```bash
cd UniInputEngine
```
3. **Give execute permission to the `install.sh` script:**
```bash
chmod +x install.sh
```
4. **Build and install:**
```bash
sudo ./install.sh
```

## Usage

After installation, you can verify that the virtual input devices are loaded correctly by checking `/proc/bus/input/devices`:

```bash
cat /proc/bus/input/devices
```

You should see output similar to the following:

- **Virtual Keyboard**
```text
I: Bus=0006 Vendor=0000 Product=0000 Version=0000
N: Name="uniinputengine-virtual-keyboard"
P: Phys=uniinputengine/input0
S: Sysfs=/devices/virtual/input/input12
U: Uniq=
H: Handlers=sysrq rfkill kbd event6 
B: PROP=0
B: EV=3
B: KEY=7fffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffffff fffffffffffffffe
```

- **Virtual Mouse**

```text
I: Bus=0006 Vendor=0000 Product=0000 Version=0000
N: Name="uniinputengine-virtual-mouse"
P: Phys=uniinputengine/input1
S: Sysfs=/devices/virtual/input/input13
U: Uniq=
H: Handlers=mouse2 event7 
B: PROP=0
B: EV=7
B: KEY=7f0000 0 0 0 0
B: REL=143
```

- **Virtual Gamepad**

```text
I: Bus=0006 Vendor=0000 Product=0000 Version=0000
N: Name="uniinputengine-virtual-gamepad"
P: Phys=uniinputengine/input2
S: Sysfs=/devices/virtual/input/input14
U: Uniq=
H: Handlers=event8 js1 
B: PROP=0
B: EV=b
B: KEY=7fdb000100000000 0 0 0 0
B: ABS=3003f
```

- **Virtual Button Device**

```text
I: Bus=0006 Vendor=0000 Product=0000 Version=0000
N: Name="uniinputengine-virtual-button"
P: Phys=uniinputengine/input3
S: Sysfs=/devices/virtual/input/input15
U: Uniq=
H: Handlers=event9 
B: PROP=0
B: EV=3
B: KEY=3ff 0 0 0 0
```

> **Device Event Numbers:**  
> Each virtual device created by UniInputEngine has a specific event number shown in the `Handlers` line of `/proc/bus/input/devices`.  
> This event number corresponds to the `/dev/input/eventX` file used to send input to that device.  
> 
> **Important:** Event numbers can change after a reboot. Always check the current numbers using:
> 
> ```bash
> cat /proc/bus/input/devices
> ```
> 
> **Example output (event numbers may differ on your system):**
> - `uniinputengine-virtual-keyboard` → Handlers: `event6` → Use `/dev/input/event6`  
> - `uniinputengine-virtual-mouse` → Handlers: `event7` → Use `/dev/input/event7`  
> - `uniinputengine-virtual-gamepad` → Handlers: `event8` → Use `/dev/input/event8`  
> - `uniinputengine-virtual-button` → Handlers: `event9` → Use `/dev/input/event9`

After checking the event number via `/proc/bus/input/devices`, you can use `evtest` to see all supported keys, buttons, and their corresponding keycodes/values for each virtual device.

> **Note:** `evtest` is not required for the UniInputEngine driver to function.
> It is only used to check supported keys/buttons and for testing purposes.
> If it’s not installed, you can install it using your package manager:  
> 
> ```bash
> # Debian/Ubuntu
> sudo apt install evtest
>
> # Fedora/RHEL
> sudo dnf install evtest
>
> # Arch Linux
> sudo pacman -Sy evtest
> ```
For example:

```bash
# virtual keyboard
sudo evtest /dev/input/event6

# virtual mouse 
sudo evtest /dev/input/event7

# virtual gamepad
sudo evtest /dev/input/event8

# virtual button device  
sudo evtest /dev/input/event9   
```

This will show the full list of supported input events (keys, buttons, relative movements, axes, etc.) and their corresponding keycodes, so you can verify everything is working correctly.
