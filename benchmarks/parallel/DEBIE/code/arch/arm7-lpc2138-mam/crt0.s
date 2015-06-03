/* vector table section
 * __ASYST_STACK_TOP is defined in lcf file 
 */

.code 32  

.extern __ASYST_STACK_TOP

/* startup
 * initialize mode
 * initialize stack pointer
 * jump to __asyst_main in crt_asyst.c 
 */
 
.section .text, "ax"

.global __asyst_start

__asyst_start:
  
  mrs r0, cpsr
  bic r0, r0, #0x1F  /* clear mode flags */  
  orr r0, r0, #0x10  /* set user mode */
  msr cpsr, r0  

  /* Initialize MAM to Mode 2, 7 cycles flash: */

  ldr r0,_MAMCR
  ldr r1,_MAMTIM
  ldr r2,=0
  str r2,[r0]
  ldr r2,=7
  str r2,[r1]
  ldr r2,=2
  str r2,[r0]
  
  ldr sp,_Lstack_top
  
  bl __asyst_main


/* constants */

_MAMCR:
  .long 0xe01fc000    /* Address of MAM Control Register. */

_MAMTIM:
  .long 0xe01fc004    /* Address of MAM Timing register. */

_Lstack_top:
  .long __ASYST_STACK_TOP

.end
