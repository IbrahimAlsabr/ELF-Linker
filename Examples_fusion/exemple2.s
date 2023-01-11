.global main
.global loop
.global other
.text
main:
    bl other
    mov r0, #5
    b loop
