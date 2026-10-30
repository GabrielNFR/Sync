#include "sync_car.h"
#include <pthread.h>

void station_init(struct station *station) {
    station->car_present = 0;
    station->free_seats = 0;
    station->waiting = 0;
    station->boarding = 0;

    pthread_mutex_init(&station->mutex, NULL);
    pthread_cond_init(&station->car_arrived, NULL);
    pthread_cond_init(&station->passenger_boarded, NULL);
}
void station_load_car(struct station *station, int count) {
    pthread_mutex_lock(&station->mutex);

    station->car_present = 1;
    station->free_seats = (count > 0) ? count : 0;
    station->boarding = 0;

    if (station->free_seats > 0) {
        pthread_cond_broadcast(&station->car_arrived);
    }

    while(station->boarding > 0 || (station->free_seats > 0 && station->waiting > 0)) {
        pthread_cond_wait(&station->passenger_boarded, &station->mutex);
    }

    station->car_present = 0;
    station->free_seats = 0;

    pthread_mutex_unlock(&station->mutex);
}
void station_wait_for_car(struct station *station) {
    pthread_mutex_lock(&station->mutex);

    station->waiting++;
    
    while(!station->car_present || station->free_seats == 0) {
        pthread_cond_wait(&station->car_arrived, &station->mutex);
    }

    station->waiting--;
    station->free_seats--;
    station->boarding++;

    pthread_mutex_unlock(&station->mutex);
}
void station_on_board(struct station *station) {
    pthread_mutex_lock(&station->mutex);

    station->boarding--;
    pthread_cond_signal(&station->passenger_boarded);

    pthread_mutex_unlock(&station->mutex);
}
