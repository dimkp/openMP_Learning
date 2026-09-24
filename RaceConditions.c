#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define NUMBER_OF_THREADS 4

int main()
{
  int signal_size = 100;
  float signal[100], total_power;
  for (int i = 0; i < 100; i++)
    signal[i] = rand() % (100 - 1 + 1) + 1;
  
  float start_time = omp_get_wtime(), run_time;
  omp_set_num_threads(NUMBER_OF_THREADS);
  #pragma omp parallel
  {
    // Which thread and how many threads are there
    int thread_id = omp_get_thread_num();
    int total_threads = omp_get_num_threads();
  
    // The data that each thread will proccess and the starting index for each thread
    int chunk_size = signal_size / total_threads;
    int start_index = thread_id * chunk_size;
  
    // Calculation of end_index
    // If its the last thread, it will use the remaining data
    // Otherwise the end_index is the starting index + the size of the chunk
    // Ex. thread 0 will use the data from start_index = 0 * 25 (signal_size/number of  
    // threads) = 0 up until end_index = 0 + 25 = 24
    int end_index;
    if (thread_id == total_threads - 1)
      end_index = signal_size;
    else
      end_index = start_index + chunk_size;
    
    float local_thread_power = 0.0f;
    for (int i = start_index; i < end_index; i++)
      local_thread_power += signal[i] * signal[i];
    
    #pragma omp atomic
      total_power += local_thread_power;
  }
  run_time = omp_get_wtime() - start_time;
  
  printf("The total power of the signal is: %f\n", total_power);
  printf("Total run time of the pipeline: %f\n", run_time);
  return 0;
}
