movz x0, #1
movz x1, #0
movz x2, #5
movz x16, #3
svc #0
movz x0, #0
movz x16, #1
svc #0

.data
valz:"Test\n"