/* Non Native alignment Pragmas 
 * place: 
 * #include "oal_aligned_packed.h" 
 * 	as close as possible before structures needing packed alignment 
 * 	and place 
 * #include  "oal_aligned_reset.h" 
 * 	as close as possible after.
 */
#if defined(sun)
#pragma pack(1)
#endif

#if defined(__osf__)
#pragma pack 1
#endif

#if defined(__hpux)
#pragma HP_ALIGN NOPADDING PUSH
#endif

#if defined(SCO) || defined(SCO53) || defined(SCO54)
#if defined(PDP11)
#pragma pack (2)
#else
#pragma pack (1)
#endif
#endif

#if defined(_AIX)
#pragma options align=packed
#endif

#if defined(USG)
#pragma Align_members(1)
#endif

#if defined(SEQUENT)
#pragma pack(1)
#endif

#if defined(WIN32)
#pragma pack(1)
#endif
