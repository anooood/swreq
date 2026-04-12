/******************************************************************************
 *  Company    : HALCON
 *  Filename   : <FIFO.h>
 *
 *  Description:
 *  ------------
 *  Header file of the FIFO used by Aux Port.
 *
 ******************************************************************************/

#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>

#define CHAR_FIFO_SIZE		((uint32_t)(0x400))
#define AUX_FIFO_SIZE		((uint32_t)(0x4000))

typedef struct {
	uint8_t FIFO_Data[CHAR_FIFO_SIZE];		/* character FIFO storage */
	uint32_t InIndex;
	uint32_t OutIndex;
} sCharFIFO;

typedef struct {
	uint8_t FIFO_Data[AUX_FIFO_SIZE];		/* character FIFO storage */
	uint32_t InIndex;
	uint32_t OutIndex;
} sCharAUX_FIFO;

#define INIT_FIFO(FIFO) { 	\
	(FIFO).InIndex =0;		\
	(FIFO).OutIndex =0;		\
}

#define PUT_FIFO(FIFO, value) {				        \
	(FIFO).FIFO_Data[(FIFO).InIndex] = (value);     \
	(FIFO).InIndex++;							    \
	if ((FIFO).InIndex >= CHAR_FIFO_SIZE)		    \
		(FIFO).InIndex = 0;					        \
}
#define PUT_AUX_FIFO(FIFO, value) {			        \
	(FIFO).FIFO_Data[(FIFO).InIndex] = (value); 	\
	(FIFO).InIndex++;							    \
	if ((FIFO).InIndex >= AUX_FIFO_SIZE)		    \
		(FIFO).InIndex = 0;					        \
}

#define PUT_FIFO_Ptr(FIFO, value) {				    \
	(FIFO)->FIFO_Data[(FIFO)->InIndex] = (value); 	\
	(FIFO)->InIndex++;							    \
	if ((FIFO)->InIndex >= CHAR_FIFO_SIZE)		    \
		(FIFO)->InIndex = 0;						\
}
#define PUT_AUX_FIFO_Ptr(FIFO, value) {			    \
	(FIFO)->FIFO_Data[(FIFO)->InIndex] = (value); 	\
	(FIFO)->InIndex++;							    \
	if ((FIFO)->InIndex >= AUX_FIFO_SIZE)			\
		(FIFO)->InIndex = 0;						\
}

#define GET_FIFO(FIFO, value) {				        \
	(value) = (FIFO).FIFO_Data[(FIFO).OutIndex]; 	\
	(FIFO).OutIndex++;						        \
	if ((FIFO).OutIndex >= CHAR_FIFO_SIZE)	        \
		(FIFO).OutIndex = 0;					    \
}

#define GET_AUX_FIFO(FIFO, value) {			        \
	(value) = (FIFO).FIFO_Data[(FIFO).OutIndex]; 	\
	(FIFO).OutIndex++;						        \
	if ((FIFO).OutIndex >= AUX_FIFO_SIZE)		    \
		(FIFO).OutIndex = 0;					    \
}

#define GET_FIFO_Ptr(FIFO, value) {			        \
	(value) = (FIFO)->FIFO_Data[(FIFO)->OutIndex];  \
	(FIFO)->OutIndex++;						        \
	if ((FIFO)->OutIndex >= CHAR_FIFO_SIZE)	        \
		(FIFO)->OutIndex = 0;					    \
}

#define GET_AUX_FIFO_Ptr(FIFO, value) {			    \
	(value) = (FIFO)->FIFO_Data[(FIFO)->OutIndex]; 	\
	(FIFO)->OutIndex++;							    \
	if ((FIFO)->OutIndex >= AUX_FIFO_SIZE)		    \
		(FIFO)->OutIndex = 0;						\
}

#define BYTES_IN_FIFO(FIFO) ((FIFO).OutIndex != (FIFO).InIndex)
#define BYTES_IN_AUX_FIFO(FIFO) ((FIFO).OutIndex != (FIFO).InIndex)

#define NUMBER_BYTES_IN_FIFO(FIFO, BytesInFIFO)	{ 						    \
	if ((FIFO).OutIndex == (FIFO).InIndex) {								\
		(BytesInFIFO) = 0;												    \
	}																	    \
	else if ((FIFO).InIndex > (FIFO).OutIndex) {							\
		(BytesInFIFO) = ((FIFO).InIndex - (FIFO).OutIndex);					\
	}																	    \
	else {																    \
		(BytesInFIFO) = (CHAR_FIFO_SIZE + (FIFO).InIndex - (FIFO).OutIndex);\
	}																	    \
}

#define NUMBER_BYTES_IN_AUX_FIFO(FIFO, BytesInFIFO)	{ 					    \
	if ((FIFO).OutIndex == (FIFO).InIndex) {								\
		(BytesInFIFO) = 0;												    \
	}																	    \
	else if ((FIFO).InIndex > (FIFO).OutIndex) {							\
		(BytesInFIFO) = ((FIFO).InIndex - (FIFO).OutIndex);					\
	}																	    \
	else {																    \
		(BytesInFIFO) = (AUX_FIFO_SIZE + (FIFO).InIndex - (FIFO).OutIndex);	\
	}																	    \
}

#endif
