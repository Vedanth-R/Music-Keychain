# nRF52840 Bluetooth Music Controller Keychain

ZMK firmware for a three-button, one-encoder BLE media remote. It sends
standard HID Consumer Control commands, so Spotify is controlled through iOS,
Android, macOS, or Windows media handling. It uses no Spotify API, credentials,
internet connection, or companion app.

> **Hardware status:** Phase 2 is configured for the reported `P0.20`, `P0.22`,
> `P0.24`, `P0.31`, and `P0.29` wiring. The board's UF2 report identifies it as
> a nice!nano-compatible nRF52840, so the build uses `nice_nano_v2`. See
> [HARDWARE_CONFIG.md](HARDWARE_CONFIG.md).

## Controls

| Physical control | HID action |
|---|---|
| Left button | Previous track (`C_PREVIOUS`) |
| Middle button | Play/Pause (`C_PLAY_PAUSE`) |
| Right button | Next track (`C_NEXT`) |
| Encoder clockwise | Volume up (`C_VOL_UP`) |
| Encoder counterclockwise | Volume down (`C_VOL_DN`) |
| Hold all three buttons for 2 seconds | Clear every bond and reopen pairing |
| Power switch toward the `006` contact | Software off; stop Bluetooth and enter low power |
| Power switch toward the `008` contact | Wake; reconnect or advertise for pairing |

The switches use ZMK's direct-GPIO scanner—there is no artificial keyboard
matrix. Internal pull-ups match switches wired from GPIO to ground. Ten
milliseconds of press and release debounce suppresses mechanical chatter. A
held key remains one HID press/release cycle rather than generating firmware
repeat events.

The three-button reset is intentionally different from an ordinary chord:
press all three within 100 ms of one another and keep all of them down for two
full seconds. Releasing early performs no action. If the buttons are not
pressed nearly together, their normal media commands may be sent instead.

## Repository layout

- `boards/shields/music_keychain/music_keychain.overlay` — final devicetree and
  the authoritative location for the five wired GPIO values.
- `boards/shields/music_keychain/music_keychain_build_test.overlay` — separate,
  plainly labeled temporary pinout used only to make compilation possible.
- `boards/shields/music_keychain/music_keychain_keymap.dtsi` — shared media
  keymap and encoder direction.
- `boards/shields/music_keychain/*.conf` — encoder and power settings.
- `src/behavior_immediate_soft_off.c` — immediate OFF handler for the maintained
  switch; it does not modify the media or Bluetooth-pairing behaviors.
- `build.yaml` — final firmware and bond-reset CI matrix.
- `.github/workflows/build.yml` — official reusable ZMK user-config workflow.

## Bluetooth behavior

ZMK provides BLE HID, secure pairing/bond storage, advertising, and automatic
reconnection. On an empty profile the controller advertises as **Music
Keychain**. Bond data survives ordinary firmware reflashing.

The maintained switch is a software power control. In the OFF position the
controller disconnects Bluetooth, stops advertising, and enters ZMK soft-off.
Moving it to ON wakes and reboots the controller. It then reconnects to its
stored device when available, or advertises as **Music Keychain** when it has
no usable bond. Allow a few seconds for waking and reconnection.

There are intentionally no Bluetooth-management key bindings because all three
physical buttons are dedicated media controls. The CI matrix also creates a
`settings_reset` artifact for clearing bonds. A host whose bond was cleared
must choose **Forget/Remove device** before that same host pairs again.

For cable-free handoff, hold all three buttons for two seconds. ZMK clears all
five stored profiles, disconnects the current host, switches to profile 0, and
advertises for a new pairing. The previous host retains a stale local entry; if
that host is used again later, forget its old **Music Keychain** entry before
pairing it again.

## Build with GitHub Actions (recommended)

1. Create an empty GitHub repository and push this folder to it.
2. Open the repository's **Actions** tab and enable workflows if prompted.
3. Run **Build ZMK firmware**, or push a commit.
4. Download the `firmware` artifact after the job succeeds.

The archive contains the normal `music-keychain` firmware and a separate
`music-keychain-settings-reset` build. Do not use the settings-reset build for
normal operation.

Prebuilt copies from the final local validation are also included in the
[`firmware`](firmware/) directory.

That directory also contains a `fallback-no-switch` recovery image. It keeps
the media controls and pairing-reset chord but disables all soft-switch
handling, providing a known recovery path while diagnosing switch hardware.

## Local development build

Install Git, CMake, Ninja, a Zephyr-compatible toolchain, Python, `west`, and
ZMK's dependencies by following the official ZMK native setup. Some Zephyr
build steps do not handle spaces in source paths reliably, so create a
space-free symbolic-link alias first. From the directory containing this
repository:

```sh
ln -s "$PWD/Music Keychain" "$PWD/music-keychain-config"
west init -m https://github.com/zmkfirmware/zmk \
  --mr v0.3 --mf app/west.yml "$PWD/zmk-workspace"
cd "$PWD/zmk-workspace"
west update
west zephyr-export
pip install -r zephyr/scripts/requirements.txt
west build -s zmk/app -b nice_nano_v2 -- \
  -DSHIELD=music_keychain \
  -DZMK_CONFIG="$PWD/../music-keychain-config/config" \
  -DZMK_EXTRA_MODULES="$PWD/../music-keychain-config"
```

If the alias already exists, do not run `ln -s` again. Toolchain setup varies
by operating system, so the GitHub build is simpler for beginners.

## Flashing with the UF2 bootloader

