#include <unistd.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <struct.h>



int main()
{
	struct msgbuf snd,rcv;
	return 0;
	int id=msgget(10000,0644|IPC_CREAT);
	snd.sender=0;
	while(1)
	{
		msgrcv(id,&rcv,270,1,0);
	}
}