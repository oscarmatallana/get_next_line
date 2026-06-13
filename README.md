_This project has been created as part of the 42 curriculum by omatalla._

# get_next_line

## Description

get_next_line is one of the core projects of the 42 Curriculum. Its objective is to implement a function capable of reading and returning a file descriptor's content one line at a time.

The project introduces students to low-level file I/O, static variables, dynamic memory allocation, and state persistence across function calls.

The mandatory part requires the implementation of:

```c
char	*get_next_line(int fd);
```

which returns one complete line per call, including the terminating newline character when present.

The function must work with:

* Regular files
* Standard input (stdin)
* Different BUFFER_SIZE values
* Files of arbitrary length

The implementation must correctly preserve unread data between calls and avoid memory leaks.

By reading this document carefully, you will understand that in a sense, the entire project can be summarized by a single sentence:

*get_next_line() is a state machine whose state is stored in line_container (or stash).* 

Every function either:
- creates state
- modifies state
- uses state
- or destroys state

I hope these very detailed explanations are helpful to you, all the best!

---

## Project Goals

The purpose of get_next_line is to:

* Understand how file descriptors work in Unix systems.
* Learn how the read() system call behaves.
* Understand the role of static variables.
* Practice dynamic memory management.
* Learn how to preserve program state between function calls.
* Develop robust error handling.
* Handle edge cases involving files, newlines and end-of-file conditions.
* Improve debugging and testing skills.

---

## Skills Acquired

Through this project I practiced:

* File descriptor manipulation
* Static variables
* Dynamic memory allocation
* Memory ownership management
* String processing
* Defensive programming
* Error handling
* Resource cleanup
* Algorithm design
* Software testing and validation
* Documentation and code organization

---

## Features

### Mandatory Requirements

The implementation:

* Reads one line at a time.
* Includes the terminating newline when present.
* Returns the last line correctly even when it does not end with a newline.
* Works with files and standard input.
* Handles arbitrary BUFFER_SIZE values.
* Preserves unread data between calls.
* Returns NULL when there is nothing left to read or when an error occurs.
* Frees allocated memory when no longer needed.

---

## Helper Functions

The project is organized around several helper functions responsible for specific tasks.

### Main Functions

| Function              | Purpose                                         |
| --------------------- | ----------------------------------------------- |
| get_next_line         | Main function exposed to the user               |
| read_and_store        | Reads data until a complete line is available   |
| extract_line          | Extracts the line that will be returned         |
| update_line_container | Preserves unread data for future calls          |
| append_buffer         | Appends newly read data to the stored container |

### Utility Functions

| Function   | Purpose                                  |
| ---------- | ---------------------------------------- |
| ft_strlen  | Calculates string length                 |
| ft_memcpy  | Copies memory blocks                     |
| ft_strdup  | Duplicates strings                       |
| ft_strchr  | Searches for a character inside a string |
| ft_strjoin | Concatenates two strings                 |

---

## Project Structure

### get_next_line.c

- get_next_line()

- read_and_store()

- extract_line()

- update_line_container()

- append_buffer()

### get_next_line_utils.c

- ft_strlen()

- ft_memcpy()

- ft_strdup()

- ft_strchr()

- ft_strjoin()

---

# Algorithm and Technical Design

## Overview

The implementation follows a container-based design.

A static string called line_container stores all unread data between function calls.

Every call to get_next_line() follows the same sequence:

File Descriptor -> read_and_store() -> line_container --> extract_line() --> update_line_container() -> Returned Line

---

## The Static Variable

The key element of the project is:

```c
static char *line_container;
```

Unlike a regular local variable, a static variable keeps its value between function calls.

This allows get_next_line() to remember unread data after returning a line.

Example:

File content:

```text
Hello
World
```

After the first call:

Returned:

```text
Hello\n
```

Stored:

```text
World\n
```

The next call continues reading exactly where the previous call stopped.

---

## Step 1: Reading and Storing Data

### read_and_store()

This function guarantees that enough data exists to build a complete line.

Algorithm:

1. Allocate a temporary buffer.
2. Initialize line_container if necessary.
3. Read BUFFER_SIZE bytes at a time.
4. Append newly read bytes to line_container.
5. Stop reading when:

   * A newline is found.
   * End-of-file is reached.

