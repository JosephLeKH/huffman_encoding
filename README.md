Huffman Coding Project

Overview

This project implements Huffman Coding, a data compression algorithm devised by David Huffman in 1952. Huffman coding is widely used in data compression for applications such as HDTV, fax machines, and file formats like JPEG. By leveraging trees and priority queues, this program can compress and decompress files effectively.

The implementation provides a complete compression and decompression pipeline. It encodes files into a compact representation using a Huffman tree and can reconstruct the original files with no loss of information. This project demonstrates a powerful and elegant algorithm for practical use in data handling and showcases the capabilities of modern C++ programming.

Features
	•	File Compression and Decompression:
	•	Compress files into a compact binary format.
	•	Decompress files to restore the original content.
	•	General File Support:
	•	Handles text, binary, and multimedia files.
	•	Works with any input containing at least two distinct characters.
	•	End-to-End Testing:
	•	Supports comprehensive testing with round-trip compression and decompression.
	•	Memory Management:
	•	Efficient handling of dynamic memory with no leaks.
	•	Interactive Console Program:
	•	A user-friendly interface to compress and decompress files.

How It Works

Core Components
	1.	Huffman Tree:
	•	Constructs an optimal binary tree using a priority queue to represent character frequencies.
	•	Each character is mapped to a unique binary sequence, with shorter sequences for more frequent characters.
	2.	Compression:
	•	Encoding: Converts characters into binary sequences based on the Huffman tree.
	•	Flattening: Stores the Huffman tree in a serialized format for future decompression.
	3.	Decompression:
	•	Unflattening: Reconstructs the Huffman tree from its serialized format.
	•	Decoding: Translates binary sequences back into the original characters.
	4.	File Handling:
	•	Compresses data into .huf files.
	•	Restores original data from .huf files, preserving the file type and content.

File Descriptions

Core Implementation
	•	huffman.cpp and huffman.h:
	•	Implements all core Huffman functions, including tree construction, compression, and decompression.
	•	bits.cpp and bits.h:
	•	Manages bit-level operations, including reading and writing bits to streams.
	•	treenode.h:
	•	Defines the EncodedTreeNode structure used for Huffman tree nodes.
	•	priorityqueue.h:
	•	Provides a priority queue implementation for building the Huffman tree.

Helper Files
	•	main.cpp:
	•	A console-based interface for testing compression and decompression functionality.

Getting Started

Prerequisites
	•	A C++ compiler (e.g., g++, clang++)
	•	A development environment capable of running C++ projects.

Compilation and Execution
	1.	Compile the project:

make


	2.	Run the interactive program:

./huffman


	3.	Follow the on-screen prompts to compress or decompress files.

Testing
Adding Custom Tests
	•	Use the round-trip compression-decompression method to validate correctness:
	•	Compress a file.
	•	Decompress the result.
	•	Compare with the original file.

Example Usage
	1.	Compress a File:
	•	Input: res/example.txt
	•	Output: res/example.txt.huf
	2.	Decompress a File:
	•	Input: res/example.txt.huf
	•	Output: res/unhuf.example.txt

Additional Notes
	•	Compression efficiency depends on the input file’s redundancy. Files with repeated patterns compress well, while already compressed files may grow slightly in size.
	•	The project ensures no memory leaks using proper tree deallocation and careful memory management.
	•	Non-text files like images and audio can be compressed and decompressed accurately.

Known Limitations
	•	Input must contain at least two distinct characters to be Huffman-encodable.
	•	Huffman encoding does not guarantee size reduction for already compressed files.

Acknowledgments

This project was inspired by the CS106B curriculum and the work of David Huffman. Thank you to the teaching team for the warmup exercises and starter code, which greatly facilitated understanding and implementation.

Enjoy compressing and decompressing! 🎉
