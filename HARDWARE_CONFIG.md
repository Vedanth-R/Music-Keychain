# Final hardware configuration

This file records the Phase 2 information supplied for the physical build.

## Board and bootloader

- Product: generic nRF52840 Pro Micro replacement with USB-C charging support.
- ZMK target: `nice_nano_v2`.
- UF2 model: `nice!nano`.
- UF2 Board-ID: `nRF52840-nicenano`.
- UF2 bootloader: 0.6.0, dated June 19, 2021.
- SoftDevice reported by bootloader: S140 6.1.1.

The bootloader identity plus the product image's explicit “Nice!Nano V2
Bootloader” instruction support the `nice_nano_v2` ZMK target. The product is
still treated as a compatible clone rather than genuine nice!nano hardware.

## Final GPIO assignments

The printed `017`–`031` labels are raw nRF52840 `P0.xx` GPIOs. The authoritative
devicetree values are centralized in
`boards/shields/music_keychain/music_keychain.overlay`.

| Firmware assignment | Board label | Raw GPIO | Connected control |
|---|---:|---:|---|
| `PIN_PREVIOUS_TRACK` | `017` | `P0.17` | Back/previous switch |
| `PIN_PLAY_PAUSE` | `020` | `P0.20` | Play/pause switch |
| `PIN_NEXT_TRACK` | `022` | `P0.22` | Forward/next switch |
| `PIN_ENCODER_A` | `031` | `P0.31` | Top encoder rotation pin |
| `PIN_ENCODER_B` | `029` | `P0.29` | Bottom encoder rotation pin |
| `PIN_SOFT_POWER` | `024` | `P0.24` | Maintained software-power switch |

All three switches' other terminals share GND. The encoder's center rotation
terminal is also connected to GND. The encoder push switch is unused.

The maintained power switch uses two connections: one terminal to GND and the
other to `024`. Closed is ON. Opening the switch enters software-off; closing
it again wakes the board. Do not connect this switch in series with B+ or B-.

OFF disconnects Bluetooth, stops advertising, and enters ZMK soft-off. ON
wakes the controller; it reconnects to a stored bond if that host is available,
or becomes discoverable if it needs pairing. This is a low-power software state,
not a physical battery disconnect.

## Items to verify during the first functional test

These do not prevent flashing:

- Encoder direction is corrected for the tested physical orientation. If the
  encoder wiring changes, swap `C_VOL_UP` and `C_VOL_DN` in
  `music_keychain_keymap.dtsi` again.
- If a detent skips or emits too many changes, tune `steps` and
  `triggers-per-rotation` in `music_keychain.overlay`.
- Confirm every physical control produces its labeled action before installing
  the electronics permanently in the enclosure.

No battery-charging parameters have been changed. The product image states a
default 100 mA charge current and says connecting its `BOOST` option raises the
current to 300 mA, intended only for batteries larger than 500 mAh. Because this
project uses exactly 500 mAh, leave `BOOST` unconnected and do not modify the
charging hardware.

