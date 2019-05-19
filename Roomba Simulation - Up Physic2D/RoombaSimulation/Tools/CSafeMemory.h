#pragma once

#include <assert.h>
#include <stdint.h>

#define MALLOC(arr, m, type) {\
	arr = (type *)malloc((m) * sizeof(type));\
	assert(arr != NULL);\
}

#define MALLOC2D(arr, m, n, type) {\
	arr = (type **)malloc((m)*sizeof(type*));\
	assert(arr!=NULL);\
	for (int i = 0; i < (m); i++){\
		arr[i] = (type *)malloc((n)*sizeof(type));\
		assert(arr[i]!=NULL);\
	}\
}

#define FREE(p) {\
	assert(p != NULL);\
	free(p);\
	p = NULL;\
}

#define FREE2D(arr, m) {\
for (int i = 0; i < (m); i++){\
	FREE(arr[i]);\
	}\
	FREE(arr);\
}