#include "MGVAlgorithm.hpp"
#include "SGAlgorithm.hpp"
#include "YYAlgorithm.hpp"
#include "BMKKAlgorithm.hpp"

/*
 ./program mgv  <path> <dataset> <k> <eps> <inde>
 ./program sg   <path> <dataset> <k>
 ./program yy   <path> <dataset> <k>
 ./program bmkk <path> <dataset> <k> <eps>
 
    <k>     int     number of sets to select
    <eps>   float   precision parameter
    <inde>  string  "full", "opt", "pairwise", "fullsamp"
 */

enum ALGO_TYPE
{
    MGV,
    SG,
    YY,
    BMKK
};

int main(int argc, const char * argv[]) {
    ALGO_TYPE algo_type;
    
    std::string path, dataset, type;
    int m, k;
    ulong n;
    
    if      (argc == 7 && std::string(argv[1]) == "mgv")    algo_type = ALGO_TYPE::MGV;
    else if (argc == 5 && std::string(argv[1]) == "sg")     algo_type = ALGO_TYPE::SG;
    else if (argc == 5 && std::string(argv[1]) == "yy")     algo_type = ALGO_TYPE::YY;
    else if (argc == 6 && std::string(argv[1]) == "bmkk")   algo_type = ALGO_TYPE::BMKK;
    else
    {
        std::cout << "INVALID ARGUMENT\n";
        return EXIT_FAILURE;
    }
    
    /*
     Setup stream
     */
    
    switch (algo_type)
    {
        case MGV:
        case SG:
        case YY:
        case BMKK:
            path    = std::string(argv[2]);
            dataset = std::string(argv[3]);
            k       = std::atoi(argv[4]);
            break;
    }
    
    std::unordered_map<std::string, Info> dataset_infos = Stream::load_infos(path);
    Stream stream(path, dataset);
    
    m = dataset_infos[dataset].m;
    n = dataset_infos[dataset].n;
        
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
            mgv_algo.setC(1.f);
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
            
        case YY:
        {
            smc::YYAlgorithm yy_algo;
            yy_algo.setStream(&stream);
            yy_algo.setK(k);
            
            smc::Result result = yy_algo.run();
            
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
            
        case BMKK:
        {
            float eps = std::atof(argv[5]);
            
            smc::BMKKAlgorithm bmkk_algo;
            bmkk_algo.setStream(&stream);
            bmkk_algo.setK(k);
            bmkk_algo.setEpsilon(eps);
            bmkk_algo.setMaxSetSize(dataset_infos[dataset].max_set_size);
            
            smc::Result result = bmkk_algo.run();
            
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
    }
        
    stream.terminate();
        
    return 0;
}
