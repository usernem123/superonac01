#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // For std::sort, std::lower_bound
#include <iomanip>   // For std::setw, std::left, std::right
#include <limits>    // For numeric_limits
#include <map>       // For std::map
#include <set>       // To help with unique department listing and date mapping

using namespace std;

// --- Existing Event Structure ---
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

// --- Segment Tree Related Structures and Global Variables ---
// Map to convert date strings to numerical indices for the segment tree
map<string, int> dateToIndexMap;
// Vector to convert numerical indices back to date strings
vector<string> indexToDateMap;
// The base array for the segment tree, storing event counts per date index
vector<int> dateEventCounts;
// The segment tree itself
vector<int> segmentTree;

// --- Segment Tree Class/Functions ---
// This will be a simple segment tree for sum queries (counting events)

// Function to build the segment tree
// `node`: current node index in segmentTree vector
// `start`: starting index of the current segment in dateEventCounts
// `end`: ending index of the current segment in dateEventCounts
void buildSegmentTree(int node, int start, int end) {
    if (start == end) {
        // Leaf node, store the count for this single date
        segmentTree[node] = dateEventCounts[start];
    } else {
        int mid = (start + end) / 2;
        // Recursively build left and right children
        buildSegmentTree(2 * node, start, mid);
        buildSegmentTree(2 * node + 1, mid + 1, end);
        // Current node stores the sum of its children
        segmentTree[node] = segmentTree[2 * node] + segmentTree[2 * node + 1];
    }
}

// Function to update a value in the segment tree
// `node`: current node index in segmentTree vector
// `start`: starting index of the current segment
// `end`: ending index of the current segment
// `idx`: index in dateEventCounts to update
// `val`: value to add/subtract (e.g., 1 for add, -1 for remove)
void updateSegmentTree(int node, int start, int end, int idx, int val) {
    if (start == end) {
        // Leaf node, update the count directly
        segmentTree[node] += val;
    } else {
        int mid = (start + end) / 2;
        if (start <= idx && idx <= mid) {
            // idx is in the left child's range
            updateSegmentTree(2 * node, start, mid, idx, val);
        } else {
            // idx is in the right child's range
            updateSegmentTree(2 * node + 1, mid + 1, end, idx, val);
        }
        // Update current node by summing children
        segmentTree[node] = segmentTree[2 * node] + segmentTree[2 * node + 1];
    }
}

// Function to query the sum (count) in a given range
// `node`: current node index
// `start`: starting index of current segment
// `end`: ending index of current segment
// `l`: left boundary of query range
// `r`: right boundary of query range
int querySegmentTree(int node, int start, int end, int l, int r) {
    // Query range outside current segment
    if (r < start || end < l) {
        return 0; // No overlap
    }
    // Query range completely covers current segment
    if (l <= start && end <= r) {
        return segmentTree[node];
    }
    // Partial overlap, recurse on children
    int mid = (start + end) / 2;
    int p1 = querySegmentTree(2 * node, start, mid, l, r);
    int p2 = querySegmentTree(2 * node + 1, mid + 1, end, l, r);
    return p1 + p2;
}

// --- Helper Functions for Segment Tree Management ---

// Updates the date mapping and prepares the base array for the segment tree
void prepareDateDataForSegmentTree() {
    set<string> uniqueDates;
    for (const auto& event : events) {
        uniqueDates.insert(event.date);
    }

    dateToIndexMap.clear();
    indexToDateMap.clear();
    int index = 0;
    for (const string& date : uniqueDates) {
        dateToIndexMap[date] = index;
        indexToDateMap.push_back(date);
        index++;
    }

    // Initialize dateEventCounts based on current events
    dateEventCounts.assign(indexToDateMap.size(), 0);
    for (const auto& event : events) {
        if (dateToIndexMap.count(event.date)) { // Ensure date is mapped
            dateEventCounts[dateToIndexMap[event.date]]++;
        }
    }
}

