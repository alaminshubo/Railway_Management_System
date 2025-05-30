#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iomanip>
#include <ctime>
#define ROWS 5
#define SEATS_PER_ROW 5
#define MAX_PASSENGERS 100
#define MAX_TRAINS 100
#define MAX_INVOICES 100
#define MAX_MESSAGES 100
#define MAX_CARGO 100
#define MAX_NAME_LENGTH 50
#define MAX_MESSAGE_LENGTH 200
#define MAX_TIMESTAMP_LENGTH 20
using namespace std;

class TicketManagementSystem;
class TrainControlSystem;
class TicketSystem;
class HelpDesk;
class CargoManager;

class Seat {
private:
    string status;
public:
    Seat() : status("Available") {}
    string getStatus() const { return status; }
    void bookSeat() { status = "Booked"; }
    friend class ReservationSystem;
};

class ReservationSystem {
private:
    Seat** seats;
    int totalRows;
    int totalSeatsPerRow;
public:
    ReservationSystem(int rows, int seatsPerRow) : totalRows(rows), totalSeatsPerRow(seatsPerRow) {
        seats = new Seat*[rows];
        for (int i = 0; i < rows; ++i) {
            seats[i] = new Seat[seatsPerRow];
        }
    }
    ~ReservationSystem() {
        for (int i = 0; i < totalRows; ++i) {
            delete[] seats[i];
        }
        delete[] seats;
    }
    void displaySeats() {
        cout << "Seat Layout: " << endl;
        for (int i = 0; i < totalRows; ++i) {
            cout << "Row " << i + 1 << ": ";
            for (int j = 0; j < totalSeatsPerRow; ++j) {
                cout << (seats[i][j].getStatus() == "Available" ? "[ ]" : "[X]") << " ";
            }
            cout << endl;
        }
    }
    bool isSeatAvailable(int row, int seat) {
        return row >= 0 && row < totalRows && seat >= 0 && seat < totalSeatsPerRow && seats[row][seat].getStatus() == "Available";
    }
    bool reserveSeat(int row, int seat) {
        if (isSeatAvailable(row, seat)) {
            seats[row][seat].bookSeat();
            return true;
        }
        return false;
    }
    void confirmReservation(int row, int seat) {
        if (!isSeatAvailable(row, seat)) {
            cout << "Reservation Confirmed! Seat " << row + 1 << "-" << seat + 1 << " is now booked." << endl;
        } else {
            cout << "Reservation Failed! Seat is not available." << endl;
        }
    }
    void bookSeat() {
        int row, seat;
        cout << "Enter row number (1 to " << totalRows << "): ";
        cin >> row;
        cout << "Enter seat number (1 to " << totalSeatsPerRow << "): ";
        cin >> seat;
        row--; seat--;
        if (row >= 0 && row < totalRows && seat >= 0 && seat < totalSeatsPerRow) {
            if (reserveSeat(row, seat)) {
                confirmReservation(row, seat);
            } else {
                cout << "Sorry, seat " << row + 1 << "-" << seat + 1 << " is already booked." << endl;
            }
        } else {
            cout << "Invalid row or seat number!" << endl;
        }
    }
    void checkAvailability() {
        cout << "Checking real-time availability: " << endl;
        displaySeats();
    }
    friend void displaySystemInfo(const ReservationSystem& system);
};

void displaySystemInfo(const ReservationSystem& system) {
    cout << "Reservation System Info: " << endl;
    cout << "Total Rows: " << system.totalRows << endl;
    cout << "Total Seats Per Row: " << system.totalSeatsPerRow << endl;
}

class Passenger {
private:
    string name;
    bool isBooked;
public:
    Passenger() : isBooked(false) {}
    void setName(string n) { name = n; }
    void bookTicket() { isBooked = true; }
    void cancelTicket() { isBooked = false; }
    string getName() const { return name; }
    bool getBookingStatus() const { return isBooked; }
    friend class PassengerInformationSystem;
};

