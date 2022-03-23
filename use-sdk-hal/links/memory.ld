
MEMORY
{
  shell_app_flash (rx) : ORIGIN = 0x00000, LENGTH = 0x04000
  shell_app_ram (rwx) :  ORIGIN = 0x20000000, LENGTH = 0x00030000
  test_lib_flash (rx) : ORIGIN = 0x04000, LENGTH = 0x04000
  test_app_ram (rwx) :  ORIGIN = 0x20030000, LENGTH = 0x20000
}

test_lib_hal_flash_addr = ORIGIN(test_lib_flash) + 0x00500;
test_lib_data_flash_addr = ORIGIN(test_lib_flash) + 0x1000;
test_lib_data_ram_addr = ORIGIN(test_app_ram);