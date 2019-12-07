#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h> 
#include "dataloader.h"
#include "networkloader.h"
#include "common.h"

typedef struct sensor
{
    double min_avg_obj_val;
    int min_vec[MAX_DIMENSION];
    double cur_avg_obj_val;
}sensor;

double _cal_avg_dis(int * vec_a, int * vec_b, int dimension){
    double res = 0;
    for(int i = 0; i < dimension; i++){
        res += vec_a[i] ^ vec_b[i];
    }
    res /= dimension; // what if res == 0?
    return res;
}

double cal_avg_dis(int * vec_a, int * vec_b, int dimension){
    double res = _cal_avg_dis(vec_a, vec_b, dimension);
    return (- (1 - res) * log(1 - res) - res * log(res));
}



int get_sensor_b(int sensor_a, int graph_matrix[][MAX_NUM_SENSOR], int num_sensor){
    int cnt_neighbor = 0;
    for(int i = 0; i < num_sensor; i++){
        if(graph_matrix[sensor_a][i] == 1 && sensor_a != i){
            cnt_neighbor++;
        }
    }
    int sensor_b_index = rand() % cnt_neighbor;
    
    cnt_neighbor = 0;
    for(int i = 0; i < num_sensor; i++){
        if(graph_matrix[sensor_a][i] == 1 && sensor_a != i){
            if(cnt_neighbor == sensor_b_index){
                return i;
            }
            cnt_neighbor++;
        }
    }
    printf("error: sensor neighbor not found!\n");
    return -1;
}

// return a sensor structure containing the min_avg_obj_val and min_vec;
sensor exact(int obsv_vec[][MAX_DIMENSION], int graph_matrix[][MAX_NUM_SENSOR], int num_sensor, int dimension, int iter_num){
    sensor * sensors = malloc(sizeof(sensor) * num_sensor);
    for(int i = 0; i < num_sensor; i++){
        sensors[i].min_avg_obj_val = (double)(1 << 30);
        memset(sensors[i].min_vec, 0, sizeof(int) * MAX_DIMENSION);
    }

    // enumerate every case for potential vector t; 
    for(int t = 0; t < (1 << dimension) - 1; t++){
        if(t % 1000 == 0){
            printf("current simulation progress: %d/%d = %lf\n", t, (1 << dimension) - 1, t * 1.0 / ((1 << dimension) - 1));
        }
        int iter_cnt = 0;
        int vec_t[MAX_DIMENSION];
        for(int i = 0; i < dimension; i++){
            vec_t[i] = (t >> i) & 1;
        }
        for(int i = 0; i < num_sensor; i++){
            sensors[i].cur_avg_obj_val = cal_avg_dis(obsv_vec[i], vec_t, dimension);
        }
        while(iter_cnt < iter_num){
            int cur_sensor_a = rand() % num_sensor;
            int cur_sensor_b = get_sensor_b(cur_sensor_a, graph_matrix, num_sensor);
            double a = sensors[cur_sensor_a].cur_avg_obj_val;
            double b = sensors[cur_sensor_b].cur_avg_obj_val;
            sensors[cur_sensor_a].cur_avg_obj_val = (a + b) / 2;
            sensors[cur_sensor_b].cur_avg_obj_val = (a + b) / 2;
            iter_cnt ++;
        }
        for(int i = 0; i < num_sensor; i++){
            if(sensors[i].min_avg_obj_val > sensors[i].cur_avg_obj_val){
                sensors[i].min_avg_obj_val = sensors[i].cur_avg_obj_val;
                memcpy(sensors[i].min_vec, vec_t, MAX_DIMENSION);
            }
        }
    }
    return sensors[0];
}

int main(int argc, char **argv){
    srand(time(NULL));

    char dataset[500];
    char network[500];
    char truth[500];
    sprintf(dataset, "./datasets/%s.dat", argv[1]);
    sprintf(network, "./networks/%s.dat", argv[2]);
    sprintf(truth, "./datasets/%s_truth.dat", argv[1]);

    int k = atoi(argv[3]);


    int * obsv_vec = malloc(sizeof(int) * MAX_NUM_SENSOR * MAX_DIMENSION);

    int dimension = dataloader(dataset, obsv_vec, MAX_NUM_SENSOR);
    printf("dimension of observation vector is %d\n", dimension);

    for(int i = 0; i < MAX_NUM_SENSOR; i++){
        for(int j = 0; j < dimension; j++){
            printf("%d ", obsv_vec[i * MAX_DIMENSION + j]);
        }
        printf("\n");
    }

    int * graph_matrix = malloc(sizeof(int) * MAX_NUM_SENSOR * MAX_NUM_SENSOR);
    int res = netloader(network, graph_matrix, MAX_NUM_SENSOR);
    printf("network loading: %d\n", res);
    
    clock_t t; 
    t = clock();
    sensor res_vec = exact(obsv_vec, graph_matrix, MAX_NUM_SENSOR, dimension, k  * 1589);
    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
    printf("time spent: %lf\n", time_taken);

    
    int truth_vec[MAX_DIMENSION];
    FILE *file = fopen(truth, "r");
    if(file == NULL){
        return -1;
    }
    int cnt = 0;
    int res_dimen;
    int bit;
    char flag;
    int cnt_sensor = 0;
        
    // read data and store into truth_vec
    while(fscanf(file, "%d%c", &bit, &flag) != EOF){
        truth_vec[cnt ++] = bit;
    }
    fclose(file);

    double error_rate = _cal_avg_dis(res_vec.min_vec, truth_vec, dimension);
    printf("error rate: %lf\n", error_rate);

    double obj_val = 0;
    for(int i = 0; i < MAX_NUM_SENSOR; i++){
        obj_val += cal_avg_dis(obsv_vec + i * MAX_DIMENSION, res_vec.min_vec, dimension);
    }
    printf("objective value: %lf\n", obj_val);

    return 0;
}