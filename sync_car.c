#include "sync_car.h"
#include <time.h>

void station_init(struct station *station) {
    station->car_present = 0;
    station->free_seats = 0;
    station->waiting = 0;
    station->boarding = 0;

    pthread_mutex_init(&station->mutex, NULL);
    pthread_cond_init(&station->car_arrived, NULL);
    pthread_cond_init(&station->passenger_boarded, NULL);
}
void station_load_car(struct station *station, int count) { }
void station_wait_for_car(struct station *station) { }
void station_on_board(struct station *station) { }
