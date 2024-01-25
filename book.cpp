#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Passenger
{
    string name;
    string cruiseId;
    string accomodation;
    int waitListNumber;
};

struct Cruise
{
    string id;
    string name;
    int economySeats;
    int businessSeats;
    int regularSeats;
    queue<Passenger> waitingList;
};

// Function to read cruise information from a file
// Function to read cruise information from a file
vector<Cruise> readCruisesFromFile(const string& filename) {
    vector<Cruise> cruises;
    ifstream file(filename);

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string id, name;
            int economySeats, businessSeats, regularSeats;

            if (getline(iss, id, ',') &&
                getline(iss, name, ',') &&
                (iss >> economySeats) &&
                (iss.ignore(), iss >> businessSeats) &&
                (iss.ignore(), iss >> regularSeats)) {
                
                Cruise cruise = {id, name, economySeats, businessSeats, regularSeats};
                cruises.push_back(cruise);
            }
        }
        file.close();
    }

    return cruises;
}


// Function to print the status of seats in a cruise
// Function to print the status of seats in a cruise
void printSeatStatus(const Cruise &cruise)
{
    cout << "Economy Seats: ";
    for (int i = 0; i < cruise.economySeats; ++i)
    {
        cout << "O ";
    }
    for (int i = cruise.economySeats; i < 10; ++i)
    {
        cout << "X ";
    }
    cout << "\nBusiness Seats: ";
    for (int i = 0; i < cruise.businessSeats; ++i)
    {
        cout << "O ";
    }
    for (int i = cruise.businessSeats; i < 5; ++i)
    {
        cout << "X ";
    }
    cout << "\nRegular Seats: ";
    for (int i = 0; i < cruise.regularSeats; ++i)
    {
        cout << "O ";
    }
    for (int i = cruise.regularSeats; i < 10; ++i)
    {
        if (i == 0) {
            cout << "X ";  // Replace 0 with X
        } else {
            cout << "O ";
        }
    }
    cout << "\n";
}


// Function to print the status of seats in a cruise
void getCruiseStatus(const vector<Cruise> &cruises, const string &cruiseId)
{
    auto cruiseIt = find_if(cruises.begin(), cruises.end(), [cruiseId](const Cruise &cruise)
                            { return cruise.id == cruiseId; });

    if (cruiseIt != cruises.end())
    {
        const Cruise &cruise = *cruiseIt;

        cout << "Cruise ID: " << cruise.id << "\n";
        cout << "Cruise Name: " << cruise.name << "\n";

        printSeatStatus(cruise);

        cout << "Waiting List: ";
        queue<Passenger> tempQueue = cruise.waitingList;
        while (!tempQueue.empty())
        {
            cout << tempQueue.front().name << " ";
            tempQueue.pop();
        }
        cout << "\n";
    }
    else
    {
        cout << "Cruise with ID " << cruiseId << " not found." << endl;
    }
}

// Function to read passenger details from a file
vector<Passenger> readPassengersFromFile(const string &filename)
{
    vector<Passenger> passengers;
    ifstream file(filename);

    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            istringstream iss(line);
            string name, cruiseId, accomodation;
            int waitListNumber;

            if (getline(iss, name, ',') &&
                getline(iss, cruiseId, ',') &&
                getline(iss, accomodation, ',') &&
                (iss >> waitListNumber))
            {

                Passenger passenger = {name, cruiseId, accomodation, waitListNumber};
                passengers.push_back(passenger);
            }
        }
        file.close();
    }

    return passengers;
}

// Function to save passenger details to a file
void savePassengersToFile(const vector<Passenger> &passengers, const string &filename)
{
    ofstream file(filename);

    if (file.is_open())
    {
        for (const Passenger &passenger : passengers)
        {
            file << passenger.name << ',' << passenger.cruiseId << ',' << passenger.accomodation << ',' << passenger.waitListNumber << '\n';
        }
        file.close();
        cout << "Passenger details saved to " << filename << " successfully." << endl;
    }
    else
    {
        cout << "Unable to open " << filename << " for writing." << endl;
    }
}