class PassengerInformationSystem {
private:
    Passenger passengers[MAX_PASSENGERS];
    int passengerCount;
    void notify(const string& message) {
        cout << "Notification: " << message << endl;
    }
public:
    PassengerInformationSystem() : passengerCount(0) {}
    void addPassenger(string name) {
        if (passengerCount < MAX_PASSENGERS) {
            passengers[passengerCount].setName(name);
            passengerCount++;
            notify("Passenger " + name + " added.");
        } else {
            notify("Passenger list is full. Cannot add more passengers.");
        }
    }
    void bookTicket(string name) {
        for (int i = 0; i < passengerCount; ++i) {
            if (passengers[i].getName() == name) {
                if (passengers[i].getBookingStatus()) {
                    notify("Passenger already has a booking.");
                } else {
                    passengers[i].bookTicket();
                    notify("Booking confirmed for " + name + ".");
                }
                return;
            }
        }
        notify("Passenger not found.");
    }
    void cancelTicket(string name) {
        for (int i = 0; i < passengerCount; ++i) {
            if (passengers[i].getName() == name) {
                if (!passengers[i].getBookingStatus()) {
                    notify("No booking found for " + name + ".");
                } else {
                    passengers[i].cancelTicket();
                    notify("Booking canceled for " + name + ".");
                }
                return;
            }
        }
        notify("Passenger not found.");
    }
    void displayPassengers() {
        for (int i = 0; i < passengerCount; ++i) {
            cout << "Name: " << passengers[i].getName();
            cout << ", Status: " << (passengers[i].getBookingStatus() ? "Booked" : "Not Booked") << endl;
        }
    }
    friend void displaySystemInfo(const PassengerInformationSystem& system);
};

void displaySystemInfo(const PassengerInformationSystem& system) {
    cout << "Passenger Information System Info:\n";
    cout << "Total Passengers: " << system.passengerCount << endl;
}

class Train {
private:
    string trainName;
    string route;
    string departureTime;
    string arrivalTime;
    bool isAvailable;
    bool isDelayed;
public:
    void setDetails(string name, string rt, string dep, string arr, bool available, bool delayed) {
        trainName = name;
        route = rt;
        departureTime = dep;
        arrivalTime = arr;
        isAvailable = available;
        isDelayed = delayed;
    }
    void displayTimetable() {
        cout << "Train Name: " << trainName << endl;
        cout << "Route: " << route << endl;
        cout << "Departure Time: " << departureTime << endl;
        cout << "Arrival Time: " << arrivalTime << endl;
    }
    void checkAvailability() {
        cout << "Train Availability: " << (isAvailable ? "Available" : "Not Available") << endl;
    }
    void checkDelayStatus() {
        cout << "Delay Alerts: " << (isDelayed ? "Train is delayed" : "Train is on time") << endl;
    }
};

class Ticket {
private:
    string passengerName;
    string passengerID;
    string passengerType;
    double ticketPrice;
    double finalPrice;
    bool isValidID;
    double calculateDiscount(string type) {
        if (type == "Student") return 0.2;
        else if (type == "Senior") return 0.3;
        else return 0.0;
    }
public:
    Ticket() : ticketPrice(100.0), finalPrice(100.0), isValidID(false) {}
    void inputDetails() {
        cout << "Enter Passenger Name: ";
        cin.ignore();
        getline(cin, passengerName);
        cout << "Enter Passenger ID (must be at least 6 characters): ";
        getline(cin, passengerID);
        cout << "Enter Passenger Type (Regular/Student/Senior): ";
        getline(cin, passengerType);
        isValidID = passengerID.length() >= 6;
        if (isValidID) {
            double discount = calculateDiscount(passengerType);
            finalPrice = ticketPrice - (ticketPrice * discount);
        } else {
            cout << "Invalid Passenger ID! Ticket booking failed.\n";
        }
    }
    void displayTicket() {
        if (isValidID) {
            cout << "\n=== Ticket Information ===" << endl;
            cout << "Passenger Name : " << passengerName << endl;
            cout << "Passenger ID   : " << passengerID << endl;
            cout << "Passenger Type : " << passengerType << endl;
            cout << "Ticket Price   : $" << ticketPrice << endl;
            cout << "Final Price    : $" << finalPrice << " (after discount)" << endl;
        } else {
            cout << "Ticket is not valid due to invalid ID.\n";
        }
    }
    string getPassengerName() const { return passengerName; }
    string getPassengerID() const { return passengerID; }
    bool isIDValid() const { return isValidID; }
    double getFinalPrice() const { return finalPrice; }
};

