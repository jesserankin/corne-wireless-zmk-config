#!/bin/bash

# Sync QMK keymap files to qmk_firmware directory

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
QMK_KEYMAP_DIR="$HOME/qmk_firmware/keyboards/crkbd/keymaps/jesse"

# Check if qmk_firmware exists
if [ ! -d "$HOME/qmk_firmware" ]; then
    echo "Error: qmk_firmware not found at ~/qmk_firmware"
    echo "Run 'qmk setup' first to set up QMK"
    exit 1
fi

# Create keymap directory if it doesn't exist
mkdir -p "$QMK_KEYMAP_DIR"

# Copy keymap files
echo "Syncing keymap files to $QMK_KEYMAP_DIR"
cp "$SCRIPT_DIR/keymap.c" "$QMK_KEYMAP_DIR/"
cp "$SCRIPT_DIR/config.h" "$QMK_KEYMAP_DIR/"
cp "$SCRIPT_DIR/rules.mk" "$QMK_KEYMAP_DIR/"

echo "Done! You can now build with:"
echo "  qmk compile -kb crkbd/rev1 -km jesse"
