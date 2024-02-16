/*
  Submitte by : Hadeer Farahat
  
  Submission for : FA21 COSC 594 - lab6 (RainbowGraph)

  This file includes the implementation of countWays(), CountPaths(), and NumWalks() functions from RainbowGraph class.
  The main goal of countWays() is to count how many ways can we reach a node from a starting node by passing all other nodes in the graph once.
  
  CountPaths() and NumWalks() are used as part of countWays() calculations.
  
  CountPaths() is a recursive function that use DFS to count the paths between each pair of nodes with the same color. 
  Each path has to include all nodes from the component.

  NumWalks() is a recursive function that uses dynamic programming to count the legal walks between one node and all other nodes of different color.
  The numWalks of each node are added together to calculate the final result in countWays() function.

*/

#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "RG.h"
using namespace std;

// Edges in the graph are specified by two vectors a and b, where the edges connect nodes a[i] and b[i].
int RainbowGraph::countWays(vector <int> color, vector <int> a, vector <int> b){
    int ncolors = 10;
    Color = color;  // get a copy of color vector to use it in other functions ..

    //int N=a.size(); // this should be the number of edges 
    int N = color.size();   // #nodes
    
    int E = a.size();       // #edges

    //check values
    //cout <<"NP.size()  = "<< NP.size()<< endl ;
    //cout <<"Edges  = "<< E << endl ;
    //cout <<"Nodes  = "<< N << endl ;

    //NP[i][j] is non-zero only if nodes i and j are in the same connected component
    //It contains the number of paths from node i to node j, where each path must contain every node in the component.
    NP.resize(N, vector<int> (N,0));
    
    //create Same matrix (Adjacency lists of intracomponent edges)    
    Same.resize(N,vector<int> (N,0));
    for (int i=0;i<N;i++){    // for each node ..
      for (int j=0;j<E;j++){  // check all edges
        if (a[j]==i && color[a[j]]== color[b[j]]){ // to see if it connects to a node of same color 
          Same[a[j]][b[j]] = 1;
          Same[b[j]][a[j]] = 1;
        }                  
      }
    }
    // For debugging -- print out Same Mat.
    //cout << "Same adjacency matrix : \n";
    /* for (int i=0;i<N;i++){      
      for (int j=0;j<N;j++){        
        cout << Same[i][j] << "   ";
      }
      cout << endl;
    } */

    //create Diff matrix (Adjacency lists of intercomponent edges)
    Diff.resize(N,vector<int> (N,0));
    int x,y;
    for (int i=0;i<E;i++){      // this should check a vs. b (edges)
      // x & y are nodes connected by an edge
      x = a[i];
      y = b[i];
      if (color[x]!=color[y]){ // if they have different colors, add them to Diff mat.
        Diff[x][y] = 1;
        Diff[y][x] = 1;
      }        
    }

     //For debugging -- print out Diff Mat.
    /* cout << "========================================== \nDiff adjacency matrix : \n";
    for (int i=0;i<N;i++){      
      for (int j=0;j<N;j++){        
        cout << Diff[i][j] << "   ";
      }
      cout << endl;
    }
    cout << "\n\n======================================\n";
    */
    // Cnodes[i] contains all nodes whose color is i.
    CNodes.resize(ncolors);    
    int cnt=0;
    for (int i=0;i<ncolors;i++){  
      for (int j=0;j<N;j++){        
        if(color[j]==i){
          CNodes[i].push_back(j);
        }
      }

    }



/*      Part 1 -- Applying DFS     */

    // int NIP    -- During the DFS, this is the number of nodes in the current path.
    // int Source -- This is the initial node for each DFS call.
    // int Target --  The size of Source's component: Cnodes[Source].size()

    V.resize(N,0);    //when you're done with a node, you set V back to zero
     for (int i = 0; i < N; i++)
    {
      V.resize(0);
      V.resize(N,0);
      NIP = 0;  //NIP (nodes in the path)      
      Source = i;
      Target = CNodes[color[Source]].size();
      if (Target > 0 ) CountPaths(Source);      
    } 
    
/*      Part 2 -- Calculating NumWalks() Dynamic Programming     */

    int setid;  // id for all color numbers
    setid = (0 << CNodes.size());
    for (int i =0; i < ncolors; i++ ){ 
      if (CNodes[i].size()>0){
        setid |= (1 << i);        
      }      
    }
                 
    Cache.resize(N,vector<long long> (setid ,-1)); //size of cache is nodes * combination of remaining sets(setid - 1 ). initialize cache to -1.

    /*  for each node, set the remaining components and call NumWalks*/
    int rset; // remaining component set
    int res = 0 ; // res will have the final result 
    for (int i = 0; i< N ; i++){      
      rset = setid ^ (1 << color[i]); // determine the remaining set-id (all colors except current node's color)
      res += NumWalks(i,rset) ;   // calculate the required value by adding num-walks of all nodes through all other colors
      res = res % 1000000007 ;    // prevents overflaw 
    }

    //If the Verbose string contains the character 'N', then you should print out all non-zero values of NP, one per line,
    // in the form: NP[i][j] = value

    //if (Verbose.find('N') != string::npos)
    if (Verbose == "N"){
      for (int i = 0; i < N; i++)
      {
       for (int j = 0; j < N; j++)
       {
         if (NP[i][j] != 0)
          printf("NP[%d][%d] = %d\n", i  , j , NP[i][j]);
       }       
      }      
    }
    
    //If the Verbose string contains the character 'C', then you should print out your dynamic programming cache, one entry per line, 
    //in the form:  Cache[node][setid] = value

    //if (Verbose.find('C') != string::npos)
    if (Verbose == "C"){

      for (int i = 0; i < N; i++)
      {        
       for (int j = 0; j < Cache[i].size(); j++)
       {         
         if( Cache[i][j] !=-1 ) // only print the cache values that has been set.
          printf("Cache[%d][0x%x] = %d\n", i  , j , Cache[i][j]);
       }       
      }      
    }

    return res ; //(res % 1000000007);
}

