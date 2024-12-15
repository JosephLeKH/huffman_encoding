# Huffman Coding Project

## Overview  
This project implements **Huffman Coding**, a data compression algorithm devised by David Huffman in 1952. Widely used in applications like HDTV, fax machines, and JPEG file formats, Huffman coding compresses data efficiently using trees and priority queues.  

The program provides a complete compression and decompression pipeline. It encodes files into a compact binary representation using a Huffman tree and can reconstruct the original files without any loss of information. This project showcases a practical, powerful algorithm while demonstrating modern C++ programming capabilities.  

---

## Features  

### File Compression and Decompression  
- Compress files into a compact binary `.huf` format.  
- Decompress files to restore original content.  

### General File Support  
- Supports text, binary, and multimedia files.  
- Handles any input containing at least two distinct characters.  

### End-to-End Testing  
- Enables round-trip testing: compress -> decompress -> verify original.  

### Memory Management  
- Efficient dynamic memory handling with zero memory leaks.  

### Interactive Console Program  
- Provides a user-friendly interface for file compression and decompression.  

---

## How It Works  

### Core Components  

1. **Huffman Tree**  
   - Constructs an optimal binary tree using a priority queue to represent character frequencies.  
   - Maps each character to a unique binary sequence, with shorter sequences for more frequent characters.  

2. **Compression**  
   - **Encoding:** Converts characters into binary sequences based on the Huffman tree.  
   - **Flattening:** Serializes the Huffman tree for future decompression.  

3. **Decompression**  
   - **Unflattening:** Reconstructs the Huffman tree from its serialized format.  
   - **Decoding:** Converts binary sequences back into original characters.  

4. **File Handling**  
   - Compresses data into `.huf` files.  
   - Restores original data from `.huf` files while preserving the file type and content.  

---

## File Descriptions  

### Core Implementation  

- **huffman.cpp** and **huffman.h**:  
  Implements core Huffman functions, including tree construction, compression, and decompression.  

- **bits.cpp** and **bits.h**:  
  Manages bit-level operations, such as reading and writing bits to streams.  

- **treenode.h**:  
  Defines the `EncodedTreeNode` structure for Huffman tree nodes.  

- **priorityqueue.h**:  
  Provides a priority queue for building the Huffman tree efficiently.  

### Helper Files  

- **main.cpp**:  
  A console-based interface for testing compression and decompression functionality.  

---

## Getting Started  

### Prerequisites  
- A C++ compiler (e.g., `g++`, `clang++`)  
- A development environment capable of running C++ projects.  

### Compilation and Execution  
1. **Compile the project:**  
   ```bash
   make
