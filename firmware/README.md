# Prebuilt UF2 files

## Normal firmware

`music-keychain-nice_nano_v2.uf2`

SHA-256:
`f2e5d94562dec01ffa41af2af78781b09aa9997271ec7eb805f9fd2805b8c216`

Flash this file for normal Bluetooth media-controller operation.

It includes the two-second, all-three-button pairing reset and the SPDT
software-power control: center to GND, OFF outer terminal to `006`, and ON/wake
outer terminal to `008`.

## Bond-reset firmware

`music-keychain-settings-reset-nice_nano_v2.uf2`

SHA-256:
`c129e8669c0b442ac94d1b556946352269874bf1a5002e72bb929a2c53b7dba9`

This is a maintenance image, not the normal controller firmware. Flash it once
to clear ZMK settings, then immediately flash the normal firmware again.

Both files were built from this repository using ZMK v0.3 and the
`nice_nano_v2` target after the final GPIO configuration was applied.

Both files include the final P0.06/P0.08 soft-power hardware configuration.

## Recovery firmware without switch handling

`music-keychain-fallback-no-switch-nice_nano_v2.uf2`

SHA-256:
`e2f4d722397795a0cdf49fe9dac99859d0b417b2a3b1d7510e15d54a75a0cfe4`

This recovery image disables all soft-power triggers while preserving the
three media buttons, encoder, Bluetooth, and two-second bond-reset chord. Use
it to restore the controller if the physical switch circuit needs diagnosis.

