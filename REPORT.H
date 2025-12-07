#ifndef BUS_RESERVATION_H
#define BUS_RESERVATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BUSES 100
#define MAX_SEATS 250
#define MAX_NAME 50
#define RESERVATION_FILE "reservations.dat"

typedef struct {
    int bus_id;
    char source[30];
    char destination[30];
    int total_seats;
    int available_seats;
    float fare;
} Bus;

typedef struct {
    char passenger_name[MAX_NAME];
    int bus_id;
    int seat_number;
    int reservation_id;
} Reservation;

extern Bus buses[MAX_BUSES];
extern Reservation reservations[MAX_SEATS * MAX_BUSES];
extern int reservation_count;
extern int next_reservation_id;

void initialize_buses();
void display_buses();
int find_bus(int bus_id);
void book_ticket();
void cancel_ticket();
void view_all_reservations();
void save_reservations();
void load_reservations();

#endif
