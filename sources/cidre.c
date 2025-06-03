/**
 * @file sources/cidre.c
 * @author https://github.com/yuriimouse/CIDRe
 * @brief Easy IPv4 CIDR manipulation
 * @version 1.2.2
 * @date 2024-07-15
 *
 * @copyright Copyright (c) 2024
 */
#include "cidre.h"
#include <ctype.h>
#include <errno.h>

/**
 * @brief Internal representation of CIDR block with metadata
 *
 * The `CIDReasy` union allows access to a compact 64-bit representation (`full`)
 * and a structured view (`part`) including:
 * - address bytes
 * - subnet mask
 * - CIDR type (A/B/C/Loopback/etc.)
 * - version
 */
typedef union CIDRe
{
    CIDRe full; // full representation
    struct      // parts representation
    {
        unsigned char type;     // CIDR is special (see [https://en.wikipedia.org/wiki/Private_network] and 0.0.0.0/0)
        unsigned char version;  // version
        unsigned char reserved; // reserved
        unsigned char mask;     // CIDR mask
        union                   // address part
        {
            uint32_t packed; // packed
            struct           // unpacked elements
            {
                unsigned char d;
                unsigned char c;
                unsigned char b;
                unsigned char a;
            } element;
        } address;
    } part;
} CIDReasy;

/**
 * @brief Sets a single byte of an address, validates range
 *
 * @param easy Internal representation
 * @param elname Element name (a, b, c, d)
 * @return 0 on error (errno set to EINVAL)
 */
#define CIDReasy_SET_ELEMENT(easy, elname) \
    if (elname > 255)                      \
    {                                      \
        errno = EINVAL;                    \
        return 0;                          \
    }                                      \
    else                                   \
        easy.part.address.element.elname = (unsigned char)(elname)

/**
 * @brief Creates a CIDR from a dotted-decimal string with optional mask
 *
 * Accepts input like "192.168.1.0/24" or "10.0.0.1".
 *
 * @param str Input string
 * @return CIDRe encoded block, or 0 on error (errno set)
 */
CIDRe CIDRe_create(const char *str)
{
    CIDReasy result = {0};

    if (str)
    {
        int mask = 32;
        unsigned int a = 0;
        unsigned int b = 0;
        unsigned int c = 0;
        unsigned int d = 0;
        int n = 0;
        if (sscanf(str, "%u.%u.%u.%u%n", &a, &b, &c, &d, &n) != 4)
        {
            errno = EINVAL;
            return 0;
        }
        if ('/' == str[n])
        {
            mask = (isdigit(str[n + 1])) ? atoi(str + n + 1) : -1;
        }
        if (mask >= 0)
        {
            // prevents invalid element values ​​from appearing
            CIDReasy_SET_ELEMENT(result, a);
            CIDReasy_SET_ELEMENT(result, b);
            CIDReasy_SET_ELEMENT(result, c);
            CIDReasy_SET_ELEMENT(result, d);

            result.part.mask = 32; // initial normalization to full mask before applying real one
            return CIDRe_mask(result.full, mask);
        }
    }

    errno = EINVAL;
    return 0;
}

/**
/**
 * @brief Sets or changes the CIDR mask
 *
 * Truncates address based on new mask and updates the CIDR type.
 *
 * @param src Original CIDR block
 * @param mask New mask value (0-32)
 * @return Updated CIDRe, or 0 on error
 */
CIDRe CIDRe_mask(CIDRe src, int mask)
{
    CIDReasy easy;
    easy.full = src;

    if (src)
    {
        if (mask >= 0 && mask < 33 && mask <= easy.part.mask)
        {
            easy.part.mask = (unsigned char)(mask);
        }
        else
        {
            errno = EINVAL;
            return 0;
        }

        if (mask)
        {
            easy.part.address.packed &= UINT32_MAX << (32 - mask);
        }
        else
        {
            easy.part.address.packed = 0;
        }

        // checks the type of result
        if (0 == easy.part.address.element.a)
        {
            easy.part.type = CIDRE_TYPE_ZERO; // 0.0.0.0/0
        }
        else if (10 == easy.part.address.element.a)
        {
            easy.part.type = CIDRE_TYPE_ANET; // 10.0.0.0/8
        }
        else if (127 == easy.part.address.element.a)
        {
            easy.part.type = CIDRE_TYPE_LOCALNET; // 127.0.0.0/8
        }
        else if (100 == easy.part.address.element.a && 64 == easy.part.address.element.b)
        {
            easy.part.type = CIDRE_TYPE_IANA; // 100.64.0.0/10
        }
        else if (172 == easy.part.address.element.a && 16 == easy.part.address.element.b)
        {
            easy.part.type = CIDRE_TYPE_BNET; // 172.16.0.0/12
        }
        else if (192 == easy.part.address.element.a && 168 == easy.part.address.element.b)
        {
            easy.part.type = CIDRE_TYPE_CNET; // 192.168.0.0/16
        }
        else if (169 == easy.part.address.element.a && 254 == easy.part.address.element.b)
        {
            easy.part.type = CIDRE_TYPE_APIPA; // 169.254.0.0/16
        }

#ifdef VERSION
        easy.part.version = (unsigned char)atoi(VERSION);
#endif
    }

    return easy.full;
}

