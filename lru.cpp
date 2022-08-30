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

//Defining L3 Cache as a 2D-Array
 // long long l3cache[32768];

//Defining counters for number of hits and misses.
unsigned long long l2access_counter, l2_hit, l2_miss, l3_hit, l3_miss;

//defining vectors,integer variables, and unsigned long long variables for implementation of Belady's Min Policy 
unordered_map<int , unsigned long long> l3cache;
unordered_map<unsigned long long,unsigned long long> blocks_in;
unordered_map<unsigned long long,unsigned long long> block_lru;
map<unsigned long long,unsigned long long> lru_block;
unsigned long long blocks_used =0,block_replaced; 
unsigned long long blocks_in_use=0;
unsigned long long index_put;
unsigned long long ref;
unsigned long long replace_index;
unsigned long long replace_block,req,ref_old;
int it=0;
int flag=0;
//Class to Implement LRU ---------------------------------------------------------------------------------------------
class Lru
{     list <int> lines2;
      list <int> lines3;
     unordered_map<int, list<int>> sets2; 
     //unordered_map<int, list<int>> sets3;
     unordered_map<int, list<int>> cold_miss;  
     public :
       void init2();
       void init3();
       void lru2Update(int set,int way);
       void lru3Update(int way);
       int lru2GetLine(int set);
       int lru3GetLine();
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

//method for initializing LRU for L3 cache
void Lru :: init3()
{
  for(int i = 0;i<32768;i++) 
    {     
          lines3.push_back(i);
           // cout<<i<<" "; 
    }
    
}

//method for updating L2 cache on a hit
void Lru::lru2Update(int set,int way)
{
     sets2.at(set).remove(way);
     sets2.at(set).push_front(way);  
   
}
// method for updating L3 cache on a hit
void Lru::lru3Update(int way)
{
     lines3.remove(way);
     lines3.push_front(way); 
}
//method for updating L2 cache on a miss
int Lru::lru2GetLine(int set)
{  int way;
   way=sets2.at(set).back();
   sets2.at(set).pop_back();
   sets2.at(set).push_front(way);
   return way;
   
}
//method for updating L3 cache on a miss
int Lru::lru3GetLine()
{  int line;
   line=lines3.back();
   lines3.pop_back();
   lines3.push_front(line);
   return line;
}
//method for Invalidating from L2 (inclusive policy)
void Lru::lru2Invalidate(int set,int way)
{
   sets2.at(set).remove(way);
   sets2.at(set).push_back(way);
}
// End of LRU -----------------------------------------------------------------------------------------------------------

// function to initialize the global variables.
void init(){
    l2access_counter = l2_hit = l2_miss = l3_hit = l3_miss = 0;
    
    //initializing the L2 Cache with 0 value
    for(int index =0; index< L2_TOTAL_SETS; index++){
        for( int line =0 ; line < 8; line++){
            l2cache[index][line] = 0;
        }
    }
}

// function to implement inclusion policy for L3 cache block eviction
void inclusion_policy_l3_evict(unsigned long long l3_replaced_address){
// Checking whether replaced block is present in l2 cache or not
    //Obtaining address_index for l2 cache
    unsigned long long block_number;
    block_number = l3_replaced_address / BLOCK_SIZE;
    int address_index = int(block_number % 1024);

    //Seaching in l2 cache set 
    for(int line =0 ; line<8; line++){
        // if present then invalidate the block and update the l2_lru else do nothing.
        if(block_number == (l2cache[address_index][line]/BLOCK_SIZE)){
            l2cache[address_index][line] =0;
            lru.lru2Invalidate(address_index,line); 
            break;
        }
    }
}

//function to implement inclusion policy for adding a block from l3 cache to l2 cache
void inclusion_policy_insert_in_l2(unsigned long long address){
    unsigned long long block_number;
    block_number = address / BLOCK_SIZE;
    int address_index = int(block_number % 1024);
    //getting line which needs to be replaced with the new block from l3 cache
    int replace_line = lru.lru2GetLine(address_index);
    
    
    //Inserting new block in l2 cache
    l2cache[address_index][replace_line] = address;
    lru.lru2Update(address_index, replace_line);
    
}

// function to implement l3 Cache
void l3_cache(unsigned long long address){
    unsigned long long block_number;
    req = address / BLOCK_SIZE;
    it++;
    
   	if(blocks_in.find(req)==blocks_in.end())
       { // cout<<"I am miss " << req<<endl;
          //if a block of main memory is accessed then marking it as 1
        lru.count_cold(req); 
          l3_miss++;
        //codde for handling misses,if number of cache lines occupied are smaller than the total number of cache lines
          if(blocks_in_use<32768)
           {
             blocks_in.insert(make_pair(req,address));
             lru_block.insert(make_pair(it,req));
             block_lru.insert(make_pair(req,it));
             blocks_in_use++ ;
			 inclusion_policy_insert_in_l2(address);
           }
         
        //code for handling misses, if the number of lines occupied are equal to cache lines.
          else{
			  unsigned long long min_ref;
             //find minimum key from the lru_block;
              auto iterator=lru_block.begin();
              min_ref = iterator->first;
                          
              replace_block = lru_block.at(min_ref);
              unsigned long long l3_replaced_address= blocks_in.at(replace_block);
			  
			  inclusion_policy_l3_evict(l3_replaced_address);
			  inclusion_policy_insert_in_l2(address);
              
			  block_lru.erase(replace_block);
          
            //place the new block(key) i.e req and value as the reference i in the block_lru
              block_lru.insert(make_pair(req,it));
          
             // now delete the minimum key, value from the lru_block
              lru_block.erase(min_ref);
          
            // place new reference(key) that is i and the new block i.e. req in the lru_block
              lru_block.insert(make_pair(it,req));
              blocks_in.erase(replace_block);
             
             // put new block to the index at the place of the old_block
              blocks_in.insert(make_pair(req,address));             
            }
       }
   //code for handling hits
     else {
              l3_hit++;
              ref_old = block_lru.at(req);
              lru_block.erase(ref_old);
              lru_block.insert(make_pair(it,req));
              block_lru[req] = it;
			  inclusion_policy_insert_in_l2(address);
              
         }
    
    //Debugging
//    cout<<"l3_misses: "<<l3_miss;
}

// function to implement l2 Cache
void l2_cache(unsigned long long address){
    //Caculating block number and index of the referenced address
    unsigned long long block_number;
    block_number = address / BLOCK_SIZE;
    int address_index = int(block_number % 1024);
    
    //Checking l2 cache hit or miss
    int flag_hit = 0;
     for( int line =0 ; line<8 ; line++){
        if(block_number == (l2cache[address_index][line]/BLOCK_SIZE)) { 
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
        l3_cache(address);
    }
    
    //Debugging
//    cout<<"l2_misses: "<<l2_miss;
}

// Main function of program.
int main(int argc, char **argv){
    // const char* input_file = "traces/h264ref.log_l1misstrace_0";
	char input_file[100];
    int k;
 // Defining char pointers
    char iord[2];
    char type[2];
    unsigned long long addr[10000];
    unsigned pc[10000];  
    // Initializing the caches and LRU
    init();
    lru.init2();
    lru.init3();
 for(k=0;k<atoi(argv[1]);k++){
 sprintf(input_file, "traces/%s.log_l1misstrace_%d", argv[2],k);
    //Opening the input file in fp
    FILE *fp = fopen(input_file, "rb");
    assert(fp != NULL);
    
//    FILE *fw = fopen("output.txt","w");
    
        
// Reading the file fp
while(!feof(fp)) {
 fread(&iord, sizeof(char), 1, fp);
 fread(&type, sizeof(char), 1, fp);
 fread(&addr, sizeof(unsigned long long), 1, fp);
 fread(&pc, sizeof(unsigned), 1, fp);
 
 
// printf("iord: %d\n",int(iord[0]));
// printf("type: %d\n",int(type[0]));
// printf(fw,"addr: %llu\n",*addr);
// printf(fw,"pc: %u\n",*pc);
 unsigned long long address = *addr;
 if(int(type[0]) ==1){
     l2access_counter++;
     l2_cache(address);
 }
}
//closing the file fp
fclose(fp);
printf("\nDone reading file!\n");
 }
// Printing output 
cout<<"Cache Policy: Inclusion"<<endl;
cout<<"Results for application: "<<input_file<<endl;
cout<<"L2 accesses: "<<l2access_counter<<endl;
// cout<<l2access_counter<<endl;
int cold_miss = lru.count_cold_miss();
int l3_capacity_miss = l3_miss-cold_miss;
cout<<"l3_misses: "<<l3_miss<<endl;
// cout<<l2_hit<<endl<< l2_miss<<endl<<l3_hit<<endl<<l3_miss;
cout<<"l3_cold_misses: "<<cold_miss<<endl<<"l3_capacity_misses: " <<l3_capacity_miss<<endl;

return 0; 
}
