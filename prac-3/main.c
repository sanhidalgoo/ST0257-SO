#include <stdlib.h>
#include <stdio.h>
#define NUM_REQUESTS 10
#define DISK_SIZE 4099

typedef struct
{
  // process id
  int id;
  // burst time
  int burstTime;
  // arrival time
  int arrivalTime;
  // priority
  int priority;
  // waitingTime
  int waitingTime;
  // lastTime
  int lastTime;
  // IsProcessed?
  int processed;
} ProcessStruct;

int turnaroundTime = 0, totalTurnaroundTime = 0, waitingTime = 0, totalWaitingTime = 0;

ProcessStruct *processes;

int main(int argc, char const *argv[])
{
  int category;
  printf("CPU and Disk Scheduling Program");
  printf("What types of algorithms do you want to use?\n");
  printf("1: CPU scheduling\n");
  printf("2: Disk scheduling\n");
  printf("Choose an option: ");
  scanf("%d", &category);
  // Check if the option is valid
  if (category != 1 && category != 2)
  {
    printf("You entered an invalid category.\n");
    exit(1);
  }

  if (category == 1)
  {
    int numberOfProcesses;
    printf("Enter the number of process in the queue (Max 10): ");
    scanf("%i", &numberOfProcesses);
    if (numberOfProcesses > 10)
    {
      printf("The maximum number of processes allowed is 10\n");
      exit(1);
    }

    processes = (int *)malloc(sizeof(int) * numberOfProcesses);
    for (int i = 0; i < numberOfProcesses; i++)
    {
      int temporalInfo;
      processes[i].id = i;

      printf("  Burst time for process   %d: ", i);
      scanf("%d", &temporalInfo);
      processes[i].burstTime = temporalInfo;

      printf("  Arrival time for process %d: ", i);
      scanf("%d", &temporalInfo);
      processes[i].arrivalTime = temporalInfo;

      printf("  Priority for process     %d: ", i);
      scanf("%d", &temporalInfo);
      processes[i].priority = temporalInfo;

      int processed = 0;
    }

    int opt;
    printf("What algorithm do you want to run?\n");
    printf("1. First Come, First Served (FCFS).\n");
    printf("2. Shortest Job First (SJF).\n");
    printf("3. Priority (Non preemptive).\n");
    printf("4. Round Robin.\n");
    printf("5. All\n");
    printf("Choose the option: ");
    scanf("%i", &opt);
    if (opt > 5 || opt < 1)
    {
      printf("Invalid option.");
      exit(1);
    }

    switch (opt)
    {
    case 1:
      fcfsCPU(processes, numberOfProcesses);
      break;
    case 2:
      sjf(processes, numberOfProcesses);
      break;
    case 3:
      priorityNonPreemptive(processes, numberOfProcesses);
      break;
    case 4:
      roundRobin(processes, numberOfProcesses);
      break;

    case 5:
      fcfsCPU(processes, numberOfProcesses);
      sjf(processes, numberOfProcesses);
      priorityNonPreemptive(processes, numberOfProcesses);
      roundRobin(processes, numberOfProcesses);
      break;
    default:
      printf("That is not a valid option.");
      exit(1);
    }
  }
  else
  {
    int opt;
    printf("What algorithm do you want to run?\n");
    printf("1. First Come, First Served (FCFS).\n");
    printf("2. Shortest Seek First (SSF).\n");
    printf("3. SCAN.\n");
    printf("4. C-SCAN.\n");
    printf("5. All\n");
    printf("Choose an option: ");
    scanf("%i", &opt);
    if (opt > 5 || opt < 1)
    {
      printf("Invalid option.");
      exit(1);
    }

    int headPointer;
    printf("Type the initial position: ");
    scanf("%d", &headPointer);


    int listA[NUM_REQUESTS];
    int listB[NUM_REQUESTS];
    int current = 0;
    printf("Type ten requests:\n");
    for (int i = 0; i < NUM_REQUESTS; i++)
    {
      printf(" Type request %d: ", i);
      scanf("%d", &current);
      listA[i] = current;
      listB[i] = current;
    }

    //int listR[8] = {98, 183, 37, 122, 14, 124, 65, 67};

    switch (opt)
    {
    case 1:
      fcfsDisk(headPointer, listA);
      break;
    case 2:
      sstf(headPointer, listB);
      break;
    case 3:
      scan(headPointer, listA);
      break;
    case 4:
      cscan(headPointer, listA);
      break;

    case 5:
      fcfsDisk(headPointer, listA);
      sstf(headPointer, listB);
      scan(headPointer, listA);
      cscan(headPointer, listA);
      break;
    default:
      printf("That is not a valid option.");
      exit(1);
    }
  }
  return 0;
}

