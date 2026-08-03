# Huffman File Compressor

A lossless file compression utility implemented in **C++** using the **Huffman Coding** algorithm. The project builds an optimal prefix tree based on character frequencies, compresses text into a compact binary representation, and reconstructs the original data through lossless decoding.

> **Note**
> This project demonstrates fundamental concepts in data compression, greedy algorithms, binary trees, serialization, and bit-level encoding.

---

# Features

## Compression

- Huffman Tree construction
- Frequency-based prefix encoding
- Variable-length binary codes
- Bit packing into bytes
- Automatic padding for byte alignment

## Decompression

- Huffman Tree reconstruction
- Binary stream decoding
- Lossless recovery of original text
- Tree deserialization

## Data Structures

- Binary Tree
- Priority Queue (Min Heap)
- Hash Maps
- Recursive DFS Traversals

---

# Algorithm Overview

The compressor follows the standard Huffman Coding pipeline.

```
Input Text
     │
     ▼
Character Frequency Count
     │
     ▼
Priority Queue (Min Heap)
     │
     ▼
Build Huffman Tree
     │
     ▼
Generate Huffman Codes
     │
     ▼
Encode Text
     │
     ▼
Pack Bits into Bytes
     │
     ▼
Compressed Output
```

During decompression:

```
Compressed Data
        │
        ▼
Deserialize Huffman Tree
        │
        ▼
Convert Bytes to Bit Stream
        │
        ▼
Remove Padding Bits
        │
        ▼
Traverse Huffman Tree
        │
        ▼
Original Text
```

---

# Project Structure

```
huffman-compressor/
│
├── main.cpp          # Compression and decompression implementation
├── README.md
```

---

# Implementation Details

## Huffman Tree Construction

- Computes character frequencies
- Uses a priority queue to repeatedly merge the two least frequent nodes
- Produces an optimal prefix code tree

---

## Encoding

The encoder:

- Generates Huffman codes using DFS
- Replaces each character with its binary code
- Packs every 8 bits into a single byte
- Stores:

  - Serialized Huffman Tree
  - Padding length
  - Compressed binary data

---

## Tree Serialization

The Huffman tree is serialized using preorder traversal.

Null children are represented using:

```
#
```

This serialized tree is stored alongside the compressed data to enable reconstruction during decoding.

---

## Decoding

The decoder:

- Reconstructs the Huffman tree
- Converts each stored byte back into an 8-bit binary string
- Removes padding bits
- Traverses the Huffman tree to recover the original text

---

# Technologies

- C++
- STL
- Priority Queue
- Unordered Map
- Binary Trees
- Recursive DFS

---

# Build

Compile using g++:

```bash
g++ main.cpp -o huffman
```

or

```bash
g++ -std=c++17 main.cpp -o huffman
```

---

# Running

```bash
./huffman
```

Windows

```bash
huffman.exe
```

---

# Example

### Input

```
hello world
```

### Output

```
Encoded string:
<compressed binary data>

Decoded string:
hello world
```

The decoded output matches the original input, demonstrating lossless compression.

---

# Complexity Analysis

| Operation | Time Complexity |
|------------|-----------------|
| Frequency Counting | O(n) |
| Huffman Tree Construction | O(k log k) |
| Encoding | O(n) |
| Decoding | O(n) |

where:

- **n** = Length of the input text
- **k** = Number of distinct characters

---

# Concepts Demonstrated

- Huffman Coding
- Greedy Algorithms
- Binary Trees
- Tree Serialization & Deserialization
- Priority Queues
- Recursive Tree Traversals
- Bit Manipulation
- Lossless Data Compression

---

# Future Improvements

Potential enhancements include:

- Compress and decompress files instead of console input
- Support binary file compression
- Canonical Huffman Coding
- Adaptive Huffman Coding
- Compression ratio statistics
- Multi-threaded compression
- Archive support for multiple files
- Command-line interface with compression/decompression modes

---

# License

This project is licensed under the MIT License.
