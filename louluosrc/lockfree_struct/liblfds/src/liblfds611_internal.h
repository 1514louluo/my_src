/***** public prototypes *****/
#include "liblfds611.h"

/***** defines *****/
#define and &&
#define or  ||

#define RAISED   1
#define LOWERED  0

#define NO_FLAGS 0x0

/***** private prototypes *****/
void *lfds611_liblfds_aligned_malloc( size_t size, size_t align_in_bytes );
void lfds611_liblfds_aligned_free( void *memory );


/***** inlined code *****/

