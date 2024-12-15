/**
 * Joseph Le
 * huffman.cpp
 * 12/04/24
 *
 * Huffman Encoding Project
 * This project implements Huffman coding to compress and decompress text efficiently.
 * It includes functions for building an optimal Huffman tree, encoding text into a bit sequence,
 * and decoding a bit sequence back into text. Additional helper functions are provided for
 * flattening and reconstructing encoding trees and deallocating memory.
 *
 * The main functionalities include:
 * - `buildHuffmanTree()` to create an encoding tree based on character frequencies.
 * - `compress()` to compress a given text using Huffman coding.
 * - `decompress()` to decode the compressed data back to its original form.
 */
#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "SimpleTest.h"  // IWYU pragma: keep (needed to quiet spurious warning)
#include <string>
using namespace std;

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * The function uses the encoding tree to traverse based on each bit in the queue.
 * Starting from the root of the tree, it moves left or right based on the bits (0 for left, 1 for right).
 * Once a leaf node is reached, the character stored at that node is added to the output string.
 * The traversal then restarts from the root of the tree for the next character.
 *
 * The function assumes the encoding tree is valid and the bit sequence represents a valid encoding
 * according to the tree structure.
 *
 * @param tree The root of the encoding tree used to decode the bits.
 * @param messageBits A queue of bits representing the encoded message to be decoded.
 * @return A string containing the decoded message text.
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    string output = "";
    EncodingTreeNode* node = tree;
    while (!messageBits.isEmpty())
    {
        Bit temp = messageBits.dequeue();
        if (temp == 0)
        {
            node = node->zero;
        }
        else
        {
            node = node->one;
        }

        if (node->isLeaf())
        {
            output = output + node->getChar();
            node = tree;
        }
    }
    return output;
}

/**
 * Helper function to recursively reconstruct an encoding tree from a flattened form.
 *
 * This function is used to rebuild the tree structure by recursively processing a
 * sequence of bits (treeShape) and characters (treeLeaves). The `treeShape` queue
 * determines whether the current node is a leaf (0) or an internal node (1). If it
 * is a leaf node, a character from `treeLeaves` is used to create a leaf node. If
 * it is an internal node, the function recursively creates left and right subtrees.
 *
 * The tree is reconstructed from the queues in a depth-first manner, with nodes
 * being created and linked as the recursive calls return.
 *
 * @param treeShape A queue of bits representing the shape of the tree, where 0 indicates a leaf node
 *                  and 1 indicates an internal node. The first bit of the queue determines the type of the root.
 * @param treeLeaves A queue of characters representing the leaves of the tree. Each leaf character is assigned
 *                   when a 0 bit is encountered in the `treeShape` queue.
 * @return A pointer to the root node of the reconstructed tree or subtree.
 */
EncodingTreeNode* unflattenTreeHelper(Queue<Bit>& treeShape, Queue<char>& treeLeaves)
{
    EncodingTreeNode* output;
    Bit temp = treeShape.dequeue();

    if (temp == 0)
    {
        output = new EncodingTreeNode(treeLeaves.dequeue());
    }
    else
    {
        output = new EncodingTreeNode(unflattenTreeHelper(treeShape, treeLeaves), unflattenTreeHelper(treeShape, treeLeaves));
    }
    return output;
}

/**
 * Reconstructs an encoding tree from its flattened form represented by two queues:
 * one for the tree's shape and one for the tree's leaves.
 *
 * This function calls the helper function `unflattenTreeHelper` to recursively
 * rebuild the tree. It first dequeues a bit from the `treeShape` queue to determine
 * the type of the root (leaf or internal node) and then invokes the helper to
 * recursively construct the subtrees.
 *
 * The function expects the input queues to be well-formed and represent a valid
 * encoding tree. After the reconstruction, the `treeShape` and `treeLeaves` queues
 * will be modified as bits and characters are dequeued during the process.
 *
 * @param treeShape A queue of bits representing the tree's shape, with 0 for leaves and 1 for internal nodes.
 * @param treeLeaves A queue of characters representing the leaves of the tree, used when creating leaf nodes.
 * @return A pointer to the root node of the fully reconstructed encoding tree.
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    treeShape.dequeue();
    EncodingTreeNode* output = new EncodingTreeNode(unflattenTreeHelper(treeShape, treeLeaves), unflattenTreeHelper(treeShape, treeLeaves));
    return output;
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * This function reconstructs the encoding tree using the `treeShape` and `treeLeaves` queues
 * from the `EncodedData` object by calling the `unflattenTree` function. It then decodes the
 * compressed message bits using the reconstructed tree by calling `decodeText`.
 *
 * The `decodeText` function will process the bit stream in the `messageBits` queue, traversing
 * the tree based on each bit, and outputting the decoded characters in sequence.
 *
 * You can assume the input data is well-formed and was created by a correct implementation
 * of compress. This means that the `treeShape` and `treeLeaves` queues represent a valid
 * encoding tree, and the `messageBits` queue contains the correctly compressed message
 * bits.
 *
 * After decompression, the original text is returned as a string.
 *
 * The implementation modifies the `data` parameter (specifically the queues within it)
 * during processing, but the reconstructed encoding tree remains unchanged after the function returns.

 *
 * @param data The encoded data, including the flattened encoding tree and the compressed message bits.
 * @return A string containing the decompressed original message text.
 */
