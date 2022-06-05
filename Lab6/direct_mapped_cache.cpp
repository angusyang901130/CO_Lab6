#include "direct_mapped_cache.h"
#include "operation.h"
#include "string"
#include <vector>
#include <fstream>
#include <set>
#include <cmath>

float direct_mapped(string filename, int block_size, int cache_size){

    int total_num = 0;
    int hit_num = 0;
    
    /*write your code HERE*/
    // set up cache table
    int block_num = cache_size / block_size;   
    int tag_bit = 32 - log2(block_num) - log2(block_size);
    
    vector<string> cache_tag(block_num);
    vector<bool> valid(block_num, false);

    ifstream file;
    file.open(filename);
    string addr;

    while(file >> addr){
        total_num += 1;

        if(addr.size() < 32/4)
            addr = padding(addr);

        // address transfer hex to bin
        string addr_bin = "";
        for(int i = 0; i < addr.size(); i++){
            string bin = hex2bin(addr[i]);
            addr_bin += bin;
        }

        string tag = addr_bin.substr(0, tag_bit);
        int row_index = bin2dec(addr_bin.substr(tag_bit, log2(block_num)));

        if(valid[row_index] && comparer(cache_tag[row_index], tag)){
            hit_num += 1;
        }else if(valid[row_index] && !comparer(cache_tag[row_index], tag)){
            cache_tag[row_index] = tag;
        }else if(!valid[row_index]){
            cache_tag[row_index] = tag;
            valid[row_index] = true;
        }
        
    }

    file.close();
  
    return (float)hit_num/total_num;
}