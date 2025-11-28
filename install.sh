#!/usr/bin/env bash
set -e

MODULE_BASENAME="uniinputengine"
MODULE_DISPLAY="UniInputEngine"
MODULE_FILE="${MODULE_BASENAME}.ko"

echo "[*] Building ${MODULE_DISPLAY}..."
make

echo "[*] Creating module directory..."
sudo mkdir -p /lib/modules/$(uname -r)/extra

echo "[*] Installing ${MODULE_FILE}..."
sudo cp "${MODULE_FILE}" /lib/modules/$(uname -r)/extra/

echo "[*] Running depmod..."
sudo depmod -a

echo "[*] Enabling auto-load at boot..."
echo "${MODULE_BASENAME}" | sudo tee /etc/modules-load.d/${MODULE_BASENAME}.conf > /dev/null

echo "[*] Loading module now..."
if sudo modprobe "${MODULE_BASENAME}"; then
    echo "[+] modprobe succeeded"
else
    echo "[!] modprobe failed, trying insmod..."
    sudo insmod "/lib/modules/$(uname -r)/extra/${MODULE_FILE}"
fi

echo "[*] Waiting for udev to settle..."
sudo udevadm settle || true

echo "[*] Verifying module is loaded..."
if lsmod | grep -q "^${MODULE_BASENAME}"; then
    echo "[+] ${MODULE_DISPLAY} is successfully loaded"
else
    echo "[!] ${MODULE_DISPLAY} not listed in lsmod; check dmesg"
fi

echo "[*] Installation complete!"
