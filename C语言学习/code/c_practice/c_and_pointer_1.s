	.file	"c_and_pointer_1.c"
	.section .rdata,"dr"
.LC0:
	.ascii "Original input : %s\12\0"
.LC1:
	.ascii "Reaarange line: %s\12\0"
	.text
	.globl	main_2
	.def	main_2;	.scl	2;	.type	32;	.endef
	.seh_proc	main_2
main_2:
	pushq	%rbp
	.seh_pushreg	%rbp
	subq	$2144, %rsp
	.seh_stackalloc	2144
	leaq	128(%rsp), %rbp
	.seh_setframe	%rbp, 128
	.seh_endprologue
	leaq	1920(%rbp), %rax
	movl	$20, %edx
	movq	%rax, %rcx
	call	read_column_numbers
	movl	%eax, 2012(%rbp)
	jmp	.L2
.L3:
	leaq	912(%rbp), %rax
	movq	%rax, %rdx
	leaq	.LC0(%rip), %rcx
	call	printf
	leaq	1920(%rbp), %r8
	movl	2012(%rbp), %ecx
	leaq	912(%rbp), %rdx
	leaq	-96(%rbp), %rax
	movq	%r8, %r9
	movl	%ecx, %r8d
	movq	%rax, %rcx
	call	rearrange
	leaq	-96(%rbp), %rax
	movq	%rax, %rdx
	leaq	.LC1(%rip), %rcx
	call	printf
.L2:
	leaq	912(%rbp), %rax
	movq	%rax, %rcx
	call	gets
	testq	%rax, %rax
	jne	.L3
	movl	$0, %eax
	addq	$2144, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC2:
	.ascii "%d\0"
	.align 8
.LC3:
	.ascii "Last column number is not paired.\0"
	.text
	.globl	read_column_numbers
	.def	read_column_numbers;	.scl	2;	.type	32;	.endef
	.seh_proc	read_column_numbers
read_column_numbers:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movl	%edx, 24(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L6
.L8:
	addl	$1, -4(%rbp)
.L6:
	movl	-4(%rbp), %eax
	cmpl	24(%rbp), %eax
	jge	.L7
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	16(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, %rdx
	leaq	.LC2(%rip), %rcx
	call	scanf
	cmpl	$1, %eax
	jne	.L7
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	16(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jns	.L8
.L7:
	movl	-4(%rbp), %eax
	andl	$1, %eax
	testl	%eax, %eax
	je	.L11
	leaq	.LC3(%rip), %rcx
	call	puts
	movl	$1, %ecx
	call	exit
.L11:
	call	getchar
	movl	%eax, -8(%rbp)
	cmpl	$-1, -8(%rbp)
	je	.L10
	cmpl	$10, -8(%rbp)
	jne	.L11
.L10:
	movl	-4(%rbp), %eax
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	rearrange
	.def	rearrange;	.scl	2;	.type	32;	.endef
	.seh_proc	rearrange
rearrange:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movl	%r8d, 32(%rbp)
	movq	%r9, 40(%rbp)
	movq	24(%rbp), %rax
	movq	%rax, %rcx
	call	strlen
	movl	%eax, -16(%rbp)
	movl	$0, -8(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L14
.L17:
	movl	-4(%rbp), %eax
	cltq
	addq	$1, %rax
	leaq	0(,%rax,4), %rdx
	movq	40(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rcx
	movq	40(%rbp), %rax
	addq	%rcx, %rax
	movl	(%rax), %eax
	subl	%eax, %edx
	movl	%edx, %eax
	addl	$1, %eax
	movl	%eax, -12(%rbp)
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	40(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	cmpl	%eax, -16(%rbp)
	jle	.L18
	cmpq	$999, 16(%rbp)
	je	.L18
	movl	-8(%rbp), %edx
	movl	-12(%rbp), %eax
	addl	%edx, %eax
	cmpl	$999, %eax
	jle	.L16
	movl	$999, %eax
	subl	-8(%rbp), %eax
	movl	%eax, -12(%rbp)
.L16:
	movl	-12(%rbp), %eax
	movslq	%eax, %rcx
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	40(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	movq	24(%rbp), %rax
	addq	%rax, %rdx
	movl	-8(%rbp), %eax
	movslq	%eax, %r8
	movq	16(%rbp), %rax
	addq	%r8, %rax
	movq	%rcx, %r8
	movq	%rax, %rcx
	call	strncpy
	movl	-12(%rbp), %eax
	addl	%eax, -8(%rbp)
	addl	$2, -4(%rbp)
.L14:
	movl	-4(%rbp), %eax
	cmpl	32(%rbp), %eax
	jl	.L17
.L18:
	nop
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (x86_64-posix-seh, Built by strawberryperl.com project) 7.1.0"
	.def	printf;	.scl	2;	.type	32;	.endef
	.def	gets;	.scl	2;	.type	32;	.endef
	.def	scanf;	.scl	2;	.type	32;	.endef
	.def	puts;	.scl	2;	.type	32;	.endef
	.def	exit;	.scl	2;	.type	32;	.endef
	.def	getchar;	.scl	2;	.type	32;	.endef
	.def	strlen;	.scl	2;	.type	32;	.endef
	.def	strncpy;	.scl	2;	.type	32;	.endef
