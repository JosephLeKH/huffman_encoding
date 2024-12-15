
## Compilation and Execution  

1. **Compile the project:**  
   ```bash
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
	•	Input Restrictions: Input must contain at least two distinct characters to be Huffman-encodable.
	•	Compression Growth: Huffman encoding does not guarantee size reduction for already compressed files.

Acknowledgments

This project was inspired by the CS106B curriculum and the work of David Huffman.
Thank you to the teaching team for the warmup exercises and starter code, which greatly facilitated understanding and implementation.

Enjoy compressing and decompressing! 🎉