Flow:

line_container -> read() -> buffer -> append_buffer() -> line_container grows


The function never reads more than necessary to obtain the next line.

---

## Step 2: Extracting the Line

### extract_line()

Once enough data exists inside line_container:

```text
Hello\nWorld\n
```

extract_line() calculates the exact length of the next line.

Result:

```text
Hello\n
```

Algorithm:

1. Find newline or end-of-string.
2. Calculate final line length.
3. Allocate memory.
4. Copy the line.
5. Null-terminate the string.
6. Return the extracted line.

This function never modifies the container.

---

## Step 3: Updating the Container

### update_line_container()

After returning:

```text
Hello\n
```

the container still contains:

```text
Hello\nWorld\n
```

The consumed line must be removed.

Algorithm:

1. Locate the end of the returned line.
2. Move past the newline if present.
3. Duplicate the remaining unread portion.
4. Free the old container.
5. Return the updated container.

Example:

Before:

```text
Hello\nWorld\n
```

After:

```text
World\n
```

This preserves the state needed for the next call.

# Understanding `read()` and the Buffer

Before understanding how `get_next_line()` extracts lines, it is important to understand how data enters the program in the first place.

The two key tools are:

```c
buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
```

and

```c
bytes_read = read(fd, buffer, BUFFER_SIZE);
```

Together, they form the bridge between the file on disk and the data stored in memory.

---

# What is a Buffer?

A buffer is simply a temporary storage area in memory.

Think of it as a small bucket that temporarily holds data read from a file.

```text
File
 │
 ▼
Buffer
 │
 ▼
line_container
```

The buffer is not intended to store the entire file.

Its job is only to hold the most recently read chunk of data.

---

# Creating the Buffer

In my implementation:

```c
buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
```

Suppose:

```c
BUFFER_SIZE = 5;
```

Then:

```c
malloc(6);
```

allocates memory for:

```text
+---+---+---+---+---+---+
| ? | ? | ? | ? | ? | ? |
+---+---+---+---+---+---+
```

Five positions are used for the characters read from the file.

The extra position is reserved for:

```c
'\0'
```

which allows the buffer to behave like a normal C string.

---

# What Does `read()` Do?

The prototype is:

```c
ssize_t read(int fd, void *buffer, size_t count);
```

Meaning:

```text
Read up to count bytes
from fd
and place them into buffer.
```

In my implementation:

```c
bytes_read = read(fd, buffer, BUFFER_SIZE);
```

Example:

```text
File:
Hello
World
```

with:

```c
BUFFER_SIZE = 5;
```

---

# First Read

```c
bytes_read = read(fd, buffer, 5);
```

Result:

```text
buffer

+---+---+---+---+---+
| H | e | l | l | o |
+---+---+---+---+---+
```

and:

```c
bytes_read == 5
```

meaning:

```text
Five bytes were successfully read.
```

---

# Adding the Null Terminator

Immediately afterwards:

```c
buffer[bytes_read] = '\0';
```

becomes:

```text
+---+---+---+---+---+----+
| H | e | l | l | o | \0 |
+---+---+---+---+---+----+
```

Now:

```c
buffer
```

can safely be used by string functions such as:

```c
ft_strjoin()
ft_strlen()
ft_strchr()
```

---

# Second Read

The file descriptor remembers its position automatically.

The next call:

```c
bytes_read = read(fd, buffer, 5);
```

does not start at the beginning again.

Instead:

```text
buffer

+---+---+---+---+---+
| \n| W | o | r | l |
+---+---+---+---+---+
```

The file descriptor has advanced.

This behavior is one of the most important concepts in the project.

```text
read()
does not forget
where it stopped.
```

---

# What Does `bytes_read` Mean?

The return value of `read()` tells us what happened.

### Positive Number

```c
bytes_read = 5;
```

means:

```text
Five bytes were successfully read.
```

---

### Zero

```c
bytes_read = 0;
```

means:

```text
End Of File (EOF)
```

No more data remains to be read.

---

### Negative Number

```c
bytes_read = -1;
```

means:

```text
An error occurred.
```

