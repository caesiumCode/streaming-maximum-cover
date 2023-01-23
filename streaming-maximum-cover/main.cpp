#include "MGVAlgorithm.hpp"
#include "SGAlgorithm.hpp"

/*
 ./program mgv <path> <dataset> <k> <eps> <inde>
 ./program sg <path> <dataset> <k>
 
    <k>     int     number of sets to select
    <eps>   float   precision parameter
    <inde>  string  "full", "opt", "pairwise", "fullsamp"
 */

enum ALGO_TYPE
{
    MGV,
    SG,
    CUSTOM
};

int main(int argc, const char * argv[]) {
    ALGO_TYPE algo_type;
    
    std::string path, dataset, type;
    int m, k;
    ulong n;
    
    if      (argc == 7 && std::string(argv[1]) == "mgv")    algo_type = ALGO_TYPE::MGV;
    else if (argc == 5 && std::string(argv[1]) == "sg")     algo_type = ALGO_TYPE::SG;
    else
    {
        algo_type = ALGO_TYPE::CUSTOM;
        //std::cout << "INVALID ARGUMENT\n";
        //return EXIT_FAILURE;
    }
    
    /*
     Setup stream
     */
    
    switch (algo_type)
    {
        case MGV:
            path    = std::string(argv[2]);
            dataset = std::string(argv[3]);
            k       = std::atoi(argv[4]);
            break;
            
        case SG:
            path    = std::string(argv[2]);
            dataset = std::string(argv[3]);
            k       = std::atoi(argv[4]);
            break;
            
        default:
            path    = "/Users/stephen/Desktop/Melbourne University/Subjects/S4 - Research Project/Datasets/Datasets/";
            dataset = "Webdocs.dat";
            k       = 4;
            break;
    }
    
    std::unordered_map<std::string, Info> dataset_infos = Stream::load_infos(path);
    Stream stream(path, dataset);
    
    m = dataset_infos[dataset].m;
    n = dataset_infos[dataset].n;
    
    //algo_type = ALGO_TYPE::SG;
    
    /*
     Setup algorithm
     */
    
    switch (algo_type)
    {
        case MGV:
        {
            float       eps     = std::atof(argv[5]);
            std::string inde    = std::string(argv[6]);
            
            smc::MGVAlgorithm mgv_algo;
            mgv_algo.setStream(&stream);
            mgv_algo.setC(6.f);
            mgv_algo.setK(k);
            mgv_algo.setM(m);
            mgv_algo.setN(n);
            mgv_algo.setEpsilon(eps);
            mgv_algo.setMaxSetSize(dataset_infos[dataset].max_set_size);
            if      (inde == "full")        mgv_algo.setInde(smc::IndeType::FULL);
            else if (inde == "opt")         mgv_algo.setInde(smc::IndeType::OPT);
            else if (inde == "pairwise")    mgv_algo.setInde(smc::IndeType::PAIRWISE);
            else if (inde == "fullsamp")    mgv_algo.setInde(smc::IndeType::FULLSAMP);
            else                            mgv_algo.setInde(smc::IndeType::FULLSAMP);
            
            smc::Result result = mgv_algo.run();
            
            std::string output;
            output += std::to_string(n) + ",";
            output += std::to_string(m) + ",";
            output += std::to_string(k) + ",";
            output += std::to_string(eps) + ",";
            output += result.to_string() + ",";
            output += dataset + "\n";
            
            std::cout << output;
            
            break;
        }
            
        case SG:
        {
            smc::SGAlgorithm sg_algo;
            sg_algo.setStream(&stream);
            sg_algo.setK(k);
            
            smc::Result result = sg_algo.run();
            
            std::string output;
            output += std::to_string(n) + ",";
            output += std::to_string(m) + ",";
            output += std::to_string(k) + ",";
            output += std::to_string(0) + ",";
            output += result.to_string() + ",";
            output += dataset + "\n";
            
            std::cout << output;
            
            break;
        }
            
        default:
        {
            float       eps     = 0.5;
            std::string inde    = "pairwise";
            
            smc::MGVAlgorithm mgv_algo;
            mgv_algo.setStream(&stream);
            mgv_algo.setC(6.f);
            mgv_algo.setK(k);
            mgv_algo.setM(dataset_infos[dataset].m);
            mgv_algo.setN(dataset_infos[dataset].n);
            mgv_algo.setEpsilon(eps);
            mgv_algo.setMaxSetSize(dataset_infos[dataset].max_set_size);
            if      (inde == "full")        mgv_algo.setInde(smc::IndeType::FULL);
            else if (inde == "opt")         mgv_algo.setInde(smc::IndeType::OPT);
            else if (inde == "pairwise")    mgv_algo.setInde(smc::IndeType::PAIRWISE);
            else if (inde == "fullsamp")    mgv_algo.setInde(smc::IndeType::FULLSAMP);
            else                            mgv_algo.setInde(smc::IndeType::FULLSAMP);
            
            smc::Result result = mgv_algo.run();
            
            std::string output;
            output += std::to_string(dataset_infos[dataset].n) + ",";
            output += std::to_string(dataset_infos[dataset].m) + ",";
            output += std::to_string(k) + ",";
            output += std::to_string(eps) + ",";
            output += result.to_string() + ",";
            output += dataset + "\n";
            
            std::cout << output;
            break;
        }
    }
        
    stream.terminate();
        
    return 0;
}
