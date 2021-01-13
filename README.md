
Programming Project
 
 
**Problem description**

The problem statement is detailed in the file "Problem Statement". The implementation specifics of the program are elaborated in the file "ADSProjectReport.pdf". The general flow of the program is explained under the section 'Program Structure' of the Report. 

A brief decsription of the project is as follows:

The aim of the project is to store a large number of records for easy retrieval and implement a scheduling algorithm to perform operations on these records. The following data structures were used:

1. A priority queue was implemented using min heap to schedule tasks.
2. A red-black tree was employed to store data and retrieve information in a quick and orderly manner

Implementation details:

* A struct was used to emulate each record.
* Records were inserted into a min heap to prioriotize the scheduling of operations on records. 
* The records were also stored in a Red-Black tree(RBT) data structure. 
* Each time a display/retrieve instruction was encounter the RBT data structure was accessed.
* Insert, delete, retreival operations on the RBT were completed in O(log n) time.

