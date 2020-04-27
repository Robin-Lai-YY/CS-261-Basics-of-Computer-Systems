	.text
	.p2align 4,,15
	.globl	arraymax
arraymax:
	movabsq $-9223372036854775808, %rax		# move really smally value to %rax
	testq	%rsi, %rsi						# %rsi & %rsi but not store the result back to register
	jle	.Ldone								# if the last result is less or equal than 0, jump tp .Ldone
	dec     %rsi	
.Lloop:
	movq    (%rdi,%rsi,8), %rcx				# %rcx = MEM[%rdi + %rsi*8]
	cmpq	%rcx, %rax						# sub %rax - %rcx
	jge     .Lskip							# if the last result is greater or equal to 0, jump to ,Lskip
	movq    %rcx, %rax						# if not, mov % the value in %rcx to %rax
.Lskip:			
	dec     %rsi							# %rsi--
	jge	.Lloop								# if the last result is greater or equal to 0, go back to ,Lloop
.Ldone:
	rep										# return
	ret										# return