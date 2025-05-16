#include <stdio.h>

int main() {
    char buffer[100];

    printf("Enter a string: ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        printf("You entered: %s", buffer);
    } else {
        printf("Error reading input.\n");
    }

    return 0;
}