// Function to schedule a passenger on a cruise
void schedulePassenger(vector<Cruise> &cruises, vector<Passenger> &passengers, const string &cruiseId, const string &passengerName, const string &accomodation)
{
    auto cruiseIt = find_if(cruises.begin(), cruises.end(), [cruiseId](const Cruise &cruise)
                            { return cruise.id == cruiseId; });

    if (cruiseIt != cruises.end())
    {
        Cruise &cruise = *cruiseIt;

        // Check available seats based on the accomodation
        if (accomodation == "economy" && cruise.economySeats > 0)
        {
            cruise.economySeats--;
        }
        else if (accomodation == "business" && cruise.businessSeats > 0)
        {
            cruise.businessSeats--;
        }
        else if (accomodation == "regular" && cruise.regularSeats > 0)
        {
            cruise.regularSeats--;
        }
        else
        {
            // If no available seats, add to the waiting list
            Passenger newPassenger = {passengerName, cruiseId, accomodation, static_cast<int>(cruise.waitingList.size()) + 1};
            cruise.waitingList.push(newPassenger);
            passengers.push_back(newPassenger);
            cout << "Passenger added to waiting list with number: " << newPassenger.waitListNumber << endl;
            savePassengersToFile(passengers, "passenger.txt"); // Save updated passenger details to file
            return;
        }

        Passenger newPassenger = {passengerName, cruiseId, accomodation, 0};
        passengers.push_back(newPassenger);
        cout << "Passenger scheduled successfully." << endl;
        savePassengersToFile(passengers, "passenger.txt"); // Save updated passenger details to file
    }
    else
    {
        cout << "Cruise with ID " << cruiseId << " not found." << endl;
    }
}

// Function to cancel a passenger booking
// Function to cancel a passenger booking
void cancelBooking(vector<Cruise> &cruises, vector<Passenger> &passengers, const string &cruiseId, const string &passengerName)
{
    auto cruiseIt = find_if(cruises.begin(), cruises.end(), [cruiseId](const Cruise &cruise)
                            { return cruise.id == cruiseId; });

    if (cruiseIt != cruises.end())
    {
        Cruise &cruise = *cruiseIt;

        auto passengerIt = remove_if(passengers.begin(), passengers.end(), [passengerName, cruiseId](const Passenger &passenger)
                                     { return passenger.name == passengerName && passenger.cruiseId == cruiseId; });

        if (passengerIt != passengers.end())
        {
            passengers.erase(passengerIt, passengers.end());

            // Refund the seat
            if (passengerIt->accomodation == "economy")
            {
                cruise.economySeats++;
            }
            else if (passengerIt->accomodation == "business")
            {
                cruise.businessSeats++;
            }
            else if (passengerIt->accomodation == "regular")
            {
                cruise.regularSeats++;
            }

            // Check waiting list and fill the seat
            if (!cruise.waitingList.empty())
            {
                Passenger waitingPassenger = cruise.waitingList.front();
                cruise.waitingList.pop();
                passengers.erase(remove_if(passengers.begin(), passengers.end(),
                                           [waitingPassenger](const Passenger &p)
                                           {
                                               return p.name == waitingPassenger.name && p.cruiseId == waitingPassenger.cruiseId;
                                           }),
                                 passengers.end());
                schedulePassenger(cruises, passengers, waitingPassenger.cruiseId, waitingPassenger.name, waitingPassenger.accomodation);
            }

            cout << "Booking canceled successfully." << endl;

            // Save updated passenger details to file after cancellation
            savePassengersToFile(passengers, "passenger.txt");
        }
        else
        {
            cout << "Passenger " << passengerName << " not found on cruise " << cruiseId << "." << endl;
        }
    }
    else
    {
        cout << "Cruise with ID " << cruiseId << " not found." << endl;
    }
}