This is why the code checks:

```c
if (bytes_read < 0)
```

before continuing.

---

# Why Not Read the Entire File?

The subject explicitly requires that we read only as much as necessary.

Suppose:

```text
File:
Hello
World
42
```

When the first newline is found:

```text
Hello\n
```

we already have enough information to return the first line.

Reading the rest of the file immediately would waste memory and violate the spirit of the project.

Instead:

```text
read()
      ↓
buffer
      ↓
line_container
      ↓
newline found
      ↓
stop reading
```

The remaining content will be processed during future calls to `get_next_line()`.

---

# How the Buffer and `line_container` Work Together

The buffer is temporary.

The container is persistent.

```text
buffer
  │
  │ read()
  ▼

"Hello"

  │
  │ append_buffer()
  ▼

line_container

"Hello"
```

Next read:

```text
buffer

"\nWorl"
```

After appending:

```text
line_container

"Hello\nWorl"
```

The buffer may be reused or destroyed.

The container survives.

This is the key difference:

```text
buffer
    = temporary storage

line_container
    = persistent unread data
```

---

# The Most Important Insight

At the beginning I thought the buffer stores the file.

It does not.

The buffer only stores the most recently read chunk of data.

The true state of the program lives inside:

```c
static char *line_container;
```

The buffer is simply a temporary messenger that transports data from the file descriptor to the container.

```text
File
 │
 ▼
read()
 │
 ▼
buffer
 │
 ▼
append_buffer()
 │
 ▼
line_container
 │
 ▼
extract_line()
 │
 ▼
Returned Line
```

Understanding this distinction is one of the key learning objectives of the entire get_next_line project.

_The buffer is a temporary messenger. The container is the memory of the program. The functions I wrote manipulate that memory (or state) with each call._

---

# Following a Line Through the Program

One of the best ways to understand get_next_line() is to follow the journey of the data as it moves through the program.

Rather than thinking about individual functions, think about the life cycle of a line from the moment it exists in a file until it is returned to the caller.

---

## Big Picture

When a user calls:

```c
line = get_next_line(fd);
```

the program performs three major tasks:

1. Read enough data to guarantee that a complete line exists.
2. Extract the next line to return.
3. Preserve any remaining unread data for future calls.

Visually:

```text
File
 │
 ▼
read_and_store()
 │
 ▼
line_container
 │
 ├── extract_line()
 │         │
 │         ▼
 │    Returned Line
 │
 └── update_line_container()
           │
           ▼
   Remaining Unread Data
```

The most important variable is:

```c
static char *line_container;
```

Everything revolves around it.

---

# Example File

Suppose the file contains:

```text
Hello
World
42
```

Internally:

```text
Hello\nWorld\n42
```

Let's follow what happens.

---

# First Call

User calls:

```c
get_next_line(fd);
```

Initially:

```text
line_container = NULL
```

---

## Step 1: read_and_store()

Since the container is NULL:

```text
line_container = ""
```

The function begins reading.

Suppose:

```text
BUFFER_SIZE = 5
```

First read:

```text
buffer = "Hello"
```

Container becomes:

```text
line_container = "Hello"
```

No newline yet.

Read again:

```text
buffer = "\nWorl"
```

Container becomes:

```text
line_container =
"Hello\nWorl"
```

A newline has been found.

Reading stops immediately.

Current state:

```text
line_container
│
▼
"Hello\nWorl"
```

---

## Step 2: extract_line()

Now we create the line that will be returned.

Input:

```text
"Hello\nWorl"
```

The function finds:

```text
Hello\n
```

and allocates:

```text
line = "Hello\n"
```

Current state:

```text
line_container
│
▼
"Hello\nWorl"

returned line
│
▼
"Hello\n"
```

---

## Step 3: update_line_container()

The returned line has been consumed.

Only the unread part should remain.

Before:

```text
"Hello\nWorl"
```

After:

```text
"Worl"
```

Current state:

```text
line_container
│
▼
"Worl"
```

The function returns:

```text
"Hello\n"
```

to the user.

---

# Second Call

User calls:

```c
get_next_line(fd);
```

again.

Notice:

```text
line_container = "Worl"
```

