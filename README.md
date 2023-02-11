# streaming-maximum-cover
Given and integer $k$ and a family of $m$ sets $\{S_1, \dots, S_m\}$ where $S_i \in [n]$, the ***Maximum Coverage Problem*** consists of finding $k$ sets whose union has largest cardinality. In my work (still in review process), I implemented several streaming algorithms for the ***Maximum Coverage Problem***. *Name* refers to the names used in our work (still in review process) and *Codename* refers to the codename used for executing the program:

| Authors | Name | Codename |
| - | - | - |
| Jaud, Wirth and Choudhury | $\mathbf{MACH}_\gamma'$ | ```mgv``` |
| [McGregor and Vu](https://link.springer.com/article/10.1007/s00224-018-9878-x) | $\mathbf{OP}$ | ```mgvo``` |
| [Saha and Getoor](https://epubs.siam.org/doi/10.1137/1.9781611972795.60) | $\mathbf{SG}$ | ```sg``` |
| [Yu and Yuan](https://epubs.siam.org/doi/10.1137/1.9781611972832.84) | $\mathbf{YY}$ | ```yy``` |
| [Norouzi-Far et al.](https://proceedings.mlr.press/v80/norouzi-fard18a.html) | $\mathbf{2P}$ | ```ajsaao``` |
| [Badanidiyuru et al.](https://dl.acm.org/doi/10.1145/2623330.2623637) | $\mathbf{BMKK}$ | ```bmkk``` |

# Compilation

```g++ -std=c++2a streaming-maximum-cover/*.cpp -o smc -O3```

# Dataset
The dataset must be of the following format (one set per line):
```
<int> <int> ... <int>
<int> <int> ... <int>
.
.
.
<int> <int> ... <int>
```
An example (chess.dat) is given in the dataset folder. The dataset must also be located next to a file named dataset_infos.txt. It must include a summary of the dataset. Each line in dataset_infos.txt is of the format:

```<dataset name> <size (Bytes)> <m> <n> <maximum set size>```

An example is given in the dataset folder.


# Execution
```./smc <algo> <path> <dataset> <k> <eps> <inde>```

| Parameter | Type | Description |
| - | - | - |
| ```<path>``` | string | path the dataset folder (must end with "/") |
| ```<dataset>```| string | filename of the dataset (must be inside ```<path>```) |
| ```<k>``` | int | number of sets to select |
| ```<eps>``` | float |  precision parameter $\varepsilon$ (only for ```mgv```, ```mgvo```, ```bmkk``` and ```ajsaao```) |
| ```<inde>``` | string | codename of the independence factor $\gamma$ of the subsampling hash function (only for ```mgv``` and ```mgvo```) |

The possible values for ```<inde>``` are 


| Codename | Description |
| - | - |
| ```fullsamp``` | To execute the full sampling algorithm $\mathbf{MACH}_\text{fs}'$ |
| ```full``` | $\gamma = \lceil 2 \varepsilon^{-2} k \log m \rceil$ |
| ```opt``` | $\gamma = \lfloor k \log m / 3\rfloor$ |
| ```pairwise``` | $\gamma = 2$ |

For more detail regarding the independence factor $\gamma$, refer to our work (still in review process) or the work of [McGregor and Vu](https://link.springer.com/article/10.1007/s00224-018-9878-x).


# Output
The output consists of one line

```<n>,<m>,<k>,<eps>,<algo>-<inde>,<|I|>,<|C|>,<space>,<subsampling time (ms)>,<total time (ms)>,<dataset name>```
    
$|I|$ is the number of sets selected by the algorithm and $|C| = |\cup_{i \in I} S_i|$ is the coverage size of the solution. *space* evaluates the space complexity of the algorithm, it is the number of element instances stored during the execution.
