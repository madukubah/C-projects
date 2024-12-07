#!bin/sh

SRC_DIR="./src"
TEST_DIR="./tests"
OUT_DIR="./tests"
OUTPUT="test_runner"
CC="gcc"
CFLAGS="-Wall -Wextra -g"

SRC_FILES="finite.c dfa.c nfa.c utils/scanner.c utils/parser.c"
TEST_FILES="test_regex.c"

echo "Cleaning up old build files..."
rm -rf  $TEST_FILES/*.o

if [ ! -d "$OUT_DIR" ]; then
    mkdir "$OUT_DIR"
fi

if [ ! -d "$OUT_DIR/utils" ]; then
    mkdir "$OUT_DIR/utils"
fi


OBJ_FILES=""
for TEST_FILE in $TEST_FILES; do
    OBJ_FILE="${TEST_FILE%.c}.o"

    $CC $CFLAGS -c "$TEST_DIR/$TEST_FILE" -o "$OUT_DIR/$OBJ_FILE"

    if [ $? -ne 0 ]; then
        echo "Error compiling $TEST_FILE"
        exit 1
    fi

    OBJ_FILES="$OBJ_FILES $OUT_DIR/$OBJ_FILE"
done

for SRC_FILE in $SRC_FILES; do
    OBJ_FILE="${SRC_FILE%.c}.o"

    $CC $CFLAGS -c "$SRC_DIR/$SRC_FILE" -o "$OUT_DIR/$OBJ_FILE"

    if [ $? -ne 0 ]; then
        echo "Error compiling $SRC_FILE"
        exit 1
    fi

    OBJ_FILES="$OBJ_FILES $OUT_DIR/$OBJ_FILE"
done

$CC $OBJ_FILES -o "$OUT_DIR/$OUTPUT"

if [ $? -ne 0 ]; then
    echo "Linking failed."
    exit 1
else
    echo "Compilation successful. executable created: $OUT_DIR/$OUTPUT"
fi

./"$OUT_DIR/$OUTPUT"