// DISK SCHEDULING ALGORITHMS

void fcfsDisk(int headPointer, int list[])
{
  printf("\n*******************************************");
  printf("\nFIRST COME, FIRST SERVED ALGORITHM (FCFS).*\n");
  printf("*******************************************\n");
  printf("The movements carried out are: \n");

  int amountOfMovements = abs(headPointer - list[0]);
  printf("%d ->", headPointer);
  for (int i = 0; i < NUM_REQUESTS - 1; i++)
  {
    printf("%d -> ", list[i]);
    int movement = abs(list[i] - list[i + 1]);
    // printf("%d ", movement);
    amountOfMovements += movement;
  }
  printf("%d ", list[7]);
  printf("\nAmount of Movements:  %d cylinders\n", amountOfMovements);
  printf("*******************************************\n\n");
}


// Based on link
void sstf(int headPointer, int list[])
{
  printf("\n*******************************************");
  printf("\nSHORTEST SEEK TIME FIRST (SSTF)           *\n");
  printf("*******************************************\n");

  int count = 0;
  int sum = 0;
  printf("%d -> ", headPointer);
  while (count != NUM_REQUESTS)
  {
    int min = 1000, d = 0, index = 0;
    for (int i = 0; i < NUM_REQUESTS; i++)
    {
      d = abs(list[i] - headPointer);
      if (min > d)
      {
        min = d;
        index = i;
      }
    }
    sum += min;
    headPointer = list[index];
    if (count != NUM_REQUESTS - 1)
    {
      printf("%d -> ", headPointer);
    }
    else
    {
      printf("%d ", headPointer);
    }

    list[index] = 1000000;
    count++;
  }
  printf("\nAmount of movements %d cylinders\n", sum);
  printf("*******************************************\n\n");
}

void scan(int headPointer, int list[])
{
  printf("\n*******************************************");
  printf("\nSCAN ALGORITHM.                           *\n");
  printf("*******************************************\n");
  printf("The movements carried out are: \n");
  int amountOfMovements = 0;
  // sort list (Bubble sort)
  int aux;
  for (int i = 0; i < NUM_REQUESTS; i++)
  {
    for (int j = i + 1; j < NUM_REQUESTS; j++)
    {
      if (list[i] > list[j])
      {
        aux = list[i];
        list[i] = list[j];
        list[j] = aux;
      }
    }
  }
  int listAux[NUM_REQUESTS + 1];
  int index = 0;
  for (int i = 0; i < NUM_REQUESTS; i++)
  {
    if (list[i] <= headPointer)
    {
      listAux[i] = list[i];
      index = i;
    }
  }

  int headIndex = index + 1;
  listAux[headIndex] = headPointer;

  //
  for (int i = headIndex; i > 0; i--)
  {
    printf("%d -> ", listAux[i]);
  }
  printf("%d -> %d -> ", listAux[0], 0);

  for (int i = headIndex + 1; i < NUM_REQUESTS; i++)
  {
    index++;
    printf("%d -> ", list[index]);
  }
  printf("%d", list[++index]);
  amountOfMovements = headPointer + list[NUM_REQUESTS - 1];
  printf("\nAmount of Movements:  %d cylinders\n", amountOfMovements);
  printf("*******************************************\n\n");
}

