#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cmath>
#include <vector>

using namespace std;

int address_bit;
int block_size;
int cache_size;
int associativity;

int tag, index, offset;
string benchmark_name;

struct Data
{
    string reference;
    string tag;
    string str_index;
    int index;
    string offset;
    bool isMiss;
};

struct block
{
    int NRUbit = 1;
    string tag;
    string data;
};

vector<Data> dataList;
vector<vector<block>> cache;

void input_org(string file)
{
    FILE *f;
    f = fopen(file.c_str(), "r");
    fscanf(f, "Address_bits: %d\n", &address_bit);
    fscanf(f, "Block_size: %d\n", &block_size);
    fscanf(f, "Cache_sets: %d\n", &cache_size);
    fscanf(f, "Associativity: %d\n", &associativity);
    fclose(f);
    
    index = log2(cache_size);
    offset = log2(block_size);
    tag = address_bit - index - offset;
}

void init_cache()
{
    for(int i=0; i<cache_size; i++){
        vector<block> set;
        for(int j=0; j<associativity; j++){
            block b;
            set.push_back(b);
        }
        cache.push_back(set);
    }
}

void input_lst(string file)
{
    ifstream ifile(file.c_str());
    
    string word;
    ifile >> word;  //.benchmark
    ifile >> benchmark_name;  //
    
    while(ifile.good()){
        
        ifile >> word;
        if(word == ".end")
            break;
        
        Data _data;
        _data.reference = word;
        _data.offset = word.substr(tag+index, offset);
        _data.str_index = word.substr(tag, index);
        _data.index = stoi(_data.str_index, nullptr, 2);
        _data.tag = word.substr(0, tag);
        
        dataList.push_back(_data);
    }
    
    ifile.close();
}

void processing()
{
    for(int i=0; i<dataList.size(); i++){
        int index = dataList[i].index;
        
        bool isFound = false;
        for(int j=0; j<cache[index].size(); j++){
            if(cache[index][j].tag == dataList[i].tag){
                cache[index][j].NRUbit = 0;
                dataList[i].isMiss = false;
                isFound = true;
                break;
            }
        }
            
        if(!isFound){
            for(int j=0; j<cache[index].size(); j++){
                if(cache[index][j].NRUbit == 1){
                    dataList[i].isMiss = true;
                    cache[index][j].NRUbit = 0;
                    cache[index][j].tag = dataList[i].tag;
                    break;
                }
                else if(cache[index][j].NRUbit == 0 && j == cache[index].size()-1){
                    dataList[i].isMiss = true;
                    cache[index][0].NRUbit = 0;
                    cache[index][0].tag = dataList[i].tag;
                    
                    for(int k=1; k<cache[index].size(); k++){
                        cache[index][k].NRUbit = 1;
                    }
                    break;
                }
            }
        }
    }
}

void output(string file)
{
    string p =  file;
    const char *path = p.c_str();
    
    ofstream ofile;
    ofile.open(path);
    
    ofile << "Address bits: " << address_bit << endl;
    ofile << "Block size: " << block_size << endl;
    ofile << "Cache sets: " << cache_size << endl;
    ofile << "Associativity: " << associativity << endl << endl;;
    
    ofile << "Offset bit count: " << offset << endl;
    ofile << "Indexing bit count: " << index << endl;
    ofile << "Indexing bits:";
    for(int i=offset; i<offset+index; i++){
        ofile << " " << i;
    }
    ofile << endl << endl;
    ofile << ".benchmark " << benchmark_name << endl;
    
    int miss_count = 0;
    for(int i=0; i<dataList.size(); i++){
        ofile << dataList[i].reference << " ";
        if(dataList[i].isMiss){
            ofile << "miss" << endl;
            miss_count++;
        }
        else
            ofile << "hit" << endl;
    }
    ofile << ".end " << endl;
    
    ofile << "Total cache miss count: " << miss_count << endl;
    
}

int main(int argc, char *argv[])
{
    input_org(argv[1]);
    init_cache();
    input_lst(argv[2]);
    
    processing();
    output(argv[3]);
    
    return 0;
}