The buttons, encoder, and battery are not required for flashing. USB power is
enough. Put the soft-power switch in its ON (`008`) position before flashing.

1. Connect the board using a USB-C **data** cable.
2. Quickly press reset twice. The UF2 volume containing `INFO_UF2.TXT` should
   mount on the computer.
3. Copy [`music-keychain-nice_nano_v2.uf2`](firmware/music-keychain-nice_nano_v2.uf2)
   onto that volume. Do not copy the file whose name contains `settings-reset`.
4. The volume should disconnect automatically and the board should reboot. A
   macOS “disk not ejected properly” notification at this point is normal.
5. Open Bluetooth settings and pair with **Music Keychain**.

After this updated firmware is installed, future device changes do not require
USB: hold all three buttons for two seconds, then select **Music Keychain** on
the new device.

To erase an old bond, first forget **Music Keychain** on the phone/computer,
flash the `music-keychain-settings-reset` UF2 once, and then flash the normal
UF2 again before pairing.

## Customization

Media actions are in `music_keychain_keymap.dtsi`. To reverse the encoder, swap
the two values here:

```dts
sensor-bindings = <&inc_dec_kp C_VOL_UP C_VOL_DN>;
```

Encoder sensitivity is controlled by `steps` and `triggers-per-rotation` in the
overlay. The provisional values are 80 electrical steps and 20 output events
per revolution; tune them after testing the specific EC11.

## Pairing and Spotify test

1. Flash the normal firmware using the UF2 instructions above.
2. Open the host's Bluetooth settings and select **Music Keychain**.
3. Accept the pairing request. ZMK stores the bond and reconnects after power
   cycles when the host is available.
4. Start Spotify and test previous, play/pause, next, and both volume directions.

These commands are operating-system media commands. Spotify need not be in the
foreground, though exact background behavior is controlled by the host OS.

## Power behavior

The SPDT switch's center terminal connects to GND. Its two outer terminals
connect to board pads `006` and `008`:

- Switch connected to `006`: OFF. Bluetooth disconnects and the controller
  enters software-off low power.
- Switch connected to `008`: ON. The controller wakes, then reconnects to its
  bonded host or advertises if it needs pairing.

If those directions are physically backward on the installed switch, swap the
two outside wires; leave the center wire on GND. This is not a hard battery
disconnect: the battery stays attached and a very small standby current
remains. USB can also continue to power the board.

After 30 seconds without activity ZMK enters idle while retaining a responsive
BLE connection. After 15 minutes it enters deep sleep and disconnects BLE. Any
of the three buttons can wake it because the direct key scanner is marked as a
wake source; reconnection can take a few seconds.

The EC11 sensor itself is not configured as a system-off wake source. Turning
the knob alone may therefore not wake the controller from deep sleep—press a
media button first. This tradeoff avoids continuous activity and favors battery
life. Actual runtime cannot be predicted until the clone's regulator, charger,
LEDs, and sleep current are measured. No battery-charging parameters are
changed by this project.

The product listing states that its default charge current is 100 mA and that a
hardware `BOOST` option raises it to 300 mA for batteries larger than 500 mAh.
This project uses a 500 mAh cell: leave `BOOST` unconnected. Firmware does not
change the charger configuration.

## Troubleshooting

- **Not visible in Bluetooth:** wake with a button and wait several seconds. If
  previously bonded, remove stale bonds on both sides before pairing again.
- **Switch does the opposite of its label:** swap only the `006` and `008`
  outside wires; keep the center terminal connected to GND.
- **Board immediately turns off after flashing:** move the switch to the `008`
  ON position and press reset once.
- **Connected but controls do nothing:** confirm the host accepts Bluetooth HID
  keyboards and test with its native media player before Spotify.
- **Encoder runs backward:** swap `C_VOL_UP` and `C_VOL_DN` in the shared keymap.
- **Encoder skips or sends too many events:** tune `steps` and
  `triggers-per-rotation` for the exact encoder.
- **No wake on knob turn:** expected in deep sleep; wake with a button.
- **UF2 volume does not appear:** confirm the cable supports data and quickly
  activate reset twice rather than once.

## Validation status

| Area | Status |
|---|---|
| Repository structure and source review | Implemented |
| Media bindings, direct keys, debounce, encoder, BLE defaults, sleep config | Implemented |
| Two-second all-button bond reset and pairing handoff | Implemented and compile-tested |
| SPDT software-off and dedicated wake input | Implemented and compile-tested |
| Temporary-pin firmware and settings-reset builds | Both compile-tested to UF2 |
| Final `P0.xx` pin configuration | Configured and compile-tested to UF2 |
| GitHub Actions workflow and YAML | Syntax-checked; not run without a GitHub remote |
| Board/bootloader identification and final pin mapping | Supplied and configured |
| Flashing and operation on the physical device | Not tested |

## Official references

- [ZMK user setup](https://zmk.dev/docs/user-setup)
- [Encoder support](https://zmk.dev/docs/features/encoders)
- [Sensor rotation behavior](https://zmk.dev/docs/keymaps/behaviors/sensor-rotate)
- [Bluetooth behavior](https://zmk.dev/docs/features/bluetooth)
- [Power management](https://zmk.dev/docs/config/power)
- [Low-power states and wake sources](https://zmk.dev/docs/features/low-power-states)
- [Soft-off behavior](https://zmk.dev/docs/keymaps/behaviors/soft-off)
- [Soft-off hardware integration](https://zmk.dev/docs/hardware-integration/soft-off-setup)

