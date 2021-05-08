# Calculator Utilities

### findByteIndex

Given a decimal number, this function will find which byte bucket it falls into.

Example:
```
uint32_t byteIndex = findByteIndex(20);
// byteIndex is now 3
```

Visualized:
```
Your value:       20
                  |
                  ^
0      8      16     24     32     40     48   ...     Bit index
|------|------|------|------|------|------|    ...     Byte Bucket (where each byte is 8 bits)
    1      2     3      4      5      6        ...     Byte Index (starting counting at one)
