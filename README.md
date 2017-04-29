# Cliques
## Description
A clique is a subset of vertices of an undirected graph.

[Clique - Wikipedia](https://en.wikipedia.org/wiki/Clique_(graph_theory))

This project finds cliques in a range of clique sizes, and max clique size of a given graph.

Project input: 
- Graph: CSV / DOT graph format.
- Upper and lower bound of clique sizes in graph.
- A threshold value which indicates whether vertices are connected or not (if above value, vertices are connected).

Project output:
- CSV graph format with cliques in wanted range.
- Shows amount of cliques or graph’s max clique size.


*note:* 
part of the objective of this exercise was to improve an existing program (Cliques_Java.jar), in terms of speed and memory.

## Installation
The project’s folder includes a **Makefile**. All input arguments of the program are passed through the Makefile.
On the top of the makefile there is the user argument section for edit: Threshold value, lower bound, upper bound and input file. 
Calculate |max clique| and run program tests available by type **yes/no** in this section. 
After passed wanted arguments, run through **$make** command.

*note:* 
- for gtest and memory leak check, **$make installonce** command is needed.
- for Windows users see [pthread for Windows](http://www.sourceware.org/pthreads-win32/) since the pthread.h header file is for UNIX systems.

## Usage
[Clique Applications - Wikipedia](https://en.wikipedia.org/wiki/Clique_(graph_theory)#Applications)

## Algorithm
The main idea is vertices groups intersection. 
Let say A is a group of vertices in which all vertices are connected to vertex number i, (A is a group of all neighbours of vertex i). 
Now suppose vertex i is connected to vertex j, and B is a group of all neighbours of vertex j, by building group C =  A ∩ B ,
C will now include all vertices that connect to vertex i and vertex j. 
Recursively do this building action until we get cliques with size in the wanted range.

There are a few conditions we need to take into account before working on a group of vertices, after the conditions are met, 
we will call this group a “Potential Group” (we mentioned this as a comment in the code):

1. Given a group A of neighbours of vertex i, we need to make sure that 
|A|  >= (lower bound) - (number of intersections done until this point, which is the number of vertices in this clique until now).
If this isn’t true, we go to the previous intersection and then go to next i

2. If 1 is true, check that |B|  >= (lower bound) - (number of intersections done until this point, which is the number of vertices in this clique until now).
If this isn’t true, we go to the next j.

To make the algorithm even more efficient, we used multi-threaded programming, and every thread is given a different vertex, from which it will begin the clique construction.
To save memory, we used pointers which point to the vertices in the clique.

*note:* whoever wants to understand more, should go over the actual code.

## History
- First we published a version of code which doesn’t use threads at all. This version was way faster than the Java version with large clique sizes and consumed much less memory.

- Next, we published a version of code which runs with two threads. This had a significant influence on runtime. This was a trial version to test the algorithm using threads.

- Final Version: This version was implemented using multi-threads, which made an outstanding change in runtime in comparison to the previous versions, though consuming more memory.
The upper bound of this version, in terms of memory space, is O(|V|^3*sizeof(int)).

### Time comparison
threshold value= 0.74 , range= [20,25]
- multi_threads / two_threads / no_threads implementations 0.8 sec
- JAVA code 22.5 sec

threshold value= 0.55 , range= [48,50]
- multi_threads 4.3 sec
- two_threads 68 sec
- no_threads 103 sec
- JAVA code: out of space exception

threshold value= 0.55 , calculate |Max clique|
- multi_threads 0.88 sec
- two_threads 5.6 sec
- no_threads 11.5 sec
- JAVA code: not implement calculate |Max clique|, so input range [|max_clique|,|max_clique|] (|max_clique|= 40). Out of space exception.

##Known Issues
If you discover any bugs, feel free to create an issue on GitHub fork and send us a pull request.

[Issues List.](https://github.com/roibe/clique/issues)

##Authors
- Roi Benitah (https://github.com/roibe)
- Daniel Blank (https://github.com/danielblank1)
- Ariel Hazan (https://github.com/Ariel3)
- Netanel Ben-Haim

##Credit
This project is part of an exercise in Software Structure course in Ariel University with Dr. Boaz Ben-Moshe.

##Contributing
1. Fork it
2. Create your feature branch (git checkout -b my-new-feature)
3. Commit your changes (git commit -am 'Add some feature')
4. Push to the branch (git push origin my-new-feature)
5. Create new Pull Request

##License
None.






