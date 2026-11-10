# Prebuilt UF2 files

## Normal firmware

`music-keychain-nice_nano_v2.uf2`

SHA-256:
`5602bfa112d1d627af19a5110f2a1dd7791ef5df94e899bbfccc2ff6f414c477`

Flash this file for normal Bluetooth media-controller operation.

It includes the two-second, all-three-button pairing reset and the maintained
software-power switch between `024` and GND. Closed is ON; open is OFF.

## Bond-reset firmware

`music-keychain-settings-reset-nice_nano_v2.uf2`

SHA-256:
`5ce43ff88e2e17258be6054c6466f4b13ff7efedaedc9d31d718ad1f3eacd4b1`

This is a maintenance image, not the normal controller firmware. Flash it once
to clear ZMK settings, then immediately flash the normal firmware again.

Both files were built from this repository using ZMK v0.3 and the
`nice_nano_v2` target after the final GPIO configuration was applied.

Both files include the final P0.24 soft-power hardware configuration.

## Recovery firmware without switch handling

`music-keychain-fallback-no-switch-nice_nano_v2.uf2`

SHA-256:
`22cd50db64ef00143bdbcf5ed68fc6120517a7055b14d0e2703808857f1b04f4`

This recovery image disables all soft-power triggers while preserving the
three media buttons, encoder, Bluetooth, and two-second bond-reset chord. Use
it to restore the controller if the physical switch circuit needs diagnosis.

