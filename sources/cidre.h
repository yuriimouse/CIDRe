// sources/cidre.h
#ifndef CIDRE_H
#define CIDRE_H

#define _GNU_SOURCE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CIDRE_VERSION_UNKNOWN 255
#ifndef VERSION
#warning "VERSION macro not defined; version field will 255 (unknown)"
#define VERSION "255"
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

// 0.0.0.0/0
// all IP's
// 0 == easy.part.address.element.a
#define CIDRE_TYPE_ZERO '0'
// 10.0.0.0/8
// RFC 1918 Private address space range
// 10 == easy.part.address.element.a
#define CIDRE_TYPE_ANET 'A'
// 127.0.0.0/8
// internal host loopback address range
// 127 == easy.part.address.element.a
#define CIDRE_TYPE_LOCALNET 'L'
// 100.64.0.0/10
// reserved for Internet Service Provider (ISP) networks and routing equipment (IANA)
// 100 == easy.part.address.element.a && 64 == easy.part.address.element.b
#define CIDRE_TYPE_IANA 'N'
// 172.16.0.0/12
// continuous block of 16 reserved private Class B-networks
// 172 == easy.part.address.element.a && 16 == easy.part.address.element.b
#define CIDRE_TYPE_BNET 'B'
// 192.168.0.0/16
// reserved private Class C-network
// 192 == easy.part.address.element.a && 168 == easy.part.address.element.b
#define CIDRE_TYPE_CNET  'C'
// 169.254.0.0/16
// reserved IP address block used for Automatic Private IP Addressing
// 169 == easy.part.address.element.a && 254 == easy.part.address.element.b
#define CIDRE_TYPE_APIPA 'R'
// all other
#define CIDRE_TYPE_IP 'I'

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

// additional API
/**
 * Gets the address part of CIDR 
 * 
 * @param src 
 * @return uint32_t 
 */
uint32_t CIDRe_addressPacked(CIDRe src);

/**
 * Gets the mask part of CIDR 
 * 
 * @param src 
 * @return unsigned char 
 */
unsigned char CIDRe_maskValue(CIDRe src);

#endif // CIDRE_H
