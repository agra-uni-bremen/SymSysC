/*
 * klee_conf.h
 *
 *  Created on: Sep 23, 2020
 *      Author: dwd
 */

// TODO: Move to a more meaningful folder

#pragma once


#if defined(USE_KLEE) && __has_include(<klee/klee.h>)
# include <klee/klee.h>
# define INFO( param ) ;
#else
# if defined(USE_KLEE)
#   warning USE_KLEE defined, but no klee include available
# endif
# include <iostream>
# define klee_int( param ) 0
# define klee_make_symbolic( p1, p2, p3 ) ;
# define INFO( param ) param
#endif
