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
    int obsv_vec[MAX_DIMENSION];
    int mixed_vec[MAX_DIMENSION];
    double sum;
    int cnt;
    int t_vec[MAX_DIMENSION];
    double f_vec[MAX_DIMENSION];
}sensor;

typedef struct edge
{
    int a;
    int b;
}edge;


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
    if(res > 0 && res < 1.0)
        return (- (1 - res) * log(1 - res) - res * log(res));
    return 0;
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

edge get_edge(int graph_matrix[][MAX_NUM_SENSOR], int num_sensor){
    edge res;
    res.a = rand() % num_sensor;
    res.b = get_sensor_b(res.a, graph_matrix, num_sensor);
    return res;
}

void mix(sensor * sensors, int graph_matrix[][MAX_NUM_SENSOR], int num_sensor, int dimension, int iter){
    printf("mix!\n");
    int iter_cnt = 0;
    while(iter_cnt < iter){
        edge e = get_edge(graph_matrix, num_sensor);
        memcpy(sensors[e.a].mixed_vec, sensors[e.b].obsv_vec, sizeof(int) * MAX_DIMENSION);
        memcpy(sensors[e.b].mixed_vec, sensors[e.a].obsv_vec, sizeof(int) * MAX_DIMENSION);
        iter_cnt ++;
    }
}
void mix_update(sensor * sensors, int graph_matrix[][MAX_NUM_SENSOR], int num_sensor, int dimension, int iter){
    printf("mix_update!\n");
    int iter_cnt = 0;
    while(iter_cnt < iter){
        edge e = get_edge(graph_matrix, num_sensor);
        sensors[e.a].sum += cal_avg_dis(sensors[e.a].obsv_vec, sensors[e.b].mixed_vec, dimension);
        sensors[e.b].sum += cal_avg_dis(sensors[e.b].obsv_vec, sensors[e.a].mixed_vec, dimension);
        sensors[e.a].cnt ++;
        sensors[e.b].cnt ++;
        iter_cnt ++;
    }
}
void min(sensor * sensors, int graph_matrix[][MAX_NUM_SENSOR], int num_sensor, int dimension, int iter){
    printf("min!\n");
    int iter_cnt = 0;
    while(iter_cnt < iter){
        edge e = get_edge(graph_matrix, num_sensor);
        double avg_a = sensors[e.a].sum / sensors[e.a].cnt;
        double avg_b = sensors[e.b].sum / sensors[e.b].cnt;
        if (avg_a > avg_b){
            sensors[e.a].sum = sensors[e.b].sum;
            sensors[e.a].cnt = sensors[e.b].cnt;
            memcpy(sensors[e.a].t_vec, sensors[e.b].obsv_vec, sizeof(int) * MAX_DIMENSION);
        }
        else{
            sensors[e.b].sum = sensors[e.a].sum;
            sensors[e.b].cnt = sensors[e.a].cnt;
            memcpy(sensors[e.b].t_vec, sensors[e.a].obsv_vec, sizeof(int) * MAX_DIMENSION);
        }
        iter_cnt ++;
    }
}
void vote(sensor * sensors, int graph_matrix[][MAX_NUM_SENSOR], int num_sensor, int dimension, int iter){
    printf("vote!\n");
    for(int i = 0; i < num_sensor; i++){
        double diff_ratio = _cal_avg_dis(sensors[i].obsv_vec, sensors[i].t_vec, dimension);
        if(diff_ratio < 0.3334){
            for(int j = 0; j < num_sensor; j++){
                sensors[i].f_vec[j] = sensors[i].obsv_vec[j];
            }
        }
        else{
            for(int j = 0; j < num_sensor; j++){
                sensors[i].f_vec[j] = 0.5;
            }
        }
    }
    int iter_cnt = 0;
    while(iter_cnt < iter){
        edge e = get_edge(graph_matrix, num_sensor);
        for(int i = 0; i < num_sensor; i++){
            double a = sensors[e.a].f_vec[i];
            double b = sensors[e.b].f_vec[i];
            sensors[e.a].f_vec[i] = (a + b) / 2;
            sensors[e.b].f_vec[i] = (a + b) / 2;
        }
        iter_cnt ++;
    }
}

sensor appro(int obsv_vec[][MAX_DIMENSION], int graph_matrix[][MAX_NUM_SENSOR], int num_sensor, int dimension, int k1, int k2){
    sensor * sensors = malloc(sizeof(sensor) * num_sensor);
    for(int i = 0; i < num_sensor; i++){
        memcpy(sensors[i].obsv_vec, obsv_vec[i], sizeof(int) * MAX_DIMENSION);
        memset(sensors[i].mixed_vec, 0, sizeof(int) * MAX_DIMENSION);
        sensors[i].sum = 0;
        sensors[i].cnt = 0;
        memset(sensors[i].t_vec, 0, sizeof(int) * MAX_DIMENSION);
    }
    mix(sensors, graph_matrix, num_sensor, dimension, k1);
    mix_update(sensors, graph_matrix, num_sensor, dimension, k2 * 1589);
    min(sensors, graph_matrix, num_sensor, dimension, 5 * 1589);
    vote(sensors, graph_matrix, num_sensor, dimension, 5 * 1589);

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

    int k1 = atoi(argv[3]);
    int k2 = atoi(argv[4]);



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
    sensor res_vec = appro(obsv_vec, graph_matrix, MAX_NUM_SENSOR, dimension, k1, k2 * 1589);
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

    int final_vec[MAX_DIMENSION];
    for(int i = 0; i < dimension; i++){
        if(res_vec.f_vec[i] < 0.5){
            final_vec[i] = 0;
        }
        else{
            final_vec[i] = 1;
        }
    }
    double error_rate = _cal_avg_dis(final_vec, truth_vec, dimension);
    printf("error rate: %lf\n", error_rate);

    double obj_val = 0;
    for(int i = 0; i < MAX_NUM_SENSOR; i++){
        obj_val += cal_avg_dis(obsv_vec + i * MAX_DIMENSION, final_vec, dimension);
    }
    printf("objective value: %lf\n", obj_val);

    return 0;
}