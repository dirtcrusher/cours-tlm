// vim: set noexpandtab:tabstop=4:softtabstop=0:shiftwidth=4

#ifndef __MEM_MAPS_H__
#define __MEM_MAPS_H__

// Address mappings are defined here
// The addresses of the different sections are separated by 0x100

// Memory section:
// The memory has 2 parts: the software (not modeled here)
// and the video buffer.
#define MEMORY_START_ADDR 0

// Software section
#define MEMORY_SOFT_START_ADDR (MEMORY_START_ADDR)
#define MEMORY_SOFT_SIZE 0x2800
#define MEMORY_SOFT_END_ADDR (MEMORY_SOFT_START_ADDR + MEMORY_SOFT_SIZE)

// Video buffer section
#define MEMORY_VIDEO_START_ADDR (MEMORY_SOFT_END_ADDR + 0x100)
#define MEMORY_VIDEO_SIZE 0x15400
#define MEMORY_VIDEO_END_ADDR (MEMORY_VIDEO_START_ADDR + MEMORY_VIDEO_SIZE)

// Total memory size
#define MEMORY_TOTAL_SIZE (MEMORY_VIDEO_END_ADDR + 0x100 - MEMORY_START_ADDR)


// LCDC section:
// In order to keep the addresses more readable, we reserve 0x100 bytes even
// though we only need 12
#define LCDC_REGS_START_ADDR (MEMORY_TOTAL_SIZE + 0x100)
#define LCDC_REGS_END_ADDR (LCDC_REGS_START_ADDR + 0x100)

// ROM section:
#define ROM_START_ADDR (LCDC_REGS_END_ADDR + 0x100)
#define ROM_END_ADDR (ROM_START_ADDR + ROM_SIZE)

#endif
