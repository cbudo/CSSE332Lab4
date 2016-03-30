/* 
  Copyright Christopher Budo, Alia Robinson
  This is where you implement the core solution.
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#include "./threads.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))



int buffer[MAX_N_SIZE][MAX_BUFFER_SIZE];
/* other global variable instantiations can go here */

/* Uses a brute force method of sorting the input list. */
void BruteForceSort(int inputList[], int inputLength) {
  int i, j, temp;
  for (i = 0; i < inputLength; i++) {
    for (j = i+1; j < inputLength; j++) {
      if (inputList[j] < inputList[i]) {
        temp = inputList[j];
        inputList[j] = inputList[i];
        inputList[i] = temp;
      }
    }
  }
}

void *BruteForceHelper(void* ptr) {
  Interval* inv = (Interval*) ptr;
  int inputLength = (*inv).end - (*inv).start;
  int *input = malloc(inputLength);
  struct timeval startTime;
  gettimeofday(&startTime, NULL);
  memcpy(input, ((*inv).costPointer + (*inv).start), inputLength);
  BruteForceSort(input, inputLength);
  free(input);
  return runTime(startTime);
}

long int runTime(struct timveval startTime) {
  long int elapsedTime = 0;
  struct timeval endTime;
  gettimeofday(&endTime);
  elapsedTime = endTime.tv_sec - startTime.tv_sec;
  elapsedTime += endTime.tv_usec - startTime.tv_usec;
  return elapsedTime;
}

/* Uses the bubble sort method of sorting the input list. */
void BubbleSort(int inputList[], int inputLength) {
  char sorted = 0;
  int i, temp;
  while (!sorted) {
    sorted = 1;
    for (i = 1; i < inputLength; i++) {
      if (inputList[i] < inputList[i-1]) {
        sorted = 0;
        temp = inputList[i-1];
        inputList[i-1] = inputList[i];
        inputList[i] = temp;
      }
    }
  }
}
void *BubbleHelper(void* ptr) {
  Interval* inv = (Interval*) ptr;
  int inputLength = (*inv).end - (*inv).start;
  int *input = malloc(inputLength);
  struct timeval startTime;
  gettimeofday(&startTime, NULL);
  memcpy(input, ((*inv).costPointer + (*inv).start), inputLength);
  BubbleSort(input, inputLength);
  free(input);
  return runTime(startTime);
}

/* Sudo merges two arrays.  Instead of having two arrays as input, it
 * merges positions in the overall array by reording data.  This saves
 * space. */
void Merge(int *array, int left, int mid, int right) {
  int tempArray[MAX_BUFFER_SIZE];
  int pos = 0, lpos = left, rpos = mid + 1;
  while (lpos <= mid && rpos <= right) {
    if (array[lpos] < array[rpos]) {
      tempArray[pos++] = array[lpos++];
    } else {
      tempArray[pos++] = array[rpos++];
    }
  }
  while (lpos <= mid)  tempArray[pos++] = array[lpos++];
  while (rpos <= right)tempArray[pos++] = array[rpos++];
  int iter;
  for (iter = 0; iter < pos; iter++) {
    array[iter+left] = tempArray[iter];
  }
  return;
}

/* Divides an array into halfs to merge together in order. */
void MergeSort(int *array, int left, int right) {
  int mid = (left+right)/2;
  if (left < right) {
    MergeSort(array, left, mid);
    MergeSort(array, mid+1, right);
    Merge(array, left, mid, right);
  }
}

void *MergeHelper(void* ptr) {
  Interval* inv = (Interval*) ptr;
  int inputLength = (*inv).end - (*inv).start;
  int *input = malloc(inputLength);
  struct timeval startTime;
  gettimeofday(&startTime, NULL);
  memcpy(input, ((*inv).costPointer + (*inv).start), inputLength);
  MergeSort(input, (*inv).start, (*inv).end);
  free(input);
  return runTime(startTime);
}
/* Merges the sorted files into an output file using the two
 * dimensional output buffer */
