#include "cidre.h"
#include <ctype.h>
#include <errno.h>

typedef union CIDRe // CIDR construction
{
    CIDRe full; // full representation
    struct      // parts representation
    {
        union // address part
        {
            uint32_t packed; // packed
            struct           // unpacked elements
            {
                unsigned char a;
                unsigned char b;
                unsigned char c;
                unsigned char d;
            } element;
        } address;
        unsigned char mask;     // CIDR mask
        unsigned char reserved; // reserved
        unsigned char version;  // version
        unsigned char type;     // CIDR is special (see [https://en.wikipedia.org/wiki/Private_network] and 0.0.0.0/0)
    } part;
} CIDReasy;

#define CIDReasy_SET_ELEMENT(easy, elname) \
    if (elname > 255)                      \
    {                                      \
        errno = EINVAL;                    \
        return 0;                          \
    }                                      \
    else                                   \
        easy.part.address.element.elname = (unsigned char)(elname)

/**
 * Creates CIDRe from string
 *
 * @param str
 * @return CIDRe | 0 on error
 */
CIDRe CIDRe_create(const char *str)
{
    CIDReasy result = {0};

    if (str)
    {
        unsigned int a = 0;
        unsigned int b = 0;
        unsigned int c = 0;
        unsigned int d = 0;
        char *strmask = NULL;
        sscanf(str, "%u.%u.%u.%u/%ms", &a, &b, &c, &d, &strmask);

        int mask = (strmask && isdigit(strmask[0])) ? atoi(strmask) : 32;
        if (strmask)
        {
            free(strmask);
        }

        CIDReasy_SET_ELEMENT(result, a);
        CIDReasy_SET_ELEMENT(result, b);
        CIDReasy_SET_ELEMENT(result, c);
        CIDReasy_SET_ELEMENT(result, d);

        result.part.mask = 32; // prevents the CIDR mask from increasing
        return CIDRe_mask(result.full, mask);
    }

    errno = EINVAL;
    return 0;
}

/**
 * Changes CIDRe mask
 *
 * @param src
 * @param mask
 * @return CIDRe | 0 on error
 */
CIDRe CIDRe_mask(CIDRe src, int mask)
{
    CIDReasy easy;
    easy.full = src;

    if (mask >= 0 && mask < 33 && mask <= easy.part.mask)
    {
        easy.part.mask = (unsigned char)(mask);
    }
    else
    {
        errno = EINVAL;
        return 0;
    }

    easy.part.address.packed &= UINT32_MAX << (32 - mask);

    // checks the type of result
    if (0 == easy.part.address.element.a)
    {
        easy.full = 0;
        easy.part.type = '0'; // 0.0.0.0/0
    }
    else if (10 == easy.part.address.element.a)
    {
        easy.part.type = 'A'; // 10.0.0.0/8
    }
    else if (127 == easy.part.address.element.a)
    {
        easy.part.type = 'L'; // 127.0.0.0/8
    }
    else if (100 == easy.part.address.element.a && 64 == easy.part.address.element.b)
    {
        easy.part.type = 'N'; // 100.64.0.0/10
    }
    else if (172 == easy.part.address.element.a && 16 == easy.part.address.element.b)
    {
        easy.part.type = 'B'; // 172.16.0.0/12
    }
    else if (192 == easy.part.address.element.a && 168 == easy.part.address.element.b)
    {
        easy.part.type = 'C'; // 192.168.0.0/16
    }
    else if (169 == easy.part.address.element.a && 254 == easy.part.address.element.b)
    {
        easy.part.type = 'R'; // 169.254.0.0/16
    }

#ifdef VERSION
    easy.part.version = (unsigned char)atoi(VERSION);
#endif

    return easy.full;
}

/**
 * Human readable representation of CIDRe
 * in format 'a.b.c.d/mask'
 *
 * @param src
 * @return allocated string | NULL
 */
char *CIDRe_string(CIDRe src)
{
    if (src)
    {
        CIDReasy e;
        e.full = src;
        char *buff = NULL;
        asprintf(&buff, "%d.%d.%d.%d/%d", e.part.address.element.a, e.part.address.element.b, e.part.address.element.c, e.part.address.element.d, e.part.mask);
        return buff;
    }
    errno = EINVAL;
    return NULL;
}

/**
 * Human readable representation of CIDRe address only
 * in format 'a.b.c.d'
 *
 * @param src
 * @return allocated string | NULL
 */
char *CIDRe_address(CIDRe src)
{
    if (src)
    {
        CIDReasy e;
        e.full = src;
        char *buff = NULL;
        asprintf(&buff, "%d.%d.%d.%d", e.part.address.element.a, e.part.address.element.b, e.part.address.element.c, e.part.address.element.d);
        return buff;
    }

    errno = EINVAL;
    return NULL;
}

/**
 * Tests whether a larger CIDRe contains a smaller CIDRe
 *
 * @param larger
 * @param smaller
 * @return int
 */
int CIDRe_contains(CIDRe larger, CIDRe smaller)
{
    if (larger && smaller)
    {
        CIDReasy e_larger;
        e_larger.full = larger;
        CIDReasy e_smaller;
        e_smaller.full = smaller;
        if (e_larger.part.mask <= e_smaller.part.mask)
        {
            e_smaller.full = CIDRe_mask(e_smaller.full, e_larger.part.mask);
            return !(e_larger.part.address.packed ^ e_larger.part.address.packed);
        }
    }

    errno = EINVAL;
    return 0;
}

#define CIDRe_isOK(x) !!(x)

/**
 * CIDRe covers any address
 * (is 0.0.0.0/0)
 *
 * @param test
 * @return int
 */
int CIDRe_isAnything(CIDRe test)
{
    CIDReasy e;
    e.full = test;
    return ('0' == e.part.type);
}

/**
 * CIDRe is private [https://en.wikipedia.org/wiki/Private_network]
 *
 * @param test
 * @return int
 */
int CIDRe_isPrivate(CIDRe test)
{
    CIDReasy e;
    e.full = test;
    return !!strchr("ABCLNR", e.part.type);
}

/**
 * CIDRe is loopback [https://en.wikipedia.org/wiki/Private_network]
 *
 * @param test
 * @return int
 */
int CIDRe_isLoopback(CIDRe test)
{
    CIDReasy e;
    e.full = test;
    return ('L' == e.part.type);
}

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
 * @return unsigned char | '\0' on empty/undefined
 */
unsigned char CIDRe_type(CIDRe test)
{
    if (test)
    {
        CIDReasy e;
        e.full = test;
        return e.part.type ? e.part.type : 'I';
    }
    return 0;
}

/**
 * Returns CIDRe major part of version
 *
 * @param test
 * @return unsigned char
 */
unsigned char CIDRe_version(CIDRe test)
{
    CIDReasy e;
    e.full = test;
    return e.part.version;
}
