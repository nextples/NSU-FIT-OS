.data

msg:
	.utf8 "Hello world!\n"
	len = . - msg

.text
	.global _start

_start:
	movq $1, %rax
	movq $1, %rdi
	movq $msg, %rsi
	movq $len, %rdx
	syscall

	movq $60, %rax
	xorq %rdi, %rdi
	syscall