class TicketManagementSystem {
private:
    Ticket tickets[MAX_PASSENGERS];
    int ticketCount;
public:
    TicketManagementSystem() : ticketCount(0) {}
    void verifyPassenger() {
        string idToVerify;
        cout << "Enter Passenger ID to verify: ";
        cin >> idToVerify;
        bool found = false;
        for (int i = 0; i < ticketCount; ++i) {
            if (tickets[i].getPassengerID() == idToVerify) {
                if (tickets[i].isIDValid()) {
                    cout << "Passenger Verified!" << endl;
                    tickets[i].displayTicket();
                } else {
                    cout << "Invalid ID. Passenger verification failed." << endl;
                }
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Passenger not found.\n";
        }
    }
    void applyDiscountOffer() {
        double discountRate = 0.0;
        string offerCode;
        cout << "Enter Offer Code: ";
        cin >> offerCode;
        if (offerCode == "SUMMER20") {
            discountRate = 0.2;
            cout << "Summer Offer Applied! 20% Discount.\n";
        } else if (offerCode == "WINTER30") {
            discountRate = 0.3;
            cout << "Winter Offer Applied! 30% Discount.\n";
        } else {
            cout << "Invalid Offer Code. No discount applied.\n";
        }
        if (discountRate > 0) {
            for (int i = 0; i < ticketCount; ++i) {
                double newPrice = tickets[i].getFinalPrice() * (1 - discountRate);
                tickets[i].displayTicket();
                cout << "New Price after offer: $" << newPrice << endl;
            }
        }
    }
    void addTicket() {
        if (ticketCount >= MAX_PASSENGERS) {
            cout << "Ticket limit reached! Cannot book more.\n";
            return;
        }
        tickets[ticketCount].inputDetails();
        tickets[ticketCount].displayTicket();
        ticketCount++;
    }
    void ticketMenu() {
        int choice;
        do {
            cout << "\n==== Ticket Management System ====" << endl;
            cout << "1. Add Ticket" << endl;
            cout << "2. Verify Passenger" << endl;
            cout << "3. Apply Discount/Offer" << endl;
            cout << "4. Back to System Menu" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1: addTicket(); break;
                case 2: verifyPassenger(); break;
                case 3: applyDiscountOffer(); break;
                case 4: return;
                default: cout << "Invalid choice! Try again.\n";
            }
        } while (choice != 4);
    }
};

class TrainControl {
public:
    int id;
    string name;
    string currentStation;
    string nextStation;
    double speed;
    bool onTime;
    TrainControl() {}
    TrainControl(int id, string name, string current, string next, double speed, bool onTime)
        : id(id), name(name), currentStation(current), nextStation(next), speed(speed), onTime(onTime) {}
    void displayStatus() {
        cout << left << setw(10) << id
             << setw(20) << name
             << setw(15) << currentStation
             << setw(15) << nextStation
             << setw(10) << speed
             << (onTime ? "On Time" : "Delayed") << endl;
    }
};

