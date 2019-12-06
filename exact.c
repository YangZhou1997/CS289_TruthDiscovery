#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dataloader.h"
#include "networkloader.h"
#include "common.h"

void exact(int obsv_vec[][MAX_DIMENSION], int graph_matrix[][MAX_NUM_SENSOR], int num_sensor, int iter_num){
    int iter_cnt = 0;
    while(iter_cnt < iter_num){
        
    }
}


int main(int argc, char **argv){
    int * obsv_vec = malloc(sizeof(int) * MAX_NUM_SENSOR * MAX_DIMENSION);

    int dimension = dataloader("./datasets/test.dat", obsv_vec, MAX_NUM_SENSOR);
    printf("dimension of observation vector is %d\n", dimension);

    for(int i = 0; i < MAX_NUM_SENSOR; i++){
        for(int j = 0; j < dimension; j++){
            printf("%d ", obsv_vec[i * MAX_DIMENSION + j]);
        }
        printf("\n");
    }

    int * graph_matrix = malloc(sizeof(int) * MAX_NUM_SENSOR * MAX_NUM_SENSOR);
    int res = netloader("./networks/facebook.dat", graph_matrix, MAX_NUM_SENSOR);
    printf("network loading: %d\n", res);

    return 0;
}