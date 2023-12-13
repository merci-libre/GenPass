#include <stdio.h>

int main() {
  FILE *file;
  char filename[100];
  float number, sum = 0.0;
  int count = 0;

  // Get the file name from the user
  printf("Enter the file name: ");
  scanf("%s", filename);

  // Open the file for reading
  file = fopen(filename, "r");

  // Check if the file opened successfully
  if (file == NULL) {
    printf("Error opening the file. Make sure the file exists.\n");
    return 1;
  }

  // Read numbers from the file and calculate the sum and count
  while (fscanf(file, "%f", &number) == 1) {
    sum += number;
    count++;
  }

  // Close the file
  fclose(file);

  // Check if any numbers were read from the file
  if (count == 0) {
    printf("No numbers found in the file.\n");
    return 2;
  }

  // Calculate and print the average
  float average = sum / count;
  printf("Average: %.3f\n", average);

  return 0;
}
