#!/bin/sh

# Set variables
SRC_DIR="./src"    
OUT_DIR="./build"          # Source directory
OUTPUT="$OUT_DIR/simple_httpd"        # Output executable name
CC="gcc"                     # Compiler
CFLAGS="-Wall -Wextra -g"    # Compilation flags

# List of source files (space-separated instead of array)
SRC_FILES="main.c server.c config.c request_handler.c response.c utils/scanner.c utils/memory.c"

# Clean up old object files and executable
echo "Cleaning up old build files..."
rm -rf $SRC_DIR/*.o $OUTPUT

if [ ! -d "$OUT_DIR/utils" ]; then
    mkdir $OUT_DIR/utils
fi

# Compile each .c file to an object file and store them in OBJ_FILES
OBJ_FILES=""
for SRC_FILE in $SRC_FILES; do
    OBJ_FILE="${SRC_FILE%.c}.o"
    
    # Compile source file into an object file
    $CC $CFLAGS -c "$SRC_DIR/$SRC_FILE" -o "$OUT_DIR/$OBJ_FILE"
    
    if [ $? -ne 0 ]; then
        echo "Error compiling $SRC_FILE"
        exit 1
    fi
    
    # Add the object file to OBJ_FILES
    OBJ_FILES="$OBJ_FILES $OUT_DIR/$OBJ_FILE"
done

# Link all object files into the final executable
$CC $OBJ_FILES -o "$OUTPUT"

if [ $? -eq 0 ]; then
    echo "Compilation successful. Executable created: $OUTPUT"
else
    echo "Linking failed."
    exit 1
fi