class TrainControlSystem {
private:
    TrainControl trains[MAX_TRAINS];
    int trainCount;
public:
    TrainControlSystem() { trainCount = 0; }
    void addTrain(TrainControl train) {
        if (trainCount < MAX_TRAINS) {
            trains[trainCount++] = train;
        } else {
            cout << "Train list is full.\n";
        }
    }
    void updateTrain(int id, string currentStation, string nextStation, double speed, bool onTime) {
        for (int i = 0; i < trainCount; i++) {
            if (trains[i].id == id) {
                trains[i].currentStation = currentStation;
                trains[i].nextStation = nextStation;
                trains[i].speed = speed;
                trains[i].onTime = onTime;
                return;
            }
        }
        cout << "Train with ID " << id << " not found.\n";
    }
    void displayDashboard() {
        cout << "======= Operational Dashboard =======\n";
        cout << left << setw(10) << "Train ID"
             << setw(20) << "Name"
             << setw(15) << "Current"
             << setw(15) << "Next"
             << setw(10) << "Speed"
             << "Status" << endl;
        cout << "----------------------------------------------------------------------------\n";
        for (int i = 0; i < trainCount; i++) {
            trains[i].displayStatus();
        }
    }
    void trainControlMenu() {
        int choice;
        do {
            cout << "\n==== Train Operations and Control ====" << endl;
            cout << "1. Add Train" << endl;
            cout << "2. Update Train" << endl;
            cout << "3. Display Dashboard" << endl;
            cout << "4. Back to System Menu" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1: {
                    int id;
                    string name, current, next;
                    double speed;
                    bool onTime;
                    cout << "Enter Train ID: ";
                    cin >> id;
                    cin.ignore();
                    cout << "Enter Train Name: ";
                    getline(cin, name);
                    cout << "Enter Current Station: ";
                    getline(cin, current);
                    cout << "Enter Next Station: ";
                    getline(cin, next);
                    cout << "Enter Speed: ";
                    cin >> speed;
                    cout << "Is On Time (1 for Yes, 0 for No): ";
                    cin >> onTime;
                    addTrain(TrainControl(id, name, current, next, speed, onTime));
                    break;
                }
                case 2: {
                    int id;
                    string current, next;
                    double speed;
                    bool onTime;
                    cout << "Enter Train ID to Update: ";
                    cin >> id;
                    cin.ignore();
                    cout << "Enter Current Station: ";
                    getline(cin, current);
                    cout << "Enter Next Station: ";
                    getline(cin, next);
                    cout << "Enter Speed: ";
                    cin >> speed;
                    cout << "Is On Time (1 for Yes, 0 for No): ";
                    cin >> onTime;
                    updateTrain(id, current, next, speed, onTime);
                    break;
                }
                case 3: displayDashboard(); break;
                case 4: return;
                default: cout << "Invalid choice! Try again.\n";
            }
        } while (choice != 4);
    }
};

class Invoice {
private:
    string passengerName;
    double amountPaid;
    string bookingDate;
public:
    Invoice() : amountPaid(0.0) {}
    Invoice(string name, double amount) {
        passengerName = name;
        amountPaid = amount;
        time_t now = time(0);
        char* dt = ctime(&now);
        bookingDate = string(dt);
        bookingDate = bookingDate.substr(0, bookingDate.length() - 1);
    }
    void displayInvoice() {
        cout << "\n--- Invoice ---" << endl;
        cout << "Passenger Name: " << passengerName << endl;
        cout << "Amount Paid: $" << amountPaid << endl;
        cout << "Booking Date: " << bookingDate << endl;
        cout << "----------------" << endl;
    }
    bool isValid() const { return !passengerName.empty(); }
};

class PaymentGateway {
public:
    bool processPayment(double amount) {
        cout << "Processing payment of $" << amount << "..." << endl;
        return true;
    }
};

class BookingHistory {
private:
    Invoice invoices[MAX_INVOICES];
    int invoiceCount;
public:
    BookingHistory() : invoiceCount(0) {}
    void addBooking(const string& passengerName, double amount) {
        if (invoiceCount < MAX_INVOICES) {
            invoices[invoiceCount] = Invoice(passengerName, amount);
            invoiceCount++;
        } else {
            cout << "Invoice storage is full.\n";
        }
    }
    void showBookingHistory() {
        if (invoiceCount == 0) {
            cout << "No booking history available." << endl;
        } else {
            cout << "\n--- Booking History ---" << endl;
            for (int i = 0; i < invoiceCount; i++) {
                if (invoices[i].isValid()) {
                    invoices[i].displayInvoice();
                }
            }
        }
    }
};

