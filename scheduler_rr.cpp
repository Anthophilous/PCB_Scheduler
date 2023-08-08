/**
* Assignment 3: CPU Scheduler
 * @file scheduler_rr.h
 * @author Joshua White
 * @brief This Scheduler class implements the RoundRobin (RR) scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code
//

#include "scheduler_rr.h"

 /**
     * @brief Construct a new SchedulerRR object
     */
SchedulerRR::SchedulerRR(int time_quantum)
{
  numPCBs = 0;
  timeQuant = time_quantum;
  timeElapsed = 0;
  PCBQueue.clear();
  finishedPCBs.clear();
}

/**
 * @brief Destroy the SchedulerRR object
 */
SchedulerRR::~SchedulerRR()
{
  PCBQueue.clear();
  finishedPCBs.clear();
}

/**
 * @brief This function is called once before the simulation starts.
 *        It is used to initialize the scheduler.
 * @param process_list The list of processes in the simulation.
 */
void SchedulerRR::init(std::vector<PCB>& process_list)
{
  numPCBs = process_list.size();
  PCBQueue.resize(numPCBs);
  finishedPCBs.resize(numPCBs);
  //add to Queue, don't calc TT or WT until sim.
  for(int i = 0; i < numPCBs; i++)
    {
      PCBQueue.at(i).data = &process_list.at(i);
      PCBQueue.at(i).turnaroundTime = 0;
      PCBQueue.at(i).waitTime = 0;
      PCBQueue.at(i).remainingBurst = PCBQueue.at(i).burstTime();

    }
}

/**
 * @brief This function is called once after the simulation ends.
 *        It is used to print out the results of the simulation.
 */
void SchedulerRR::print_results()
{
  //Averages
  float avgWaitTimes = 0;
  float avgTurnaroundTimes = 0;

  for(int i = 0; i < numPCBs; i++)
    {
      cout << "T" << i+1 << " turnaround time = " << finishedPCBs.at(i).turnaroundTime << ", waiting time = " << finishedPCBs.at(i).waitTime << endl;

      avgTurnaroundTimes += finishedPCBs.at(i).turnaroundTime;
      avgWaitTimes += finishedPCBs.at(i).waitTime;
    }
  
  //Calc Avg
  avgTurnaroundTimes /= numPCBs;
  avgWaitTimes /= numPCBs;

  //Display
  cout << "Average turn-around time = " << avgTurnaroundTimes << ", Average waiting time = " << avgWaitTimes << endl;
}

/**
 * @brief This function simulates the scheduling of processes in the ready queue.
 *        It stops when all processes are finished.
 */
void SchedulerRR::simulate()
{
  int runTime = timeQuant;
  bool finishesRun = false;
  string tmp;
  organizePCB* pointPCB;
  
  while(!PCBQueue.empty())
  {    
    //pull front out of Q
    pointPCB = &PCBQueue.front();
    
    runTime = timeQuant;
    finishesRun = false;
    
    //determine run length
    if(timeQuant >= pointPCB->remainingBurst)
    {
      runTime = pointPCB->remainingBurst;
      finishesRun = true;
    }

    //run Process
    cout << "Running Process " << pointPCB->getName() << " for " << runTime << " time units." << endl;

    //update timeElapsed
    timeElapsed += runTime;

    if(finishesRun)
    {
      pointPCB->turnaroundTime = timeElapsed;
      pointPCB->waitTime = pointPCB->turnaroundTime - pointPCB->burstTime();

      tmp.clear();
      tmp = pointPCB->getName();
      tmp.erase(tmp.begin());
      finishedPCBs.at(stoi(tmp)-1).turnaroundTime = pointPCB->turnaroundTime;
      finishedPCBs.at(stoi(tmp)-1).waitTime = pointPCB->waitTime;
      pointPCB = nullptr;
    }
    else
    {
      pointPCB->remainingBurst -= runTime;
      PCBQueue.push_back(*pointPCB);
    } 
    PCBQueue.erase(PCBQueue.begin());
  }
}