#ifndef __DATALOADER_H__
#define __DATALOADER_H__
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"

void get_new_obsv_vec(int obsv_vec[][MAX_DIMENSION], int reference_i, int target_i, int dimension){
    for(int i = 0; i < dimension; i++){
        if(rand() * 1.0 / RAND_MAX < 0.1){
            obsv_vec[target_i][i] = 1 - obsv_vec[reference_i][i];
        }
        else{
            obsv_vec[target_i][i] = obsv_vec[reference_i][i];
        }
        // printf("%d ", obsv_vec[target_i][i]);
    }
}

// given the number of sersors (1589)
// return the dimension of the observation vector. 
int dataloader(char *filename, int obsv_vec[][MAX_DIMENSION], int num_sensor){
    srand(time(NULL));

    FILE *file = fopen(filename, "r");
    if(file == NULL){
        return -1;
    }
    int dimension = 0;
    int res_dimen;
    int bit;
    char flag;
    int cnt_sensor = 0;
        
    // read data and store into obsv_vec
    while(fscanf(file, "%d%c", &bit, &flag) != EOF){
        obsv_vec[cnt_sensor][dimension ++] = bit;
        if(flag == '\n'){
            res_dimen = dimension;
            dimension = 0;
            cnt_sensor++;
        }
    }
    fclose(file);

    int ori_cnt = cnt_sensor;
    if(ori_cnt < num_sensor){
        for(; cnt_sensor < num_sensor; cnt_sensor++){
            get_new_obsv_vec(obsv_vec, cnt_sensor % ori_cnt, cnt_sensor, res_dimen);
        }
    }
    return res_dimen;
}

#endif//__DATALOADER_H__
