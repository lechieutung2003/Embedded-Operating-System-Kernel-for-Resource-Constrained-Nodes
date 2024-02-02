# Porting Guide

## Adapting to a new Cortex-M MCU
This kernel is currently targeted towards Cortex-M3 (e.g., `lm3s6965evb` on QEMU) but can be easily adapted to M4 or M7.

1. **Update Linker Script**: Modify `linker/linker.ld` to match the physical Flash and RAM addresses of your target MCU.
2. **Update Vector Table**: Add target-specific external interrupt handlers in `startup/startup.s` if you need them.
3. **FPU Support**: If porting to Cortex-M4F or M7, modify `context_switch.s` to save and restore the FPU registers (`s16-s31`) and utilize the lazy stacking feature.
4. **Clock Configuration**: In `kernel/src/kernel.c`, update `CPU_FREQ` to match your MCU's configured clock speed to ensure `task_delay` timings are accurate.
