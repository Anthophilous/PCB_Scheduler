/**
* Assignment 3: CPU Scheduler
 * @file scheduler_priority.cpp
 * @author Joshua White
 * @brief This Scheduler class implements the Priority scheduling algorithm.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code


#include "scheduler_priority.h"

/*
* @brief compares the priorities of two PCBs, 
* @params two PCBs to be compared
* @return returns true if first is higher priority
*/
static bool compPriority(PCB a, PCB b)
{
  return a.priority > b.priority;
}

    /**
     * @brief Construct a new SchedulerPriority object
     */
SchedulerPriority::SchedulerPriority()
{
  numPCBs = 0;
  schedulerVector.clear();
  PCBVector.clear();
}

/**
 * @brief Destroy the SchedulerPriority object
 */
SchedulerPriority::~SchedulerPriority()
{
  schedulerVector.clear();
  PCBVector.clear();
}

/**
 * @brief This function is called once before the simulation starts.
 *        It is used to initialize the scheduler.
 * @param process_list The list of processes in the simulation.
 */
void SchedulerPriority::init(std::vector<PCB>& process_list)
{
  schedulerVector = process_list;
  numPCBs = schedulerVector.size();
  PCBVector.resize(schedulerVector.size());
  //call sorting functions
  stable_sort(schedulerVector.begin(), schedulerVector.end(),compPriority);
  //reverse(schedulerVector.begin(),schedulerVector.end());
}


/**
 * @brief This function is called once after the simulation ends.
 *        It is used to print out the results of the simulation.
 */
void SchedulerPriority::print_results()
{
  //Averages
  float avgWaitTimes = 0;
  float avgTurnaroundTimes = 0;
  
  //Display Each PCB's stats
  for(int i = 0; i < numPCBs; i++)
    {
      cout << "T" << i+1 << " turnaround time = " << PCBVector.at(i).turnaroundTime << ", waiting time = " << PCBVector.at(i).waitTime << endl;

      avgTurnaroundTimes += PCBVector.at(i).turnaroundTime;
      avgWaitTimes += PCBVector.at(i).waitTime;
    }

  //Calc Avg
  avgTurnaroundTimes /= numPCBs;
  avgWaitTimes /= numPCBs;

  //Display Average turn-around time = 94.375, Average waiting time = 73.125
  cout << "Average turn-around time = " << avgTurnaroundTimes << ", Average waiting time = " << avgWaitTimes << endl;
}

/**
* @brief This function simulates the scheduling of processes in the ready queue.
*        It stops when all processes are finished.
*/
void SchedulerPriority::simulate()
{
  int PCBIndex = 0;
  int prevTurnaroundTime = 0;
  string tmp;
  
  for(int i = 0; i < numPCBs; i++)
  {
    //Cout Duration of Running Current Process
  	cout << "Running Process " << schedulerVector.at(i).name << " for " << schedulerVector.at(i).burst_time << " time units." << endl;

    //Get Index
    tmp = schedulerVector.at(i).name;
    tmp.erase(tmp.begin());
    PCBIndex = stoi(tmp) - 1;
    tmp.clear();
    
    //Calculate TT And Add to PCBVector
    PCBVector.at(PCBIndex).turnaroundTime = (schedulerVector.at(i).burst_time + prevTurnaroundTime) - schedulerVector.at(i).arrival_time;
    prevTurnaroundTime = PCBVector.at(PCBIndex).turnaroundTime;

    //Calculate WT
    PCBVector.at(PCBIndex).waitTime = PCBVector.at(PCBIndex).turnaroundTime - schedulerVector.at(i).burst_time;
  }
}