// Rebuilds the entire segment tree. Call this when dates change or many events are added/removed.
void rebuildSegmentTree() {
    prepareDateDataForSegmentTree(); // Update date mapping and base counts first

    if (dateEventCounts.empty()) {
        segmentTree.assign(1, 0); // No dates, tree is empty
        return;
    }

    int n = dateEventCounts.size();
    // Segment tree array size is usually 4*N for safety
    segmentTree.assign(4 * n, 0);
    buildSegmentTree(1, 0, n - 1);
}

// --- Existing Helper Functions ---

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
    // Crucially, rebuild the segment tree data structures after event changes
    rebuildSegmentTree();
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

    // Update secondary data structures, which now also rebuilds the segment tree
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

// --- New Function: Query Events by Date Range (using Segment Tree) ---
void queryEventsByDateRange() {
    cout << "\n" << string(45, '*') << endl;
    cout << center("* --- Count UEvents by Date Range --- *", 45) << endl;
    cout << string(45, '*') << endl;

    if (indexToDateMap.empty()) {
        cout << "No events available to query by date. 😔" << endl;
        cout << string(45, '*') << endl;
        return;
    }

    cout << setw(30) << left << "| Enter Start Date (YYYY-MM-DD):";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string startDateStr;
    getline(cin, startDateStr);

    cout << setw(30) << left << "| Enter End Date (YYYY-MM-DD):";
    string endDateStr;
    getline(cin, endDateStr);
    cout << string(45, '*') << endl;

    // Convert date strings to indices
    // We need to find the lower bound for startDateStr and upper bound for endDateStr
    // in the sorted `indexToDateMap` to get the correct range of indices.
    
    // Find the actual start and end indices in the sorted date list
    auto it_start = lower_bound(indexToDateMap.begin(), indexToDateMap.end(), startDateStr);
    auto it_end = upper_bound(indexToDateMap.begin(), indexToDateMap.end(), endDateStr);

    int startIdx = (it_start == indexToDateMap.end()) ? -1 : distance(indexToDateMap.begin(), it_start);
    // it_end points to the element *after* the last one that matches endDateStr
    int endIdx = (it_end == indexToDateMap.begin()) ? -1 : distance(indexToDateMap.begin(), it_end) -1; 
    
    if (startIdx == -1 || endIdx == -1 || startIdx > endIdx) {
        cout << "\nNo valid date range or events found in the specified range. 😔" << endl;
        return;
    }

    if (startIdx >= indexToDateMap.size() || endIdx < 0) { // Safety check
        cout << "\nError: Invalid date range mapping. 😔" << endl;
        return;
    }

    // Perform the segment tree query
    int numDates = indexToDateMap.size();
    if (numDates == 0) { // If no dates are mapped, there are no events
        cout << "\nTotal UEvents in range [" << startDateStr << " to " << endDateStr << "]: 0" << endl;
        return;
    }

    int eventCount = querySegmentTree(1, 0, numDates - 1, startIdx, endIdx);

    cout << "\nTotal UEvents in range [" << startDateStr << " to " << endDateStr << "]: " << eventCount << " ✨" << endl;
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
    cout << "  [6] 📅 View UEvents Sorted by Date (Bubble Sort)\n";
    cout << "  [7] 🔎 Search UEvents by Date (Linear Search)\n";
    cout << "  [8] 📊 Count UEvents by Date Range (Segment Tree)\n"; // New Segment Tree option
    cout << "  [9] 🚪 Exit\n"; // Exit is now 9
    cout << "  " << string(45, '-') << "\n";
    cout << "  ➡️ Enter your choice: ";
}

int main() {
    // Initial update of secondary data structures and segment tree
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
            case 8: // New case for Segment Tree
                queryEventsByDateRange();
                break;
            case 9: // Exit option changed
                cout << "\n👋 Exiting UEvent Organizer. Have a great day! 👋\n";
                break;
            default:
                cout << "\n⚠️ Invalid choice. Please try again. ⚠️\n";
        }
    } while (choice != 9); // Loop condition changed

    return 0;
}