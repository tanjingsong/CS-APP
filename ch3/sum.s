	.file	"sum.c"
	.text
	.p2align 4,,15
	.globl	sum
	.type	sum, @function
sum:
.LFB24:
	.cfi_startproc
	movl	8(%esp), %eax
	addl	4(%esp), %eax
	addl	%eax, accum
	ret
	.cfi_endproc
.LFE24:
	.size	sum, .-sum
	.globl	accum
	.bss
	.align 4
	.type	accum, @object
	.size	accum, 4
accum:
	.zero	4
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
