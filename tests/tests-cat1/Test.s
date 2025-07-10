.section .text
movz x0, #1 // FD Stdout
movz x1, #0 // Offset in .data
movz x2, #8 // String Length
movz x16, #3 // Call Write From .data
svc #0 // Supervisor Call

movz x0, #0 // Exit Code 0 - Success
movz x16, #1 // Call Exit
svc #0 // Supervisor Call

.section .data
.valz "WILLIAM\n" // NULL-Padded 16-Byte Data Block