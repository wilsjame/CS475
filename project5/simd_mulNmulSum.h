/* SIMD functions for mul and mulSum--------------------------------------------------------------------------------

X86-64 has sixteen (almost) general purpose 64-bit integer registers:
%rax	%rbx	%rcx	%rdx	%rsi	%rdi	%rbp	%rsp	%r8	%r9	%r10	%r11	%r12	%r13	%r14	%r15

$ - used for immediate values % - used to indicate registers 

Note that the assembly GCC output is in AT&T syntax, which:
∗    uses "%" in front of register names (not $ as in MIPS assembly)
∗    has the destination is always on the right (e.g. movupd source, destination)
∗    an operand with parenthesis represents a memory location:
       –    (%rax) uses %rax as a pointer
       –    8(%rax) uses %rax as a base and 8 as an offset
       –    (%rax,%rbx) adds %rax and %rbx to form a memory address
       –    (%rax,%rbx,4) adds %rax and 4 times %rbx to form a memory address
∗    "leaq 8(%rax), %rbx" loads address of 8(%rax), i.e. %rax+8, into %rbx
∗    %rsp is the stack pointer; %rbp is the frame pointer
∗    %rXX registers are 64-bit; %eXX represent the lower 32 bits of them
∗    addq and addl are 64- and 32-bit versions of add respectively

*There is a lot to keep track of here: the arguments given to the function, the information necessary to return, 
and space for local computations. For this purpose, we use the base register pointer %rbp.

*The called function may use any registers, but it must restore the values of the registers %rbx, %rbp, %rsp, 
and %r12-%r15, if it changes them.

=>A base-relative value is given by adding a constant to the name of a register. 
For example, -16(%rcx) refers to the value at the memory location sixteen bytes below the address indicated 
by %rcx. This mode is important for manipulating stacks, local values, and function parameters.

=>the base pointer (%rbp) locates the start of the stack frame.
 The arguments to the function follow the base pointer, so argument zero is at -8(%rbp), argument one at -16(%rbp), and so forth. 
 Past those are local variables to the function at -32(%rbp) and then saved registers at -48(%rbp). 
 The stack pointer points to the last item on the stack. 

 =>There are actually two flavors of the SSE move instruction: "movups" moves a value between *unaligned* addresses 
 (not a multiple of 16);
  "movaps" moves a value between *aligned* addresses.  The aligned move is substantially faster, 
  but it will segfault if the address you give isn't a multiple of 16!  
 

 Conditions of the .globl function (situated somewhere):
for SimdMul function
old %rbp register	(%rbp)	<-- %rbp points here
argument 0			-24(%rbp)	
argument 1			-32(%rbp)	
argument 2			-40(%rbp)	

for SimdMulSum function
argument 0			-40(%rbp)	
argument 1			-48(%rbp)	
argument 2			-32(%rbp)	

References:
1. https://www3.nd.edu/~dthain/courses/cse40243/fall2017/intel-intro.html
2. https://inst.eecs.berkeley.edu/~cs61c/sp11/labs/08/
3. http://www.godevtool.com/TestbugHelp/XMMfpins.htm
-----------------------------------------------------------------------------------------------------------------
 */

//function which accepts 4 args to perform SIMD multiplication
void
SimdMul( float *a, float *b,   float *c,   int len ) 
{
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;

	//load each arg into a scratch register
	__asm
	(
		".att_syntax\n\t"
		"movq    -24(%rbp), %rbx\n\t"		// a | move arg0 value pointed by -24(%rbp) into rbx
		"movq    -32(%rbp), %rcx\n\t"		// b | move arg1 value pointed by -32(%rbp) into rcx
		"movq    -40(%rbp), %rdx\n\t"		// c | move arg2 value pointed by -40(%rbp) into rdx
	);

	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		__asm
		(
			".att_syntax\n\t"
			"movups	(%rbx), %xmm0\n\t"	// load the first sse register from scratch register %rbx (moves the value pointed by rbx into xxm0)
			"movups	(%rcx), %xmm1\n\t"	// load the second sse register from the scratch register %rcx
			"mulps	%xmm1, %xmm0\n\t"	// do the multiply
			"movups	%xmm0, (%rdx)\n\t"	// store the result into the %rdx register 
			//(restore the values of registers)
			"addq $16, %rbx\n\t" 		//adds immediate value 16 to rbx 
			"addq $16, %rcx\n\t" 		//adds immediate value 16 to rcx
			"addq $16, %rdx\n\t" 		//adds immediate value 16 to rdx
		);
	}

	for( int i = limit; i < len; i++ ) 
	{
		c[i] = a[i] * b[i];
	}
}


//function which accepts 3 args to perform SIMD multiplication and Summation
float
SimdMulSum( float *a, float *b, int len )
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;

	//load each arg into a scratch register
	__asm
	(
		".att_syntax\n\t"
		"movq    -40(%rbp), %rbx\n\t"		// a | move arg0 value pointed by -40(%rbp) into rbx
		"movq    -48(%rbp), %rcx\n\t"		// b | move arg1 value pointed by -48(%rbp) into rcx
		"leaq    -32(%rbp), %rdx\n\t"		// &sum[0] arg2 | loads -32(%rbp) into %rdx 
		"movups	 (%rdx), %xmm2\n\t"			// 4 copies of 0. in xmm2 | load the second sse register from the scratch register %rcx
	);
	//for loop for performing additions till the arraySize with a size of SSE-WIDTH(4)
	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		__asm
		(
			".att_syntax\n\t"
			"movups	(%rbx), %xmm0\n\t"	// load the first sse register
			"movups	(%rcx), %xmm1\n\t"	// load the second sse register
			"mulps	%xmm1, %xmm0\n\t"	// do the multiply
			"addps	%xmm0, %xmm2\n\t"	// do the add
			//(restore the values of the registers rbx and rcx)
			"addq $16, %rbx\n\t" 		//adds immediate value 16 to rbx
			"addq $16, %rcx\n\t" 		//adds immediate value 16 to rcx
			//not restoring rdx as the summation is pending!
		);
	}

	__asm
	(
		".att_syntax\n\t"
		"movups	 %xmm2, (%rdx)\n\t"	// copy the sums back to sum[ ]
	);

	for( int i = limit; i < len; i++ )
	{
		sum[i-limit] += a[i] * b[i];
	}

	return sum[0] + sum[1] + sum[2] + sum[3];
}