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

Bus buses[MAX_BUSES];
Reservation reservations[MAX_SEATS * MAX_BUSES];
int reservation_count = 0;
int next_reservation_id = 1001;

void initialize_buses() {
    int bus_data[][6] = {
        {101, 30, 500},  
        {102, 35, 50},
        {103, 40, 60},
        {104, 32, 55},
        {105, 38, 80}
    };
    
    char* sources[] = {"New Delhi", "Bhiwani", "Chandigrah", "Lucknow", "Manali"};
    char* destinations[] = {"lahore", "Pune", "Dehradun", "Sonipat", "Amritsar"};
    
    for(int i = 0; i < 5; i++) {
        buses[i].bus_id = bus_data[i][0];
        buses[i].total_seats = bus_data[i][1];
        buses[i].available_seats = bus_data[i][1];
        buses[i].fare = bus_data[i][2];
        strcpy(buses[i].source, sources[i]);
        strcpy(buses[i].destination, destinations[i]);
    }
}

void display_buses() {
    printf("\n AVAILABLE BUSES \n");
    printf("Bus ID\nSource\t\nDestination\nAvailable Seats\nFare\n");
    printf("\n");
    
    for(int i = 0; i < 5; i++) {
        if(buses[i].bus_id != 0) {
            printf("%d\t%-12s\t%-12s\t%d\t\t$%.2f\n", 
                   buses[i].bus_id, 
                   buses[i].source, 
                   buses[i].destination, 
                   buses[i].available_seats, 
                   buses[i].fare);
        }
    }
    printf("\n");
}

int find_bus(int bus_id) {
    for(int i = 0; i < MAX_BUSES; i++) {
        if(buses[i].bus_id == bus_id) {
            return i;
        }
    }
    return -1;
}

void book_ticket() {
    int bus_id, bus_index;
    char name[MAX_NAME];
    
    display_buses();
    
    printf("Enter Bus ID: ");
    scanf("%d", &bus_id);
    
    bus_index = find_bus(bus_id);
    if(bus_index == -1) {
        printf("Invalid Bus ID!\n");
        return;
    }
    
    if(buses[bus_index].available_seats <= 0) {
        printf("Sorry, no seats available on this bus!\n");
        return;
    }
    
    printf("Enter Passenger Name: ");
    getchar(); 
    fgets(name, MAX_NAME, stdin);
    name[strcspn(name, "\n")] = 0; 
    
    Reservation new_res;
    strcpy(new_res.passenger_name, name);
    new_res.bus_id = bus_id;
    new_res.seat_number = buses[bus_index].total_seats - buses[bus_index].available_seats + 1;
    new_res.reservation_id = next_reservation_id++;
    
    reservations[reservation_count++] = new_res;
    buses[bus_index].available_seats--;
    
    printf("\n TICKET BOOKED SUCCESSFULLY \n");
    printf("Reservation ID: %d\n", new_res.reservation_id);
    printf("Passenger: %s\n", new_res.passenger_name);
    printf("Bus ID: %d\n", new_res.bus_id);
    printf("Seat Number: %d\n", new_res.seat_number);
    printf("Total Fare: $%.2f\n", buses[bus_index].fare);
}

void cancel_ticket() {
    int reservation_id, found = 0;
    
    printf("Enter Reservation ID to cancel: ");
    scanf("%d", &reservation_id);
    
    for(int i = 0; i < reservation_count; i++) {
        if(reservations[i].reservation_id == reservation_id) {
            found = 1;
            int bus_index = find_bus(reservations[i].bus_id);
            
            printf("\n RESERVATION FOUND \n");
            printf("Reservation ID: %d\n", reservations[i].reservation_id);
            printf("Passenger: %s\n", reservations[i].passenger_name);
            printf("Bus ID: %d\n", reservations[i].bus_id);
            printf("Seat Number: %d\n", reservations[i].seat_number);
            
            char confirm;
            printf("\nAre you sure you want to cancel? (y/n): ");
            scanf(" %c", &confirm);
            
            if(tolower(confirm) == 'y') {
                
                buses[bus_index].available_seats++;
                
                for(int j = i; j < reservation_count - 1; j++) {
                    reservations[j] = reservations[j + 1];
                }
                reservation_count--;
                
                printf("Reservation cancelled successfully!\n");
            } else {
                printf("Cancellation aborted.\n");
            }
            break;
        }
    }
    
    if(!found) {
        printf("Reservation ID not found!\n");
    }
}

void view_all_reservations() {
    if(reservation_count == 0) {
        printf("No reservations found!\n");
        return;
    }
    
    printf("\n=== ALL RESERVATIONS ===\n");
    printf("Res ID\tPassenger Name\t\tBus ID\tSeat No\n");
    printf("------------------------------------------------\n");
    
    for(int i = 0; i < reservation_count; i++) {
        printf("%d\t%-20s\t%d\t%d\n", 
               reservations[i].reservation_id,
               reservations[i].passenger_name,
               reservations[i].bus_id,
               reservations[i].seat_number);
    }
}

void save_reservations() {
    FILE *file = fopen(RESERVATION_FILE, "wb");
    if(file == NULL) {
        printf("Error saving reservations!\n");
        return;
    }
    
    fwrite(&reservation_count, sizeof(int), 1, file);
    fwrite(&next_reservation_id, sizeof(int), 1, file);
    fwrite(reservations, sizeof(Reservation), reservation_count, file);
    
    fclose(file);
}

void load_reservations() {
    FILE *file = fopen(RESERVATION_FILE, "rb");
    if(file == NULL) {
        return; 
    }
    
    fread(&reservation_count, sizeof(int), 1, file);
    fread(&next_reservation_id, sizeof(int), 1, file);
    fread(reservations, sizeof(Reservation), reservation_count, file);
    
    fclose(file);
}

int main() {
    int choice;
    
    initialize_buses();
    load_reservations();
    
    printf(" BUS RESERVATION SYSTEM \n");
    
    do {
        printf("\nMain Menu:\n");
        printf("1. View Available Buses\n");
        printf("2. Book Ticket\n");
        printf("3. Cancel Ticket\n");
        printf("4. View All Reservations\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                display_buses();
                break;
            case 2:
                book_ticket();
                break;
            case 3:
                cancel_ticket();
                break;
            case 4:
                view_all_reservations();
                break;
            case 5:
                save_reservations();
                printf("Thank you for using Bus Reservation System!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while(choice != 5);
    
    return 0;
}