string decompress(EncodedData& data) {
    EncodingTreeNode* root = unflattenTree(data.treeShape, data.treeLeaves);
    string output = decodeText(root, data.messageBits);
    deallocateTree(root);
    return output;
}

/**
 * Constructs an optimal Huffman encoding tree for the given input text.
 *
 * The function counts the frequency of each character in the text and builds
 * a priority queue of leaf nodes. It then constructs the tree by repeatedly
 * dequeuing two trees with the lowest frequencies, creating a new internal node,
 * and enqueuing it back with the combined frequency until a single tree remains.
 *
 * Reports an error if the input text does not contain at least two distinct characters.
 *
 * @param text The input text for which the Huffman tree is built.
 * @return A pointer to the root of the constructed Huffman encoding tree.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    Map<char,int> map;
    for (char c: text)
    {
        map[c]++;
    }

    PriorityQueue<EncodingTreeNode*> pq;
    for (char c: map.keys())
    {
        pq.enqueue(new EncodingTreeNode(c), map[c]);
    }

    EncodingTreeNode* temp1;
    EncodingTreeNode* temp2;
    int sum;
    while (pq.size() > 1)
    {
        sum = pq.peekPriority();
        temp1 = pq.dequeue();
        sum += pq.peekPriority();
        temp2 = new EncodingTreeNode(temp1, pq.dequeue());

        pq.enqueue(temp2, sum);
    }

    return pq.dequeue();
}

/**
 * Recursively traverses the given encoding tree and builds a map that associates each character
 * with its corresponding Huffman encoding (represented as a binary string).
 *
 * This function processes the tree from root to leaf, appending '0' or '1' to the current string
 * (`curr`) based on whether the traversal goes to the left or right child, respectively. When a leaf node
 * is encountered, it adds the character and its encoding to the provided map.
 *
 * The map is updated during the recursion with the Huffman encoding for each character in the tree.
 *
 * @param tree The current node of the encoding tree.
 * @param map A map where the key is the character and the value is its Huffman encoding as a string.
 * @param curr The current binary string representing the path to the current node.
 */
void createTreeMap(EncodingTreeNode* tree, Map<char,string>& map, string curr)
{
    if (tree->isLeaf())
    {
        map.put(tree->getChar(), curr);
    }
    else
    {
        createTreeMap(tree->zero, map, curr + "0");
        createTreeMap(tree->one, map, curr + "1");
    }
}

/**
 * Encodes the given text using the provided encoding tree and returns the resulting encoded bit sequence
 * as a Queue<Bit>. The function uses the Huffman encoding stored in the tree to map each character
 * to its corresponding bit sequence and then appends these bits to form the complete encoded message.
 *
 * The function first constructs a map from the encoding tree by calling the `createTreeMap` function.
 * Then, it iterates over the input text, looking up the encoding for each character in the map, and
 * appending the corresponding bits to the queue.
 *
 * You can assume the tree is a valid non-empty encoding tree and contains an encoding for every character
 * in the input text. The `text` parameter is a string representing the message to be encoded.
 *
 * @param tree The root node of the encoding tree used to encode the text.
 * @param text The input text to be encoded.
 * @return A Queue<Bit> containing the encoded bit sequence corresponding to the input text.
 */
Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    Map<char,string> map;
    createTreeMap(tree, map, "");

    Queue<Bit> queue;
    string temp;
    for (char c: text)
    {
        temp = map.get(c);
        for (char c: temp)
        {
            if (c == '0')
            {
                queue.enqueue(0);
            }
            else
            {
                queue.enqueue(1);
            }
        }
    }
    return queue;
}

