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

	2.	Run the interactive program:

./huffman


	3.	Follow on-screen prompts to compress or decompress files.

Testing

Adding Custom Tests

Use the round-trip compression-decompression method to validate correctness:
	1.	Compress a file.
	2.	Decompress the result.
	3.	Compare the output with the original file.

Example Usage
	1.	Compress a File:
	•	Input: res/example.txt
	•	Output: res/example.txt.huf
	2.	Decompress a File:
	•	Input: res/example.txt.huf
	•	Output: res/unhuf.example.txt

Additional Notes
	•	Compression efficiency depends on the input file’s redundancy. Files with repeated patterns compress well, while already compressed files may slightly grow in size.
	•	The project ensures no memory leaks using proper tree deallocation and careful memory management.
	•	Non-text files (e.g., images, audio) can also be compressed and decompressed accurately.

Known Limitations
	•	Input Restrictions: The input must contain at least two distinct characters to be Huffman-encodable.
	•	Compression Growth: Huffman encoding may not reduce the size of already compressed files.

Acknowledgments

This project was inspired by the CS106B curriculum and the pioneering work of David Huffman. Special thanks to the teaching team for their warmup exercises and starter code, which facilitated a deeper understanding of the algorithm.

Enjoy compressing and decompressing! 🎉

