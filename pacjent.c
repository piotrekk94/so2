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
	snd.sender=getpid();
	return 0;
}