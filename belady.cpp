#include<iostream>
#include<math.h>
#include<assert.h>
#include <bits/stdc++.h> 
#include <string>
#include <fstream>
using namespace std;
#define BLOCK_SIZE 64
#define L2_TOTAL_SETS 1024 
#define L3_TOTAL_SETS 32768
//Defining L2 Cache as a 2D-Array
unsigned long long l2cache[L2_TOTAL_SETS][8];

//Defining counters for number of hits and misses.
unsigned long long l2access_counter=0, l2_hit=0, l2_miss=0, l3_hit=0, l3_miss=0,l3access_counter;
   
//defining vectors,integer variables, and unsigned long long variables for implementation of Belady's Min Policy 
vector<unsigned long long>  l3cache(32768,0);
vector<unsigned long long> ::iterator check_it; 
vector<unsigned long long> early_block_mark(32768,0);
vector<unsigned long long>blocks_access;
unsigned long long blocks_used =0,block_replaced; 
int blocks_in_use=0;
int index_put;
unsigned long long replace_index;
int flag=0;

//Class to Implement LRU for L2 cache and to count number of cold misses in Fully Associative cache ----------------------------------------------------------------------------------------------------
class Lru
{     list <int> lines2;
      unordered_map<int, list<int>> sets2; 
      unordered_map<int, list<int>> cold_miss;  
     public :
       void init2();
       void lru2Update(int set,int way);
       int lru2GetLine(int set);
       void lru2Invalidate(int set,int way);
       void  count_cold(int block);
       int count_cold_miss();
}lru;
//mark every visited block
void Lru::count_cold(int block)
{
  cold_miss.insert(make_pair(block,1));
}
//count cold misses
int Lru :: count_cold_miss()
{ int miss;
  miss = cold_miss.size();
  return miss;
}
//method for initializing LRU for L2 cache  
void Lru :: init2()
{
   for(int i = 0;i<1024;i++) 
    {  for(int j=0;j<8;j++)
       {
          lines2.push_back(j);
       } 
      sets2.insert(make_pair(i,lines2));
     lines2.clear();
   }
}
//method for updating L2 cache on a hit
void Lru::lru2Update(int set,int way)
{
     sets2.at(set).remove(way);
     sets2.at(set).push_front(way);  
   
}
//method for updating L2 cache on a miss
int Lru::lru2GetLine(int set)
{  int way;
   way=sets2.at(set).back();
   sets2.at(set).pop_back();
   sets2.at(set).push_front(way);
   return way;
   
}
//method for Invalidating from L2 (inclusive policy)
void Lru::lru2Invalidate(int set,int way)
{
   sets2.at(set).remove(way);
   sets2.at(set).push_back(way);
}
// End of LRU -----------------------------------------------------------------------------------------------------------
//Method to implement L3 cache with Belady's Min Policy 
int belady (unsigned long long reference_num)
{      flag=0;
       int is_replace=0;
        
        //cout<<reference_num<<endl;
        
         if(blocks_in_use<32768)
          {  l3cache[blocks_in_use] = blocks_access[reference_num];
             blocks_in_use++;
             return 0;
          }
           else
          {
            for(int j=0;j<l3cache.size();j++)
              {
                if(early_block_mark[j]<reference_num)
                {
                    
                    check_it = find(blocks_access.begin()+reference_num+1,blocks_access.end(),l3cache[j]);
                    index_put =distance(blocks_access.begin(),check_it);
                    if(index_put == blocks_access.size())
                      {
                         flag=1;
                         //Debugging
                         // cout<<" block "<<blocks[j] << " found at index "<<index_put<<endl;   
                         replace_index=j;     
                         break;
                      }
                           
                    early_block_mark[j]=index_put;     
                //Debugging
                //cout<<" block "<<blocks[j] << " found at index "<<index_put<<endl;                                
         
                 }  
                 
               }
               
            } 
               if(flag!=1)
                {  
    replace_index = distance(early_block_mark.begin(), max_element(early_block_mark.begin() ,early_block_mark.end()));
                    block_replaced = l3cache[replace_index];
                    l3cache[replace_index] = blocks_access[reference_num]; 
                    is_replace = 1;
                }
             else 
                { //Debugging
                  // cout<< "I am now replacing  "<< blocks[b] << " and I am "<<blocks_access[i]<<endl;
                  block_replaced = l3cache[replace_index];   
                  l3cache[replace_index] = blocks_access[reference_num]; 
                }
              if(is_replace==1)
                {
                   early_block_mark[replace_index]=0;
                       
                } 
                
              return block_replaced;
            
 }
// End of Belady's MIN Policy -----------------------------------------------------------------------------------------------------------

// function to initialize the global variables.
void init(){
    //initializing the L2 Cache with 0
    for(int index =0; index< L2_TOTAL_SETS; index++){
        for( int line =0 ; line < 8; line++){
            l2cache[index][line] = 0;
        }
    }
 }
// function to implement inclusion policy for L3 cache block eviction
void inclusion_policy_l3_evict(unsigned long long block_number){
// Checking whether replaced block is present in l2 cache or not
    //Obtaining address_index for l2 cache
    int address_index = int(block_number % 1024);

    //Seaching in l2 cache set 
    for(int line =0 ; line<8; line++){
        // if present then invalidate the block and update the l2_lru else do nothing.
        if(block_number == (l2cache[address_index][line])){
            l2cache[address_index][line] =0;
            lru.lru2Invalidate(address_index,line); 
            break;
        }
    }
}
//function to implement inclusion policy for adding a block from l3 cache to l2 cache
void inclusion_policy_insert_in_l2(unsigned long long block_number){

    int address_index = int(block_number % 1024);
    //getting line which needs to be replaced with the new block from l3 cache
    int replace_line = lru.lru2GetLine(address_index);
    
    
    //Inserting new block in l2 cache
    l2cache[address_index][replace_line] = block_number;
    lru.lru2Update(address_index, replace_line);
    
}