The program remembers where it stopped.

This is the purpose of the static variable.

---

## Step 1: read_and_store()

Current container:

```text
"Worl"
```

Read:

```text
buffer = "d\n42"
```

Append:

```text
line_container =
"World\n42"
```

A newline is found.

Reading stops.

---

## Step 2: extract_line()

Input:

```text
"World\n42"
```

Extract:

```text
"World\n"
```

---

## Step 3: update_line_container()

Before:

```text
"World\n42"
```

After:

```text
"42"
```

Current state:

```text
line_container
│
▼
"42"
```

Returned:

```text
"World\n"
```

---

# Third Call

Current container:

```text
"42"
```

read_and_store() continues reading.

The file has reached EOF.

Container remains:

```text
"42"
```

extract_line() returns:

```text
"42"
```

update_line_container() discovers that no unread data remains:

```text
line_container = NULL
```

Returned:

```text
"42"
```

---

# Fourth Call

Current state:

```text
line_container = NULL
```

read_and_store() immediately reaches EOF.

No line can be extracted.

Result:

```c
NULL
```

The reading process is complete.

---

# The Evolution of line_container

The easiest way to understand the project is to watch how line_container changes.

```text
Initial state
NULL

        ↓

After first read
"Hello\nWorl"

        ↓

After returning "Hello\n"
"Worl"

        ↓

After second read
"World\n42"

        ↓

After returning "World\n"
"42"

        ↓

After returning "42"
NULL
```

Notice that:

```text
line_container never stores
already returned data.

It only stores
unread data.
```

This is the central idea of the entire project.

---

# The Role of Each Function

Think of the project as a small team.

### read_and_store()

Responsibility:

```text
Make sure enough data exists
to build a complete line.
```

---

### extract_line()

Responsibility:

```text
Create the line that will be
returned to the caller.
```

---

### update_line_container()

Responsibility:

```text
Remove the returned line and
preserve only unread data.
```

---

### append_buffer()

Responsibility:

```text
Grow the container safely
after each read.
```

---

### get_next_line()

Responsibility:

```text
Coordinate the entire process.
```

It does not perform the work itself.

It simply orchestrates the collaboration between the other functions.

---

# The Most Important Insight

The project is not really about reading files.

It is about managing state.

The challenge is remembering what has already been returned and what still remains unread.

The static variable solves this problem by preserving information between function calls.

Without:

```c
static char *line_container;
```

every call would start from scratch and the function would never know where it left off.

That single variable is what allows get_next_line() to behave like a bookmark inside a file.


# Bonus Part: Managing Multiple File Descriptors

The bonus part extends the original design by allowing get_next_line() to read from multiple file descriptors simultaneously without losing track of the reading state of any file.

## The Problem

In the mandatory implementation, a single static variable is used to preserve unread data:

```c
static char *line_container;
```

This works perfectly when reading from only one file descriptor at a time.

However, imagine the following sequence:

```c
get_next_line(fd1);
get_next_line(fd2);
get_next_line(fd1);
get_next_line(fd2);
```

With only one static container, the unread data from one file descriptor would overwrite the unread data of another. The function would lose track of where each file was being read.

## The Solution

The bonus implementation keeps the same overall algorithm but gives each file descriptor its own dedicated container:

```c
static char *line_container[1024];
```

Each file descriptor stores its unread data in a separate position of the array:

```text
fd 3 -> line_container[3]
fd 4 -> line_container[4]
fd 5 -> line_container[5]
```

As a result, each file descriptor maintains its own independent reading state.

## Example

Suppose two files contain:

```text
File A:
Hello
World

File B:
Oscar
42
```

The following sequence becomes possible:

```c
get_next_line(fdA); // returns "Hello\n"
get_next_line(fdB); // returns "Oscar\n"
get_next_line(fdA); // returns "World\n"
get_next_line(fdB); // returns "42\n"
```

Each file continues exactly where the previous call stopped.

## Key Learning Points

The bonus part demonstrates an important software engineering principle:

_Different resources often require independent state management._

Instead of redesigning the entire algorithm, the solution simply changes where the state is stored.

The helper functions remain unchanged because they already operate on the container passed as an argument. Only the ownership of the container changes.

