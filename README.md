# Decentralized Truth Discovery

## Observartion Datasets:
- Three Synthetic Datasets:
    - mu 0.7, sigma 0.25
    - mu 0.7, sigma 0.15
    - mu 0.8, sigma 0.25
- Stock Record

## Network Topologies: 
- Geometric Network: 1589 nodes
- preferential Network: 1589 nodes
- Sensor Network: 1589 nodes
- Social Network: 1589 nodes

## Notes
- The dimension of observation vector varies: 18, 7, 6, 22.
- Number of sensors is fixed: 1589

## Run the algorithms: 

Dataset name = `['stock', 'syn_0.7_0.15', 'syn_0.7_0.25', 'syn_0.8_0.25']`
Network name = `['geometric', 'preferential', 'supermaket', 'facebook']`

```
make exact
# ./exact $dataset_name $network_name $k
./exact stock geometric 1
```

```
make appro
# ./appro $dataset_name $network_name $k1 $k2
./appro stock geometric 1000 1
```