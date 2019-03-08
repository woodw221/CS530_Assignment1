# Assignment 1

William Wood

woodw221@gmail.com

## Purpose

Familiarize ourselves with ELF files. The programs create various CRC32 Checksums
for potential use in the future.

## Program Descriptions

### Program 1-1

This program takes a single command line argument of a target ELF file and creates
a crc32 checksum of the file. The program first generates a crc32 checksum table
then reads the entire file into a buffer then generates the checksum from the contents
of the binary file.

### Program 1-2

This program takes a single command line argument, a target ELF file, and creates
a crc32 checksum of the program header. As the program above the program creates a 
crc32 checksum table, then reads the entire file in binary into a buffer. From there
the program header offset and size is calculated. The header is then copied into 
another array where the checksum is calculated then returned.

### Program 1-3

Ths program takes a two command line arguments, a target ELF file and a section name,
and generates a crc32 checksum of the section header based on the name given. Just like
the other programs, a crc32 checksum table is created then the file in binary is read
into a buffer. The section header offset, size, number of headers, string table location,
and the offsets for string locations are calculated. We use this information to find the
name of each section and compare it to the given section name from the command line and
generate a crc32 checksum if the names match.

## Compiling

Compile each with gcc and run accordingly.

## Usage

Usage based on example file given to us

./prog1_1 hello // returns 4652481b

./prog1_2 hello // returns fb93e9b0

./prog1_3 hello .shstrtab // returns 6ecaabeb

