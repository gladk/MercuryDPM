#ifndef GENERALDEFINE_H
#define GENERALDEFINE_H

#ifdef HIGH_PRECISION
typedef long double Mdouble;
#else 
typedef double Mdouble;
#endif

#define UNUSED  __attribute__ ((__unused__))

#endif
