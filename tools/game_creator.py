#!/bin/sh
import struct
import sys

output_data = []

def read_bof_file(input_file):
    print("Boundless Fusion binary file detected. You don't need to do anything to this file.")

def read_jar_file(input_file):
    print("Jar file detected, use of these files is not yet supported.")

def read_dll_file(input_file):
    print(".NET Framework DLL file detected, use of these files is not yet supported.")

file_formats = {
    "bof": read_bof_file,
    "jar": read_jar_file,
    "dll": read_dll_file,
}

game_version = 1
game_magic = 0x424F4647  # BOFG
SECTION_PROGRAM = 1

game_section_t = struct.Struct('B I')
game_t = struct.Struct('I B B')

def write_game_file(file):
    # Write the game_t header
    header = game_t.pack(game_magic, game_version, len(output_data))
    file.write(header)

    section_data = []

    for section in output_data:
        # Write the game_section_t sections
        section_header = game_section_t.pack(section[0], len(section[1]))
        file.write(section_header)

        section_data.append(section[1])

    for section in section_data:
        # Write the data for each section
        file.write(section)

def main():
    #Parse command line arguments
    if len(sys.argv) != 3:
        print("Usage: python game_creator.py <input_file> <output_file>")
        return
    
    input_file = sys.argv[1]
    input_file_extension = input_file.split(".")[-1]

    output_file = sys.argv[2]

    #Read the input file using the file_formats dictionary
    if input_file_extension in file_formats:
        with open(input_file, 'rb') as f:
            file_formats[input_file_extension](f)
    else:
        print("Error: file format not supported")
        return
    
    #Write the output data to the output file
    if len(output_data) == 0:
        print("Hmm, something went wrong.")
        return
    else:
        with open(output_file, 'w') as f:
            write_game_file(f)
        print("Successfully wrote to output file")
        return

if __name__ == '__main__':
    main()
