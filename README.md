# CIDRe

**CIDRe** is a lightweight C library for easy manipulation and classification of IPv4 CIDR blocks.  
It provides a compact `CIDRe` type and utility functions for parsing, masking, comparing, and inspecting IPv4 subnets.

---

## 🔍 Overview

CIDRe encodes IPv4 addresses and subnet metadata into a 64-bit value for fast processing.  
It supports:

- Parsing from strings like `192.168.1.0/24`
- CIDR masking and truncation
- Private, loopback, and special address detection
- Conversion to/from dotted-decimal strings
- Fast containment checks

---

## ⚙️ Installation & Build

### Requirements:
- C compiler (GCC/Clang)
- Standard C library (no external dependencies)

### To use in your project:

1. Clone or copy files:
```bash
git clone https://github.com/yuriimouse/CIDRe.git
```

2. Add to your build:
- Include sources/cidre.c and sources/cidre.h in your build system
- Define the VERSION macro (optional)
- Example with gcc:
```bash
gcc -DVERSION=\"1.2.2\" your_code.c sources/cidre.c -o your_program
```

## 📚 API Reference

| Function                | Description                              |
| ----------------------- | ---------------------------------------- |
| `CIDRe_create()`        | Parse CIDR from string                   |
| `CIDRe_mask()`          | Change or set mask                       |
| `CIDRe_string()`        | Get full CIDR string                     |
| `CIDRe_address()`       | Get address-only string                  |
| `CIDRe_contains()`      | Test if one CIDR contains another        |
| `CIDRe_isPrivate()`     | Detect private ranges                    |
| `CIDRe_isLoopback()`    | Detect loopback                          |
| `CIDRe_type()`          | Get CIDR type (`A`, `B`, `C`, `L`, etc.) |
| `CIDRe_addressPacked()` | Get raw address as uint32                |
| `CIDRe_maskValue()`     | Get subnet mask                          |

See full inline documentation in `cidre.c`.

## 🚀 Examples
```c
#include "cidre.h"

CIDRe net = CIDRe_create("192.168.0.0/24");

if (CIDRe_isPrivate(net)) {
    printf("Private range: %s\n", CIDRe_string(net));
}

CIDRe host = CIDRe_create("192.168.0.42");

if (CIDRe_contains(net, host)) {
    printf("Host is within the network.\n");
}
```

## 🧩 Integration

You can embed CIDRe as a single file module:
- Place cidre.c and cidre.h in your project
- Use CIDRe functions as part of your networking or ACL logic
- CIDRe is pure C and highly portable — no dependencies, no heap unless using CIDRe_string() and CIDRe_address()

For clean integration, consider wrapping it in your own networking abstraction.

