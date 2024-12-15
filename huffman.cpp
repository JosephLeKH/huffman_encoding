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
