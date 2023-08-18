.data
output: .ascii "hello world\n"
.text
.globl  _start
_start:
    movq $4, %rax
    movq $1, %rbx
    movq $output, %rcx
    movq $12, %rdx
    int $0x80
    movq $1, %rax
    int $0x80
