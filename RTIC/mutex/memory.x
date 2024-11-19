/* Linker script para STM32G070RBT6 */

MEMORY {
  /* NOTE K = KiBi = 1024 bytes */
  FLASH  (rx)  : ORIGIN = 0x08000000, LENGTH = 128K 
  RAM    (rwx) : ORIGIN = 0x20000000, LENGTH = 36K
}