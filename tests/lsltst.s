.section .text
movz x1, #65
movk x1, #65, lsl #16
movz x0, #1
movz x2, #2
movz x16, #4
svc #0
movz x0, #0
movz x16, #1
svc #0