/* START OF functions.c --------------------------------------------------------
 *
 *      Memory function wrappers mostly.
 *
 *	---
 *	THIS GUI IS TOTALLY *BROKEN*! PLEASE DO NOT USE IT!
 *	---
 */


#include <functions.h>


#define EG_DEBUG_MAXMALLOCS 50000


/* malloc and free:
 */

#ifdef EG_DEBUG
typedef struct{
	void *ptr;
	size_t size;
}MallocAllocList;

static MallocAllocList EG_Malloc_AllocationList[EG_DEBUG_MAXMALLOCS];
static unsigned long EG_Malloc_AllocationCount = 0;

static void EG_Malloc_InitialiseList()
{
	long i;
	for(i=0; i<EG_DEBUG_MAXMALLOCS; i++){
		EG_Malloc_AllocationList[i].ptr = NULL;
		EG_Malloc_AllocationList[i].size = 0;
	}
}

static void EG_Malloc_AddAllocationToList(void *p, size_t size)
{
	long i;
	for(i=0; i<EG_DEBUG_MAXMALLOCS; i++)
		if (EG_Malloc_AllocationList[i].ptr == NULL){
			EG_Malloc_AllocationList[i].ptr = p;
			EG_Malloc_AllocationList[i].size = size;
			return;
		}
}

static size_t EG_Malloc_GetSize(void *p)
{
	long i;
	for(i=0; i<EG_DEBUG_MAXMALLOCS; i++)
		if (EG_Malloc_AllocationList[i].ptr == p)
			return(EG_Malloc_AllocationList[i].size);

	return(0);
}

/* Returns TRUE if removed, FALSE if not found.
 */
static EG_BOOL EG_Malloc_RemoveAllocationFromList(void *p)
{
	long i;
	for(i=0; i<EG_DEBUG_MAXMALLOCS; i++)
		if (EG_Malloc_AllocationList[i].ptr == p){
			EG_Malloc_AllocationList[i].ptr = NULL;
			EG_Malloc_AllocationList[i].size = 0;
			return(EG_TRUE);
		}

	return(EG_FALSE);
}

EG_BOOL EG_Malloc_CanFindAllocationInList(void *p)
{
	EG_PTR_AS_INT pointer, pointer_lowerboundry, pointer_upperboundry;
	long i;

	for(i=0; i<EG_DEBUG_MAXMALLOCS; i++){
		pointer = EG_CAST_PTR_TO_INT(p);
		pointer_lowerboundry =
		 EG_CAST_PTR_TO_INT(EG_Malloc_AllocationList[i].ptr);

		pointer_upperboundry = pointer_lowerboundry
		 + (EG_PTR_AS_INT) EG_Malloc_AllocationList[i].size -1;

		if (pointer >= pointer_lowerboundry && pointer
		 <= pointer_upperboundry)
			return(EG_TRUE);
	}
	return(EG_FALSE);
}

void* EG_Malloc(size_t size)
{
	int *ptr_tmp;

	static EG_BOOL b = EG_FALSE;

	if (b == EG_FALSE){
		EG_Malloc_InitialiseList();
		b=EG_TRUE;
	}
	char *ptr;

	/* Increase size of allocation so we can prefix and append a value
	 * to check whether we write out of bounds.
	 */
	size += (sizeof(int)*2);

	if ( (ptr= (char*) malloc(size)) != NULL){
		unsigned long i;
//		char *ptr2 = (char*) ptr;

		EG_Malloc_AllocationCount++;
		
		EG_Log(EG_LOG_DEBUG5, dL"malloc(%08ld) = "EG_PTRFMT" [%08lu]",dR
		 , (long) size, EG_CAST_PTR_TO_INT(ptr), EG_Malloc_AllocationCount);

		EG_Malloc_AddAllocationToList(ptr, size);

		for(i=0; i< (unsigned long) size; i++)
//			*(ptr2++)=EG_MALLOCFILLER;
			ptr[i] = EG_MALLOCFILLER;

	}else{
		EG_Log(EG_LOG_DEBUG
		 , dL"Unable to malloc %ld bytes (ALL YOUR BYTES ARE BELONG TO US)."
		 , dR, (long) size);
	}

	/* Set the check values.
	 */
	ptr_tmp = (int*) ptr; *ptr_tmp = -1;
	ptr_tmp = (int*) ( ptr+size-(sizeof(int)) ); *ptr_tmp = -2;

	ptr+= sizeof(int);
	return( ptr );
}

