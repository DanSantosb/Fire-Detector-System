MEMORY {
  RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 256M
}

SECTIONS {
  .vectors : {
    *(.vectors)
  } > RAM

  .text : {
    *(.text)
  } > RAM

  .data : {
    *(.data)
  } > RAM

  .bss : {
    __bss_start = .;
    *(.bss)
    __bss_end = .;
  } > RAM

  .stack : {
    . = ALIGN(8);
    _svc_stack = .;
    . += 1024;
    _svc_stack_end = .;

    _irq_stack = .;
    . += 1024;
    _irq_stack_end = .;
  } > RAM
}
