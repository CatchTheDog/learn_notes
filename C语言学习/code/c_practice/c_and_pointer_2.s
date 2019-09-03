	.file	"c_and_pointer_2.c"
	.text
	.globl	main_4
	.def	main_4;	.scl	2;	.type	32;	.endef
	.seh_proc	main_4
main_4:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$112, %rsp
	.seh_stackalloc	112
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movabsq	$7021788497383006275, %rax
	movq	%rax, -64(%rbp)
	movq	$11885, -56(%rbp)
	movq	$0, -48(%rbp)
	movq	$0, -40(%rbp)
	movq	$0, -32(%rbp)
	movq	$0, -24(%rbp)
	movw	$0, -16(%rbp)
	movabsq	$7956012606654128195, %rax
	movq	%rax, -77(%rbp)
	movl	$779249012, -69(%rbp)
	movb	$0, -65(%rbp)
	leaq	-64(%rbp), %rax
	movq	%rax, %rcx
	call	stringx
	leaq	-64(%rbp), %rax
	movl	$112, %edx
	movq	%rax, %rcx
	call	stringchr
	nop
	addq	$112, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	stringcat
	.def	stringcat;	.scl	2;	.type	32;	.endef
	.seh_proc	stringcat
stringcat:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movq	24(%rbp), %rax
	movq	%rax, %rdx
	movq	16(%rbp), %rcx
	call	strcat
	movq	16(%rbp), %rcx
	call	puts
	nop
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	stringcpy
	.def	stringcpy;	.scl	2;	.type	32;	.endef
	.seh_proc	stringcpy
stringcpy:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movq	24(%rbp), %rax
	movq	%rax, %rdx
	movq	16(%rbp), %rcx
	call	strcpy
	movq	16(%rbp), %rcx
	call	puts
	nop
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	stringncpy
	.def	stringncpy;	.scl	2;	.type	32;	.endef
	.seh_proc	stringncpy
stringncpy:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movq	24(%rbp), %rax
	movl	$3, %r8d
	movq	%rax, %rdx
	movq	16(%rbp), %rcx
	call	strncpy
	movq	16(%rbp), %rcx
	call	puts
	nop
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
	.align 8
.LC0:
	.ascii "\327\326\267\373\264\256a:%s\323\353\327\326\267\373\264\256b:%s\261\310\275\317\275\341\271\373\243\272%d\12\0"
	.text
	.globl	stringcmp
	.def	stringcmp;	.scl	2;	.type	32;	.endef
	.seh_proc	stringcmp
stringcmp:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	%rdx, 24(%rbp)
	movq	24(%rbp), %rax
	movq	%rax, %rdx
	movq	16(%rbp), %rcx
	call	strcmp
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %edx
	movq	24(%rbp), %rax
	movl	%edx, %r9d
	movq	%rax, %r8
	movq	16(%rbp), %rdx
	leaq	.LC0(%rip), %rcx
	call	printf
	nop
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC1:
	.ascii "\327\326\267\373\264\256a\243\272%s \263\244\266\310\316\252\243\272%d \12\0"
	.text
	.globl	stringx
	.def	stringx;	.scl	2;	.type	32;	.endef
	.seh_proc	stringx
stringx:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	16(%rbp), %rcx
	call	strlen
	movq	%rax, %r8
	movq	16(%rbp), %rdx
	leaq	.LC1(%rip), %rcx
	call	printf
	nop
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
	.align 8
.LC2:
	.ascii "\327\326\267\373'%c'\324\332\327\326\267\373\264\256\"%s\"\326\320\265\304\316\273\326\303\312\307\243\272%p\0"
	.text
	.globl	stringchr
	.def	stringchr;	.scl	2;	.type	32;	.endef
	.seh_proc	stringchr
stringchr:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movl	%edx, %eax
	movb	%al, 24(%rbp)
	movsbl	24(%rbp), %eax
	movl	%eax, %edx
	movq	16(%rbp), %rcx
	call	strchr
	movq	%rax, -8(%rbp)
	movsbl	24(%rbp), %eax
	movq	-8(%rbp), %rdx
	movq	%rdx, %r9
	movq	16(%rbp), %r8
	movl	%eax, %edx
	leaq	.LC2(%rip), %rcx
	call	printf
	nop
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (x86_64-posix-seh, Built by strawberryperl.com project) 7.1.0"
	.def	strcat;	.scl	2;	.type	32;	.endef
	.def	puts;	.scl	2;	.type	32;	.endef
	.def	strcpy;	.scl	2;	.type	32;	.endef
	.def	strncpy;	.scl	2;	.type	32;	.endef
	.def	strcmp;	.scl	2;	.type	32;	.endef
	.def	printf;	.scl	2;	.type	32;	.endef
	.def	strlen;	.scl	2;	.type	32;	.endef
	.def	strchr;	.scl	2;	.type	32;	.endef
