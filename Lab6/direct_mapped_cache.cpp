#include "direct_mapped_cache.h"
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

float direct_mapped(string filename, int block_size, int cache_size){

    int total_num = 0;
    int hit_num = 0;
    
    /*write your code HERE*/
    // set up cache table
    int block_num = cache_size / block_size;   
    int tag_bit = 32 - bitnum(block_num) - bitnum(block_size);
    
    vector<string> cache_tag(block_num);
    vector<bool> valid(block_num, false);

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

        string tag = addr_bin.substr(0, tag_bit);
        int row_index = bin2dec(addr_bin.substr(tag_bit, bitnum(block_num)));

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
    cout << total_num << endl;
  
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