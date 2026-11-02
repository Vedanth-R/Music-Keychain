# Prebuilt UF2 files

## Normal firmware

`music-keychain-nice_nano_v2.uf2`

SHA-256:
`e96801b996d554071ffd1d92270dd7cc91ce1c3d2c89f53d7af675a04b65d5fd`

Flash this file for normal Bluetooth media-controller operation.

It includes the two-second, all-three-button pairing reset and the SPDT
software-power control: center to GND, OFF outer terminal to `006`, and ON/wake
outer terminal to `008`.

## Bond-reset firmware

`music-keychain-settings-reset-nice_nano_v2.uf2`

SHA-256:
`446cc9e96048acea5feea48a6570816a4522a4871a79ed788b942e8e943eb8e7`

This is a maintenance image, not the normal controller firmware. Flash it once
to clear ZMK settings, then immediately flash the normal firmware again.

Both files were built from this repository using ZMK v0.3 and the
`nice_nano_v2` target after the final GPIO configuration was applied.

Both files include the final P0.06/P0.08 soft-power hardware configuration.

