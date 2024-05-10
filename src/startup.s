.section .vectors
    .word _estack   /* end of stack  */
    .word _reset  /* reset handler */

.global _reset 
.section .text
_reset:
    BL c_entry
    B .