// Function to get passenger status
void getPassengerStatus(const vector<Passenger> &passengers, const string &passengerName, const string &cruiseId)
{
    auto passengerIt = find_if(passengers.begin(), passengers.end(), [passengerName, cruiseId](const Passenger &passenger)
                               { return passenger.name == passengerName && passenger.cruiseId == cruiseId; });

    if (passengerIt != passengers.end())
    {
        const Passenger &passenger = *passengerIt;

        cout << "Passenger Name: " << passenger.name << "\n";
        cout << "Cruise ID: " << passenger.cruiseId << "\n";
        cout << "Accomodation: " << passenger.accomodation << "\n";

        if (passenger.waitListNumber != 0)
        {
            cout << "Waiting List Number: " << passenger.waitListNumber << "\n";
        }
        else
        {
            cout << "Ticket Confirmed\n";
        }
    }
    else
    {
        cout << "Passenger " << passengerName << " not found on cruise " << cruiseId << "." << endl;
    }
}

int main()
{
    cout << endl;
    cout << endl;
    cout << "           WELCOME TO KPIT CRUISE BOOKING PORTAL               " << endl;
    vector<Cruise> cruises = readCruisesFromFile("cruise.txt");

    // Read existing passenger details from file
    vector<Passenger> passengers = readPassengersFromFile("passenger.txt");

    char choice;
    do
    {
        cout << "\nOptions:\n";
        cout << "s - Schedule passenger\n";
        cout << "c - Cancel booking\n";
        cout << "p - Passenger status\n";
        cout << "f - Cruise status\n";
        cout << "q - Quit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 's':
        {
            string cruiseId, passengerName, accomodation;
            cout << "Enter cruise ID: ";
            cin >> cruiseId;
            cout << "Enter passenger name: ";
            cin >> passengerName;
            cout << "Select accomodation (economy/business/regular): ";
            cin >> accomodation;
            schedulePassenger(cruises, passengers, cruiseId, passengerName, accomodation);
            break;
        }
        case 'f':
        {
            string cruiseId;
            cout << "Enter cruise ID: ";
            cin >> cruiseId;
            getCruiseStatus(cruises, cruiseId);
            break;
        }
        case 'c':
        {
            string cruiseId, passengerName;
            cout << "Enter cruise ID: ";
            cin >> cruiseId;
            cout << "Enter passenger name: ";
            cin >> passengerName;
            cancelBooking(cruises, passengers, cruiseId, passengerName);
            break;
        }
        case 'p':
        {
            string passengerName, cruiseId;
            cout << "Enter passenger name: ";
            cin >> passengerName;
            cout << "Enter cruise ID: ";
            cin >> cruiseId;
            cout<<endl;
            cout<<"The passenger details are as follows: "<<endl;
            getPassengerStatus(passengers, passengerName, cruiseId);
            break;
        }
        case 'q':
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 'q');

    return 0;
}

// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <vector>
// #include <queue>
// #include <algorithm>

// using namespace std;

// struct Passenger {
//     string name;
//     string cruiseId;
//     string accomodation;
//     int waitListNumber;
//      bool operator==(const Passenger& other) const {
//         return name == other.name && cruiseId == other.cruiseId && accomodation == other.accomodation && waitListNumber == other.waitListNumber;
//      }
// };

// struct Cruise {
//     string id;
//     string name;
//     int economySeats;
//     int businessSeats;
//     int regularSeats;
//     string economySeatsStatus;  // Added for seat status
//     string businessSeatsStatus; // Added for seat status
//     string regularSeatsStatus;  // Added for seat status
//     queue<Passenger> waitingList;
// };

// // Function to read cruise information from a file
// vector<Cruise> readCruisesFromFile(const string& filename) {
//     vector<Cruise> cruises;
//     ifstream file(filename);

//     if (file.is_open()) {
//         string line;
//         while (getline(file, line)) {
//             istringstream iss(line);
//             string id, name;
//             int economySeats, businessSeats, regularSeats;

