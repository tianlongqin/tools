#ifndef __TTYPES__HH__19910610
#define __TTYPES__HH__19910610
#include <stdint.h>
#include <stddef.h>

#define GCC_VERSION (__GNUC__ * 10000           \
		+ __GNUC_MINOR__ * 100     \
		+ __GNUC_PATCHLEVEL__)

#define min(x,y) ({ \
		typeof(x) _x = (x);	\
		typeof(y) _y = (y);	\
		(void) (&_x == &_y);	\
		_x < _y ? _x : _y; })

#define max(x,y) ({ \
		typeof(x) _x = (x);	\
		typeof(y) _y = (y);	\
		(void) (&_x == &_y);	\
		_x > _y ? _x : _y; })

#define min_t(type, a, b) min(((type) a), ((type) b))
#define max_t(type, a, b) max(((type) a), ((type) b))

#define __same_type(a, b) 	__builtin_types_compatible_p(typeof(a), typeof(b))
#define BUILD_BUG_ON_ZERO(e) 	(sizeof(struct{int:-!!(e);}))
#define __must_be_array(a)	BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))

#define ARRAY_SIZE(x) 		(sizeof(x) / sizeof((x)[0]) + __must_be_array(a))

#define container_of(ptr, type, member) ({			\
			const typeof(((type *)0)->member) * __mptr = (ptr);	\
			(type *)((char *)__mptr - offsetof(type, member)); })
#endif