class TicketSystem {
private:
    PaymentGateway paymentGateway;
    BookingHistory bookingHistory;
public:
    void makePayment(const string& passengerName, double amount) {
        if (paymentGateway.processPayment(amount)) {
            cout << "Payment successful!" << endl;
            bookingHistory.addBooking(passengerName, amount);
        } else {
            cout << "Payment failed. Try again later." << endl;
        }
    }
    void viewBookingHistory() {
        bookingHistory.showBookingHistory();
    }
    void ticketSystemMenu() {
        int choice;
        do {
            cout << "\n==== Payment Gateway Integration ====" << endl;
            cout << "1. Make Payment" << endl;
            cout << "2. View Booking History" << endl;
            cout << "3. Back to System Menu" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1: {
                    string passengerName;
                    double amount;
                    cout << "Enter passenger name: ";
                    cin.ignore();
                    getline(cin, passengerName);
                    cout << "Enter payment amount: $";
                    cin >> amount;
                    makePayment(passengerName, amount);
                    break;
                }
                case 2: viewBookingHistory(); break;
                case 3: return;
                default: cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 3);
    }
};

class Cargo {
public:
    int id;
    string type;
    string source;
    string destination;
    string scheduleTime;
    double weight;

    Cargo() {}

    Cargo(int id, string type, string source, string destination, string time, double weight)
        : id(id), type(type), source(source), destination(destination), scheduleTime(time), weight(weight) {}

    void display() {
        cout << left << setw(10) << id
             << setw(20) << type
             << setw(15) << source
             << setw(15) << destination
             << setw(20) << scheduleTime
             << weight << " tons" << endl;
    }
};

class CargoManager {
private:
    Cargo cargos[MAX_CARGO];
    int count;
public:
    CargoManager() : count(0) {}
    void addCargo(Cargo cargo) {
        if (count < MAX_CARGO) {
            cargos[count++] = cargo;
        } else {
            cout << "Cargo limit reached!" << endl;
        }
    }
    void displaySchedule() {
        cout << "======= Cargo Schedule =======\n";
        cout << left << setw(10) << "ID"
             << setw(20) << "Type"
             << setw(15) << "Source"
             << setw(15) << "Destination"
             << setw(20) << "Scheduled Time"
             << "Weight" << endl;
        cout << "-----------------------------------------------------------------------------------------\n";
        for (int i = 0; i < count; i++) {
            cargos[i].display();
        }
    }
};

class ChatMessage {
public:
    string user;
    string message;
    string timestamp;
    void displayMessage() const {
        cout << "[" << timestamp << "] " << user << ": " << message << endl;
    }
};

class LiveChat {
private:
    ChatMessage chatHistory[MAX_MESSAGES];
    int messageCount;
public:
    LiveChat() : messageCount(0) {}
    void sendMessage(const string& user, const string& message, const string& timestamp) {
        if (messageCount < MAX_MESSAGES) {
            chatHistory[messageCount].user = user;
            chatHistory[messageCount].message = message;
            chatHistory[messageCount].timestamp = timestamp;
            messageCount++;
        } else {

            for (int i = 1; i < MAX_MESSAGES; i++) {
                chatHistory[i - 1] = chatHistory[i];
            }
            chatHistory[MAX_MESSAGES - 1] = {user, message, timestamp};
            cout << "(Chat history full. Oldest message removed.)" << endl;
        }
    }
    void viewChatHistory() const {
        if (messageCount == 0) {
            cout << "No messages in chat history." << endl;
        } else {
            cout << "\n=== Chat History ===" << endl;
            for (int i = 0; i < messageCount; i++) {
                chatHistory[i].displayMessage();
            }
        }
    }
};