//             if (getline(iss, id, ',') &&
//                 getline(iss, name, ',') &&
//                 (iss >> economySeats) &&
//                 (iss.ignore(), iss >> businessSeats) &&
//                 (iss.ignore(), iss >> regularSeats)) {

//                 Cruise cruise = {id, name, economySeats, businessSeats, regularSeats, "", "", ""};
//                 cruises.push_back(cruise);
//             }
//         }
//         file.close();
//     }

//     return cruises;
// }

// // Function to read passenger details from a file
// vector<Passenger> readPassengersFromFile(const string& filename) {
//     vector<Passenger> passengers;
//     ifstream file(filename);

//     if (file.is_open()) {
//         string line;
//         while (getline(file, line)) {
//             istringstream iss(line);
//             string name, cruiseId, accomodation;
//             int waitListNumber;

//             if (getline(iss, name, ',') &&
//                 getline(iss, cruiseId, ',') &&
//                 getline(iss, accomodation, ',') &&
//                 (iss >> waitListNumber)) {

//                 Passenger passenger = {name, cruiseId, accomodation, waitListNumber};
//                 passengers.push_back(passenger);
//             }
//         }
//         file.close();
//     }

//     return passengers;
// }

// // Function to save passenger details to a file
// void savePassengersToFile(const vector<Passenger>& passengers, const string& filename) {
//     ofstream file(filename);

//     if (file.is_open()) {
//         for (const Passenger& passenger : passengers) {
//             file << passenger.name << ',' << passenger.cruiseId << ',' << passenger.accomodation << ',' << passenger.waitListNumber << '\n';
//         }
//         file.close();
//         cout << "Passenger details saved to " << filename << " successfully." << endl;
//     } else {
//         cout << "Unable to open " << filename << " for writing." << endl;
//     }
// }

// // Function to update seat status with 'X' for booked seat
// void updateSeatStatus(vector<Cruise>& cruises, const string& cruiseId, const string& accomodation, bool booked) {
//     auto cruiseIt = find_if(cruises.begin(), cruises.end(), [cruiseId](const Cruise& cruise) {
//         return cruise.id == cruiseId;
//     });

//     if (cruiseIt != cruises.end()) {
//         Cruise& cruise = *cruiseIt;

//         // Update seat status based on accomodation and booked status
//         if (accomodation == "economy") {
//             cruise.economySeatsStatus = booked ? "X" : "O";
//         } else if (accomodation == "business") {
//             cruise.businessSeatsStatus = booked ? "X" : "O";
//         } else if (accomodation == "regular") {
//             cruise.regularSeatsStatus = booked ? "X" : "O";
//         }
//     } else {
//         cout << "Cruise with ID " << cruiseId << " not found." << endl;
//     }
// }

// // Function to schedule a passenger on a cruise
// void schedulePassenger(vector<Cruise>& cruises, vector<Passenger>& passengers, const string& cruiseId, const string& passengerName, const string& accomodation) {
//     auto cruiseIt = find_if(cruises.begin(), cruises.end(), [cruiseId](const Cruise& cruise) {
//         return cruise.id == cruiseId;
//     });

//     if (cruiseIt != cruises.end()) {
//         Cruise& cruise = *cruiseIt;

//         // Check available seats based on the accomodation
//         if (accomodation == "economy" && cruise.economySeats > 0) {
//             cruise.economySeats--;
//         } else if (accomodation == "business" && cruise.businessSeats > 0) {
//             cruise.businessSeats--;
//         } else if (accomodation == "regular" && cruise.regularSeats > 0) {
//             cruise.regularSeats--;
//         } else {
//             // If no available seats, add to the waiting list
//             Passenger newPassenger = {passengerName, cruiseId, accomodation, static_cast<int>(cruise.waitingList.size()) + 1};
//             cruise.waitingList.push(newPassenger);
//             passengers.push_back(newPassenger);
//             cout << "Passenger added to waiting list with number: " << newPassenger.waitListNumber << endl;
//             savePassengersToFile(passengers, "passenger.txt");  // Save updated passenger details to file
//             return;
//         }

