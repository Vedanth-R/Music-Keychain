/* SPDX-License-Identifier: MIT */

#define DT_DRV_COMPAT zmk_behavior_immediate_soft_off

#include <zephyr/device.h>

#include <drivers/behavior.h>

#include <zmk/behavior.h>
#include <zmk/pm.h>

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

static int on_pressed(struct zmk_behavior_binding *binding,
                      struct zmk_behavior_binding_event event) {
    zmk_pm_soft_off();
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api immediate_soft_off_driver_api = {
    .binding_pressed = on_pressed,
    .locality = BEHAVIOR_LOCALITY_EVENT_SOURCE,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .get_parameter_metadata = zmk_behavior_get_empty_param_metadata,
#endif
};

#define IMMEDIATE_SOFT_OFF_INST(n)                                                                \
    BEHAVIOR_DT_INST_DEFINE(n, NULL, NULL, NULL, NULL, POST_KERNEL,                              \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                                  \
                            &immediate_soft_off_driver_api);

DT_INST_FOREACH_STATUS_OKAY(IMMEDIATE_SOFT_OFF_INST)

#endif