//function to implement l3 cache
void l3_cache(unsigned long long block_number, unsigned long long reference_num){
      

        check_it = find(l3cache.begin(),l3cache.end(),blocks_access[reference_num]);
        if(check_it!=l3cache.end())  {
         l3_hit++;
         //cout<<i<<" hit on "<<blocks_access[i]<<endl; 
        }
       else {
          l3_miss++;
          unsigned long long replace;
          lru.count_cold(block_number); 
          //cout<<"block number " << block_number<<endl;
          //cout<<i<<" miss on "<<blocks_access[i]<<endl;
           replace=0;
           replace = belady(reference_num);
           if(replace) {
             inclusion_policy_l3_evict(replace);

           
           }     
           

       }    
           inclusion_policy_insert_in_l2(block_number);

}

// function to implement l2 Cache
void l2_cache(unsigned long long block_number, unsigned long long reference_num){
    //Caculating block number and index of the referenced address
    int address_index = int(block_number % 1024);
    //Checking l2 cache hit or miss
    int flag_hit = 0;
     for( int line =0 ; line<8 ; line++){
        if(block_number == (l2cache[address_index][line])) { 
            flag_hit =1;
            //incrementing l2 cache hit count
            l2_hit++;
            //updating lru data structure for block hit
            lru.lru2Update(address_index,line);
            break;
        }
    }
    
    //Calling L3 cache in case of a L2 miss
    if(!flag_hit){
        l2_miss++;
        l3_cache(block_number,reference_num);
    }

}

 
//main function    
int main(int argc, char **argv){
    // const char* input_file = "traces/h264ref.log_l1misstrace_0";
	char input_file[100];
    int k;
    unsigned long long block_num;
 // Defining char pointers
    char iord[2];
    char type[2];
    unsigned long long addr[10000];
    unsigned pc[10000]; 
  
    //Initializing the caches and LRU
      init();
      lru.init2();

  for(k=0;k<atoi(argv[1]);k++){
  sprintf(input_file, "traces/%s.log_l1misstrace_%d", argv[2],k);
    //Opening the input file in fp
    FILE *fp = fopen(input_file, "rb");
    assert(fp != NULL);
    
//    FILE *fw = fopen("output.txt","w");

 while(!feof(fp)) {
  
 fread(&iord, sizeof(char), 1, fp);
 fread(&type, sizeof(char), 1, fp);
 fread(&addr, sizeof(unsigned long long), 1, fp);
 fread(&pc, sizeof(unsigned), 1, fp);
 unsigned long long address = *addr;
 if(int(type[0]) ==1){
	block_num = address/64;
        blocks_access.push_back(block_num);
       //cout<<"From second while loop block num " <<block_num<<endl;
       //cout<<"from block access "<<blocks_access[i-1]<<endl;

       
  }     
 }
}
 printf("\nAll the references are stored in an array for future references for implementing Belady's Min !\n");
 cout<<endl;

 //fflush(fp);
 //rewind(fp);
 
// Reading the file fp

for(k=0;k<atoi(argv[1]);k++){
  sprintf(input_file, "traces/%s.log_l1misstrace_%d", argv[2],k);
    //Opening the input file in fp
    FILE *fp = fopen(input_file, "rb");
    assert(fp != NULL);
    
//    FILE *fw = fopen("output.txt","w");

 while(!feof(fp)) {
  
 fread(&iord, sizeof(char), 1, fp);
 fread(&type, sizeof(char), 1, fp);
 fread(&addr, sizeof(unsigned long long), 1, fp);
 fread(&pc, sizeof(unsigned), 1, fp);
 unsigned long long address = *addr;
 if(int(type[0]) ==1){
	block_num = address/64;
	l2access_counter++;
        //blocks_access.push_back(block_num);
       //cout<<"From second while loop block num " <<block_num<<endl;
       //cout<<"from block access "<<blocks_access[i-1]<<endl;
        l2_cache(block_num,l2access_counter-1);

       
  }     
 }


//closing the file fp
fclose(fp);
printf("\n Done reading file !\n");
}


// Printing output 
cout<<"Cache Policy: Inclusion"<<endl;
cout<<"Results for application: "<<input_file<<endl;
//cout<<"L2 accesses: "<<l2access_counter<<endl;
// cout<<l2access_counter<<endl;
int cold_miss = lru.count_cold_miss();
int l3_capacity_miss = l3_miss-cold_miss;
//cout<<"l2_hit: "<<l2_hit<<endl<<"l2_miss: "<< l2_miss<<endl<<"l3_hit: "<<l3_hit<<endl<<"l3_miss: "<<l3_miss<<endl;
cout<<"l3_miss: "<<l3_miss<<endl;
cout<<"l3_cold_misses: "<<cold_miss<<endl<<"l3_capacity_misses: " <<l3_capacity_miss<<endl;
//cout<<"l3_conflict_misses : 0 (as there is no conflict misses in fully associative cache)";

return 0; 
}
