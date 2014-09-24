/* Non Native alignment Pragmas
 * place:
 * #include "oal_aligned_packed.h" 
 *      as close as possible before structures needing packed alignment
 *      and place
 * #include  "oal_aligned_reset.h" 
 *      as close as possible after.
 * 
 */     


#if defined(sun)
#pragma pack()
#endif

#if defined(__osf__)
#    pragma pack 0
#endif

#if defined(__hpux)
#pragma HP_ALIGN POP
#endif

#if defined(_AIX)
#pragma options align=reset
#endif

#if defined(WIN32)
#pragma pack()
#endif

/*
 * Ideally the pragmas should 'pop' the alignment to the previous value,
 * unfortunatly some platforms do not support that,..
 */     

#if defined(SCO)
#pragma pack (4)
#endif

#if defined(USG)
#pragma Align_members(4)
#endif

#if defined(SEQUENT)
#pragma pack(4)
#endif

