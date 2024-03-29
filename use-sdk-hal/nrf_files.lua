
nrf_include_files = {
	"/components", "/modules/nrfx/mdk", "/components/libraries/fifo", 
	"/components/libraries/strerror", "/components/toolchain/cmsis/include", 
	"/components/libraries/util", 
	"/components/libraries/balloc",
	"/components/libraries/ringbuf",
	"/modules/nrfx/hal",
	"/components/libraries/bsp",
	"/components/libraries/uart",
	"/components/libraries/log",
	"/modules/nrfx",
	"/components/libraries/experimental_section_vars",
	"/integration/nrfx/legacy",
	"/components/libraries/delay",
	"/integration/nrfx",
	"/components/drivers_nrf/nrf_soc_nosd",
	"/components/libraries/atomic",
	"/components/boards",
	"/components/libraries/memobj",
	"/modules/nrfx/drivers/include",
	"/external/fprintf",
	"/components/libraries/log/src",
}

nrf_src_files = {
	"/components/libraries/log/src/nrf_log_frontend.c",
	"/components/libraries/log/src/nrf_log_str_formatter.c",
	"/components/boards/boards.c",
	"/components/libraries/util/app_error.c",
	"/components/libraries/util/app_error_handler_gcc.c",
	"/components/libraries/util/app_error_weak.c",
	"/components/libraries/fifo/app_fifo.c",
	"/components/libraries/uart/app_uart_fifo.c",
	"/components/libraries/util/app_util_platform.c",
	"/components/libraries/util/nrf_assert.c",
	"/components/libraries/atomic/nrf_atomic.c",
	"/components/libraries/balloc/nrf_balloc.c",
	"/external/fprintf/nrf_fprintf.c",
	"/external/fprintf/nrf_fprintf_format.c",
	"/components/libraries/memobj/nrf_memobj.c",
	"/components/libraries/ringbuf/nrf_ringbuf.c",
	"/components/libraries/strerror/nrf_strerror.c",
	"/components/libraries/uart/retarget.c",
	"/integration/nrfx/legacy/nrf_drv_uart.c",
	"/modules/nrfx/soc/nrfx_atomic.c",
	"/modules/nrfx/drivers/src/prs/nrfx_prs.c",
	"/modules/nrfx/drivers/src/nrfx_uart.c",
	"/modules/nrfx/drivers/src/nrfx_uarte.c",
}

nrf_defines = {
	"BOARD_PCA10056",
	"BSP_DEFINES_ONLY",
	"CONFIG_GPIO_AS_PINRESET",
	"FLOAT_ABI_HARD",
	"NRF52840_XXAA",
	"__HEAP_SIZE=8192"
}

mrf_startup = {
	"/modules/nrfx/mdk/gcc_startup_nrf52840.S",
	"/modules/nrfx/mdk/system_nrf52840.c",
}