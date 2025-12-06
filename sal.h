/* Minimal sal.h for GCC/MinGW compatibility with MS-MPI */
#ifndef _SAL_H
#define _SAL_H

/* Define SAL annotations as empty macros for GCC */
#define _In_
#define _Out_
#define _Inout_
#define _In_opt_
#define _Out_opt_
#define _Inout_opt_
#define _Ret_maybenull_
#define _Ret_notnull_
#define _Success_(expr)
#define _Check_return_
#define _Null_terminated_
#define _Post_satisfies_(expr)
#define _Pre_satisfies_(expr)
#define _Out_writes_(x)
#define _Out_writes_opt_(x)
#define _Out_writes_to_(x,y)
#define _In_reads_(x)
#define _In_reads_opt_(x)
#define _Inout_updates_(x)
#define _Out_writes_bytes_(x)
#define _In_reads_bytes_(x)
#define _Inout_updates_bytes_(x)
#define _Deref_out_
#define _Deref_out_opt_
#define _Deref_opt_out_
#define _When_(x,y)
#define _At_(x,y)
#define _Printf_format_string_
#define _Scanf_format_string_
#define _Scanf_s_format_string_
#define _Printf_format_string_params_(x)
#define _Scanf_format_string_params_(x)
#define _Scanf_s_format_string_params_(x)

#endif /* _SAL_H */