class RatingSystem {
private:
    int ratings[6];
public:
    RatingSystem() {
        for (int i = 0; i <= 5; i++) {
            ratings[i] = 0;
        }
    }
    void submitRating(int rating) {
        if (rating < 1 || rating > 5) {
            cout << "Invalid rating! Please enter a rating between 1 and 5." << endl;
            return;
        }
        ratings[rating]++;
        cout << "Thank you for your feedback!" << endl;
    }
    void displayAverageRating() const {
        int total = 0, count = 0;
        for (int i = 1; i <= 5; i++) {
            total += i * ratings[i];
            count += ratings[i];
        }
        if (count == 0) {
            cout << "No ratings submitted yet." << endl;
        } else {
            double average = static_cast<double>(total) / count;
            cout << "Average Rating: " << average << endl;
        }
    }
    void viewAllRatings() const {
        bool hasRating = false;
        for (int i = 1; i <= 5; i++) {
            if (ratings[i] > 0) {
                hasRating = true;
                break;
            }
        }
        if (!hasRating) {
            cout << "No ratings submitted yet." << endl;
        } else {
            cout << "\n=== Ratings Breakdown ===" << endl;
            for (int i = 1; i <= 5; i++) {
                cout << "Rating " << i << ": " << ratings[i] << " vote(s)" << endl;
            }
        }
    }
};

class HelpDesk {
private:
    LiveChat liveChat;
    RatingSystem ratingSystem;
public:
    string getCurrentTime() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%I:%M %p", ltm);
        return string(timeStr);
    }
    void startChat(const string& user) {
        string message;
        cout << "\nStarting chat with customer support...\n(Type 'exit' to end the chat)\n";
        cin.ignore();
        while (true) {
            cout << user << ": ";
            getline(cin, message);
            if (message == "exit") {
                cout << "Chat ended.\n";
                break;
            }
            string timestamp = getCurrentTime();
            liveChat.sendMessage(user, message, timestamp);
        }
        liveChat.viewChatHistory();
    }
    void viewChatHistory() {
        liveChat.viewChatHistory();
    }
    void submitRating() {
        int rating;
        cout << "Enter rating (1-5): ";
        cin >> rating;
        ratingSystem.submitRating(rating);
    }
    void displayAverageRating() {
        ratingSystem.displayAverageRating();
    }
    void viewAllRatings() {
        ratingSystem.viewAllRatings();
    }
    void helpDeskMenu() {
        int choice;
        do {
            cout << "\n==== Customer Support & Feedback ====" << endl;
            cout << "1. Live Chat with Help Desk" << endl;
            cout << "2. View Chat History" << endl;
            cout << "3. Submit Feedback Rating (1-5)" << endl;
            cout << "4. View Average Rating" << endl;
            cout << "5. View All Ratings" << endl;
            cout << "6. Back to System Menu" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1: {
                    string user;
                    cout << "Enter your name: ";
                    cin.ignore();
                    getline(cin, user);
                    startChat(user);
                    break;
                }
                case 2: viewChatHistory(); break;
                case 3: submitRating(); break;
                case 4: displayAverageRating(); break;
                case 5: viewAllRatings(); break;
                case 6: return;
                default: cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 6);
    }
};

class User {
protected:
    string userId;
    string password;
public:
    virtual void login() = 0;
    virtual void registration() = 0;
    virtual void mainMenu() = 0;
};

