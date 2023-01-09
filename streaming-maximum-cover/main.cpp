#include <unordered_map>
#include <fstream>
#include <sstream>

#include "SMCAlgorithm.hpp"

/*
 ./program real <path> <dataset> <k> <eps> <inde>
 ./program gen <type> <k> <eps> <inde>
 
    <k>     int     number of sets to select
    <eps>   float   precision parameter
    <inde>  string  "full", "opt", "2"
 */

int main(int argc, const char * argv[]) {
    std::string path, dataset, type;
    int k;
    float eps;
    std::string inde;
        
    if (argc == 7)
    {
        if (std::string(argv[1]) == "real")
        {
            path    = std::string(argv[2]);
            dataset = std::string(argv[3]);
            k       = atoi(argv[4]);
            eps     = atof(argv[5]);
            inde    = std::string(argv[6]);
        }
        else if (std::string(argv[1]) == "gen")
        {
            std::cout << "gen option not implemented" << std::endl;
            return 0;
        }
        else
        {
            std::cout << "Unknown option used" << std::endl;
            return 0;
        }
    }
    // Default options
    else
    {
        path    = "/Users/stephen/Desktop/Melbourne University/Subjects/S4 - Research Project/Datasets/Datasets/";
        dataset = "Webdocs.dat";
        k       = 2;
        eps     = 0.4f;
        inde    = "pairwise";
    }
        
    std::unordered_map<std::string, Info> dataset_infos = Stream::load_infos(path);
        
    Stream stream(path, dataset);
    
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
    
    std::cout <<    dataset_infos[dataset].n    << "," <<
                    dataset_infos[dataset].m    << "," <<
                    k                           << "," <<
                    eps                         << "," <<
                    result                      << "," <<
                    dataset                     << std::endl;
    
    std::cout << result.coverage_size << std::endl;
     
    stream.terminate();
    
    return 0;
}
