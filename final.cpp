#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // For std::sort, std::lower_bound, std::upper_bound
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
    // This allows std::sort to work directly on the events vector by name.
    bool operator<(const Event& other) const {
        return name < other.name; // Sort by event name
    }
};

// Global vector to store all events (primary storage, sorted by name)
vector<Event> events;

// Secondary data structures for efficient lookups.
// These demonstrate proper use of different data structures for specific purposes.
// Map event name to a pointer to the event in the 'events' vector for O(log N) name lookup.
map<string, Event*> eventNameMap;
// Map department name to a vector of pointers to events in that department for O(log N) department lookup.
map<string, vector<Event*>> eventsByDepartment;

// --- Segment Tree Related Structures and Global Variables ---
// Map to convert date strings to numerical indices for the segment tree.
// Ensures O(log K) lookup where K is number of unique dates.
map<string, int> dateToIndexMap;
// Vector to convert numerical indices back to date strings.
// Kept sorted to allow binary search (lower_bound/upper_bound) for range queries.
vector<string> indexToDateMap;
// The base array for the segment tree, storing event counts per date index.
// dateEventCounts[i] holds the number of events on the date represented by index i.
vector<int> dateEventCounts;
// The segment tree itself. Stores pre-computed sums for ranges of dateEventCounts.
// Its size is typically 4 * N (where N is dateEventCounts.size()) for ample space.
vector<int> segmentTree;

// --- Segment Tree Functions ---
// These functions implement a basic segment tree for sum queries (counting events).

// Function to build the segment tree recursively.
// node: current node's index in the segmentTree vector.
// start: starting index of the current segment in dateEventCounts.
// end: ending index of the current segment in dateEventCounts.
void buildSegmentTree(int node, int start, int end) {
    if (start == end) {
        // Leaf node: stores the count for a single date.
        segmentTree[node] = dateEventCounts[start];
    } else {
        int mid = (start + end) / 2;
        // Recursively build left and right children.
        buildSegmentTree(2 * node, start, mid);       // Left child: 2*node
        buildSegmentTree(2 * node + 1, mid + 1, end); // Right child: 2*node + 1
        // Current node stores the sum of its children's values.
        segmentTree[node] = segmentTree[2 * node] + segmentTree[2 * node + 1];
    }
}

// Function to query the sum (count) of events in a given date index range [l, r].
// node: current node's index in segmentTree.
// start: starting index of the current segment covered by 'node'.
// end: ending index of the current segment covered by 'node'.
// l: left boundary of the query range.
// r: right boundary of the query range.
// Returns the total count of events in the queried date range.
int querySegmentTree(int node, int start, int end, int l, int r) {
    // Case 1: Query range is completely outside the current segment.
    // No overlap, so return 0.
    if (r < start || end < l) {
        return 0;
    }
    // Case 2: Query range completely covers the current segment.
    // Return the pre-computed sum stored in the current node.
    if (l <= start && end <= r) {
        return segmentTree[node];
    }
    // Case 3: Partial overlap. Recurse on children and sum their results.
    int mid = (start + end) / 2;
    int p1 = querySegmentTree(2 * node, start, mid, l, r);       // Query left child
    int p2 = querySegmentTree(2 * node + 1, mid + 1, end, l, r); // Query right child
    return p1 + p2;
}

// --- Helper Functions for Segment Tree Management ---

// Gathers all unique dates from existing events, maps them to numerical indices,
// and populates the base array (dateEventCounts) for the segment tree.
void prepareDateDataForSegmentTree() {
    set<string> uniqueDates; // Use a set to automatically handle unique and sorted dates
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

    // Initialize dateEventCounts based on current events and their new indices.
    // This array will be the input for building the segment tree.
    dateEventCounts.assign(indexToDateMap.size(), 0);
    for (const auto& event : events) {
        // Ensure the date is valid and mapped before incrementing count.
        if (dateToIndexMap.count(event.date)) {
            dateEventCounts[dateToIndexMap[event.date]]++;
        }
    }
}