void cscan(int headPointer, int list[])
{
  printf("\n*******************************************");
  printf("\nC-SCAN ALGORITHM.                         *\n");
  printf("*******************************************\n");
  printf("The movements carried out are: \n");
  int amountOfMovements = 0;
  // sort list (Bubble sort)
  int aux;
  for (int i = 0; i < NUM_REQUESTS; i++)
  {
    for (int j = i + 1; j < NUM_REQUESTS; j++)
    {
      if (list[i] > list[j])
      {
        aux = list[i];
        list[i] = list[j];
        list[j] = aux;
      }
    }
  }

  int listAux[NUM_REQUESTS + 1];
  int index = 0;
  for (int i = 0; i < NUM_REQUESTS; i++)
  {
    if (list[i] <= headPointer)
    {
      listAux[i] = list[i];
      index = i;
    }
  }

  printf("%d -> ", headPointer);
  for (int i = index+1; i < NUM_REQUESTS; i++)
  {
    printf("%d -> ", list[i]);
  }

  printf("%d -> %d -> ", DISK_SIZE, 0);

  for (int i = 0; i < index; i++)
  {
    printf("%d -> ", list[i]);
  }

  printf("%d", list[index]);

  amountOfMovements = DISK_SIZE - headPointer + DISK_SIZE + list[index];
  printf("\nAmount of Movements:  %d cylinders\n", amountOfMovements);
  printf("*******************************************\n\n");
}

// CPU SCHEDULING ALGORITHMS

void fcfsCPU(ProcessStruct processes[], int numberOfProcesses)
{
  // Reset Times
  turnaroundTime = 0, totalTurnaroundTime = 0, waitingTime = 0, totalWaitingTime = 0;

  printf("\n*******************************************");
  printf("\nFIRST COME, FIRST SERVED ALGORITHM (FCFS).*\n");
  printf("*******************************************\n");

  // sort processes according to arrival time (Bubble sort)
  for (int i = 0; i < numberOfProcesses; i++)
  {
    for (int j = i + 1; j < numberOfProcesses; j++)
    {
      if (processes[i].arrivalTime > processes[j].arrivalTime)
      {
        ProcessStruct aux = processes[i];
        processes[i] = processes[j];
        processes[j] = aux;
      }
    }
  }
  calculateTimes(processes, numberOfProcesses);
  averageTimes(numberOfProcesses);
}

void sjf(ProcessStruct processes[], int numberOfProcesses)
{
  turnaroundTime = 0, totalTurnaroundTime = 0, waitingTime = 0, totalWaitingTime = 0;

  printf("\n*******************************************");
  printf("\nSHORTEST JOB FIRST ALGORITHM (SJF).       *\n");
  printf("*******************************************\n");
  // sort processes according to burst time (Bubble sort)
  for (int i = 0; i < numberOfProcesses; i++)
  {
    for (int j = i + 1; j < numberOfProcesses; j++)
    {
      if (processes[i].burstTime > processes[j].burstTime)
      {
        ProcessStruct aux = processes[i];
        processes[i] = processes[j];
        processes[j] = aux;
      }
    }
  }
  calculateTimes(processes, numberOfProcesses);
  averageTimes(numberOfProcesses);
}

void priorityNonPreemptive(ProcessStruct processes[], int numberOfProcesses)
{
  turnaroundTime = 0, totalTurnaroundTime = 0, waitingTime = 0, totalWaitingTime = 0;
  printf("\n*******************************************");
  printf("\nPRIORITY (NON PREEMPTIVE) ALGORITHM       *\n");
  printf("*******************************************\n");
  // sort processes according to priority (Bubble sort)
  for (int i = 0; i < numberOfProcesses; i++)
  {
    for (int j = i + 1; j < numberOfProcesses; j++)
    {
      if (processes[i].priority > processes[j].priority)
      {
        ProcessStruct aux = processes[i];
        processes[i] = processes[j];
        processes[j] = aux;
      }
    }
  }
  calculateTimes(processes, numberOfProcesses);
  averageTimes(numberOfProcesses);
}

