#ifndef CIDRE_H
#define CIDRE_H

#define _GNU_SOURCE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef VERSION
#define VERSION "0"
#endif

typedef uint64_t CIDRe; // CIDR representation

/**
 * Creates CIDRe from string
 *
 * @param str
 * @return CIDRe | 0 on error
 */
CIDRe CIDRe_create(const char *str);

/**
 * Changes CIDRe mask
 *
 * @param src
 * @param mask
 * @return CIDRe | 0 on error
 */
CIDRe CIDRe_mask(CIDRe src, int mask);

/**
 * Tests whether a larger CIDRe contains a smaller CIDRe
 *
 * @param larger
 * @param smaller
 * @return int
 */
int CIDRe_contains(CIDRe larger, CIDRe smaller);

/**
 * Human readable representation of CIDRe
 * in format 'a.b.c.d/mask'
 *
 * @param src
 * @return allocated string | NULL
 */
char *CIDRe_string(CIDRe src);

/**
 * Human readable representation of CIDRe address only
 * in format 'a.b.c.d'
 *
 * @param src
 * @return allocated string | NULL
 */
char *CIDRe_address(CIDRe src);

#define CIDRe_isOK(x) !!(x)

/**
 * CIDRe covers any address
 * (is 0.0.0.0/0)
 *
 * @param test
 * @return int
 */
int CIDRe_isAnything(CIDRe test);

/**
 * CIDRe is private [https://en.wikipedia.org/wiki/Private_network]
 *
 * @param test
 * @return int
 */
int CIDRe_isPrivate(CIDRe test);

/**
 * CIDRe is loopback [https://en.wikipedia.org/wiki/Private_network]
 *
 * @param test
 * @return int
 */
int CIDRe_isLoopback(CIDRe test);

/**
 * Returns CIDRe type:
 * '0' = 0.0.0.0/0
 * 'A' = 10.0.0.0/8     // RFC 1918
 * 'B' = 172.16.0.0/12  // RFC 1918
 * 'C' = 192.168.0.0/16 // RFC 1918
 * 'L' = 127.0.0.0/8
 * 'N' = 100.64.0.0/10  // (IANA)
 * 'R' = 169.254.0.0/16
 *
 * 'I' = otherwise
 *
 * @param test
 * @return unsigned char
 */
unsigned char CIDRe_type(CIDRe test);

/**
 * Returns CIDRe major part of version
 *
 * @param test
 * @return unsigned char
 */
unsigned char CIDRe_version(CIDRe test);

#endif // CIDRE_H
