.equ CPSR_I,   0x80
.equ CPSR_F,   0x40
.equ CPSR_IRQ, 0x12
.equ CPSR_SVC, 0x13

.section .vectors, "ax"
.global _start
.global _vector_table

_vector_table:
    ldr pc, =_start      // Reset
    ldr pc, =undef_handler
    ldr pc, =swi_handler
    ldr pc, =pabt_handler
    ldr pc, =dabt_handler
    nop
    ldr pc, =irq_handler // IRQ
    ldr pc, =fiq_handler // FIQ

.section .text
_start:
    // Desabilita modo V=1 (endereços altos para vetor de interrupção)
    mrc p15, 0, r0, c1, c0, 0
    bic r0, r0, #(1 << 13)
    mcr p15, 0, r0, c1, c0, 0

    // Habilita IRQ e FIQ (limpa bits I e F)
    mrs r0, cpsr
    bic r0, r0, #(CPSR_I | CPSR_F)
    msr cpsr_c, r0

    // Configura VBAR
    ldr r0, =_vector_table
    mcr p15, 0, r0, c12, c0, 0

    // Inicializa pilhas
    msr cpsr_c, #CPSR_SVC
    ldr sp, =_svc_stack_end

    msr cpsr_c, #CPSR_IRQ
    ldr sp, =_irq_stack_end

    msr cpsr_c, #CPSR_SVC

    // Limpa seção BSS
    ldr r0, =__bss_start
    ldr r1, =__bss_end
    mov r2, #0
bss_clear:
    cmp r0, r1
    strlt r2, [r0], #4
    blt bss_clear

    bl main

loop:
    b loop

// Handlers padrões
undef_handler: b undef_handler
swi_handler:   b swi_handler
pabt_handler:  b pabt_handler
dabt_handler:  b dabt_handler
fiq_handler:   b fiq_handler

irq_handler:
    sub lr, lr, #4
    srsdb sp!, #0x13
    cps #0x13
    push {r0-r12, lr}
    bl ISR_Handler
    pop {r0-r12, lr}
    rfeia sp!