/*void MergeAndOutputBuffer(char* outputFile) {
  FILE *outFile = fopen(outputFile, "w");
  if (outFile == NULL) {
    fprintf(stderr, "Unable to open the file %s\n", outputFile);
    exit(2);
  }
  int i;
  int64_t j;
  int indexes[MAX_N_SIZE], maxIndexes[MAX_N_SIZE];
  for (i = 0; i < n; i++) {
    for (j = 0; buffer[i][j] > 0; j++) {
    }
    maxIndexes[i] = j;
    indexes[i] = 0;
  }
  int smallIndex;
  j = 0;
  while (1) {
    smallIndex = -1;
    for (i = 0; i < n; i++) {
      if (indexes[i] < maxIndexes[i]) {
        smallIndex = i;
        break;
      }
    }
    if (smallIndex == -1) {
      break;
    }
    for (i = 1; i < n; i++) {
      if ((indexes[i] < maxIndexes[i]) &&
          (buffer[i][indexes[i]] < buffer[smallIndex][indexes[smallIndex]])) {
        smallIndex = i;
      }
    }
    fprintf(outFile, "%d\n", buffer[smallIndex][indexes[smallIndex]]);
    fflush(outFile);
    indexes[smallIndex]++;
    j++;
  }
  fclose(outFile);
}*/

void findMax(long int *array, int length) {
  int i = 0;
  long int maxVal = 0;
  for (i = 0; i < length; i++) {
    if (*(array + i) > maxVal) {
      maxVal = *(array + i);
    }
  }
  printf("The longest thread ran for %ldus\n", maxVal);
}
void findMin(long int *array, int length) {
  int i = 0;
  long int minVal = *(array);
  for (i = 1; i < length; i++) {
    if (*(array + i) < minVal) {
      minVal = *(array + i);
    }
  }
  printf("The shortest thread ran for %ldus\n", minVal);
}
void findMean(long int *array, int length) {
  int i;
  long int average = 0;
  for (i = 0; i < length; i++) {
    average += *(array + i);
  }
  average /= length;
  printf("The mean runtime for a thread was %ldus\n", average);
}

int main(int argc, char* argv[]) {
  int numThread, i, linesPer, fileSize = 0;
  pthread_t* tid;
  FILE *fileIn, *fileOut;
  if (argc != 4) {
    fprintf(stderr, "Incorrect number of arguments - proper number of arguments is 3.");
  }
  numThread = atoi(argv[1]);
  fileIn = fopen(argv[2], "r");
  fileOut = fopen(argv[3], "w");

  tid = malloc(sizeof(pthread_t) * numThread);
  Interval *threadData = malloc(sizeof(Interval) * numThread);

  while (fscanf(fileIn, "\n") != EOF) {
    fileSize++;
  }
  fclose(fileIn);
  int *cost = malloc(sizeof(int) * fileSize);
  long int *timing = malloc(sizeof(long int) * numThread);

  fileIn = fopen(argv[2], "r");
  i = 0;
  while (fscanf(fileIn, "%d\n", &cost[i]) != EOF) {
    i++;
  }

  linesPer = ceil(fileSize/numThread);

  for (i = 0; i < numThread; i++) {
    threadData[i] = (Interval) {linesPer*i, MIN(linesPer*(i+1), fileSize), cost};
    if (i < numThread / 3){
      // run brute force
      pthread_create(&tid[i], NULL, BruteForceHelper, &threadData[i]);
    } else if (i < (2 * numThread) / 3) {
      // run bubble sort
      pthread_create(&tid[i], NULL, BubbleHelper, &threadData[i]);
    } else {
      // run merge sort
      pthread_create(&tid[i], NULL, MergeHelper, &threadData[i]);
    }
  }
  for (i = 0; i < numThread; i++) {
    pthread_join(tid[i], &timing[i]);
  }
  // find max, min, mean
  findMin(timing, numThread);
  findMax(timing, numThread);
  findMean(timing, numThread);
  // sort thirds
  BruteForceSort(cost, fileSize / 3);
  BubbleSort((cost + (fileSize / 3)), (fileSize * 2) / 3);
  MergeSort(cost, (fileSize * 2) / 3, fileSize);
  // merge thirds
  MergeSort(cost, 0, fileSize);
  // write file to fileOut
  for (i = 0; i < fileSize; i++) {
    fprintf(fileOut, "%d\n", cost[i]);
  }
  fclose(fileIn);
  fclose(fileOut);
  free(threadData);
  free(cost);
  return 0;
}