#include <iostream>
#include "bits.h"
#include "console.h"
#include "filelib.h"
#include "huffman.h"
#include "simpio.h"
#include "strlib.h"
#include "SimpleTest.h"
using namespace std;


void huffmanConsoleProgram();

int main() {
    /*
     * In order to run the console program to compress/decompress whole files
     * respond 0 when asked for which tests, and this falls through
     * to the code below.
     */
    huffmanConsoleProgram();

    cout << "All done, exiting" << endl;
    return 0;
}


/*
 * Sets up the output console and explains the program to the user.
 */
void intro() {
    cout << "This program uses the Huffman coding algorithm for compression." << endl;
    cout << "Any type of file can be encoded using a Huffman code." << endl;
    cout << "Decompressing the result will faithfully reproduce the original." << endl;
}

/*
 * Prints a menu of choices for the user and reads/returns the user's response.
 */
string menu() {
    cout << endl;
    cout << "Your options are:" << endl;
    cout << "C) compress file" << endl;
    cout << "D) decompress file" << endl;
    cout << "Q) quit" << endl;

    cout << endl;
    return toUpperCase(trim(getLine("Enter your choice: ")));
}

const string kCompressedExtension = ".huf";
const string kDecompressedExtension = "unhuf.";

/*
 * Prompts for names of files to use for compress/decompress.
 */
bool getInputAndOutputFiles(string& inFilename, string& outFilename, bool compressing) {
    inFilename = promptUserForFilename("Input file name: ", "No file found with that name. Try again.");
    string defaultName;
    if (compressing) {
        defaultName = inFilename + kCompressedExtension;
    } else {
        string head = getHead(inFilename);
        string tail = getTail(inFilename);
        defaultName = (!head.empty()? head + getDirectoryPathSeparator() : "") + kDecompressedExtension + getRoot(tail);
    }
    outFilename = trim(getLine("Output file name (Enter for " + defaultName + "): "));
    if (outFilename == "") {
        outFilename = defaultName;
    }
    if (inFilename == outFilename) {
        cout << "You cannot specify the same filename as both the input file" << endl;
        cout << "and the output file.  Canceling operation." << endl;
        return false;
    }
    if (fileExists(outFilename)) {
        return getYesOrNo(outFilename + " already exists. Overwrite? (y/n) ");
    }
    return true;
}


string readEntireBinaryFile(string filename) {
    ifstream in(filename, std::ios::binary);
    string str;
    str.assign((std::istreambuf_iterator<char>(in)),
                std::istreambuf_iterator<char>());
    return str;
}

void writeEntireBinaryFile(string filename, string data) {
    ofstream out(filename, std::ios::binary);
    out.write(data.c_str(), data.size());
}

/*
 * Compress a file.
 * Prompts for input/output file names and opens streams on those files.
 * Then calls your compress function and displays information about size of
 * compressed output.
 */
void compressFile() {
    string inFilename, outFilename;

    if (!getInputAndOutputFiles(inFilename, outFilename, true)) {
        return;
    }
    cout << "Reading " << fileSize(inFilename) << " input bytes." << endl;
    try {
        string text = readEntireBinaryFile(inFilename);
        cout << "Compressing ..." << endl;
        EncodedData data = compress(text);
        ofstream out(outFilename, ios::binary);
        writeData(data, out);
    } catch (ErrorException& e) {
        cout << "Ooops! " << e.getMessage() << endl;
    }

    if (fileExists(outFilename)) {
        cout << "Wrote " << fileSize(outFilename) << " compressed bytes." << endl;
    } else {
        cout << "Compressed output file was not found; perhaps there was an error." << endl;
    }
}

/*
 * Decompress a file.
 * Prompts for input/output file names and opens streams on those files.
 * Then calls your decompress function and displays information about size of
 * decompressed output.
 */
void decompressFile() {
    string inFilename, outFilename;

    if (!getInputAndOutputFiles(inFilename, outFilename, false)) {
        return;
    }
    cout << "Reading " << fileSize(inFilename) << " input bytes." << endl;
    try {
        ifstream input(inFilename, ios::binary);
        EncodedData data = readData(input);
        cout << "Decompressing ..." << endl;
        string text = decompress(data);
        writeEntireBinaryFile(outFilename, text);
    } catch (ErrorException& e) {
        cout << "Ooops! " << e.getMessage() << endl;
    }

    if (fileExists(outFilename)) {
        cout << "Wrote " << fileSize(outFilename) << " decompressed bytes." << endl;
    } else {
        cout << "Decompressed output file was not found; perhaps there was an error." << endl;
    }
}

void huffmanConsoleProgram() {
    intro();
    while (true) {
        string choice = menu();
        if (choice == "Q") {
            break;
        } else if (choice == "C") {
            compressFile();
        } else if (choice == "D") {
            decompressFile();
        }
    }
}