// Rebuilds the entire segment tree.
// This is called when the underlying set of unique dates might have changed
// (e.g., when adding a new event with a previously unseen date).
void rebuildSegmentTree() {
    prepareDateDataForSegmentTree(); // Update date mapping and base counts first

    if (dateEventCounts.empty()) {
        segmentTree.assign(1, 0); // No dates, tree is effectively empty (size 1, value 0)
        return;
    }

    int n = dateEventCounts.size();
    segmentTree.assign(4 * n, 0); // Allocate enough space for the segment tree
    buildSegmentTree(1, 0, n - 1); // Build from the root node (1) covering the full range
}

// --- Existing General Helper Functions ---

// Simple function to center a string within a given width.
string center(const string& s, int w) {
    if (s.length() >= w) return s;
    int padding = (w - s.length()) / 2;
    return string(padding, ' ') + s + string(w - s.length() - padding, ' ');
}

// Function to update secondary data structures after adding/modifying events.
// This is a central point for maintaining consistency across all data structures.
void updateSecondaryDataStructures() {
    // Clear and repopulate maps for event name and department lookup.
    eventNameMap.clear();
    eventsByDepartment.clear();

    for (auto& event : events) {
        eventNameMap[event.name] = &event;
        eventsByDepartment[event.department].push_back(&event);
    }
    // Crucially, rebuild the segment tree data structures after event changes.
    // This handles cases where new dates are introduced or existing dates gain/lose events.
    rebuildSegmentTree();
}

