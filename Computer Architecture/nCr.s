
.globl nCr
        .type   nCr, @function
nCr:
	pushl %ebp
	movl %esp,%ebp
	pushl %ebx
	pushl %ecx
	pushl %edx
	subl $12,%esp
	movl 8(%ebp), %ebx
	movl 12(%ebp),%ecx
	movl %ebx, -12(%ebp)
	subl %ecx, -12(%ebp)
L1://n!
	movl %ebx,(%esp)
	call Factorial
	cmp $0,%eax
	je overflows
	movl %eax,-20(%ebp)
L2://r!
	movl %ecx,(%esp)
	call Factorial
	cmp $0, %eax
	je overflows
	movl %eax,%ecx
A1: //N = Factorial(n)/Factorial(r)
	movl -20(%ebp),%eax
	cdq
	idivl %ecx
	movl %eax, -16(%ebp)
L3:
	movl -12(%ebp),%edx
	movl %edx,(%esp)
	call Factorial
	cmp $0,%eax
	je overflows
	movl %eax,-12(%ebp)
A2:
//N /= Factorial(n-r)
	movl -16(%ebp),%eax
	cdq
	idivl -12(%ebp)
	jmp ends
overflows:
	movl $0,%eax
ends:
	addl $8,%esp
	popl %edx
	popl %ecx
	popl %ebx
	movl %ebp,%esp
	popl %ebp
	ret

.globl Factorial
        .type   Factorial, @function
Factorial:
	pushl %ebp
	movl %esp,%ebp
	pushl %ebx
	movl 8(%ebp),%ebx
	movl $1,%eax
	cmpl $1,%ebx
	jle end
fact:
	imull %ebx,%eax
	jo overflow
	decl %ebx
	cmp $1,%ebx
	je end
	call fact
overflow:
	movl $0,%eax
end:
	popl %ebx
	movl %ebp,%esp
	popl %ebp
	ret

