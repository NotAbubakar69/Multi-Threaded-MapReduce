# Multi-Threaded-MapReduce

## Overview

This project is a basic implementation of a MapReduce-style framework, demonstrating concurrent execution and synchronization for parallel data processing. The system simulates a distributed data processing environment using pthreads in C.

## Objective

To build a simplified MapReduce framework capable of processing input text and counting word occurrences using multiple threads for mapping, shuffling, and reducing.

## Key Components

* *Map Phase*: Processes chunks of input data in parallel and emits key-value pairs.
* *Shuffle Phase*: Groups identical keys together for reduction.
* *Reduce Phase*: Aggregates values for each key to produce the final output.

## System Design

* *Concurrency*: Uses POSIX threads (pthreads) for parallelism.
* *Synchronization*: Mutexes and semaphores ensure thread-safe operations.
* *Thread Types*:

  * Mappers: Parse and emit key-value pairs.
  * Shufflers: Sort and group intermediate results.
  * Reducers: Aggregate word counts.

## Data Structures

c
struct KeyValuePair {
    char word[MAX_WORD_LENGTH];
    int count;
};

struct MapperArgs {
    char** input_data;
    int data_size;
    int mapper_id;
};

struct ReducerArgs {
    struct KeyValuePair* intermediate_data;
    int data_size;
    int reducer_id;
};


## Implementation Phases

### Phase 1: Mapping

* Input is chunked and each chunk is processed by a mapper thread.
* Preprocessing includes punctuation removal and case normalization.

### Phase 2: Shuffling

* Key-value pairs are grouped and sorted by key.

### Phase 3: Reducing

* Grouped data is distributed to reducer threads which sum word counts.

## Synchronization and Concurrency

* pthread_mutex_t global_mutex: Protects shared resources.
* Semaphores for signaling between mappers and reducers.

## Input Handling

* Accepts both manual entry and file-based input.
* Preprocessing includes tokenization, case normalization, and punctuation removal.

## Diagrams

### Sequence Diagram

* Describes the interactions between mapping, shuffling, and reducing threads.

### Flow Chart

* Describes the full data flow from input handling through to final output.

## Output Example

For input: "Hello my name is Qasim. My group members also have names. Group has members"

* *Mapper Output*:

  * ("hello", 1), ("my", 1), ("name", 1), ...
* *Shuffler Output*:

  * ("my", [1, 1]), ("group", [1, 1])
* *Reducer Output*:

  * ("my", 2), ("group", 2)

## Test Cases

### 1. Single Word Input

* Input: "hello"
* Output: ("hello", 1)

### 2. Multiple Words

* Input: "apple banana apple apple banana"
* Output: ("apple", 3), ("banana", 2)

### 3. Mixed Case Words

* Input: "Hello HELLO hello world World"
* Output: ("hello", 3), ("world", 2)

### 4. Numeric Values

* Input: "42 test 42 hello 007 test 42"
* Output: ("007", 1), ("42", 3), ("test", 2), ("hello", 1)

### 5. Symbols and Special Characters

* Input: "code++ python@ java# c"
* Output: ("code", 1), ("python", 1), ("java", 1), ("c", 1)

### 6. Long Sentence

* Input: "The quick brown fox jumps over the lazy dog. The dog barks. The fox runs away."
* Output: ("the", 4), ("dog", 2), ("fox", 2), ...

### 7. Very Large Text Input

* Handles large files by chunking and parallel processing.
* Output: Word counts for entire dataset.

## Technologies Used

* C with pthreads for concurrency
* Standard C libraries for input/output and memory handling

## How to Run

1. Compile the program using gcc -pthread -o mapreduce main.c.
2. Run the executable: ./mapreduce input.txt
3. View the word count output in the terminal or output file.
