# QMK Firmware for Corne (Elite-C)

QMK keymap for wired Corne keyboard with Elite-C controllers, matching the ZMK wireless config.

## Files

| File       | Purpose                                      |
|------------|----------------------------------------------|
| keymap.c   | Layer definitions and custom key behaviors   |
| config.h   | Tapping term, combo settings, RGB config     |
| rules.mk   | Enable features (combos, RGB matrix)         |

## Quick Start

If QMK is already set up:

```bash
# Sync files to qmk_firmware
./sync.sh

# Build
qmk compile -kb crkbd/rev1 -km jesse

# Flash (run for each half)
qmk flash -kb crkbd/rev1 -km jesse -bl dfu
```

## Full Setup Guide

### 1. Install QMK CLI

**Arch Linux:**
```bash
yay -S qmk
# or
sudo pacman -S qmk
```

**Other distros:**
```bash
python3 -m pip install --user qmk
```

### 2. Set Up QMK Environment

```bash
# Clone QMK firmware and set up environment
qmk setup

# Install udev rules (Linux - needed for flashing)
sudo cp ~/qmk_firmware/util/udev/50-qmk.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger

# Add yourself to required groups (may need logout/login)
sudo usermod -aG dialout $USER
sudo usermod -aG uucp $USER
```

### 3. Verify Setup

```bash
qmk doctor
```

All checks should pass. Install any missing dependencies it reports.

### 4. Sync Keymap Files

```bash
cd /path/to/corne-wireless-zmk-config/qmk
./sync.sh
```

This copies the keymap files to `~/qmk_firmware/keyboards/crkbd/keymaps/jesse/`.

### 5. Build Firmware

```bash
qmk compile -kb crkbd/rev1 -km jesse
```

Output: `~/qmk_firmware/crkbd_rev1_jesse.hex`

### 6. Flash Firmware

**Elite-C uses DFU bootloader** (not Caterina like regular Pro Micros).

```bash
qmk flash -kb crkbd/rev1 -km jesse -bl dfu
```

When prompted:
1. **Single press** the reset button on the Elite-C
2. Wait for flashing to complete
3. Repeat for the other half

Both halves use the same firmware.

## Keymap Overview

### Layers

| Layer | Name       | Activation                        | LED Color  |
|-------|------------|-----------------------------------|------------|
| 0     | Default    | Base layer                        | Dim white  |
| 1     | Raise      | Hold inner thumb (ESC/TAB)        | Green      |
| 2     | Lower      | Hold middle left thumb (Space)    | Orange     |
| 3     | Win Left   | Hold B for 150ms                  | Blue       |
| 4     | Win Right  | Hold N for 150ms                  | Blue       |
| 5     | Win Num    | B + inner left thumb (combo)      | Cyan       |

### Key Features

- **Win Layers (B/N hold)**: Hold LGUI + activate layer for Hyprland bindings
  - Only activates after 150ms tapping term (fast typing won't trigger)
  - Tap B/N normally to type the letter

- **Win Num Combo**: Press B + inner left thumb simultaneously for instant LGUI + numbers layer (no 150ms delay)

- **Mod-Tap Keys**:
  - C = Tap: C, Hold: Alt
  - V = Tap: V, Hold: Ctrl
  - M = Tap: M, Hold: Ctrl
  - , = Tap: comma, Hold: Alt

- **Layer-Tap Thumbs**:
  - Left inner: Tap: ESC, Hold: Raise
  - Left middle: Tap: Space, Hold: Lower
  - Right inner: Tap: TAB, Hold: Raise
  - Right middle: Tap: Space, Hold: Shift

### LED Layer Indication

LEDs change color based on active layer:
- **Dim white**: Default layer
- **Green**: Raise layer (symbols/numbers)
- **Orange**: Lower layer (F-keys/navigation)
- **Blue**: Win layers (SUPER held)
- **Cyan**: Win+Num layer (SUPER + numbers)

## Troubleshooting

### Keyboard not detected when flashing

1. Check udev rules are installed
2. Verify you're in `dialout` and `uucp` groups
3. Try a different USB cable/port
4. Make sure you're using `-bl dfu` for Elite-C

### Keys not working as expected

1. Make sure you flashed both halves
2. Check tapping term (150ms) - hold keys longer for hold behavior
3. Verify the correct side is connected to USB (either works as master)

### LEDs not changing color

1. RGB Matrix must be enabled in rules.mk
2. Colors only change when layer actually activates (after tapping term for B/N)

## Modifying the Keymap

1. Edit files in this directory
2. Run `./sync.sh` to copy to qmk_firmware
3. Compile and flash

See [QMK Documentation](https://docs.qmk.fm/) for keycode reference and advanced features.
