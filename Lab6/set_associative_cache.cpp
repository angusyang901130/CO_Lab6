#include "set_associative_cache.h"
#include "string"
#include <vector>
#include <fstream>
#include <bitset>
#include <sstream>
#include <set>

using namespace std;
int bitnum(int);
string hex2bin(char);
int bin2dec(string);
string dec2bin(int);
bool comparer(string, string);
string padding(string);


float set_associative(string filename, int way, int block_size, int cache_size){
    int total_num = 0;
    int hit_num = 0;

    /*write your code HERE*/
    // set up cache table
    int block_num = cache_size / block_size;
    int row_num = block_num / way;
    
    int tag_bit = 32 - bitnum(row_num) - bitnum(block_size);
    
    vector< vector<string> > cache_tag(row_num, vector<string>(way));
    vector< vector<bool> > valid(row_num, vector<bool>(way, false));
    vector<string> used_order;
    
    // read file
    ifstream file;
    file.open(filename, 'r');
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

        // take address [tag_bit:tag_bit+bitnum(rownum)]
        string tag = addr_bin.substr(0, tag_bit);
        int row_index = bin2dec(addr_bin.substr(tag_bit, bitnum(row_num)));
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
                break
            }
        }

        // LRU
        if(!saved){
            string* ptr = &used_order.back();
            set<string> st;
            
            // put stored tag of cache_tag[row_index] into set 
            for(int i = 0; i < way; i++)
                st.insert(cache_tag[row_index][i]);

            // search in used_order, if found one used is stored in set, erase it until set size = 1 
            // if set size == 1, means only one tag left, it is the least recently used tag
            while(st.size() > 1){
                pos_iter = st.find(*ptr);

                if(pos_iter != st.end())
                    st.erase(pos_iter);

                else ptr--;
            }

            // replace the tag with new tag
            for(int i = 0; i < way; i++){
                if(cache_tag[row_index][i] == *st.begin()){
                    cache_tag[row_index][i] = tag;
                    break;
                }
            }
        }

        used_order.push_back(addr_bin.substr(0, tag_bit + bitnum(row_num)));

    }

    print(total_num);
    
    return (float)hit_num/total_num;
}

int bitnum(int num){
    int power = -1;
    while(num != 0){
        power += 1;
        num /= 2;
    }

    return power;
}

string hex2bin(char c){
    switch(c){
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'a': return "1010";
        case 'b': return "1011";
        case 'c': return "1100";
        case 'd': return "1101";
        case 'e': return "1110";
        case 'f': return "1111";
    }
}

int bin2dec(string bin){
    int base = 1;
    int dec = 0;
    for(int i = bin.size()-1; i >= 0; i--){
        dec += (bin[i] == '1') * base;
        base *= 2;
    }
    return dec;
}

string dec2bin(int num){
    string bin = "";
    while(num != 0){
        int mod = num % 2;
        num /= 2;
        if(mod)
            bin = "1" + bin;
        else 
            bin = "0" + bin;
    }
    return bin;
}

bool comparer(string s1, string s2){
    if(s1.size() != s2.size())
        return false;
    else{
        for(int i = 0; i < s1.size(); i++){
            if(s1[i] != s2[i])
                return false;
        }
        return true;
    }
}

string padding(string str){
    int len = str.size();
    int pad_len = 32/4 - len;
    for(int i = 0; i < pad_len; i++){
        str = '0' + str;
    }
    return str;
}