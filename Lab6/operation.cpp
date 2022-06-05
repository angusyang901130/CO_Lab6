#include "operation.h"

string hex2bin(char c){
    string result = "";
    switch(c){
        case '0': result = "0000"; break;
        case '1': result = "0001"; break;
        case '2': result = "0010"; break;
        case '3': result = "0011"; break;
        case '4': result = "0100"; break;
        case '5': result = "0101"; break;
        case '6': result = "0110"; break;
        case '7': result = "0111"; break;
        case '8': result = "1000"; break;
        case '9': result = "1001"; break;
        case 'a': result = "1010"; break;
        case 'b': result = "1011"; break;
        case 'c': result = "1100"; break;
        case 'd': result = "1101"; break;
        case 'e': result = "1110"; break;
        case 'f': result = "1111"; break;
    }
    return result;
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
    if(s1.size() != s2.size()){
        return false;
    }else{
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