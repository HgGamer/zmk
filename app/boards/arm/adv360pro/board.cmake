<<<<<<< HEAD
#
# Copyright (c) 2023 The ZMK Contributors
# SPDX-License-Identifier: MIT
#

board_runner_args(nrfjprog "--nrf-family=NRF52" "--softreset")

include(${ZEPHYR_BASE}/boards/common/uf2.board.cmake)
include(${ZEPHYR_BASE}/boards/common/nrfjprog.board.cmake)
||||||| parent of a82932f0 (Adv360: official PR)
=======
#
# Copyright (c) 2022 The ZMK Contributors
# SPDX-License-Identifier: MIT
#

board_runner_args(nrfjprog "--nrf-family=NRF52" "--softreset")

include(${ZEPHYR_BASE}/boards/common/nrfjprog.board.cmake)
#include(${ZEPHYR_BASE}/boards/common/blackmagicprobe.board.cmake)
>>>>>>> a82932f0 (Adv360: official PR)
