#include <stdint.h>

extern void main(void);
void reset_handler(void)
{
	/* Copy the data segment initializers from flash to SRAM */
	uint32_t *idata_begin = &_sidata;
	uint32_t *data_begin = &_sdata;
	uint32_t *data_end = &_edata;
	while (data_begin < data_end) *data_begin++ = *idata_begin++;

	/* Zero fill the bss segment. */
	uint32_t *bss_begin = &_sbss;
	uint32_t *bss_end = &_ebss;
	while (bss_begin < bss_end) *bss_begin++ = 0;
	
	/* jump to C entry point */
	main();
}

__attribute((section(".isr_vector")))
uint32_t *isr_vectors[] = {
	0,
	(uint32_t *) reset_handler,	/* code entry point */
};