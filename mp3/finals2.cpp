#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // For std::sort, std::lower_bound
#include <iomanip>   // For std::setw, std::left, std::right
#include <limits>    // For numeric_limits
#include <map>       // For std::map
#include <set>       // To help with unique department listing if needed

using namespace std;

// Structure to represent a school event
struct Event {
    int id;
    string name;
    string date;
    string time;
    string location;
    string department;
    int capacity;
    int participants;

    // Overload the less than operator for sorting by name (for main events vector)
    bool operator<(const Event& other) const {
        return name < other.name; // Sort by event name
    }
};

// Global vector to store all events (primary storage, sorted by name)
vector<Event> events;

// Secondary data structures for efficient lookups and demonstrating other DS
// Map event name to a pointer to the event in the 'events' vector
map<string, Event*> eventNameMap;
// Map department name to a vector of pointers to events in that department
map<string, vector<Event*>> eventsByDepartment;

// Simple function to center a string within a given width
string center(const string& s, int w) {
    int padding = (w - s.length()) / 2;
    return string(padding, ' ') + s + string(w - s.length() - padding, ' ');
}

// Function to update secondary data structures after adding/modifying events
void updateSecondaryDataStructures() {
    eventNameMap.clear();
    eventsByDepartment.clear();

    for (auto& event : events) {
        eventNameMap[event.name] = &event; // Store pointer to event
        eventsByDepartment[event.department].push_back(&event); // Group by department
    }
}

// Function to add a new event
void addEvent() {
    cout << "\n" << string(45, '*') << endl;
    cout << center("* --- Adding a New UEvent --- *", 45) << endl;
    cout << string(45, '*') << endl;
    Event newEvent;
    newEvent.id = events.empty() ? 1 : events.back().id + 1; // Simple ID generation
    cout << setw(25) << left << "| Event Name:";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
    getline(cin, newEvent.name);

    // Check if event name already exists
    if (eventNameMap.count(newEvent.name)) {
        cout << "\n⚠️ UEvent with name '" << newEvent.name << "' already exists. Please choose a different name. ⚠️" << endl;
        return;
    }

    cout << setw(25) << left << "| Date (YYYY-MM-DD):";
    cin >> newEvent.date;
    cout << setw(25) << left << "| Time (HH:MM):";
    cin >> newEvent.time;
    cout << setw(25) << left << "| Location:";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, newEvent.location);
    cout << setw(25) << left << "| Department:";
    getline(cin, newEvent.department);
    cout << setw(25) << left << "| Capacity:";
    cin >> newEvent.capacity;
    newEvent.participants = 0;
    cout << string(45, '*') << endl;

    events.push_back(newEvent);
    sort(events.begin(), events.end()); // Maintain sorted order for binary search (used by `lower_bound` internally if we still needed it for some cases)

    // Update secondary data structures
    updateSecondaryDataStructures();

    cout << "\nUEvent '" << newEvent.name << "' added successfully! ✨" << endl;
}

// Function to display events (reusable)
void displayEventsList(const vector<Event*>& eventList, const string& title) {
    cout << "\n" << string(99, '=') << endl;
    cout << center("✨ --- " + title + " --- ✨", 99) << endl;
    cout << string(99, '=') << endl;
    if (eventList.empty()) {
        cout << center("No UEvents available. 😔", 99) << endl;
        cout << string(99, '=') << endl;
        return;
    }
    cout << setw(5) << left << "ID" << " | "
         << setw(20) << left << "Name" << " | "
         << setw(12) << left << "Date" << " | "
         << setw(8) << left << "Time" << " | "
         << setw(15) << left << "Location" << " | "
         << setw(15) << left << "Department" << " | "
         << setw(10) << right << "Capacity" << " | "
         << setw(12) << right << "Participants" << endl;
    cout << string(99, '-') << endl;
    for (const auto& eventPtr : eventList) {
        const auto& event = *eventPtr; // Dereference pointer
        cout << setw(5) << left << event.id << " | "
             << setw(20) << left << event.name << " | "
             << setw(12) << left << event.date << " | "
             << setw(8) << left << event.time << " | "
             << setw(15) << left << event.location << " | "
             << setw(15) << left << event.department << " | "
             << setw(10) << right << event.capacity << " | "
             << setw(12) << right << event.participants << endl;
    }
    cout << string(99, '=') << endl;
    cout << endl;
}


