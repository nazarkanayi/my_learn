#include <stdio.h>
#include <string.h>

// Function to reverse a segment of a string
void reverseSegment(char* start, char* end) {
    char temp;
    while (start < end) {
        temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

// Function to reverse words in a string
void reverseWords(char* s) {
    char* word_start = s;
    char* temp = s; // temp is used to traverse the string

    // Iterate through the string
    while (*temp) {
        // If the current character is a space, it marks the end of a word
        if (*temp == ' ') {
            reverseSegment(word_start, temp - 1);
            word_start = temp + 1;
        }
        temp++;
    }

    // Reverse the last word (since there's no trailing space to trigger the if condition)
    reverseSegment(word_start, temp - 1);
}

int main() {
    char str[] = "abcd efgh";
    printf("Original string: %s\n", str);
    reverseWords(str);
    printf("Reversed words: %s\n", str);
    return 0;
}
