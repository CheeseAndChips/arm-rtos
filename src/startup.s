.global vector
vector:
    .word sram_end
    .word _startup 


.global _startup

.section text
_startup:
    BL c_entry
    B .
