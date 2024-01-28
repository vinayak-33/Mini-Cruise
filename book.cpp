#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const string ADMIN_PASSWORD = "CRUISE123"; // Admin password for adding new cruise

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
    string departure_time;
    string arrival_time;
    int economySeats;
    int businessSeats;
    int regularSeats;
    double economyPrice;
    double businessPrice;
    double regularPrice;
    queue<Passenger> waitingList;
};
// Function to read cruise information from a file
vector<Cruise> readCruisesFromFile(const string &filename)
{
    vector<Cruise> cruises;
    ifstream file(filename);

    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            istringstream iss(line);
            string id, name,departure_time,arrival_time;
            int economySeats, businessSeats, regularSeats;

            if (getline(iss, id, ',') &&
                getline(iss, name, ',') &&
                (iss >> economySeats) &&
                (iss.ignore(), iss >> businessSeats) &&
                (iss.ignore(), iss >> regularSeats))
            {

                Cruise cruise = {id, name, departure_time,arrival_time, economySeats, businessSeats, regularSeats};
                cruises.push_back(cruise);
            }
        }
        file.close();
    }

    return cruises;
}

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
        cout << "X ";
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
        cout<<"Departure Time: "<<cruise.departure_time<<"\n";
        cout<<"Arrival Time: "<<cruise.arrival_time<<"\n";

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
                                           { return p.name == waitingPassenger.name && p.cruiseId == waitingPassenger.cruiseId; }),
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

// Function to add a new cruise (admin-only operation)
void addNewCruise(vector<Cruise> &cruises)
{
    string id, name,departure_time,arrival_time;
    int economySeats, businessSeats, regularSeats;
    double economyPrice, businessPrice, regularPrice;
    cout << "Enter new cruise ID: ";
    cin >> id;
    cout << "Enter new cruise name: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, name);
    cout<<"Enter Departure Time: "<<endl;
    cin>>departure_time;
    cout<<"Enter Arrival Time: "<<endl;
    cin>>arrival_time;
    cout << "Enter number of economy seats: ";
    cin >> economySeats;
    cout << "Enter price for economy class: ";
    cin >> economyPrice;
    cout << "Enter number of business seats: ";
    cin >> businessSeats;
    cout << "Enter price for business class: ";
    cin >> businessPrice;
    cout << "Enter number of regular seats: ";
    cin >> regularSeats;
    cout << "Enter price for regular class: ";
    cin >> regularPrice;

    Cruise newCruise = {id, name, departure_time,arrival_time, economySeats, businessSeats, regularSeats, economyPrice, businessPrice, regularPrice};
    cruises.push_back(newCruise);
    cout << "New cruise added successfully." << endl;

    // Save updated cruise details to file
    ofstream file("cruise.txt", ios::app); // Append mode
    if (file.is_open())
    {
        file << id << ',' << name << ',' << economySeats << ',' << businessSeats << ',' << regularSeats << ',' << economyPrice << ',' << businessPrice << ',' << regularPrice << '\n';
        file.close();
        cout << "Cruise details saved to file." << endl;
    }
    else
    {
        cout << "Unable to open file to save cruise details." << endl;
    }
}

// Function to delete a cruise (admin-only operation)
void deleteCruise(vector<Cruise> &cruises)
{
    string cruiseId;
    cout << "Enter cruise ID to delete: ";
    cin >> cruiseId;

    auto cruiseIt = find_if(cruises.begin(), cruises.end(), [cruiseId](const Cruise &cruise)
                            { return cruise.id == cruiseId; });

    if (cruiseIt != cruises.end())
    {
        cruises.erase(cruiseIt);
        cout << "Cruise with ID " << cruiseId << " deleted successfully." << endl;

        // Save updated cruise details to file after deletion
        ofstream file("cruise.txt");
        if (file.is_open())
        {
            for (const Cruise &cruise : cruises)
            {
                file << cruise.id << ',' << cruise.name << ',' << cruise.economySeats << ',' << cruise.businessSeats << ',' << cruise.regularSeats << '\n';
            }
            file.close();
            cout << "Cruise details saved to file." << endl;
        }
        else
        {
            cout << "Unable to open file to save cruise details." << endl;
        }
    }
    else
    {
        cout << "Cruise with ID " << cruiseId << " not found." << endl;
    }
}

// Admin menu
void adminMenu(vector<Cruise> &cruises)
{
    char choice;
    cout << "\nAdmin Menu:\n";
    cout << "A - Add new cruise\n";
    cout << "D - Delete cruise\n";
    cout << "Enter choice: ";
    cin >> choice;

    switch (choice)
    {
    case 'A':
        addNewCruise(cruises);
        break;
    case 'D':
        deleteCruise(cruises);
        break;
    default:
        cout << "Invalid choice. Please try again." << endl;
        break;
    }
}
int main()
{
    cout << "WELCOME TO KPIT CRUISE BOOKING PORTAL" << endl;

    // Read existing cruise details from file
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
        cout << "N - Admin\n";
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
            cout << "Select accommodation (economy/business/regular): ";
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
            cout << endl;
            cout << "The passenger details are as follows: " << endl;
            getPassengerStatus(passengers, passengerName, cruiseId);
            break;
        }
        case 'N':
        {
            string adminPassword;
            cout << "Enter admin password: ";
            cin >> adminPassword;
            if (adminPassword == ADMIN_PASSWORD)
            {
                adminMenu(cruises);
            }
            else
            {
                cout << "Invalid admin password." << endl;
            }
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



