.code 32

.section .vectors, "ax"

  B	__asyst_start         /* RESET INTERRUPT */
  B	__DefaultHandler      /* UNDEFINED INSTRUCTION INTERRUPT */
  B	__DefaultHandler      /* SOFTWARE INTERRUPT */
  B	__DefaultHandler      /* ABORT (PREFETCH) INTERRUPT */
  B	__DefaultHandler      /* ABORT (DATA) INTERRUPT */
  B	__DefaultHandler      /* RESERVED */
  B	__DefaultHandler      /* IRQ INTERRUPT */
  B	__DefaultHandler      /* FIQ INTERRUPT */

.text

__DefaultHandler:
  b  __DefaultHandler

.end
