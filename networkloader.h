#ifndef __NETWORKLOADER_H__
#define __NETWORKLOADER_H__
#include <stdio.h>
#include <string.h>
#include "common.h"

int raw_matrix[MAX_RAW_NUM_SENSOR][MAX_RAW_NUM_SENSOR];
int add_node(int max_num, int final_sensor[MAX_RAW_NUM_SENSOR], int sensor_flag[MAX_RAW_NUM_SENSOR], int raw_row[MAX_RAW_NUM_SENSOR], int *cnt){
    for(int i = 0; i <= max_num; i++){
        if(sensor_flag[i] == 0 && raw_row[i] == 1){
            final_sensor[(*cnt)++] = i;
            sensor_flag[i] = 1;
        }
    }
    return 1;
}

// given the number of sersors (1589)
int netloader(char *filename, int graph_matrix[][MAX_NUM_SENSOR], int num_sensor){
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        return -1;
    }

    memset(raw_matrix, 0, sizeof(int) * MAX_RAW_NUM_SENSOR * MAX_RAW_NUM_SENSOR);
    int a, b;
    int max_num = 0;
    while(fscanf(file, "%d %d\n", &a, &b) != EOF){
        if(a >= MAX_RAW_NUM_SENSOR || b >= MAX_RAW_NUM_SENSOR){
            fclose(file);
            return -1;
        }
        raw_matrix[a][b] = 1;
        max_num = MAX(max_num, a);
        max_num = MAX(max_num, b);
        // printf("%d %d\n", a, b);
    }
    fclose(file);

    int final_sensor[MAX_RAW_NUM_SENSOR];
    int sensor_flag[MAX_RAW_NUM_SENSOR];

    memset(final_sensor, 0, sizeof(int) * MAX_RAW_NUM_SENSOR);
    memset(sensor_flag, 0, sizeof(int) * MAX_RAW_NUM_SENSOR);

    int cnt = 0;
    int runner = 0;
    final_sensor[cnt++] = runner;
    sensor_flag[runner] = 1;

    while(cnt < num_sensor){
        for(int i = 0; i < cnt; i++){
            add_node(max_num, final_sensor, sensor_flag, raw_matrix[i], &cnt);
            if(cnt >= num_sensor){
                break;
            }
        }
    }
    printf("cnt = %d\n", cnt);

    for(int i = 0; i < num_sensor; i++){
        for(int j = 0; j < num_sensor; j++){
            graph_matrix[i][j] = raw_matrix[final_sensor[i]][final_sensor[j]];
            // printf("%d ", graph_matrix[i][j]);
        }
        // printf("\n");
        break;
    }
    return 0;
}

#endif//__NETWORKLOADER_H__
