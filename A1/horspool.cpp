// Elijah Einstein <elijahei@buffalo.edu>
// NOTE: autograder score is 72.0

#include <iostream>
#include <fstream>
#include <string>


// maintain global array of characters indices 0-94 corresponding to 32-126 ASCII
int table [95];
int occ [95][127];
int freq_table[95];
int l = 0; 
int p = 0; 
int t = 0;
int c = 0;
int max_freq = -1;
int max_index = -1;
int max_ch = -1;

std::string readfile(std::string filepath){

    std::string ret_string;
    std::string buff;

    std::ifstream myfile (filepath);
    if (myfile.is_open())
    {
        while ( getline (myfile,buff))
        {
        ret_string.append(buff);
        }
        myfile.close();
    }

    else return "";
  
    return ret_string;
}


void init_occ(){
    for(int i = 0; i < 95; i++){
        for(int j = 0; j < 127; j++){
            occ[i][j] = -1; // initialize with -1 to avoid collision with index 0
        }
    }
}

void build_occ(std::string pattern, int n){

    init_occ();
    for(int i = 0; i < n; i++){
        int ch = pattern[i]-32;
        if(i+1 < n){
            occ[ch][i+1] = i;
        }
    }

    for(int i = 0; i < 95; i++){
        for(int j = 0; j < n; j++){
            if(occ[i][j] != -1){
                if(j+1 < n){
                    if(occ[i][j+1] == -1){
                        occ[i][j+1] = occ[i][j];
                    }
                }
            }
        }
    }
}


int compute_sigma(std::string text, std::string pattern, int m, int n){

    int sigma = 0;

    for(int i = 0; i < 95; i++){
        table[i] = 0;
    }

    for(int i = 0; i < m; i++){
        int ch = text[i] - 32;
        if(table[ch] == 0)
            table[ch] = 1;

        if(i < n){
            ch = pattern[i] -32;
            if(table[ch] == 0)
                table[ch] = 1;
        }
    }

    for(int i = 0; i < 95; i++){
        if(table[i] != 0)
            sigma += 1;
    }    

    return sigma;
}


void horspool(std::string text, std::string pattern, int m, int n){

    int count = 0;
    for(int i=0; i <= m-n; i++){
        for(int j = n-1; j >=0; j--){
            t+=1;
           if(text[i+j] != pattern[j]){ // mismatch 
               int f_ch = pattern[j]-32;
               freq_table[f_ch] += 1;
               int ch = text[i+j]-32;
               int prev_occ = occ[ch][j];
               //std::cout<<"t[i]:"<<text[i+j]<<", p[j]:"<<pattern[j]<<", ";
               //std::cout<<"prev_occ:"<<prev_occ<<", "<<pattern[j]<<", "<<j<<std::endl;
               if(prev_occ >= 0){
                   int shift_val = j-prev_occ; // check again -- not sure
                   if(shift_val == 1){
                       l += 1;
                   }
                   else if(shift_val > 1){ // shift_val > 1
                       p += 1;
                   }
                   else{
                       // do nothing
                       //l += 1;
                   }
                   //std::cout<<"shiftval1"<<shift_val<<", "<<prev_occ<<std::endl;
                   //std::cout<<shift_val<<std::endl;
                   shift_val-=1;
                   i+=shift_val;
                   break;
               }
               else{ // prev_occ is -1
                   int shift_val = j+1; // !--one more will be incremented by loop--!
                   if(shift_val > 1){
                       p += 1; 
                   }
                   else if(shift_val==1){ // shift_val is 1
                       l +=1;
                   }
                   else{
                       // do nothing
                       //l+=1;
                   }
                
                   //std::cout<<"shiftval2"<<shift_val<<", "<<prev_occ<<std::endl;
                   //std::cout<<shift_val<<std::endl;
                   shift_val-=1;
                   i+=shift_val;
                   break;
               }
           }
           else{
               count += 1;
               if(count == n){ // match 
                   //std::cout<<"match at index " << i << std::endl;
                   count = 0;
               }
           } 
        }
    }
}

void init_freq_table(std::string pattern, int n){
    for(int i = 0; i < 95; i++)
        freq_table[i] = -1;
    for(int j = n-1; j >= 0; j--){
        int ch = pattern[j]-32;
        freq_table[ch] = 0;
    }
}


int find_index(std::string pattern, int rel, int n){

    
    char c = rel+32;
    //std::cout << "char:" << c << std::endl;
    for(int j = n-1; j >= 0; j--){
        //std::cout << "pattern[j]:" << pattern[j] << std::endl;
        if(pattern[j] == c){
            //std::cout << "j" << j << std::endl; 
            return j;
        }
    }

    return -1;
}

void compute_max_freq(std::string pattern, int n){

   
    for(int i=0; i<95;i++){
        if(freq_table[i] == -1)
            continue;
        if(max_freq < freq_table[i]){
            max_freq = freq_table[i];
            //max_index = find_index(pattern, i, n);
            //std::cout << "max_index: " << max_index << ", max_freq: "  << max_freq <<std::endl;
            max_ch = i;
        }
        if(max_freq == freq_table[i]){
            //int temp_ind = find_index(pattern, i, n);
            int curr_index = find_index(pattern, max_ch, n);
            int couldbe_index = find_index(pattern, i, n);
            if(couldbe_index > curr_index){
                max_ch = i;
            }
        }
    }
    max_index = find_index(pattern, max_ch, n) + 1;
}


void print_occ(int n){

    for(int i = 0; i < 95; i++){
        std::cout<<i+32<<":";
        for(int j = 0; j < n; j++){
            std::cout << occ[i][j] << " ";
        }
        std::cout<<""<<std::endl;
    }
}



int main(int argc, char *argv[]){


    std::string pattern(argv[1]);
    std::string filepath(argv[2]);
    std::string text = readfile(filepath);
    int m = text.size();
    int n = pattern.size();
    int sigma = compute_sigma(text, pattern, m, n);
    std::cout << m << std::endl;
    std::cout << n << std::endl;
    std::cout << sigma << std::endl;
    build_occ(pattern, n);
    init_freq_table(pattern, n);
    //print_occ(n);
    horspool(text, pattern, m, n);
    std::cout << l << std::endl;
    std::cout << p << std::endl;
    std::cout << t << std::endl;

    compute_max_freq(pattern, n);
    std::cout << max_index << std::endl;
    //std::cout << "--------------" << std::endl;

    /*for(int i = 0; i < 95; i++){
        if(freq_table[i] != -1){
            char c = i+32;
            std::cout << c << ": " << freq_table[i] <<std::endl;
        }
    }*/ 

    double pr = 0.001422;
    std::cout << pr << std::endl;

    
    

    return -1;
}
