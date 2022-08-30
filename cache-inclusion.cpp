#include<iostream>
#include<math.h>
#include<assert.h>
#include <bits/stdc++.h> 
#include <string>
#include <fstream>
using namespace std;
#define BLOCK_SIZE 64
#define L2_TOTAL_SETS 1024 
#define L3_TOTAL_SETS 2048
//Defining L2 Cache as a 2D-Array
unsigned long long l2cache[L2_TOTAL_SETS][8];

//Defining L3 Cache as a 2D-Array
 long long l3cache[L3_TOTAL_SETS][16];

//Defining counters for number of hits and misses.
unsigned long long l2access_counter, l2_hit, l2_miss, l3_hit, l3_miss;

//Class to Implement LRU ---------------------------------------------------------------------------------------------
class Lru
{     list <int> lines2;
      list <int> lines3;
     unordered_map<int, list<int>> sets2; 
     unordered_map<int, list<int>> sets3; 
     public :
       void init2();
       void init3();
       void lru2Update(int set,int way);
       void lru3Update(int set,int way);
       int lru2GetLine(int set);
       int lru3GetLine(int set);
       void lru2Invalidate(int set,int way);
      
}lru;
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
   for(int i = 0;i<2048;i++) 
    {  for(int i=0;i<16;i++)
       {
          lines3.push_back(i);
       } 
      sets3.insert(make_pair(i,lines3));
     lines3.clear();
   }
}

//method for updating L2 cache on a hit
void Lru::lru2Update(int set,int way)
{
     sets2.at(set).remove(way);
     sets2.at(set).push_front(way);  
     list <int> :: iterator it; 
//    for(it = sets2.at(set).begin(); it != sets2.at(set).end(); ++it) 
//        cout<< *it<<" ";
//       cout<<endl;
//    for(int i =0; i < )
}
// method for updating L3 cache on a hit
void Lru::lru3Update(int set,int way)
{
     sets3.at(set).remove(way);
     sets3.at(set).push_front(way);  
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
int Lru::lru3GetLine(int set)
{  int way;
   way=sets3.at(set).back();
   sets3.at(set).pop_back();
   sets3.at(set).push_front(way);
   return way;
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
    
    //initializing the L2 Cache with -1 value
    for(int index =0; index< L2_TOTAL_SETS; index++){
        for( int line =0 ; line < 8; line++){
            l2cache[index][line] = 0;
        }
    }
    //initializing the L2 Cache with -1 value
    for(int index =0; index< L3_TOTAL_SETS; index++){
        for( int line =0 ; line < 8; line++){
            l3cache[index][line] = 0;
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
    
//    // Storing replaced address to ensure that its block is present in L3 cache, if not then adding it
//    unsigned long long l2_replaced_address = l2cache[address_index][replace_line];
    
    //Inserting new block in l2 cache
    l2cache[address_index][replace_line] = address;
    lru.lru2Update(address_index, replace_line);
    
//Since replaced block will always be present in L3 cache (for inclustion policy) we don't need to do anything here for L3 cache.    
//    // If L2 cache replaced block is a valid block then make sure it is present in L3 cache - Inclusion policy
//    if(l2_replaced_address){
//    //Checking whether this replaced block is present in L3 cache or not
//        //Obtaining L3 cache index for this l2_replaced_address
//        unsigned long long replace_block_number = l2_replaced_address / BLOCK_SIZE;
//        int replacedBlock_l3_index =  int(replace_block_number % 2048);
//        unsigned long long replaced_tag = l2_replaced_address /131072;
//        //Searching in L3 cache - if hits:- do nothing;  else:-replace in l3 cache
//        int flag_hit =0;
//        for( int line =0 ; line < 16; line++){
//               if(replaced_tag == (l3cache[replacedBlock_l3_index][line]/131072)) {
//                    flag_hit =1;
//                    break;
//                } 
//        }
//            
//         
//        // if block is not present in L3 cache then place it in it
//        if(!flag_hit){
//            unsigned long long l3_replace_block_number = l2_replaced_address / BLOCK_SIZE;
//            int l3_replace_index = int(block_number % 2048);
//            //function that returns the L3 cache replaced line number
//            int l3_replace_line = lru.lru3GetLine(l3_replace_index); 
//            unsigned long long l3_replaced_address = l3cache[l3_replace_index][l3_replace_line];
//            if(l3_replaced_address){
//                //calling function to implement inclusion policy for block evicted from l3 cache.
//                inclusion_policy_l3_evict(l3_replaced_address);
//            }
//            l3cache[l3_replace_index][l3_replace_line] = l2_replaced_address;
//            lru.lru3Update(l3_replace_index,l3_replace_line);
//        }
//    }
}
    



// function to handle L3 cache mises and bringing the block from Main Memory.
void memory(unsigned long long address, int address_index){
    // Find the cache line number which needs to be replaced from L3 cache 
    int replace_line = lru.lru3GetLine(address_index);      
    unsigned long long l3_replaced_address = l3cache[address_index][replace_line];
    // If Replaced block is a valid block then invalidate that block in l2 cache (if present) - inclusion policy
    if(l3_replaced_address){
        //calling function to implement inclusion policy for block evicted from l3 cache.
        inclusion_policy_l3_evict(l3_replaced_address);
    }

    //Placing the missed block from M.M. to l3 Cache
    l3cache[address_index][replace_line] = address;
    lru.lru3Update(address_index,replace_line);
    
    // Providing this new block to L2 cache through inclusion policy.
     inclusion_policy_insert_in_l2(address);
}

// function to implement l3 Cache
void l3_cache(unsigned long long address){
    unsigned long long block_number;
    block_number = address / BLOCK_SIZE;
    int address_index = int(block_number % 2048);
    
    //Checking L3 cache hit or miss
    int flag_hit = 0;
     for( int line =0 ; line < 16; line++){
        if(block_number == (l3cache[address_index][line]/BLOCK_SIZE)) {
            flag_hit =1;
            //incrementing l3 cache hit count
            l3_hit++;
            //updating lru data structure for block hit
            lru.lru3Update(address_index,line);
            //inserting the block in L2 Cache
            inclusion_policy_insert_in_l2(address);
            break;
        }
    }
    
    //Handling L3 miss by calling memory function
    if(!flag_hit){
        l3_miss++;
        memory(address, address_index);
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
cout<<"l2_hit: "<<l2_hit<<endl<<"l2_miss: "<< l2_miss<<endl<<"l3_hit: "<<l3_hit<<endl<<"l3_miss: "<<l3_miss<<endl;
// cout<<l2_hit<<endl<< l2_miss<<endl<<l3_hit<<endl<<l3_miss;

return 0; 
}