void roundRobin(ProcessStruct processes[], int numberOfProcesses)
{
  turnaroundTime = 0, totalTurnaroundTime = 0, waitingTime = 0, totalWaitingTime = 0;
  int quantum;
  printf("\n*******************************************");
  printf("\nROUND ROBIN ALGORITHM                     *\n");
  printf("*******************************************\n");
  printf("Type the quantum value: ");
  scanf("%d", &quantum);
  // sort processes according to arrival time (Bubble sort)
  for (int i = 0; i < numberOfProcesses; i++)
  {
    for (int j = i + 1; j < numberOfProcesses; j++)
    {
      if (processes[i].arrivalTime > processes[j].arrivalTime)
      {
        ProcessStruct aux = processes[i];
        processes[i] = processes[j];
        processes[j] = aux;
      }
    }
  }

  int updatedTime = 0;
  int firstProcess = 1;
  // Processing
  while (1)
  {
    int finishedProcesses = 1;
    finishedProcesses = 1;
    for (int i = 0; i < numberOfProcesses; i++)
    {
      if (processes[i].burstTime > 0)
      {
        if (processes[i].burstTime > quantum)
        {
          // There are more processes
          finishedProcesses = 0;
          printf("*******************************************\n");
          printf("* Process:                    %d\n", processes[i].id);
          printf("* Waiting time (partial):     %d\n", updatedTime);
          printf("* Turnaround time (partial):  %d\n", quantum);
          if (i == 0 && firstProcess == 1)
          {
            processes[i].waitingTime = 0;
            firstProcess = 0;
            processes[i].lastTime = 3;
            updatedTime += quantum;
          }
          else
          {
            processes[i].waitingTime += (updatedTime - processes[i].lastTime);
            updatedTime += quantum;
            processes[i].lastTime = updatedTime;
          }
          processes[i].burstTime -= quantum;
        }
        //  Process can end in the quantum time
        else
        {
          if (processes[i].processed == 1)
          {
            continue;
          }

          printf("*******************************************\n");
          printf("* Process Finished:           %d\n", processes[i].id);
          printf("* Waiting time (partial):     %d\n", updatedTime);
          printf("* Turnaround time (partial):  %d\n", processes[i].burstTime);
          processes[i].waitingTime += (updatedTime - processes[i].lastTime);
          waitingTime = processes[i].waitingTime;
          printf("* Waiting time (total):       %d\n", waitingTime);
          turnaroundTime = processes[i].waitingTime + processes[i].burstTime;
          printf("* Turnaround time (total):    %d\n", turnaroundTime);
          float averageProcessWaitingTime = (float)waitingTime / (float)numberOfProcesses;
          float averageProcessTurnaroundTime = (float)turnaroundTime / (float)numberOfProcesses;
          printf("* Average Waiting time:       %.2f\n", averageProcessWaitingTime);
          printf("* Average Turnaround time:    %.2f\n", averageProcessWaitingTime);
          totalTurnaroundTime += turnaroundTime;
          totalWaitingTime += waitingTime;
          processes[i].processed = 1;
          updatedTime += processes[i].burstTime;
        }
        // printf("\nLast time for process: %d is %d", processes[i].id, processes[i].lastTime);
      }
    }
    // End cycle if all processes end their execution
    if (finishedProcesses == 1)
      break;
  }

  averageTimes(numberOfProcesses);
}

void calculateProcessTime(ProcessStruct process, int numberOfProcesses, int first)
{
  if (first == 1)
  {
    printf("*******************************************\n");
    printf("* Process:          %d\n", process.id);
    printf("* Waiting time:     0\n");
    printf("* Turnaround time:  %d\n", process.burstTime);
    turnaroundTime = process.burstTime;
    totalTurnaroundTime += turnaroundTime;
  }
  else
  {
    int id = process.id;
    int burstTime = process.burstTime;
    printf("*******************************************\n");
    printf("* Process:          %d\n", id);
    printf("* Waiting time:     %d\n", turnaroundTime);
    printf("* Turnaround time:  %d\n", turnaroundTime + burstTime);
    printf("*******************************************\n");
    waitingTime = turnaroundTime;
    totalWaitingTime += waitingTime;
    turnaroundTime += burstTime;
    totalTurnaroundTime += turnaroundTime;
  }
}

void calculateTimes(ProcessStruct processes[], int numberOfProcesses)
{

  for (int i = 0; i < numberOfProcesses; i++)
  {
    if (i == 0)
    {
      calculateProcessTime(processes[i], numberOfProcesses, 1);
    }
    else
    {
      calculateProcessTime(processes[i], numberOfProcesses, 0);
    }
  }
}

void averageTimes(int numberOfProcesses)
{
  // calculate averages
  printf("*******************************************\n");
  printf("              Total Averages               *\n");
  printf("*******************************************\n");
  float averageTurnaroundTime = (float)totalTurnaroundTime / (float)numberOfProcesses;
  float averageWaitingTime = (float)totalWaitingTime / (float)numberOfProcesses;
  printf("*     Average waiting time    = %.2f     *\n", averageWaitingTime);
  printf("*     Average turnaround time = %.2f     *\n", averageTurnaroundTime);
  printf("*******************************************\n\n");
}