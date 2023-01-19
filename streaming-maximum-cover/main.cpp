#include "SMCAlgorithm.hpp"

/*
 ./program <path> <dataset> <k> <eps> <inde>
 
    <k>     int     number of sets to select
    <eps>   float   precision parameter
    <inde>  string  "full", "opt", "pairwise", "fullsamp"
 */

int main(int argc, const char * argv[]) {    
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
        dataset = "Webdocs.dat";
        k       = 1;
        eps     = .4f;
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
    
    stream.terminate();
    
    std::string output;
    output += std::to_string(dataset_infos[dataset].n) + ",";
    output += std::to_string(dataset_infos[dataset].m) + ",";
    output += std::to_string(k) + ",";
    output += std::to_string(eps) + ",";
    output += result.to_string() + ",";
    output += dataset + "\n";

    std::cout << output;
        
    return 0;
}
