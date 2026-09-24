#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define NUMBER_OF_THREADS 4

int main()
{
  srand(time(NULL));
  int signal_size = 200;
  float signal[signal_size];
  for (int i = 0; i < signal_size; i++)
    signal[i] = rand() % (100 - 1 + 1) + 1;
  int total_noise_count = 0;
  double start_time = omp_get_wtime(), run_time;
  omp_set_num_threads(NUMBER_OF_THREADS);
  #pragma omp parallel
  {
    int thread_index = omp_get_thread_num();
    int total_threads = omp_get_num_threads();
    int chunk_size = signal_size / total_threads;
    int start_index = thread_index * chunk_size;
    int end_index;
    if (thread_index == total_threads - 1)
      end_index = signal_size;
    else
      end_index = start_index + chunk_size;
    int local_noise_counter = 0;
    for (int i = start_index; i < end_index; i++)
      if (signal[i] > 80)
        local_noise_counter++;
    
    #pragma omp atomic  
      total_noise_count += local_noise_counter;  
  }
  
  if (total_noise_count > (signal_size / 2))
    printf("Signal rejected. Total noise count: %f\n", (double)total_noise_count / signal_size * 100);
  else
    printf("Signal accepted. Total noise count: %f\n", (double)total_noise_count / signal_size * 100);
  return 0;
}