class RegisteredUser : public User {
private:
    string name, email, contact, address, nidNo;
    map<string, int> failedAttempts;
    void loadFailedAttempts() {
        ifstream input("attempts.txt");
        string id; int attempts;
        while (input >> id >> attempts) failedAttempts[id] = attempts;
        input.close();
    }
    void saveFailedAttempts() {
        ofstream output("attempts.txt");
        for (const auto& entry : failedAttempts) output << entry.first << ' ' << entry.second << endl;
        output.close();
    }
    bool isAccountLocked(const string& id) {
        return failedAttempts[id] >= 3;
    }
    void incrementFailedAttempts(const string& id) {
        failedAttempts[id]++;
        saveFailedAttempts();
    }
    void resetFailedAttempts(const string& id) {
        failedAttempts[id] = 0;
        saveFailedAttempts();
    }
    void trainSchedule() {
        Train trains[3];
        trains[0].setDetails("Express 101", "City A to City B", "08:00 AM", "10:00 AM", true, false);
        trains[1].setDetails("Local 202", "City B to City C", "09:30 AM", "12:00 PM", true, true);
        trains[2].setDetails("Express 303", "City A to City C", "01:00 PM", "03:30 PM", false, false);
        cout << "\nTrain Schedule Information" << endl;
        for (int i = 0; i < 3; ++i) {
            cout << "-------------------------\n";
            trains[i].displayTimetable();
            trains[i].checkAvailability();
            trains[i].checkDelayStatus();
        }
    }
    void reservationSystemMenu() {
        ReservationSystem system(ROWS, SEATS_PER_ROW);
        while (true) {
            int choice;
            cout << "\n==== Passenger Reservation System ====\n1. Book a Seat\n2. Check Seat Availability\n3. Display System Info\n4. Train Timetables\n5. Return to Main Menu\nChoice: ";
            cin >> choice;
            switch (choice) {
                case 1: system.bookSeat(); break;
                case 2: system.checkAvailability(); break;
                case 3: displaySystemInfo(system); break;
                case 4: trainSchedule(); break;
                case 5: return;
                default: cout << "Invalid choice! Try again.\n";
            }
        }
    }
    void passengerInfoSystemMenu() {
        PassengerInformationSystem pis;
        while (true) {
            int choice;
            cout << "\n==== Passenger Info System ====\n1. Add Passenger\n2. Book Ticket\n3. Cancel Ticket\n4. Display Passengers\n5. Display System Info\n6. Return to Main Menu\nChoice: ";
            cin >> choice;
            string name;
            switch (choice) {
                case 1: cout << "Enter name: "; cin.ignore(); getline(cin, name); pis.addPassenger(name); break;
                case 2: cout << "Enter name: "; cin.ignore(); getline(cin, name); pis.bookTicket(name); break;
                case 3: cout << "Enter name: "; cin.ignore(); getline(cin, name); pis.cancelTicket(name); break;
                case 4: pis.displayPassengers(); break;
                case 5: displaySystemInfo(pis); break;
                case 6: return;
                default: cout << "Invalid choice! Try again.\n";
            }
        }
    }
    void cargoSystemMenu() {
        CargoManager manager;
        manager.addCargo(Cargo(301, "Textile Goods", "Dhaka", "Chattogram", "2025-04-18 09:00", 12.5));
        manager.addCargo(Cargo(302, "Cement Bags", "Khulna", "Sylhet", "2025-04-20 14:00", 20.0));
        manager.addCargo(Cargo(303, "Frozen Food", "Rajshahi", "Dhaka", "2025-04-21 07:30", 8.75));
        int choice;
        do {
            cout << "\n==== Cargo & Freight Management ====" << endl;
            cout << "1. Add Cargo" << endl;
            cout << "2. Display Cargo Schedule" << endl;
            cout << "3. Back to System Menu" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1: {
                    int id;
                    string type, source, destination, scheduleTime;
                    double weight;
                    cout << "Enter Cargo ID: ";
                    cin >> id;
                    cin.ignore();
                    cout << "Enter Cargo Type: ";
                    getline(cin, type);
                    cout << "Enter Source: ";
                    getline(cin, source);
                    cout << "Enter Destination: ";
                    getline(cin, destination);
                    cout << "Enter Scheduled Time (e.g., YYYY-MM-DD HH:MM): ";
                    getline(cin, scheduleTime);
                    cout << "Enter Weight (tons): ";
                    cin >> weight;
                    manager.addCargo(Cargo(id, type, source, destination, scheduleTime, weight));
                    cout << "Cargo added successfully!" << endl;
                    break;
                }
                case 2:
                    manager.displaySchedule();
                    break;
                case 3:
                    return;
                default:
                    cout << "Invalid choice! Try again.\n";
            }
        } while (choice != 3);
    }