/* System free wrapper.
 */
void EG_Free(void *ptr)
{
	size_t i, len;
	char *c_ptr = (char*) ptr;
	int *ptr_tmp, a, b;

	c_ptr -= sizeof(int);

	/* Check if we wrote out of bounds.
	 */
	ptr_tmp = (int*) c_ptr; a = *ptr_tmp;

	/* Set the malloc'd buffer to a known state prior to freeing.
	 */
	if ( (len=EG_Malloc_GetSize(c_ptr)) >0){

		EG_Malloc_AllocationCount--;
		EG_Log(EG_LOG_DEBUG5, dL"free("EG_PTRFMT") [%lu]",dR
		 , EG_CAST_PTR_TO_INT(c_ptr), EG_Malloc_AllocationCount);

		ptr_tmp = (int*) (c_ptr + len - sizeof(int));
		b = *ptr_tmp;
		if ( a != -1){
			fprintf(stderr, "BUFFER UNDER RUN.\n");
			//exit(9);
		}
			
		if (b != -2){
			fprintf(stderr, "BUFFER OVER RUN.\n");

//			char *tmp_c_ptr = (c_ptr); // + sizeof(int));
//			fprintf(stderr, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", (int) tmp_c_ptr[0], (int) tmp_c_ptr[1], (int) tmp_c_ptr[2], (int) tmp_c_ptr[3], (int) tmp_c_ptr[4], (int) tmp_c_ptr[5], (int) tmp_c_ptr[6], (int) tmp_c_ptr[7], (int) tmp_c_ptr[8], (int) tmp_c_ptr[9], (int) tmp_c_ptr[10], (int) tmp_c_ptr[11], (int) tmp_c_ptr[12], (int) tmp_c_ptr[13], (int) tmp_c_ptr[14], (int) tmp_c_ptr[15], (int) tmp_c_ptr[16], (int) tmp_c_ptr[17]);

			//exit(10);
		}

		for(i=0; i<len; i++){
			c_ptr[i] = EG_FREEFILLER;
		}

	}else{
		EG_Log(EG_LOG_FATAL, dL"Could not find ["EG_PTRFMT"] in malloc ref, shit.."
	 	 , dR, EG_CAST_PTR_TO_INT(c_ptr) );
	}

	EG_Malloc_RemoveAllocationFromList(c_ptr);
	free(c_ptr);
}
#endif

/* Returns an unsigned long containing a numerical hash of the passed
 * string using the following:
 */
unsigned long EG_MakeStringHash(char *p)
{
        unsigned long h;
        size_t i;

        h = 0; for (i = 0; i < strlen(p); i++)
                h = 31UL * h + ( (unsigned long) p[i]);

        return(h);
}










/* Converts the source string to uppercase, then hashes it.
 *
 * Note: Does not check for NULL. If the malloc fails then EG_HASH_FAILED is
 *       returned.
 */

//unsigned int EG_UpperCaseHash(char *p)
//{
//        char *buf_p;
//        unsigned int hash;
//
//	size_t size;
//
//	if (p==NULL) return EG_HASH_FAILED;
//
//	size = (size_t) strlen(p)+1;
//       if (size < 20) size = 1024;
//
//        if ( (buf_p = (char*) EG_Malloc(size)) == NULL)
//                return EG_HASH_FAILED;
//
//        EG_ToUpperCase(buf_p);
//
//        hash = EG_MakeStringHash(buf_p);
//
//        EG_Free(buf_p);
//
//	hash = EG_MakeStringHash(p);
//
//        return hash;
//}

/* Convert a string to uppercase:
 */
char* EG_ToUpperCase(char *p)
{
        size_t i;

        if (p == NULL) return p;

        for(i=0; i<strlen(p); i++)
                p[i] = (char) toupper(p[i]);

        return p;
}

