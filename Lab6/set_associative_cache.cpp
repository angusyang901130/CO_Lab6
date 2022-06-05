#include "set_associative_cache.h"
#include "operation.h"
#include "string"
#include <vector>
#include <fstream>
#include <set>
#include <cmath>

float set_associative(string filename, int way, int block_size, int cache_size){
    int total_num = 0;
    int hit_num = 0;

    /*write your code HERE*/
    // set up cache table
    int block_num = cache_size / block_size;
    //cout << "block_num" << block_num << endl;
    int row_num = block_num / way;
    
    int tag_bit = 32 - log2(row_num) - log2(block_size);
    
    
    vector< vector<string> > cache_tag(row_num, vector<string>(way, ""));
    vector< vector<bool> > valid(row_num, vector<bool>(way, false));
    vector<string> used_order;
    
    // read file
    ifstream file;
    file.open(filename);
    string addr;

    while(file >> addr){
        total_num += 1;
        
        //cout << total_num << endl;

        if(addr.size() < 32/4)
            addr = padding(addr);
        
        // address transfer hex to bin
        string addr_bin = "";
        for(int i = 0; i < addr.size(); i++){
            string bin = hex2bin(addr[i]);
            addr_bin += bin;
        }

        // take address [tag_bit:tag_bit+log2(rownum)]
        string tag = addr_bin.substr(0, tag_bit);
        int row_index = bin2dec(addr_bin.substr(tag_bit, log2(row_num)));
        bool saved = false;

        // if saved in cache, no need to replace
        // if not saved, follow LRU
        for(int i = 0; i < way; i++){
            if(valid[row_index][i] && comparer(cache_tag[row_index][i], tag)){
                hit_num += 1;
                saved = true;
                break;
            }else if(!valid[row_index][i]){
                cache_tag[row_index][i] = tag;
                valid[row_index][i] = true;
                saved = true;
                break;
            }
        }

        // LRU
        if(!saved){
            string* ptr = &used_order.back();
            set<string> st;
            
            // put stored tag of cache_tag[row_index] into set 
            for(int i = 0; i < way; i++)
                st.insert(cache_tag[row_index][i] + addr_bin.substr(tag_bit, log2(row_num)));

            // search in used_order, if found one used is stored in set, erase it until set size = 1 
            // if set size == 1, means only one tag left, it is the least recently used tag
            while(st.size() > 1){
                set<string>::iterator pos_iter = st.find(*ptr);
                
                if(pos_iter != st.end())
                    st.erase(pos_iter);

                ptr--;
            }

            // replace the tag with new tag
            for(int i = 0; i < way; i++){
                if(cache_tag[row_index][i] == (*st.begin()).substr(0, tag_bit)){
                    cache_tag[row_index][i] = tag;
                    break;
                }
            }
        }

        used_order.push_back(addr_bin.substr(0, tag_bit + log2(row_num)));

    }

    file.close();

    return (float)hit_num/total_num;
}