/*
 * Function: flattenTree
 * ---------------------
 * Flattens an encoding tree into two queues:
 * - `treeShape` (Queue<Bit>) stores the structure of the tree, where 1 represents an internal node
 *   and 0 represents a leaf node.
 * - `treeLeaves` (Queue<char>) stores the characters at the leaf nodes in the order they appear.
 *
 * The function performs a depth-first traversal of the tree, recursively visiting each node.
 * For internal nodes, it enqueues a 1 in `treeShape` and recursively processes the left and right subtrees.
 * For leaf nodes, it enqueues a 0 in `treeShape` and stores the character at that leaf in `treeLeaves`.
 *
 * The function assumes that the `tree` parameter is a valid, well-formed encoding tree and that both
 * `treeShape` and `treeLeaves` are empty before the function is called.
 *
 * This method is used to create a serialized representation of the tree structure and its leaf characters,
 * which can later be used for encoding or decoding operations.
 *
 * Parameters:
 * - `tree`: The root node of the encoding tree to be flattened.
 * - `treeShape`: The queue to store the flattened tree structure.
 * - `treeLeaves`: The queue to store the characters from the leaf nodes.
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    if (!tree->isLeaf())
    {
        treeShape.enqueue(1);
        flattenTree(tree->zero, treeShape, treeLeaves);
        flattenTree(tree->one, treeShape, treeLeaves);
    }
    else
    {
        treeShape.enqueue(0);
        treeLeaves.enqueue(tree->getChar());
    }
}

/**
 * Compresses the given text using Huffman coding and produces an `EncodedData` object,
 * which contains the encoded message, as well as the flattened form of the encoding tree.
 *
 * The function follows these steps:
 * 1. Builds an optimal Huffman encoding tree for the input text using `buildHuffmanTree()`.
 * 2. Flattens the tree using `flattenTree()` to create queues representing the tree's structure and leaves.
 * 3. Encodes the text using `encodeText()` to produce a bit queue of the encoded message.
 * 4. Deallocates the tree to free up memory.
 *
 * Reports an error if the input text has fewer than two distinct characters.
 *
 * @param messageText The input text to be compressed.
 * @return An `EncodedData` object containing the compressed bit sequence, tree structure, and leaves.
 */
EncodedData compress(string messageText) {
    EncodingTreeNode* tree = buildHuffmanTree(messageText);
    EncodedData output;
    Queue<Bit> shape;
    Queue<char> leaves;
    flattenTree(tree, shape, leaves);
    output.treeShape = shape;
    output.treeLeaves = leaves;
    output.messageBits = encodeText(tree, messageText);
    deallocateTree(tree);

    return output;
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    // Create leaf nodes
    EncodingTreeNode* t = new EncodingTreeNode('T');
    EncodingTreeNode* r = new EncodingTreeNode('R');
    EncodingTreeNode* s = new EncodingTreeNode('S');
    EncodingTreeNode* e = new EncodingTreeNode('E');

    // Create internal nodes
    EncodingTreeNode* rs = new EncodingTreeNode(r, s);
    EncodingTreeNode* rightSubtree = new EncodingTreeNode(rs, e);

    // Create the root node
    EncodingTreeNode* output = new EncodingTreeNode(t, rightSubtree);

    return output;
}