// Function to display all events (using primary sorted vector)
void displayAllEvents() {
    vector<Event*> allEventsPtrs;
    for(auto& event : events) {
        allEventsPtrs.push_back(&event);
    }
    displayEventsList(allEventsPtrs, "All UEvents");
}

// Function to search for an event by name (using std::map for efficiency)
void searchEvent() {
    cout << "\n" << string(45, '*') << endl;
    cout << center("* --- Search for a UEvent --- *", 45) << endl;
    cout << string(45, '*') << endl;
    cout << setw(30) << left << "| Enter UEvent name to search:";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string searchName;
    getline(cin, searchName);
    cout << string(45, '*') << endl;

    auto it = eventNameMap.find(searchName); // O(log N) search
    if (it != eventNameMap.end()) {
        cout << "\n✨ UEvent Found! ✨" << endl;
        vector<Event*> foundEvent = {it->second}; // Pass as a vector of one pointer
        displayEventsList(foundEvent, "Search Result");
    } else {
        cout << "\nUEvent '" << searchName << "' not found. 😔" << endl;
    }
    cout << endl;
}

// Function to register a participant for an event (uses eventNameMap for lookup)
void registerParticipant() {
    cout << "\n" << string(45, '*') << endl;
    cout << center("* --- Register for a UEvent --- *", 45) << endl;
    cout << string(45, '*') << endl;
    cout << setw(30) << left << "| Enter the name of the UEvent:";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string eventName;
    getline(cin, eventName);
    cout << string(45, '*') << endl;

    auto it = eventNameMap.find(eventName); // O(log N) lookup
    if (it != eventNameMap.end()) {
        Event* eventPtr = it->second;
        if (eventPtr->participants < eventPtr->capacity) {
            eventPtr->participants++;
            cout << "🎉 Successfully registered for '" << eventPtr->name << "'! 🎉" << endl;
        } else {
            cout << "⚠️ UEvent '" << eventPtr->name << "' is already full. 😟" << endl;
        }
    } else {
        cout << "UEvent '" << eventName << "' not found. 😔" << endl;
    }
    cout << endl;
}

// Function to display events based on a specific department (uses eventsByDepartment map)
void displayEventsByDepartment() {
    cout << "\n" << string(45, '*') << endl;
    cout << center("* --- UEvents by Department --- *", 45) << endl;
    cout << string(45, '*') << endl;
    cout << setw(30) << left << "| Enter the department to filter by:";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string filterDepartment;
    getline(cin, filterDepartment);
    cout << string(45, '*') << endl;

    // Use a set to collect unique matching departments for cleaner display
    set<string> matchingDepartments;
    for (const auto& pair : eventsByDepartment) {
        if (pair.first.find(filterDepartment) != string::npos) { // Partial match
            matchingDepartments.insert(pair.first);
        }
    }

    if (matchingDepartments.empty()) {
        cout << "No UEvents found with department containing '" << filterDepartment << "'. 😔" << endl;
    } else {
        vector<Event*> filteredEvents;
        for (const string& dept : matchingDepartments) {
            // Append all events from this matching department
            for(Event* eventPtr : eventsByDepartment[dept]) {
                filteredEvents.push_back(eventPtr);
            }
        }
        // It's good practice to sort results for consistent display, even if collected from map
        sort(filteredEvents.begin(), filteredEvents.end(), [](Event* a, Event* b) {
            return a->name < b->name;
        });
        displayEventsList(filteredEvents, "UEvents with department containing '" + filterDepartment + "'");
    }
    cout << endl;
}

// Bubble Sort implementation (second sorting algorithm)
void bubbleSortEventsByDate(vector<Event*>& eventList) {
    int n = eventList.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (eventList[j]->date > eventList[j+1]->date) {
                swap(eventList[j], eventList[j+1]);
            }
        }
    }
}