This bonus reinforced several important concepts:

* Persistent state across function calls.
* File descriptor management.
* Scalable program design.
* Separation of responsibilities between functions.
* Reusing existing code by changing data structures rather than rewriting algorithms.

Perhaps the most valuable lesson is that a small change in data organization can significantly increase the capabilities of a program while preserving the original logic.

## Memory Management Strategy

Memory ownership is carefully controlled throughout the project.

### append_buffer()

Whenever new data is appended:

```c
tmp_line_container = ft_strjoin(line_container, buffer);
free(line_container);
```

The previous container is immediately freed after creating the updated version.

This prevents accumulation of abandoned memory blocks.

---

### Read Errors

If read() fails:

```c
read(...) < 0
```

both:

* buffer
* line_container

are freed before returning NULL.

This prevents leaks during unexpected failures.

---

### End of File

When no unread data remains:

```c
free(line_container);
return (NULL);
```

The container is destroyed and no memory remains allocated.

---

### Allocation Failures

All malloc() calls are verified.

If allocation fails:

```c
if (!pointer)
	return (NULL);
```

the function exits safely without dereferencing invalid pointers.

---

## Edge Cases

### Empty File

Input:

```text
```

Result:

```c
NULL
```

No memory leak occurs.

---

### File Without Final Newline

Input:

```text
Hello
```

Result:

First call:

```text
Hello
```

Second call:

```c
NULL
```

The final line is returned correctly even without '\n'.

---

### Consecutive Empty Lines

Input:

```text


```

Result:

```text
\n
\n
\n
```

Each empty line is treated as a valid line.

---

### BUFFER_SIZE = 1

The implementation correctly handles the smallest practical buffer size.

The line is assembled through multiple reads and concatenations.

---

### Very Large BUFFER_SIZE

The implementation also works with extremely large buffer sizes.

The algorithm remains logically correct regardless of the value provided through:

```bash
-D BUFFER_SIZE=n
```

---

### Standard Input

Example:

```c
get_next_line(0);
```

The implementation works correctly with keyboard input and other standard input streams.

---

## Repository

Clone the repository using:

```bash
git clone https://github.com/oscarmatallana/get_next_line.git
cd get_next_line
```

---

## Instructions

### Compile

Example:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
get_next_line.c get_next_line_utils.c main.c
```

---

## Usage Example

Include the header:

```c
#include "get_next_line.h"
```

Example:

```c
#include <fcntl.h>
#include <stdlib.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("example.txt", O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}
```

---

## Testing

The project was tested using:

### Custom Tests

* Empty files
* Single-character files
* Files with only newlines
* Files without a final newline
* Multiple-line files
* Very long lines
* Standard input
* Invalid file descriptors

### Different BUFFER_SIZE Values

```text
1
10
42
9999
10000000
```

### Community Testers

Tripouille gnlTester

https://github.com/Tripouille/gnlTester

https://github.com/RobertChesters/getnextline/blob/main/tester_gnl_rc.c

---

## Resources

### Documentation

Linux Manual Pages

```bash
man 2 read
```

```bash
man 2 open
```

```bash
man 3 malloc
```

### Books

The C Programming Language - Brian Kernighan & Dennis Ritchie

C Programming For Dummies - Dan Gookin

### Useful References

https://www.geeksforgeeks.org/c/static-variables-in-c/

https://en.cppreference.com/w/c/memory

https://www.youtube.com/watch?v=qsomkTmFQkA

---

## AI Usage

AI tools were used as supplementary learning resources during the development of this project.

The AI was primarily used for:

* Visualizing the behavior of static variables.
* Understanding the interaction between read() and file descriptors.
* Exploring memory ownership and leak prevention strategies.
* Discussing edge cases involving EOF and newline handling.
* Reviewing testing methodologies.
* Improving project documentation and explanations.
* Running conceptual reviews before validation with community testers.

All source code, implementation decisions, debugging, compilation, testing, and final validation were completed and verified by the author.

---

## Author

Oscar Matallana

42 Berlin - omatalla

Email: [omatalla@student.42berlin.de](mailto:omatalla@student.42berlin.de)

GitHub: https://github.com/oscarmatallana