//         Passenger newPassenger = {passengerName, cruiseId, accomodation, 0};
//         passengers.push_back(newPassenger);
//         cout << "Passenger scheduled successfully." << endl;
//         savePassengersToFile(passengers, "passenger.txt");  // Save updated passenger details to file

//         // Update seat status with 'X' for booked seat and 'O' for available seat
//         updateSeatStatus(cruises, cruiseId, accomodation, true);
//     } else {
//         cout << "Cruise with ID " << cruiseId << " not found." << endl;
//     }
// }
// // Function to print the status of seats in a cruise
// void printSeatStatus(const Cruise& cruise) {
//     cout << "Economy Seats: " << cruise.economySeatsStatus << "\n";
//     cout << "Business Seats: " << cruise.businessSeatsStatus << "\n";
//     cout << "Regular Seats: " << cruise.regularSeatsStatus << "\n";
// }

// // Function to print the status of a cruise
// void getCruiseStatus(const vector<Cruise>& cruises, const string& cruiseId) {
//     auto cruiseIt = find_if(cruises.begin(), cruises.end(), [cruiseId](const Cruise& cruise) {
//         return cruise.id == cruiseId;
//     });

//     if (cruiseIt != cruises.end()) {
//         const Cruise& cruise = *cruiseIt;

//         cout << "Cruise ID: " << cruise.id << "\n";
//         cout << "Cruise Name: " << cruise.name << "\n";

//         printSeatStatus(cruise);

//         if (!cruise.waitingList.empty()) {
//             cout << "Waiting List: ";
//             queue<Passenger> tempQueue = cruise.waitingList;
//             while (!tempQueue.empty()) {
//                 cout << tempQueue.front().name << " ";
//                 tempQueue.pop();
//             }
//             cout << "\n";
//         } else {
//             cout << "Confirm Ticket\n";
//         }
//     } else {
//         cout << "Cruise with ID " << cruiseId << " not found." << endl;
//     }
// }

// // Function to cancel a passenger booking
// void cancelBooking(vector<Cruise>& cruises, vector<Passenger>& passengers, const string& cruiseId, const string& passengerName) {
//     auto cruiseIt = find_if(cruises.begin(), cruises.end(), [cruiseId](const Cruise& cruise) {
//         return cruise.id == cruiseId;
//     });

//     if (cruiseIt != cruises.end()) {
//         Cruise& cruise = *cruiseIt;

//         auto passengerIt = find_if(passengers.begin(), passengers.end(), [passengerName, cruiseId](const Passenger& passenger) {
//             return passenger.name == passengerName && passenger.cruiseId == cruiseId;
//         });

//         if (passengerIt != passengers.end()) {
//             Passenger& passenger = *passengerIt;

//             // Remove passenger from booked seat
//             if (passenger.accomodation == "economy") {
//                 cruise.economySeats++;
//             } else if (passenger.accomodation == "business") {
//                 cruise.businessSeats++;
//             } else if (passenger.accomodation == "regular") {
//                 cruise.regularSeats++;
//             }

//             // Update seat status with 'O' for canceled seat
//             updateSeatStatus(cruises, cruiseId, passenger.accomodation, false);

//             // Remove passenger from the list
//             passengers.erase(remove(passengers.begin(), passengers.end(), passenger), passengers.end());
//             cout << "Booking for passenger " << passengerName << " canceled successfully." << endl;

//             // Check and process the waiting list
//             if (!cruise.waitingList.empty()) {
//                 Passenger waitingPassenger = cruise.waitingList.front();
//                 cruise.waitingList.pop();
//                 waitingPassenger.waitListNumber = 0;
//                 passengers.push_back(waitingPassenger);
//                 cout << "Booking for passenger " << waitingPassenger.name << " from waiting list confirmed." << endl;
//                 savePassengersToFile(passengers, "passenger.txt");  // Save updated passenger details to file

