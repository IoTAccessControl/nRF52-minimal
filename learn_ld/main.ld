MEMORY
{
  rom      (rx)  : ORIGIN = 0x00000000, LENGTH = 0x00040000
  ram      (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
}

STACK_SIZE = 0x2000; /* 8 kB */

SECTIONS
{
  .text :
  {
    KEEP(*(.isr_vector))
    *(.text)
  } > rom

  .bss (NOLOAD) :
  {
      *(.bss*)
      *(COMMON)
  } > ram
  .stack (NOLOAD) :
  {
      . = ALIGN(8);
      . = . + STACK_SIZE;
      . = ALIGN(8);
  } > ram
}