	.text
	.globl	max
max:
	cmpq    %rdi, %rsi			# computing %rsi-%rdi without setting destination
	jg      second				# if the result of %rsi-%rdi is greater than 0, jump to second
	movq    %rdi, %rax			# if the result is not greater than 0 (less or equal to 0), move  value in #rdi -> %rax 
	ret							# return  the value in %rax
second:
	movq    %rsi, %rax			# move the value %rsi to %rax
	ret							# return %rax
