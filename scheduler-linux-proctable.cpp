#include <string>
#include <iostream>
#include <fstream>
#include <queue>
#include <iomanip>

using namespace std;


int numofcores = 0;
int processindex = -1;

enum processTypes {core,block,nonblock,user};
enum processStatus {ready,running,waiting,terminated,offline};

//process types
struct processes 
{
float starttime;
float coretime;
int processnumber;
queue <processTypes> proctype;
queue <float> proctyme;
processStatus status;
bool started;
int processsize;


};

//simulate devices
struct devicecore
{
int processnumber;
bool corebusy;
float timeuntil;
};

struct devicegeneral
{
int processnumber;
bool devicebusy;
float timeuntil;
bool nonblock;
};
//simulate other devices

struct devicequeue
{
queue <int> processnumber;
queue <float> occupytime;
queue <bool> isblocking;
};


int main (int argc, char* argv[] )
{
	//cin >>

	ifstream input("input10.txt");
	
	processes processList[256];

	string word;
	float floating;
	int	integer;

	int processsize = 0;

	if (input.fail()) { cout <<"input fail, check input file name!!" << endl;}

	//reading file locally 
	//when reading file using linux io redirection change input -> cin instead

	while (cin >>  word )
	{
		//set number of cores
		if (word == "NCORES")
		{
			cin >> integer;
			numofcores = integer;
		}
		
		//set process number and index for process array

		if (word == "START")
		{
			processindex = processindex +1;
			cin >> floating;
			processList[processindex].starttime = floating;
			processList[processindex].started = false;
			processList[processindex].processnumber = processindex;
		}
		
		//push all process types into queue for process 0-nth proesses until end of file
		
		if (word == "CORE")
		{   cin >> floating;
		processList[processindex].proctype.push(core);
		processList[processindex].proctyme.push(floating);
		processList[processindex].processsize = processList[processindex].processsize + 1 ;
		}
		if (word == "BLOCK")
		{   cin >> floating;
		processList[processindex].proctype.push(block);
		processList[processindex].proctyme.push(floating);
		processList[processindex].processsize = processList[processindex].processsize + 1 ;
		}
		if (word == "NOBLOCK")
		{   cin >> floating;
		processList[processindex].proctype.push(nonblock);
		processList[processindex].proctyme.push(floating);
		processList[processindex].processsize = processList[processindex].processsize + 1 ;

		}
		if (word == "USER")
		{   cin >> floating;
		processList[processindex].proctype.push(user);
		processList[processindex].proctyme.push(floating);
		processList[processindex].processsize = processList[processindex].processsize + 1 ;
		}

	}

	/**

	cout << "num of cores is " << numofcores << endl << endl;
	
	for (int i = 0; i < processindex +1 ; i++){
	//call out all process numbers 1-n with its start time

		cout << "process" << processList[i].processnumber << "\'s start time is " << processList[i].starttime << " contains numbers of processes : " << processList[i].processnumber << endl;
	//call out first sub process with type value in process n

	cout << "first process is type " << processList[i].proctype.front() << " amount of time " << processList[i].proctyme.front() << endl << endl;

	}
	system ("pause");
	
	**/



	//program simulation logic can go after here
	/*** foo *****/


	int processend = 0;
	float currenttime = 0.0;

	//declare devices
	devicegeneral diskdev;
	devicegeneral userdev;

	devicecore mycore[265];

	devicequeue corequeue;
	devicequeue diskqueue;
	devicequeue userqueue;

	cout.precision(1);
	cout << fixed;

	//initialize core devices

	diskdev.devicebusy = false;
	diskdev.processnumber = 00;
	diskdev.timeuntil = 0;

	userdev.devicebusy = false;
	userdev.processnumber = 00;
	userdev.timeuntil = 0;

	for (int i = 0; i < numofcores; i++)
	{
		mycore[i].corebusy = false;
		mycore[i].processnumber = 00;
		mycore[i].timeuntil = 0;
	}

	while (processend < processindex +1 )
	{


		//loop to initiate proccesses
		for (int i = 0; i < processindex+1; i++)
		{
			if (processList[i].starttime <= currenttime && processList[i].started != true)
			{
			processList[i].started = true;

			//cout <<  processList[i].starttime << endl;


			//cout << "initiate process" << i << " at time " << currenttime << endl;

			
				if(processList[i].proctype.front() == core)
				{
					corequeue.occupytime.push(processList[i].proctyme.front());
					corequeue.processnumber.push(i);
					processList[i].status = ready;
					processList[i].proctyme.pop();
					processList[i].proctype.pop();

				//	cout << "process " << i << " first process goes into corequeue" << endl;

				}
				/**
				else if(processList[i].proctype.front() == block)
				{
					diskqueue.occupytime.push(processList[i].proctyme.front());
					diskqueue.isblocking.push(true);
					diskqueue.processnumber.push(i);
					processList[i].status = waiting;
					processList[i].proctyme.pop();
					processList[i].proctype.pop();

					cout << "process " << i << " first process goes into diskqueue and is blocking" << endl;
				
				}
				else if(processList[i].proctype.front() == nonblock)
				{
					diskqueue.occupytime.push(processList[i].proctyme.front());
					diskqueue.isblocking.push(false);
					diskqueue.processnumber.push(i);
					processList[i].status = waiting;
					processList[i].proctyme.pop();
					processList[i].proctype.pop();
		
					cout << "process " << i << " first process goes into diskqueue and is nonblocking" << endl;
				}
				else if(processList[i].proctype.front() == user)
				{
					userqueue.occupytime.push(processList[i].proctyme.front());
					userqueue.processnumber.push(i);
					processList[i].status = waiting;
					processList[i].proctyme.pop();
					processList[i].proctype.pop();

					cout << "process " << i << " first process goes into user queue" << endl;
				}

				**/
			}
		


		}

		//end process initialization loop

		//now start devices

		//core device

		for (int coreinx = 0; coreinx < numofcores; coreinx ++ )
		{
			//cout << "currently at core " << coreinx << endl;
			//grab processes from the core queue
			if (mycore[coreinx].corebusy != true && !corequeue.processnumber.empty())
			{   
				mycore[coreinx].timeuntil = corequeue.occupytime.front() + currenttime;
				mycore[coreinx].processnumber = corequeue.processnumber.front();
				mycore[coreinx].corebusy = true;
				processList[corequeue.processnumber.front()].status = running;
				
				processList[corequeue.processnumber.front()].coretime = corequeue.occupytime.front();

				//cout << mycore[coreinx].timeuntil << endl;
				corequeue.occupytime.pop();
				corequeue.processnumber.pop();	
		
			}

			//make core free once time is up
			
			else if (mycore[coreinx].timeuntil <= currenttime && mycore[coreinx].corebusy == true)
			{   
				int procnum = mycore[coreinx].processnumber;
				
				//terminate process if list is empty
				if (processList[procnum].proctype.empty() && processList[procnum].status != offline)
				{   
					mycore[coreinx].corebusy = false;
					processList[procnum].status = terminated;
					//cout << "process " << procnum << " terminated" << endl;

				}
				//else queue up next process
			
				//system ("pause");

				
				else 
				{
					/**
					if (processList[procnum].proctype.front() == core)
					{
						corequeue.occupytime.push(processList[procnum].proctyme.front());
						corequeue.processnumber.push(procnum);

						processList[procnum].proctyme.pop();
						processList[procnum].proctype.pop();

						processList[procnum].status = ready;

						mycore[coreinx].corebusy = false;
						cout << currenttime <<  " from core to core " << endl;

					}
					**/
					 if (processList[procnum].proctype.front() == block)
					{
						diskqueue.occupytime.push(processList[procnum].proctyme.front());
						diskqueue.processnumber.push(procnum);
						diskqueue.isblocking.push(true);

						processList[procnum].proctyme.pop();
						processList[procnum].proctype.pop();

						processList[procnum].status = waiting;

						mycore[coreinx].corebusy = false;
					//	cout << "process number " << procnum << " @ " <<  currenttime << "from core " << coreinx <<  " from core to disk queue" << endl;
					
					}
					else if (processList[procnum].proctype.front() == nonblock) 
					{
						diskqueue.occupytime.push(processList[procnum].proctyme.front());
						diskqueue.isblocking.push(false);
						diskqueue.processnumber.push(procnum);
						processList[procnum].status = waiting;
						processList[procnum].proctyme.pop();
						processList[procnum].proctype.pop();

						mycore[coreinx].corebusy = false;
						//cout << "process number " << procnum << " @ " <<  currenttime << "from core " << coreinx <<  " from core to disk queue" << endl;
					
					}
					else if (processList[procnum].proctype.front () == user )
					{
						userqueue.occupytime.push(processList[procnum].proctyme.front());
						userqueue.processnumber.push(procnum);
						processList[procnum].status = waiting;
						processList[procnum].proctyme.pop();
						processList[procnum].proctype.pop();
									
						mycore[coreinx].corebusy = false;
					
						//cout << "process number " << procnum << " @ " <<  currenttime << "from core " << coreinx <<  " from core to user queue" << endl;
					
					}
	
				}
						
			}
		
		}
		//finish core scheduler

			//start disk scheduler from processes in disk queue

		if (diskdev.devicebusy != true && !diskqueue.occupytime.empty())
			{   
				if (diskqueue.isblocking.front() == true)
					{
					diskdev.timeuntil = diskqueue.occupytime.front() + currenttime;
					diskdev.processnumber = diskqueue.processnumber.front();
					diskdev.devicebusy = true;
					diskdev.nonblock = false;
					processList[diskqueue.processnumber.front()].status = waiting;

				//	cout << "process number " << diskdev.processnumber << " @ " <<  currenttime <<" from disk queue to disk as blocking " << endl;
					//cout << mycore[coreinx].timeuntil << endl;
					diskqueue.isblocking.pop();
					diskqueue.occupytime.pop();
					diskqueue.processnumber.pop();
					

					}
				 else if (diskqueue.isblocking.front() == false)
					{
					diskdev.timeuntil = diskqueue.occupytime.front() + currenttime;
					diskdev.processnumber = diskqueue.processnumber.front();
					diskdev.devicebusy = true;
					diskdev.nonblock = true;
					processList[diskqueue.processnumber.front()].status = ready;

					//cout << "process number " << diskdev.processnumber << " @ " <<  currenttime <<" from disk queue to disk as nonblocking " << endl;
					diskqueue.isblocking.pop();
					diskqueue.occupytime.pop();
					diskqueue.processnumber.pop();

					corequeue.occupytime.push(processList[diskdev.processnumber].proctyme.front());
					corequeue.processnumber.push(diskdev.processnumber);

					processList[diskdev.processnumber].proctyme.pop();
					processList[diskdev.processnumber].proctype.pop();

				
					}
				
		
			}


		//free disk 

		if (diskdev.timeuntil <= currenttime && diskdev.devicebusy == true)
		{
			if (diskdev.nonblock == true) 
			{
				//cout << "process number " << diskdev.processnumber << " @ " <<  currenttime <<" release non-blocking process " << endl;
				diskdev.devicebusy = false;
			}
			else if (diskdev.nonblock == false)
			{
				diskdev.devicebusy = false;

				//cout << "process number " << diskdev.processnumber << " @ " <<  currenttime <<" release blocking disk process from disk to core " << endl;
					corequeue.occupytime.push(processList[diskdev.processnumber].proctyme.front());
					corequeue.processnumber.push(diskdev.processnumber);

					processList[diskdev.processnumber].proctyme.pop();
					processList[diskdev.processnumber].proctype.pop();


			}
		
		} 

		//start user process scheduler

		if (userdev.devicebusy !=true && !userqueue.occupytime.empty())
		{
					userdev.timeuntil = userqueue.occupytime.front() + currenttime;
					userdev.processnumber = userqueue.processnumber.front();
					userdev.devicebusy = true;

					processList[userqueue.processnumber.front()].status = waiting;

					cout << "process number " << userdev.processnumber << " @ " <<  currenttime <<" from user queue to user device " << endl;
					//cout << mycore[coreinx].timeuntil << endl;
					
					userqueue.occupytime.pop();
					userqueue.processnumber.pop();
		
		}

		//release user device
		if (userdev.timeuntil <= currenttime && userdev.devicebusy == true)
		{
				userdev.devicebusy = false;

			//	cout << "process number " << userdev.processnumber << " @ " <<  currenttime <<" release user device to core " << endl;
					corequeue.occupytime.push(processList[userdev.processnumber].proctyme.front());
					corequeue.processnumber.push(userdev.processnumber);

					processList[userdev.processnumber].proctyme.pop();
					processList[userdev.processnumber].proctype.pop();
		}



		//print process terminated process table

		for (int j=0; j<processindex+1; j++)
		{
			if (processList[j].status == terminated)
			{
			processend = processend +1;
			int busycores = 0;



			for (int corenum =0; corenum < numofcores ; corenum ++)
			{
				if (mycore[corenum].corebusy == true) {busycores = busycores + 1;}
			}

			cout << "PROCESS --" << j << " terminates at time "<< currenttime << endl << endl;
			cout << "CURRENT NUMBER OF BUSY CORES = " << busycores << endl << endl;
			if (diskdev.devicebusy == true)
			{cout << "Disk is busy" << endl;}
			else { cout << "Disk is Free" << endl;}
			cout << "READY QUEUE CONTAINS: " << corequeue.occupytime.size() << " processes" << endl;
			cout << "DISK QUEUE CONTAINS: " << diskqueue.occupytime.size() << " processes" << endl;
			cout << "USER QUEUE CONTAINS: " << userqueue.occupytime.size() << " processes" << endl;

			cout << "ProcessID" << setw(15) << "start time" << setw(10) << "coretime" << setw(15) << "status" << endl;
			
			for (int k=0; k<processindex+1; k++)
			{
				if (processList[k].status != offline)
				{
					cout << k << setw(20) << processList[k].starttime << setw(10) << processList[k].coretime ;
					if (processList[k].status == ready) { cout << " " << setw(18) << "ready" << endl;}
					if (processList[k].status == waiting) {cout << " "<< setw(18) << "waiting" << endl;}
					if (processList[k].status  == running){cout << " " << setw(18) << "running" << endl;}
					if (processList[k].status == terminated){cout << " "<< setw(18) << "terminated" << endl;}
				}
			}


			processList[j].status = offline;

			
			}
		
		
		}

			

		currenttime = currenttime + 0.1;
	



	
	
	
	}

	cout << "finish all processes" << endl;





	//system ("pause");
}