// Function to display events sorted by date using Bubble Sort
void displayEventsSortedByDate() {
    if (events.empty()) {
        cout << "\n" << string(99, '=') << endl;
        cout << center("No UEvents available to sort. 😔", 99) << endl;
        cout << string(99, '=') << endl;
        return;
    }

    // Create a copy of pointers to events to sort, so we don't alter the main 'events' vector
    vector<Event*> eventsCopy;
    for(auto& event : events) {
        eventsCopy.push_back(&event);
    }

    
    // Apply Bubble Sort
    bubbleSortEventsByDate(eventsCopy);

    displayEventsList(eventsCopy, "UEvents Sorted by Date (Bubble Sort)");
}

// Linear Search implementation (second searching algorithm)
void searchEventsByDate() {
    cout << "\n" << string(45, '*') << endl;
    cout << center("* --- Search UEvents by Date --- *", 45) << endl;
    cout << string(45, '*') << endl;
    cout << setw(30) << left << "| Enter date (YYYY-MM-DD) to search:";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string searchDate;
    getline(cin, searchDate);
    cout << string(45, '*') << endl;

    vector<Event*> foundEvents;
    // Linear search through all events
    for (auto& event : events) {
        if (event.date == searchDate) {
            foundEvents.push_back(&event);
        }
    }

    if (foundEvents.empty()) {
        cout << "\nNo UEvents found on '" << searchDate << "'. 😔" << endl;
    } else {
        cout << "\n✨ UEvents found on '" << searchDate << "'! ✨" << endl;
        // Sort results by name for consistent display
        sort(foundEvents.begin(), foundEvents.end(), [](Event* a, Event* b) {
            return a->name < b->name;
        });
        displayEventsList(foundEvents, "UEvents on " + searchDate);
    }
    cout << endl;
}


// Creative Terminal Interface - UEvent Organizer
void displayMenu() {
    cout << "\n";
    cout << "  ╔═════════════════════════════════════════════════╗\n";
    cout << "  ║                                                 ║\n";
    cout << "  ║            ██████╗ ███████╗ ██████╗             ║\n";
    cout << "  ║           ██╔════╝ ██╔════╝██╔═══██╗            ║\n";
    cout << "  ║           ██║  ███╗███████╗██║   ██║            ║\n";
    cout << "  ║           ██║  ██╔╝╚════██║██║   ██║            ║\n";
    cout << "  ║           ╚██████║ ███████║╚██████╔╝            ║\n";
    cout << "  ║            ╚═════╝ ╚══════╝ ╚═════╝             ║\n";
    cout << "  ║                                                 ║\n";
    cout << "  ║             ✨ UEvent Organizer ✨              ║\n";
    cout << "  ║                                                 ║\n";
    cout << "  ╚═════════════════════════════════════════════════╝\n";
    cout << "  [1] ➕ Add New UEvent\n";
    cout << "  [2] 📜 View All UEvents (Sorted by Name)\n";
    cout << "  [3] 🔍 Search UEvent by Name (Map Lookup)\n";
    cout << "  [4] ✍️ Register for a UEvent\n";
    cout << "  [5] 🏷️ View UEvents by Department (Map Lookup)\n";
    cout << "  [6] 📅 View UEvents Sorted by Date (Bubble Sort)\n"; // New option for 2nd sort
    cout << "  [7] 🔎 Search UEvents by Date (Linear Search)\n";   // New option for 2nd search
    cout << "  [8] 🚪 Exit\n"; // Exit is now 8
    cout << "  " << string(45, '-') << "\n";
    cout << "  ➡️ Enter your choice: ";
}

int main() {
    // Initial update of secondary data structures in case there were pre-existing events
    // (though in this specific 'main' function, events starts empty)
    updateSecondaryDataStructures();

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                addEvent();
                break;
            case 2:
                displayAllEvents();
                break;
            case 3:
                searchEvent();
                break;
            case 4:
                registerParticipant();
                break;
            case 5:
                displayEventsByDepartment();
                break;
            case 6: // New case for Bubble Sort
                displayEventsSortedByDate();
                break;
            case 7: // New case for Linear Search
                searchEventsByDate();
                break;
            case 8: // Exit option changed
                cout << "\n👋 Exiting UEvent Organizer. Have a great day! 👋\n";
                break;
            default:
                cout << "\n⚠️ Invalid choice. Please try again. ⚠️\n";
        }
    } while (choice != 8); // Loop condition changed

    return 0;
}