/**
 * @brief Returns CIDR string in "a.b.c.d/mask" format
 *
 * The returned string is dynamically allocated and must be freed by the caller.
 *
 * @param src CIDR block
 * @return malloc-allocated string or NULL on error
 */
char *CIDRe_string(CIDRe src)
{
    if (src)
    {
        CIDReasy e;
        e.full = src;
        char *buff = NULL;
        return (0 <= asprintf(&buff, "%d.%d.%d.%d/%d",
                              e.part.address.element.a,
                              e.part.address.element.b,
                              e.part.address.element.c,
                              e.part.address.element.d,
                              e.part.mask))
                   ? buff
                   : NULL;
    }
    errno = EINVAL;
    return NULL;
}

/**
 * @brief Returns address part of CIDR as "a.b.c.d"
 *
 * The returned string is dynamically allocated and must be freed by the caller.
 *
 * @param src CIDR block
 * @return malloc-allocated string or NULL on error
 */
char *CIDRe_address(CIDRe src)
{
    if (src)
    {
        CIDReasy e;
        e.full = src;
        char *buff = NULL;
        return (0 <= asprintf(&buff, "%d.%d.%d.%d",
                              e.part.address.element.a,
                              e.part.address.element.b,
                              e.part.address.element.c,
                              e.part.address.element.d))
                   ? buff
                   : NULL;
    }
    errno = EINVAL;
    return NULL;
}

/**
 * @brief Returns true if CIDR contains the given subnetwork
 *
 * Checks if `larger` CIDR block includes `smaller` CIDR.
 *
 * @param larger Superset CIDR block
 * @param smaller Subnet to test
 * @return 1 if contains, 0 otherwise
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
            uint32_t mask = UINT32_MAX << (32 - e_larger.part.mask);
            return (e_larger.part.address.packed & mask) == (e_smaller.part.address.packed & mask);
        }
    }

    errno = EINVAL;
    return 0;
}

/**
 * @brief Returns true if CIDR is 0.0.0.0/0 (universal)
 *
 * @param test CIDR to test
 * @return 1 if CIDR covers all addresses, 0 otherwise
 */
int CIDRe_isAnything(CIDRe test)
{
    CIDReasy e;
    e.full = test;
    return (CIDRE_TYPE_ZERO == e.part.type);
}

/**
 * @brief Returns true if CIDR is in private range
 *
 * Checks for A, B, C, APIPA, IANA, and loopback ranges.
 *
 * @param test CIDR to test
 * @return 1 if private, 0 otherwise
 */
int CIDRe_isPrivate(CIDRe test)
{
    CIDReasy e;
    e.full = test;
    switch (e.part.type)
    {
    case CIDRE_TYPE_ANET:
    // fallthrough
    case CIDRE_TYPE_BNET:
    // fallthrough
    case CIDRE_TYPE_CNET:
    // fallthrough
    case CIDRE_TYPE_LOCALNET:
    // fallthrough
    case CIDRE_TYPE_IANA:
    // fallthrough
    case CIDRE_TYPE_APIPA:
        return 1;
    }
    return 0;
}

/**
 * @brief Returns true if CIDR is a loopback address (127.0.0.0/8)
 *
 * @param test CIDR to test
 * @return 1 if loopback, 0 otherwise
 */
int CIDRe_isLoopback(CIDRe test)
{
    CIDReasy e;
    e.full = test;
    return (CIDRE_TYPE_LOCALNET == e.part.type);
}

/**
 * @brief Returns CIDR type code
 *
 * Possible return values:
 * - `'0'` = 0.0.0.0/0
 * - `'A'` = 10.0.0.0/8
 * - `'B'` = 172.16.0.0/12
 * - `'C'` = 192.168.0.0/16
 * - `'L'` = 127.0.0.0/8
 * - `'N'` = 100.64.0.0/10
 * - `'R'` = 169.254.0.0/16
 * - `'I'` = Otherwise (Internet)
 *
 * @param test CIDR to test
 * @return Type character or '\0' on error
 */
unsigned char CIDRe_type(CIDRe test)
{
    if (test)
    {
        CIDReasy e;
        e.full = test;
        return e.part.type;
    }
    errno = EINVAL;
    return 0;
}

/**
 * @brief Returns the version tag assigned to the CIDR block
 *
 * The version is extracted from the macro `VERSION`, or `255` if not defined.
 *
 * @param test CIDR to test
 * @return version byte
 */
unsigned char CIDRe_version(CIDRe test)
{
    CIDReasy e;
    e.full = test;
    return e.part.version;
}

/**
 * @brief Gets packed address portion (host byte order)
 *
 * @param src CIDR block
 * @return 32-bit address or 0 on error
 */
uint32_t CIDRe_addressPacked(CIDRe src)
{
    if (src)
    {
        CIDReasy e;
        e.full = src;
        return e.part.address.packed;
    }
    errno = EINVAL;
    return 0;
}

/**
 * @brief Gets the subnet mask value (0–32)
 *
 * @param src CIDR block
 * @return subnet mask or 255 on error
 */
unsigned char CIDRe_maskValue(CIDRe src)
{
    if (src)
    {
        CIDReasy e;
        e.full = src;
        return e.part.mask;
    }
    errno = EINVAL;
    return 255;
}