public:
    RegisteredUser() { loadFailedAttempts(); }
    void login() override {
        cout << "\t\t\tUsername: "; cin >> userId;
        cout << "\t\t\tPassword: "; cin >> password;
        if (isAccountLocked(userId)) {
            cout << "\nYour account is locked.\n"; mainMenu(); return;
        }
        ifstream input("record.txt");
        string temp, idIn, passIn;
        bool found = false;
        while (input >> idIn >> passIn) {
            getline(input, temp);
            if (idIn == userId && passIn == password) { found = true; break; }
        }
        input.close();
        if (found) {
            resetFailedAttempts(userId);
            cout << userId << "\nLogin successful!\n";
            systemMenu();
        } else {
            incrementFailedAttempts(userId);
            cout << "Login failed.\n";
        }
        mainMenu();
    }
    void registration() override {
        string confirmPassword;
        cout << "\t\t\tEnter your Name: "; cin.ignore(); getline(cin, name);
        cout << "\t\t\tEmail: "; getline(cin, email);
        cout << "\t\t\tContact: "; getline(cin, contact);
        cout << "\t\t\tAddress: "; getline(cin, address);
        cout << "\t\t\tNID No.: "; getline(cin, nidNo);
        cout << "\t\t\tUsername: "; cin >> userId;
        do {
            cout << "\t\t\tPassword: "; cin >> password;
            cout << "\t\t\tConfirm Password: "; cin >> confirmPassword;
            if (password != confirmPassword) cout << "Passwords do not match!\n";
        } while (password != confirmPassword);
        ofstream f1("record.txt", ios::app);
        f1 << userId << ' ' << password << ' ' << name << ' ' << email << ' ' << contact << ' ' << address << ' ' << nidNo << endl;
        f1.close();
        cout << "\nRegistration successful!\n";
        mainMenu();
    }
    void systemMenu() {
        TicketManagementSystem tms;
        TrainControlSystem tcs;
        TicketSystem ts;
        HelpDesk hd;
        tcs.addTrain(TrainControl(101, "Shundarban Express", "Tejgaon", "Joydevpur", 90.5, true));
        tcs.addTrain(TrainControl(102, "Ekota Express", "Komolapur", "Airport", 60.0, false));
        tcs.addTrain(TrainControl(103, "Titash commuter", "CTG", "Cox's Bazar", 45.0, true));
        while (true) {
            int choice;
            cout << "\n====== Select System ======\n"
                 << "1. Passenger Reservation System\n"
                 << "2. Train Scheduling and Management\n"
                 << "3. Ticket Management System\n"
                 << "4. Train Operations and Control\n"
                 << "5. Passenger Information System\n"
                 << "6. Payment Gateway Integration\n"
                 << "7. Customer Support & Feedback\n"
                 << "8. Cargo & Freight Management\n"
                 << "9. Return to Main Menu\n"
                 << "Choice: ";
            cin >> choice;
            switch (choice) {
                case 1: reservationSystemMenu(); break;
                case 2: trainSchedule(); break;
                case 3: tms.ticketMenu(); break;
                case 4: tcs.trainControlMenu(); break;
                case 5: passengerInfoSystemMenu(); break;
                case 6: ts.ticketSystemMenu(); break;
                case 7: hd.helpDeskMenu(); break;
                case 8: cargoSystemMenu(); break;
                case 9: return;
                default: cout << "Invalid choice! Try again.\n";
            }
        }
    }
    void mainMenu() override {
        int c;
        cout << "\t\t\t<====================================>\n\n\n";
        cout << "\t\t\t         Welcome to Login Page        \n\n\n";
        cout << "\t\t\t<=============== Menu ===============>\n\n\n";
        cout << endl;
        cout << "\t\t\t| Press 1 To Login    |" << endl;
        cout << "\t\t\t| Press 2 To Register |" << endl;
        cout << "\t\t\t| Press 3 To Exit     |" << endl;
        cout << "\n\t\t\t Please enter your choice : ";
        cin >> c;
        cout << endl;
        switch (c) {
            case 1: login();
            break;
            case 2: registration();
            break;
            case 3: cout << "Thank you!\n";
            break;
            default: cout << "Invalid choice! Try again.\n";
            mainMenu();
        }
    }
};

int main() {
    RegisteredUser user;
    user.mainMenu();
    return 0;
}