void deallocateTree(EncodingTreeNode* t) {
    if (!t->isLeaf())
    {
        deallocateTree(t->zero);
        deallocateTree(t->one);
    }
    delete t;
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    if (a == nullptr && b == nullptr)
    {
        return true;
    }
    if (a == nullptr || b == nullptr)
    {
        return false;
    }
    if (a->isLeaf() && b->isLeaf())
    {
        return a->getChar() == b->getChar();
    }
    if (!a->isLeaf() && !b->isLeaf())
    {
        return (areEqual(a->zero, b->zero) && areEqual(a->one, b->one));
    }
    return false;
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Write your own student tests. */
STUDENT_TEST("Test createExampleTree and deallocateTree to ensure no memory leaks") {
    // Allocate the tree
    EncodingTreeNode* tree = createExampleTree();

    // Perform basic checks (optional)
    EXPECT(tree != nullptr);
    EXPECT(tree->zero != nullptr);
    EXPECT(tree->one != nullptr);
    EXPECT(tree->zero->ch == 'T');
    EXPECT(tree->one->one->ch == 'E');
    EXPECT(tree->one->zero->zero->ch == 'R');
    EXPECT(tree->one->zero->one->ch == 'S');

    // Clean up the tree
    deallocateTree(tree);
}

STUDENT_TEST("Test that two empty trees are equal") {
    EncodingTreeNode* tree1 = nullptr;
    EncodingTreeNode* tree2 = nullptr;
    EXPECT(areEqual(tree1, tree2));
}

STUDENT_TEST("Test that a non-empty tree is not equal to an empty tree") {
    EncodingTreeNode* tree1 = new EncodingTreeNode('A');
    EncodingTreeNode* tree2 = nullptr;
    EXPECT(!areEqual(tree1, tree2));

    // Clean up
    delete tree1;
}

STUDENT_TEST("Test that two identical simple trees are equal") {
    EncodingTreeNode* tree1 = new EncodingTreeNode(new EncodingTreeNode('A'), new EncodingTreeNode('B'));
    EncodingTreeNode* tree2 = new EncodingTreeNode(new EncodingTreeNode('A'), new EncodingTreeNode('B'));
    EXPECT(areEqual(tree1, tree2));

    // Clean up
    deallocateTree(tree1);
    deallocateTree(tree2);
}

STUDENT_TEST("Test that a tree with swapped children is not equal to the original") {
    EncodingTreeNode* tree1 = new EncodingTreeNode(new EncodingTreeNode('A'), new EncodingTreeNode('B'));
    EncodingTreeNode* tree2 = new EncodingTreeNode(new EncodingTreeNode('B'), new EncodingTreeNode('A'));
    EXPECT(!areEqual(tree1, tree2));

    // Clean up
    deallocateTree(tree1);
    deallocateTree(tree2);
}

STUDENT_TEST("Test that trees with different characters in leaf nodes are not equal") {
    EncodingTreeNode* tree1 = new EncodingTreeNode(new EncodingTreeNode('A'), new EncodingTreeNode('B'));
    EncodingTreeNode* tree2 = new EncodingTreeNode(new EncodingTreeNode('A'), new EncodingTreeNode('C'));
    EXPECT(!areEqual(tree1, tree2));

    // Clean up
    deallocateTree(tree1);
    deallocateTree(tree2);
}

STUDENT_TEST("Test that createExampleTree is not equal to a simple tree") {
    EncodingTreeNode* exampleTree = createExampleTree();
    EncodingTreeNode* simpleTree = new EncodingTreeNode(new EncodingTreeNode('A'), new EncodingTreeNode('B'));
    EXPECT(!areEqual(exampleTree, simpleTree));

    // Clean up
    deallocateTree(exampleTree);
    deallocateTree(simpleTree);
}

STUDENT_TEST("Test that two identical example trees are equal") {
    EncodingTreeNode* tree1 = createExampleTree();
    EncodingTreeNode* tree2 = createExampleTree();
    EXPECT(areEqual(tree1, tree2));

    // Clean up
    deallocateTree(tree1);
    deallocateTree(tree2);
}

STUDENT_TEST("Test that an example tree is not equal to one of its subtrees") {
    EncodingTreeNode* exampleTree = createExampleTree();
    EXPECT(!areEqual(exampleTree, exampleTree->one));  // Comparing the root tree to its right subtree

    // Clean up
    deallocateTree(exampleTree);
}

STUDENT_TEST("decodeText with various message bit sequences") {
    EncodingTreeNode* tree = createExampleTree();
    EXPECT(tree != nullptr);

    // Test with a single character
    Queue<Bit> messageBits = { 0 }; // T
    EXPECT_EQUAL(decodeText(tree, messageBits), "T");

    // Test with a simple sequence
    messageBits = { 1, 0, 0 }; // R
    EXPECT_EQUAL(decodeText(tree, messageBits), "R");

    // Test with multiple characters
    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    // Test with a longer sequence
    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    // Test with an empty message
    messageBits = {}; // Empty queue
    EXPECT_EQUAL(decodeText(tree, messageBits), ""); // Should return an empty string

    // Test with repeated characters
    messageBits = { 0, 0, 0 }; // TTT
    EXPECT_EQUAL(decodeText(tree, messageBits), "TTT");

    // Test with a more complex message
    messageBits = { 1, 1, 1, 1 }; // EE
    EXPECT_EQUAL(decodeText(tree, messageBits), "EE");

    // Clean up
    deallocateTree(tree);
}

STUDENT_TEST("decompress, basic test") {
    // Example compressed data with its corresponding encoding tree and bits.

    // Constructing the encoding tree manually:
    Queue<Bit> treeShape;
    Queue<char> treeLeaves;

    // Encoding tree:
    //     *
    //    / \
    //   A   *
    //      / \
    //     B   C

    // Tree shape (0 for leaf, 1 for internal node)
    treeShape.enqueue(1); // Root is internal
    treeShape.enqueue(0); // Left child is leaf (A)
    treeShape.enqueue(1); // Right child is internal
    treeShape.enqueue(0); // Left child of right subtree is leaf (B)
    treeShape.enqueue(0); // Right child of right subtree is leaf (C)

    // Tree leaves (characters for leaf nodes)
    treeLeaves.enqueue('A');
    treeLeaves.enqueue('B');
    treeLeaves.enqueue('C');

    // Example encoded data: bits for "ABC"
    Queue<Bit> messageBits;
    messageBits.enqueue(0); // A
    messageBits.enqueue(1);
    messageBits.enqueue(0); // B
    messageBits.enqueue(1);
    messageBits.enqueue(1); // C

    // Encoded data object
    EncodedData data;
    data.treeShape = treeShape;
    data.treeLeaves = treeLeaves;
    data.messageBits = messageBits;

    // Perform decompression
    string result = decompress(data);

    // Verify the result matches the original string "ABC"
    EXPECT_EQUAL(result, "ABC");
}

STUDENT_TEST("encodeText and decodeText, basic encoding and decoding") {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    // Set up a simple encoding tree for testing
    EncodingTreeNode* tree = createExampleTree(); // Use the previously tested function for the example tree
    EXPECT(tree != nullptr);

    // Test 1: Encode and decode a simple string
    string text = "SET";  // A string to encode and decode
    Queue<Bit> encodedBits = encodeText(tree, text); // Encode the string using the tree
    string decodedText = decodeText(tree, encodedBits); // Decode the encoded bits using the same tree

    // Check that the decoded text matches the original input
    EXPECT_EQUAL(decodedText, text);

    // Test 2: Encode and decode another string
    text = "STREETS";
    encodedBits = encodeText(tree, text); // Encode the string
    decodedText = decodeText(tree, encodedBits); // Decode the encoded bits

    // Check that the decoded text matches the original input
    EXPECT_EQUAL(decodedText, text);

    // Clean up memory
    deallocateTree(tree);
}

STUDENT_TEST("Test flattenTree and unflattenTree consistency") {
    // Step 1: Create an example encoding tree
    EncodingTreeNode* originalTree = createExampleTree();

    // Step 2: Flatten the tree using flattenTree
    Queue<Bit> treeShape;
    Queue<char> treeLeaves;
    flattenTree(originalTree, treeShape, treeLeaves);

    // Step 3: Reconstruct the tree using unflattenTree
    EncodingTreeNode* reconstructedTree = unflattenTree(treeShape, treeLeaves);

    // Step 4: Check that the original and reconstructed trees are equal
    EXPECT_EQUAL(areEqual(originalTree, reconstructedTree), true);

    // Step 5: Clean up
    deallocateTree(originalTree);
    deallocateTree(reconstructedTree);
}

STUDENT_TEST("buildHuffmanTree with non-repeated frequencies creates a specific valid tree") {
    // Input text with non-repeated frequencies for characters
    string text = "ABBCCCCDDDDDDDDEEEEEEEEEEEEEEEE";  // Frequencies: A=1, B=2, C=4, D=8, E=16

    // Build the Huffman tree
    EncodingTreeNode* tree = buildHuffmanTree(text);

    EncodingTreeNode* a = new EncodingTreeNode('A');
    EncodingTreeNode* b = new EncodingTreeNode('B');
    EncodingTreeNode* c = new EncodingTreeNode('C');
    EncodingTreeNode* d = new EncodingTreeNode('D');
    EncodingTreeNode* e = new EncodingTreeNode('E');

    EncodingTreeNode* ab = new EncodingTreeNode(a, b);
    EncodingTreeNode* abc = new EncodingTreeNode(ab, c);
    EncodingTreeNode* abcd = new EncodingTreeNode(abc, d);
    EncodingTreeNode* expectedTree = new EncodingTreeNode(abcd, e);

    // Check if the built tree matches the expected tree
    EXPECT(areEqual(tree, expectedTree));

    // Deallocate both trees to avoid memory leaks
    deallocateTree(tree);
    deallocateTree(expectedTree);
}

STUDENT_TEST("compress() and decompress() should reconstruct the original text") {
    string originalText = "ABBCCCCDDDDDDDDEEEEEEEEEEEEEEEE";  // Example input

    // Compress the text
    EncodedData compressedData = compress(originalText);

    // Decompress to reconstruct the original
    string decompressedText = decompress(compressedData);

    // Check that the decompressed text matches the original
    EXPECT_EQUAL(decompressedText, originalText);
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman",
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}