// CountPaths() is the extended DFS.  Set Source, Target, V and NIP before you call CountPaths(Source) to set NP[Source][j].
//when you're done with a node, you set V back to zero

void RainbowGraph::CountPaths(int n){
  // you can calculate NP[i][j] for all j using an enhanced DFS which travels every intracomponent path from node i. 
  if (V[n] == 1) return;  // if node was already visited, return...
  NIP++;
  V[n]=1;

  //if (Source != n && NIP == CNodes[Color[Source]].size()){ // some cases has just one node in a component, so source == n !
  if (NIP == CNodes[Color[Source]].size()){ // if number of nodes in the path equals to the number of nodes in the component,
                                            // that means that we found a path includes all nodes of same component
        NP [Source][n] ++;
        //NP [n][Source] ++;  //check that one
  }
  for (int i = 0; i < Target; i ++){ 
    int x = CNodes[Color[Source]][i];
    //int y = CNodes[Source][n];   // deal with n directly
    
    if (V[x] == 0 && Same[x][n] == 1){   // check the intracomponent path
      CountPaths(x);      
      // set V back to zero to participate in other paths
      // and decrement 1 from NIP as this path is loosing one node from it
      V[x]=0; 
      NIP --;
    }
  }
} 

// Number of walks starting at node n that 
// still need to go through the nodes in setid.
long long RainbowGraph::NumWalks(int n, int s){
  
  if (Cache[n][s] != -1 ){ // check if the value is already in cache
    return Cache[n][s];     // return it!
  }
  long long val = 0;
  int nc = CNodes[Color[n]].size();   // this is the number of nodes that has the same color of node n
  int m;  // m will represent each node from n's component (nodes that have same color as n)

  //You need a base case for this -- if s = {}, then NumWalks(n, s) is the sum of all NP[n][m]
  if (s == 0){
    for (int i = 0; i < nc; i ++){
      m = CNodes[Color[n]][i];    
      val +=NP[n][m];
      val = val ; //% 1000000007 
    }
    Cache[n][s] = val;  // sum of all NP[n][m]
    return   Cache[n][s];
  }    

  // remaining_components is an integer that stores a set of components using bit arithmetic.
  // You are going to sum up NumWalks(n, s) for every node n, and every set s composed of all of the components except for n's component.   
  Cache[n][s] = 0;
  for (int i = 0; i < nc; i ++){
    m = CNodes[Color[n]][i];
    if (NP[n][m] > 0){
      //then you look at every node l which is connected to m and in a component in s (setid)      
      for (int l =0 ; l<Color.size()/*#nodes*/; l++){
        if( Diff[l][m] == 1  && ((1 << Color[l]) & s)){// l connected to m, and in a component of s

          //You will add the product of that and NP[n][m] to the return value for NumWalks(n, s)
          // taking the mod by 1000000007 for each operation prevents overflaw 
          Cache[n][s] = (Cache[n][s] + (NP[n][m]* (NumWalks(l,s^(1 << Color[l]))% 1000000007))% 1000000007)% 1000000007 ;
        }
      }      
    }            
  }

  return Cache[n][s];
}