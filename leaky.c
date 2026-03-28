#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(){

  int bucket_capacity;
  int output_rate;
  int sequence_length;
  int stored = 0;
  int current_time = 0;
  int sequence[100];
  srand(time(NULL)); 

  printf("Enter bucket capacity (packets): ");
  scanf("%d", &bucket_capacity);

  printf("Enter output rate (packets per second): ");
  scanf("%d", &output_rate);

  printf("Enter incoming sequence size:");
  scanf("%d", &sequence_length);
  printf("Input Sequence : ");
  for(int i=0;i<sequence_length;i++)
      scanf("%d",&sequence[i]);
      printf("\nRecieve  sent  Drop Remaining\n");
  for(int i=0;i<sequence_length;i++)
  {
    int dropped=0;
    int incoming = sequence[i];
    if (incoming + stored <= bucket_capacity)
    {
      stored += incoming;
    }
    else
    {
      dropped = incoming;
    }

    int sent = (stored >= output_rate) ? output_rate : stored;
    stored -= sent;
    printf("%d\t%d\t%d\t%d\n",incoming,sent,dropped,stored);
    sleep(1);
  }

  printf("\nSimulation completed.\n");
  return 0;
}



