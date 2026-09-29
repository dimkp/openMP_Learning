#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define NUMBER_OF_THREADS 4

int main()
{
  srand(time(NULL));
  int signal_size = 150;
  float ant1[150], ant2[150];
  float combined_signal[150];
  for (int i = 0; i < signal_size; i++)
  {
    ant1[i] = rand() % (signal_size - 1 + 1) + 1;
    ant2[i] = rand() % (signal_size - 1 + 1) + 1;
  }
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
    
    for (int i = start_index; i < end_index; i++)
      combined_signal[i] = ant1[i] + ant2[i];
  }

  return 0;
}
