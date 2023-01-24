# streaming-maximum-cover
Implementation of 3 different streaming algorithms for Maximum Set Cover:
- McGregor and Vu - [paper](https://link.springer.com/article/10.1007/s00224-018-9878-x)
- Saha and Getoor - [paper](https://epubs.siam.org/doi/10.1137/1.9781611972795.60)
- Yu and Yuan - [paper](https://epubs.siam.org/doi/10.1137/1.9781611972832.84)

# Compilation
g++ -std=c++2a streaming-maximum-cover/streaming-maximum-cover/*.cpp -o smc -O3

# Execution
Run McGregor and Vu algorithm   ```./smc mgv <path> <dataset> <k> <eps> <inde>```

Run Sahaa and Getoor algorithm  ```./smc sg  <path> <dataset> <k>```

Run Yu and Yuan algorithm       ```./smc yy  <path> <dataset> <k>```

    <path>    string  path the dataset folder (must end with "/")
    <dataset> string  filename of the dataset (must be inside <path>)
    <k>       int     number of sets to select
    <eps>     float   precision parameter (only for McGregor and Vu algorithm)
    <inde>    string  independence factor of the sampling hash function (only for McGregor and Vu algorithm)
                      "full", "opt", "pairwise" or "fullsamp" 
    
