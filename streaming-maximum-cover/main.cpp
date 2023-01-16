#include <unordered_map>
#include <fstream>
#include <sstream>

#include "SMCAlgorithm.hpp"

/*
 ./program <path> <dataset> <k> <eps> <inde>
 
    <k>     int     number of sets to select
    <eps>   float   precision parameter
    <inde>  string  "full", "opt", "pairwise"
 */

int main(int argc, const char * argv[]) {
    std::cout << "START\n";
    
    std::string path, dataset, type;
    int k;
    float eps;
    std::string inde;
        
    if (argc == 6)
    {
        path    = std::string(argv[1]);
        dataset = std::string(argv[2]);
        k       = atoi(argv[3]);
        eps     = atof(argv[4]);
        inde    = std::string(argv[5]);
    }
    else
    {
        path    = "/Users/stephen/Desktop/Melbourne University/Subjects/S4 - Research Project/Datasets/Datasets/";
        dataset = "Webbase.dat";
        k       = 8;
        eps     = .4f;
        inde    = "opt";
    }
    
    std::cout << "BEFORE LOAD INFO\n";
    std::unordered_map<std::string, Info> dataset_infos = Stream::load_infos(path);
    
    std::cout << "BEFORE STREAM INIT\n";
    Stream stream(path, dataset);
    std::cout << "AFTER STREAM INIT\n";
    
    smc::Algorithm smc_algo;
    smc_algo.setStream(&stream);
    smc_algo.setC(6.f);
    smc_algo.setK(k);
    smc_algo.setM(dataset_infos[dataset].m);
    smc_algo.setN(dataset_infos[dataset].n);
    smc_algo.setEpsilon(eps);
    smc_algo.setMaxSetSize(dataset_infos[dataset].max_set_size);
    if      (inde == "full")        smc_algo.setInde(smc::IndeType::FULL);
    else if (inde == "opt")         smc_algo.setInde(smc::IndeType::OPT);
    else if (inde == "pairwise")    smc_algo.setInde(smc::IndeType::PAIRWISE);
    else if (inde == "fullsamp")    smc_algo.setInde(smc::IndeType::FULLSAMP);
    else                            smc_algo.setInde(smc::IndeType::FULLSAMP);
    
    smc::Result result = smc_algo.run();
    
    std::cout << "AFTER RUN\n";
    
    std::cout <<    dataset_infos[dataset].n    << "," <<
                    dataset_infos[dataset].m    << "," <<
                    k                           << "," <<
                    eps                         << "," <<
                    result                      << "," <<
                    dataset                     << std::endl;
         
    stream.terminate();
    
    std::cout << "END\n";
    
    return 0;
}
