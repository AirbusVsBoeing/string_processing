//ELIJAH EINSTEIN <elijahei@buffalo.edu>


#include <iostream>
#include <fstream>
#include <string>



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



int app_match(std::string filepath, std::string pattern, int alpha){

    std::string text = readfile(filepath);
    int diverge = 0;

    int m = text.size();
    int n = pattern.size();

    

    for(int i = 0; i <= m-n; i++){
        
        for(int j = 0; j < n; j++){
            if(text[i+j] != pattern[j]){
                diverge += 1;
                if(diverge > alpha){
                    diverge = 0;
                    break;
                }
            }
            if(j == n-1 && diverge <= alpha){ // approx match!
                return i+1;
            }
        }
    }

    return -1;
    
}


int main(int argc, char *argv[]){

  
    std::string filepath(argv[1]);
    std::string pattern(argv[2]);
    int alpha = std::stoi(argv[3]);

    int index = app_match(filepath, pattern, alpha);

    std::cout << index << std::endl;

    return index;
}
