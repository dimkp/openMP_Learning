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
  float peak = 0.0;
  for (int i = 0; i < signal_size; i++)
    signal[i] = rand() % (1000 - 1 + 1) + 1;
  
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
    float local_peak = 0.0;
    for (int i = start_index; i < end_index; i++)
      if (signal[i] > local_peak)
        local_peak = signal[i];
  
    #pragma omp critical
    {
      if (local_peak > peak)
        peak = local_peak;
    }
  }
  
  run_time = omp_get_wtime() - start_time;
  printf("Peak: %f Hz\n", peak);
  printf("Run time: %f\n", run_time);
  return 0;
}
