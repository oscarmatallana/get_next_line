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

---

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
