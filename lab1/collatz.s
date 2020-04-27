	.text
	.globl	collatz
collatz:
	xorq %rsi, %rsi                 # %rsi -> count = 0
    cmpq $1, %rdi                   # result = %rdi - 1
    je .Lzero

.Lloop: 
    cmpq $1, %rdi                   # result = %rdi - 1
    je .Ldone
    testq $1, %rdi                  # determine whether it is even or odd
    je .Leven
    jmp .Lodd

.Leven:
    inc %rsi                        # %rsi++
    shrq $1, %rdi                   # %rdi = %rdi / 2
    jmp .Lloop

.Lodd:
    inc %rsi                        # %rsi++
    imulq $3, %rdi                  # %rdi = %rdi * 3
    addq $1, %rdi                   # %rdi = %rdi + 1
    jmp .Lloop

.Lzero:
    movq $0, %rax                   # set to 0
    jmp .Ldone

.Ldone:
    movq %rsi, %rax                 # store the return value
    rep
    ret                             # return