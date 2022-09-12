#!/user/bin/bash

#Task1.sh - Cesar Quitazol

#Read Files
echo "Reading Data:"
DIRTY_FILE = "$1"
CLEAN_FILE = "$2"

#Begin Filtering:
    # Remove Punctuation
    # Keep words of length 3-15. Remove everything else.
    # Remove Duplicates
    # Sort data

echo "Filtering Data:"

cat "$DIRTY_FILE" \
    | grep -oP '^[A-Za-z]{3,15}$' \
    | uniq \
    | sort > "$CLEAN_FILE"

echo "Finished Cleaning Data!"