/* SPDX-License-Identifier: GPL-2.0 */
#ifndef RBTREE_HELPER_H
#define RBTREE_HELPER_H

/* Is this type a native word size -- useful for atomic operations */
#define __native_word(t) \
        (sizeof(t) == sizeof(char) || sizeof(t) == sizeof(short) || \
         sizeof(t) == sizeof(int) || sizeof(t) == sizeof(long))

#define __always_inline inline __attribute__((__always_inline__))

/**
 * static_assert - check integer constant expression at build time
 *
 * static_assert() is a wrapper for the C11 _Static_assert, with a
 * little macro magic to make the message optional (defaulting to the
 * stringification of the tested expression).
 */
#define static_assert(expr, ...) __static_assert(expr, ##__VA_ARGS__, #expr)
#define __static_assert(expr, msg, ...) _Static_assert(expr, msg)

#define WRITE_ONCE(x, val)                                                \
do {                                                                        \
        static_assert(__native_word(x) || sizeof(x) == sizeof(long long)\
            , "Unsupported access size for {READ,WRITE}_ONCE().");\
        *(volatile typeof(x) *)&(x) = (val);                                \
} while (0)

#define __same_type(a, b)          __builtin_types_compatible_p(typeof(a), typeof(b))
#define typeof_member(T, m)        typeof(((T*)0)->m)

#define likely(x)          __builtin_expect(!!(x), 1)
#define unlikely(x)        __builtin_expect(!!(x), 0)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:        the type of the container struct this is embedded in.
 * @member:        the name of the member within the struct.
 *
 * WARNING: any const qualifier of @ptr is lost.
 */
#define container_of(ptr, type, member) ({                                \
        void *__mptr = (void *)(ptr);                                        \
        static_assert(__same_type(*(ptr), ((type *)0)->member) ||        \
                      __same_type(*(ptr), void),                        \
                      "pointer type mismatch in container_of()");        \
        ((type *)(__mptr - offsetof(type, member))); })

/**
 * container_of_const - cast a member of a structure out to the containing
 *                        structure and preserve the const-ness of the pointer
 * @ptr:                the pointer to the member
 * @type:                the type of the container struct this is embedded in.
 * @member:                the name of the member within the struct.
 */
#define container_of_const(ptr, type, member)                                \
        _Generic(ptr,                                                        \
                const typeof(*(ptr)) *: ((const type *)container_of(ptr, type, member)),\
                default: ((type *)container_of(ptr, type, member))        \
        )

#endif        /* RBTREE_HELPER_H */
