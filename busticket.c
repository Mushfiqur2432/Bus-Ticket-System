#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global constants and file paths
#define MAX_SEATS 40
#define ADMIN_PASSWORD "admin123"
#define DATA_FILE "bus_data.txt"

// Function prototypes
void adminLogin();
void adminPanel();
void travelerInterface();
void reserveSeat();
void viewAvailableSeats();
void reserveSeatsForGroup(int numSeats, char seatType[], char passengerName[], int origin, int destination);
void cancelReservation();
void updateSeatPrices();
void saveDataToFile();
void loadDataFromFile();
void clearScreen();
float calculateFare(int origin, int destination, char seatType[]);

// Data structure for seats
typedef struct {
    int seatNumber;
    int isReserved; // 0 = available, 1 = reserved
    char passengerName[50];
    char seatType[10]; // e.g., "economy", "premium"
    int originDistance; // Distance from the origin
    int destinationDistance; // Distance to the destination
} Seat;

// Global array to store seat information
Seat seats[MAX_SEATS];

// Main function
int main() {
    int choice;
    // Load seat data from file
    loadDataFromFile();

    while (1) {
        clearScreen();
        printf("=====================================\n");
        printf("     Bus Ticket Reservation System\n");
        printf("=====================================\n");
        printf("1. Admin Login\n");
        printf("2. Traveler Interface\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                adminLogin();
                break;
            case 2:
                travelerInterface();
                break;
            case 3:
                saveDataToFile();
                printf("Exiting... Data saved successfully.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}

// Function to clear the screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Admin login function
void adminLogin() {
    char password[20];
    clearScreen();
    printf("Enter Admin Password: ");
    scanf("%s", password);

    if (strcmp(password, ADMIN_PASSWORD) == 0) {
        printf("Login successful!\n");
        adminPanel();
    } else {
        printf("Invalid password. Returning to main menu.\n");
    }
}

// Admin panel function
void adminPanel() {
    int choice;
    while (1) {
        clearScreen();
        printf("=====================================\n");
        printf("          Admin Panel\n");
        printf("=====================================\n");
        printf("1. View Available Seats\n");
        printf("2. Update Seat Prices\n");
        printf("3. Return to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewAvailableSeats();
                break;
            case 2:
                updateSeatPrices();
                break;
            case 3:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Traveler interface function
void travelerInterface() {
    int choice;
    while (1) {
        clearScreen();
        printf("=====================================\n");
        printf("         Traveler Interface\n");
        printf("=====================================\n");
        printf("1. View Available Seats\n");
        printf("2. Reserve a Seat\n");
        printf("3. Reserve Seats for Group\n");
        printf("4. Cancel Reservation\n");
        printf("5. Return to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewAvailableSeats();
                break;
            case 2:
                reserveSeat();
                break;
            case 3:
                {
                    int numSeats;
                    char seatType[10];
                    char passengerName[50];
                    printf("Enter number of seats to reserve: ");
                    scanf("%d", &numSeats);
                    printf("Enter seat type (economy/premium): ");
                    scanf("%s", seatType);
                    printf("Enter your name: ");
                    scanf("%s", passengerName);
                    int origin, destination;
                    printf("Enter the origin distance: ");
                    scanf("%d", &origin);
                    printf("Enter the destination distance: ");
                    scanf("%d", &destination);
                    reserveSeatsForGroup(numSeats, seatType, passengerName, origin, destination);
                }
                break;
            case 4:
                cancelReservation();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to view available seats
void viewAvailableSeats() {
    clearScreen();
    printf("=====================================\n");
    printf("         Available Seats\n");
    printf("=====================================\n");

    for (int i = 0; i < MAX_SEATS; i++) {
        if (!seats[i].isReserved) {
            printf("Seat %d: Available\n", i + 1);
        } else {
            printf("Seat %d: Reserved by %s (%s)\n", i + 1, seats[i].passengerName, seats[i].seatType);
        }
    }
    printf("Press Enter to return to the menu...");
    while (getchar() != '\n'); // Clear the input buffer
    getchar(); // Wait for Enter key
}

// Function to calculate fare based on seat type and distance
float calculateFare(int origin, int destination, char seatType[]) {
    int distance = abs(destination - origin);  // Calculate distance
    float basePrice = 0.1 * distance;  // Base price per kilometer
    float priceMultiplier = 1.0;

    // Price adjustment based on seat type
    if (strcmp(seatType, "premium") == 0) {
        priceMultiplier = 1.5; // 50% more for premium seats
    }

    return basePrice * priceMultiplier;
}

// Function to reserve a seat
void reserveSeat() {
    int seatNumber, origin, destination;
    char seatType[10];
    clearScreen();
    printf("Enter the seat number you want to reserve (1-%d): ", MAX_SEATS);
    scanf("%d", &seatNumber);

    while (seatNumber < 1 || seatNumber > MAX_SEATS) {
        printf("Invalid seat number. Please try again.\n");
        printf("Enter the seat number you want to reserve (1-%d): ", MAX_SEATS);
        scanf("%d", &seatNumber);
    }

    if (seats[seatNumber - 1].isReserved) {
        printf("Seat %d is already reserved.\n", seatNumber);
        return;
    }

    printf("Enter the origin distance: ");
    scanf("%d", &origin);
    printf("Enter the destination distance: ");
    scanf("%d", &destination);
    printf("Enter seat type (economy/premium): ");
    scanf("%s", seatType);
    if (strcmp(seatType, "economy") != 0 && strcmp(seatType, "premium") != 0) {
        printf("Invalid seat type. Please enter either 'economy' or 'premium'.\n");
        return;
    }
    float fare = calculateFare(origin, destination, seatType);
    printf("Fare for this seat: $%.2f\n", fare);

    printf("Enter your name: ");
    scanf("%s", seats[seatNumber - 1].passengerName);
    strcpy(seats[seatNumber - 1].seatType, seatType);
    seats[seatNumber - 1].originDistance = origin;
    seats[seatNumber - 1].destinationDistance = destination;
    seats[seatNumber - 1].isReserved = 1;
    printf("Seat %d reserved successfully for %s.\n", seatNumber, seats[seatNumber - 1].passengerName);
    printf("Fare for this seat: $%.2f\n", calculateFare(origin, destination, seatType));

  
    printf("SMS: Your seat has been successfully booked.\n");
    printf("Press any key to return to the Traveler Interface...");
    while (getchar() != '\n'); // Clear the input buffer
    getchar(); // Wait for any key
}

// Function to reserve seats together for groups
void reserveSeatsForGroup(int numSeats, char seatType[], char passengerName[], int origin, int destination) {
    if (strcmp(seatType, "economy") != 0 && strcmp(seatType, "premium") != 0) {
        printf("Invalid seat type. Please enter either 'economy' or 'premium'.\n");
        return;
    }

    int count = 0;
    for (int i = 0; i < MAX_SEATS && count < numSeats; i++) {
        if (!seats[i].isReserved) {
            seats[i].isReserved = 1;
            strcpy(seats[i].passengerName, passengerName);
            strcpy(seats[i].seatType, seatType);
            seats[i].originDistance = origin;
            seats[i].destinationDistance = destination;
            count++;
        }
    }

    if (count == numSeats) {
        printf("Group of %d seats reserved successfully.\n", numSeats);
    } else {
        printf("Not enough available seats for a group of %d.\n", numSeats);
    }

    printf("Seat Numbers reserved: ");
    for (int i = 0; i < MAX_SEATS; i++) {
        if (seats[i].isReserved && strcmp(seats[i].passengerName, passengerName) == 0) {
            printf("%d ", seats[i].seatNumber);
        }
    }
    printf("\n");

    float totalCost = 0;
    for (int i = 0; i < MAX_SEATS; i++) {
        if (seats[i].isReserved && strcmp(seats[i].passengerName, passengerName) == 0) {
            float fare = calculateFare(seats[i].originDistance, seats[i].destinationDistance, seats[i].seatType);
            totalCost += fare;
            printf("Seat %d: Distance %d to %d, Cost: $%.2f\n", seats[i].seatNumber, seats[i].originDistance, seats[i].destinationDistance, fare);
        }
    }
    printf("Total cost for the reserved seats: $%.2f\n", totalCost);
    printf("SMS: Your seats have been successfully booked.\n");
    printf("Press Enter to return to the Traveler Interface...");
    while (getchar() != '\n'); // Clear the input buffer
    getchar(); // Wait for Enter key
}

// Function to cancel a reservation
void cancelReservation() {
    int seatNumber;
    clearScreen();
    printf("Enter the seat number to cancel reservation (1-%d): ", MAX_SEATS);
    scanf("%d", &seatNumber);

    if (seatNumber < 1 || seatNumber > MAX_SEATS || !seats[seatNumber - 1].isReserved) {
        printf("Invalid seat number or no reservation exists.\n");
        return;
    }

    seats[seatNumber - 1].isReserved = 0;
    strcpy(seats[seatNumber - 1].passengerName, "");
    strcpy(seats[seatNumber - 1].seatType, "economy");
    seats[seatNumber - 1].originDistance = 0;
    seats[seatNumber - 1].destinationDistance = 0;
    printf("Reservation for seat %d has been cancelled.\n", seatNumber);
    printf("SMS: Your reservation has been successfully cancelled.\n");
    printf("Press Enter to return to the Traveler Interface...");
    while (getchar() != '\n'); // Clear the input buffer
    getchar(); // Wait for Enter key
}

// Function to update seat prices
void updateSeatPrices() {
    int seatNumber;
    char seatType[10];
    clearScreen();
    printf("Enter the seat number to update price (1-%d): ", MAX_SEATS);
    scanf("%d", &seatNumber);
    printf("Enter the seat type (economy/premium): ");
    scanf("%s", seatType);

    if (seatNumber < 1 || seatNumber > MAX_SEATS) {
        printf("Invalid seat number.\n");
        return;
    }

    strcpy(seats[seatNumber - 1].seatType, seatType);
    printf("Price for seat %d updated to %s.\n", seatNumber, seatType);
}

// Function to save data to file
void saveDataToFile() {
    FILE *file = fopen(DATA_FILE, "w");
    if (!file) {
        printf("Error saving data to file.\n");
        return;
    }

    for (int i = 0; i < MAX_SEATS; i++) {
        fprintf(file, "%d %d %s %s %d %d\n", seats[i].seatNumber, seats[i].isReserved, seats[i].passengerName,
                seats[i].seatType, seats[i].originDistance, seats[i].destinationDistance);
    }

    fclose(file);
    printf("Data saved successfully.\n");
}

// Function to load data from file
void loadDataFromFile() {
    FILE *file = fopen(DATA_FILE, "r");
    if (!file) {
        printf("Error loading data from file. Initializing new data.\n");
        for (int i = 0; i < MAX_SEATS; i++) {
            seats[i].seatNumber = i + 1;
            seats[i].isReserved = 0;
            strcpy(seats[i].passengerName, "");
            strcpy(seats[i].seatType, "economy");
            seats[i].originDistance = 0;
            seats[i].destinationDistance = 0;
        }
        return;
    }

    for (int i = 0; i < MAX_SEATS; i++) {
        fscanf(file, "%d %d %s %s %d %d", &seats[i].seatNumber, &seats[i].isReserved, seats[i].passengerName,
               seats[i].seatType, &seats[i].originDistance, &seats[i].destinationDistance);
    }

    fclose(file);
}