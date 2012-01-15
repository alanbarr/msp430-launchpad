/* Copied from the mspgcc souceforge page: 
** http://mspgcc.sourceforge.net/manual/c1408.html
*/

static void __inline__ brief_pause(register unsigned int n)
{
    __asm__ __volatile__ (
		"1: \n"
		" dec	%[n] \n"
		" jne	1b \n"
        : [n] "+r"(n));
}