// Function to add a new event.
void addEvent() {
    cout << "\n" << string(45, '*') << endl;
    cout << center("* --- Adding a New UEvent --- *", 45) << endl;
    cout << string(45, '*') << endl;
    Event newEvent;
    // Simple ID generation: 1 if no events, or last event's ID + 1.
    newEvent.id = events.empty() ? 1 : events.back().id + 1;
    cout << setw(25) << left << "| Event Name:";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer for getline
    getline(cin, newEvent.name);

    // Check if event name already exists using map for efficiency.
    if (eventNameMap.count(newEvent.name)) {
        cout << "\n⚠️ UEvent with name '" << newEvent.name << "' already exists. Please choose a different name. ⚠️" << endl;
        return;
    }

    cout << setw(25) << left << "| Date (YYYY-MM-DD):";
    cin >> newEvent.date;
    cout << setw(25) << left << "| Time (HH:MM):";
    cin >> newEvent.time;
    cout << setw(25) << left << "| Location:";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer for getline
    getline(cin, newEvent.location);
    cout << setw(25) << left << "| Department:";
    getline(cin, newEvent.department);
    cout << setw(25) << left << "| Capacity:";
    // Input validation for capacity
    while (!(cin >> newEvent.capacity) || newEvent.capacity <= 0) {
        cout << "Invalid capacity. Please enter a positive integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    newEvent.participants = 0; // New events start with 0 participants.
    cout << string(45, '*') << endl;

    events.push_back(newEvent);
    // Maintain sorted order of the primary 'events' vector by name.
    sort(events.begin(), events.end());

    // Update all secondary data structures and the segment tree.
    updateSecondaryDataStructures();

    cout << "\nUEvent '" << newEvent.name << "' added successfully! ✨" << endl;
}

// Function to display events (reusable for different lists of events).
void displayEventsList(const vector<Event*>& eventList, const string& title) {
    cout << "\n" << string(99, '=') << endl;
    cout << center("✨ --- " + title + " --- ✨", 99) << endl;
    cout << string(99, '=') << endl;
    if (eventList.empty()) {
        cout << center("No UEvents available. 😔", 99) << endl;
        cout << string(99, '=') << endl;
        return;
    }
    // Table header formatting.
    cout << setw(5) << left << "ID" << " | "
         << setw(20) << left << "Name" << " | "
         << setw(12) << left << "Date" << " | "
         << setw(8) << left << "Time" << " | "
         << setw(15) << left << "Location" << " | "
         << setw(15) << left << "Department" << " | "
         << setw(10) << right << "Capacity" << " | "
         << setw(12) << right << "Participants" << endl;
    cout << string(99, '-') << endl;
    // Iterate and display each event using pointers.
    for (const auto& eventPtr : eventList) {
        const auto& event = *eventPtr; // Dereference pointer to access Event members.
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

// Function to display all events, leveraging the primary sorted vector.
void displayAllEvents() {
    vector<Event*> allEventsPtrs;
    // Populate a vector of pointers to display all events.
    for(auto& event : events) {
        allEventsPtrs.push_back(&event);
    }
    displayEventsList(allEventsPtrs, "All UEvents (Sorted by Name)");
}

// Function to search for an event by name using std::map for efficient O(log N) search.
void searchEvent() {
    cout << "\n" << string(45, '*') << endl;
    cout << center("* --- Search for a UEvent --- *", 45) << endl;
    cout << string(45, '*') << endl;
    cout << setw(30) << left << "| Enter UEvent name to search:";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string searchName;
    getline(cin, searchName);
    cout << string(45, '*') << endl;

    auto it = eventNameMap.find(searchName); // O(log N) map lookup.
    if (it != eventNameMap.end()) {
        cout << "\n✨ UEvent Found! ✨" << endl;
        vector<Event*> foundEvent = {it->second}; // Found event, put into a vector for display.
        displayEventsList(foundEvent, "Search Result for '" + searchName + "'");
    } else {
        cout << "\nUEvent '" << searchName << "' not found. 😔" << endl;
    }
    cout << endl;
}

// Function to register a participant for an event.
// Uses eventNameMap for quick event lookup.
void registerParticipant() {
    cout << "\n" << string(45, '*') << endl;
    cout << center("* --- Register for a UEvent --- *", 45) << endl;
    cout << string(45, '*') << endl;
    cout << setw(30) << left << "| Enter the name of the UEvent:";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string eventName;
    getline(cin, eventName);
    cout << string(45, '*') << endl;

    auto it = eventNameMap.find(eventName); // O(log N) event lookup.
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

// Function to display events based on a specific department.
// Uses eventsByDepartment map for efficient departmental grouping.
void displayEventsByDepartment() {
    cout << "\n" << string(45, '*') << endl;
    cout << center("* --- UEvents by Department --- *", 45) << endl;
    cout << string(45, '*') << endl;
    cout << setw(30) << left << "| Enter the department to filter by:";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string filterDepartment;
    getline(cin, filterDepartment);
    cout << string(45, '*') << endl;

    vector<Event*> filteredEvents;
    // Iterate through all departments in the map.
    for (const auto& pair : eventsByDepartment) {
        // Use string::find for partial match in department names.
        if (pair.first.find(filterDepartment) != string::npos) {
            // Append all events from this matching department.
            for(Event* eventPtr : pair.second) { // pair.second is vector<Event*>
                filteredEvents.push_back(eventPtr);
            }
        }
    }

    if (filteredEvents.empty()) {
        cout << "No UEvents found with department containing '" << filterDepartment << "'. 😔" << endl;
    } else {
        // Sort results by name for consistent display, even if collected from map.
        // Uses a lambda for custom comparison with pointers.
        sort(filteredEvents.begin(), filteredEvents.end(), [](Event* a, Event* b) {
            return a->name < b->name;
        });
        displayEventsList(filteredEvents, "UEvents with department containing '" + filterDepartment + "'");
    }
    cout << endl;
}

// Bubble Sort implementation (second sorting algorithm demonstrated).
// Sorts a vector of Event pointers in ascending order by date.
void bubbleSortEventsByDate(vector<Event*>& eventList) {
    int n = eventList.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            // Compare dates lexicographically (YYYY-MM-DD format allows direct string comparison).
            if (eventList[j]->date > eventList[j+1]->date) {
                swap(eventList[j], eventList[j+1]); // Swap pointers
            }
        }
    }
}

// Function to display events sorted by date using Bubble Sort.
void displayEventsSortedByDate() {
    if (events.empty()) {
        cout << "\n" << string(99, '=') << endl;
        cout << center("No UEvents available to sort. 😔", 99) << endl;
        cout << string(99, '=') << endl;
        return;
    }

    // Create a copy of pointers to events to sort.
    // This ensures the main 'events' vector (sorted by name) is not altered.
    vector<Event*> eventsCopy;
    for(auto& event : events) {
        eventsCopy.push_back(&event);
    }

    // Apply Bubble Sort on the copy.
    bubbleSortEventsByDate(eventsCopy);

    displayEventsList(eventsCopy, "UEvents Sorted by Date (Bubble Sort)");
}

// Linear Search implementation (second searching algorithm demonstrated).
// Searches for all events on a specific date.
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
    // Perform a linear search through all events.
    for (auto& event : events) {
        if (event.date == searchDate) {
            foundEvents.push_back(&event);
        }
    }

    if (foundEvents.empty()) {
        cout << "\nNo UEvents found on '" << searchDate << "'. 😔" << endl;
    } else {
        cout << "\n✨ UEvents found on '" << searchDate << "'! ✨" << endl;
        // Sort results by name for consistent display.
        sort(foundEvents.begin(), foundEvents.end(), [](Event* a, Event* b) {
            return a->name < b->name;
        });
        displayEventsList(foundEvents, "UEvents on " + searchDate);
    }
    cout << endl;
}


// --- New Function: Query Events by Date Range (using Segment Tree) ---
// This function demonstrates the efficiency of the Segment Tree for range queries.
void queryEventsByDateRange() {
    cout << "\n" << string(45, '*') << endl;
    cout << center("* --- Count UEvents by Date Range --- *", 45) << endl;
    cout << string(45, '*') << endl;

    if (indexToDateMap.empty() || segmentTree.empty()) {
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

    // Convert date strings to their corresponding numerical indices.
    // Use lower_bound to find the first index >= startDateStr.
    auto it_start = lower_bound(indexToDateMap.begin(), indexToDateMap.end(), startDateStr);
    // Use upper_bound to find the first index > endDateStr, then subtract 1 to get the last index <= endDateStr.
    auto it_end = upper_bound(indexToDateMap.begin(), indexToDateMap.end(), endDateStr);

    int startIdx = distance(indexToDateMap.begin(), it_start);
    int endIdx = distance(indexToDateMap.begin(), it_end) - 1; // Corrected to be inclusive

    // Validate the obtained indices to ensure a valid range for query.
    if (startIdx >= indexToDateMap.size() || endIdx < 0 || startIdx > endIdx) {
        cout << "\nNo events found in the date range [" << startDateStr << " to " << endDateStr << "]. 😔" << endl;
        return;
    }

    // Perform the segment tree query on the valid index range.
    int numDates = indexToDateMap.size(); // Total number of unique dates mapped.
    int eventCount = querySegmentTree(1, 0, numDates - 1, startIdx, endIdx);

    cout << "\nTotal UEvents in range [" << startDateStr << " to " << endDateStr << "]: " << eventCount << " ✨" << endl;
    cout << endl;
}


// Creative Terminal Interface - UEvent Organizer
// Displays the main menu for the application.
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
    cout << "  [2] 📜 View All UEvents\n";
    cout << "  [3] 🔍 Search UEvent by Name\n";
    cout << "  [4] ✍️ Register for a UEvent\n";
    cout << "  [5] 🏷️ View UEvents by Department\n";
    cout << "  [6] 📅 View UEvents Sorted by Date \n"; // Demonstrates Bubble Sort
    cout << "  [7] 🔎 Search UEvents by Date \n";   // Demonstrates Linear Search
    cout << "  [8] 📊 Count UEvents by Date Range \n"; // Demonstrates Segment Tree for range query
    cout << "  [9] 🚪 Exit\n"; // Exit option.
    cout << "  " << string(45, '-') << "\n";
    cout << "  ➡️ Enter your choice: ";
}

int main() {
    // Initial update of secondary data structures and segment tree.
    // This sets up all necessary data structures before the menu loop begins.
    updateSecondaryDataStructures();

    int choice;
    do {
        displayMenu();
        // Input validation for menu choice.
        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

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
            case 6:
                displayEventsSortedByDate();
                break;
            case 7:
                searchEventsByDate();
                break;
            case 8: // New case for Segment Tree functionality
                queryEventsByDateRange();
                break;
            case 9: // Exit option
                cout << "\n👋 Exiting UEvent Organizer. Have a great day! 👋\n";
                break;
            default:
                cout << "\n⚠️ Invalid choice. Please try again. ⚠️\n";
        }
    } while (choice != 9); // Loop continues until user chooses to exit.

    return 0;
}