//                 // Update seat status with 'X' for booked seat from waiting list
//                 updateSeatStatus(cruises, cruiseId, waitingPassenger.accomodation, true);
//             } else {
//                 cout << "No passengers in the waiting list." << endl;
//             }
//         } else {
//             cout << "Passenger " << passengerName << " not found on cruise " << cruiseId << "." << endl;
//         }
//     } else {
//         cout << "Cruise with ID " << cruiseId << " not found." << endl;
//     }
// }

// // Function to print passenger status
// void getPassengerStatus(const vector<Passenger>& passengers, const string& passengerName, const string& cruiseId) {
//     auto passengerIt = find_if(passengers.begin(), passengers.end(), [passengerName, cruiseId](const Passenger& passenger) {
//         return passenger.name == passengerName && passenger.cruiseId == cruiseId;
//     });

//     if (passengerIt != passengers.end()) {
//         const Passenger& passenger = *passengerIt;

//         cout << "Passenger Name: " << passenger.name << "\n";
//         cout << "Cruise ID: " << passenger.cruiseId << "\n";
//         cout << "Accomodation: " << passenger.accomodation << "\n";

//         if (passenger.waitListNumber != 0) {
//             cout << "Waiting List Number: " << passenger.waitListNumber << "\n";
//         } else {
//             cout << "Ticket Confirmed\n";
//         }
//     } else {
//         cout << "Passenger " << passengerName << " not found on cruise " << cruiseId << "." << endl;
//     }
// }

// // Function to print the menu
// void printMenu() {
//     cout << "Menu:\n";
//     cout << "s -> Schedule Passenger\n";
//     cout << "c -> Cancel Booking\n";
//     cout << "p -> Passenger Status\n";
//     cout << "f -> Cruise Status\n";
//     cout << "q -> Quit\n";
// }

// int main() {
//     // Read cruise information from file
//     vector<Cruise> cruises = readCruisesFromFile("cruise.txt");

//     // Read passenger information from file
//     vector<Passenger> passengers = readPassengersFromFile("passenger.txt");

//     char choice;
//     do {
//         printMenu();
//         cout << "Enter choice: ";
//         cin >> choice;

//         switch (choice) {
//             case 's': {
//                 string passengerName, cruiseId, accomodation;
//                 cout << "Enter passenger name: ";
//                 cin >> passengerName;
//                 cout << "Enter cruise ID: ";
//                 cin >> cruiseId;
//                 cout << "Enter accomodation (economy/business/regular): ";
//                 cin >> accomodation;
//                 schedulePassenger(cruises, passengers, cruiseId, passengerName, accomodation);
//                 break;
//             }
//             case 'c': {
//                 string passengerName, cruiseId;
//                 cout << "Enter passenger name: ";
//                 cin >> passengerName;
//                 cout << "Enter cruise ID: ";
//                 cin >> cruiseId;
//                 cancelBooking(cruises, passengers, cruiseId, passengerName);
//                 break;
//             }
//             case 'p': {
//                 string passengerName, cruiseId;
//                 cout << "Enter passenger name: ";
//                 cin >> passengerName;
//                 cout << "Enter cruise ID: ";
//                 cin >> cruiseId;
//                 getPassengerStatus(passengers, passengerName, cruiseId);
//                 break;
//             }
//             case 'f': {
//                 string cruiseId;
//                 cout << "Enter cruise ID: ";
//                 cin >> cruiseId;
//                 getCruiseStatus(cruises, cruiseId);
//                 break;
//             }
//             case 'q':
//                 cout << "Exiting program.\n";
//                 break;
//             default:
//                 cout << "Invalid choice. Please try again.\n";
//         }

//     } while (choice != 'q');

//     return 0;
// }
