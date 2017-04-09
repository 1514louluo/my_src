/*
 * hazard.h
 *
 *  Created on: Feb 7, 2012
 *      Author: sears
 */
#include <assert.h>
#ifndef _HAZARD_H_
#define _HAZARD_H_

#ifndef LF_HAZARD_K
#define LF_HAZARD_K (2)
#endif


struct GSList
{
  void  *data;
  GSList *next;
};

typedef struct _lf_hazard lf_hazard;
struct _lf_hazard {
	void  					*hp[LF_HAZARD_K];
	lf_hazard 				*next;
	int  						 active;
	int      					 rcount;
	list   					 *rlist;
	GTree    					 *plist;
};


#endif /* _HAZARD_H_ */



