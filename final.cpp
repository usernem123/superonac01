#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <limits>

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

    // Overload the less than operator for sorting
    bool operator<(const Event& other) const {
        return name < other.name; // Sort by event name
    }
};

// Segment Tree Node Structure
struct SegmentTreeNode {
    int count; // Count of events in this segment
};

// Global vector to store all events
vector<Event> events;

// Global segment tree
vector<SegmentTreeNode> tree;

// Function to build the segment tree
void buildSegmentTree(int node, int start, int end) {
    if (start == end) {
        tree[node].count = 1;
    } else {
        int mid = (start + end) / 2;
        buildSegmentTree(2 * node, start, mid);
        buildSegmentTree(2 * node + 1, mid + 1, end);
        tree[node].count = tree[2 * node].count + tree[2 * node + 1].count;
    }
}

// Function to query the number of events in a range
int querySegmentTree(int node, int start, int end, int l, int r) {
    if (r < start || end < l) {
        return 0;
    }
    if (l <= start && end <= r) {
        return tree[node].count;
    }
    int mid = (start + end) / 2;
    int p1 = querySegmentTree(2 * node, start, mid, l, r);
    int p2 = querySegmentTree(2 * node + 1, mid + 1, end, l, r);
    return p1 + p2;
}

// Simple function to center a string within a given width
string center(const string& s, int w) {
    int padding = (w - s.length()) / 2;
    return string(padding, ' ') + s + string(w - s.length() - padding, ' ');
}

// Function to add a new event
void addEvent() {
    cout << "\n" << string(45, '*') << endl;
    cout << center("* --- Adding a New UEvent --- *", 45) << endl;
    cout << string(45, '*') << endl;
    Event newEvent;
    newEvent.id = events.size() + 1;
    cout << setw(25) << left << "| Event Name:";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
    getline(cin, newEvent.name);
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
    sort(events.begin(), events.end()); // Maintain sorted order

    // Rebuild the segment tree after adding an event
    tree.resize(4 * events.size());
    if (!events.empty()) {
        buildSegmentTree(1, 0, events.size() - 1);
    }

    cout << "\nUEvent '" << newEvent.name << "' added successfully! ✨" << endl;
}

// Function to display all events
void displayEvents() {
    cout << "\n" << string(99, '=') << endl;
    cout << center("✨ --- All UEvents --- ✨", 99) << endl;
    cout << string(99, '=') << endl;
    if (events.empty()) {
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
    for (const auto& event : events) {
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

// Binary search function to find an event by name (for searching)
int binarySearchEvent(const string& name) {
    int left = 0;
    int right = events.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (events[mid].name == name) {
            return mid;
        } else if (events[mid].name < name) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1; // Event not found
}

// Function to search for an event by name
void searchEvent() {
    cout << "\n" << string(45, '*') << endl;
    cout << center("* --- Search for a UEvent --- *", 45) << endl;
    cout << string(45, '*') << endl;
    cout << setw(30) << left << "| Enter UEvent name to search:";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string searchName;
    getline(cin, searchName);
    cout << string(45, '*') << endl;

    int index = binarySearchEvent(searchName);
    if (index != -1) {
        cout << "\n✨ UEvent Found! ✨" << endl;
        cout << string(99, '-') << endl;
        cout << setw(5) << left << "ID" << " | "
             << setw(20) << left << "Name" << " | "
             << setw(12) << left << "Date" << " | "
             << setw(8) << left << "Time" << " | "
             << setw(15) << left << "Location" << " | "
             << setw(15) << left << "Department" << " | "
             << setw(10) << right << "Capacity" << " | "
             << setw(12) << right << "Participants" << endl;
        cout << string(99, '-') << endl;
        const auto& event = events[index];
        cout << setw(5) << left << event.id << " | "
             << setw(20) << left << event.name << " | "
             << setw(12) << left << event.date << " | "
             << setw(8) << left << event.time << " | "
             << setw(15) << left << event.location << " | "
             << setw(15) << left << event.department << " | "
             << setw(10) << right << event.capacity << " | "
             << setw(12) << right << event.participants << endl;
        cout << string(99, '-') << endl;
    } else {
        cout << "\nUEvent '" << searchName << "' not found. 😔" << endl;
    }
    cout << endl;
}

// Function to register a participant for an event
void registerParticipant() {
    cout << "\n" << string(45, '*') << endl;
    cout << center("* --- Register for a UEvent --- *", 45) << endl;
    cout << string(45, '*') << endl;
    cout << setw(30) << left << "| Enter the name of the UEvent:";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string eventName;
    getline(cin, eventName);
    cout << string(45, '*') << endl;

    int index = binarySearchEvent(eventName);
    if (index != -1) {
        if (events[index].participants < events[index].capacity) {
            events[index].participants++;
            cout << "🎉 Successfully registered for '" << events[index].name << "'! 🎉" << endl;
        } else {
            cout << "⚠️ UEvent '" << events[index].name << "' is already full. 😟" << endl;
        }
    } else {
        cout << "UEvent '" << eventName << "' not found. 😔" << endl;
    }
    cout << endl;
}

// Function to display events based on a specific department
void displayEventsByDepartment() {
    cout << "\n" << string(45, '*') << endl;
    cout << center("* --- UEvents by Department --- *", 45) << endl;
    cout << string(45, '*') << endl;
    cout << setw(30) << left << "| Enter the department to filter by:";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string filterDepartment;
    getline(cin, filterDepartment);
    cout << string(45, '*') << endl;

    bool found = false;
    cout << "\n✨ UEvents with department '" << filterDepartment << "' ✨" << endl;
    cout << string(99, '-') << endl;
    cout << setw(5) << left << "ID" << " | "
         << setw(20) << left << "Name" << " | "
         << setw(12) << left << "Date" << " | "
         << setw(8) << left << "Time" << " | "
         << setw(15) << left << "Location" << " | "
         << setw(15) << left << "Department" << " | "
         << setw(10) << right << "Capacity" << " | "
         << setw(12) << right << "Participants" << endl;
    cout << string(99, '-') << endl;
    for (const auto& event : events) {
        if (event.department.find(filterDepartment) != string::npos) {
            cout << setw(5) << left << event.id << " | "
                 << setw(20) << left << event.name << " | "
                 << setw(12) << left << event.date << " | "
                 << setw(8) << left << event.time << " | "
                 << setw(15) << left << event.location << " | "
                 << setw(15) << left << event.department << " | "
                 << setw(10) << right << event.capacity << " | "
                 << setw(12) << right << event.participants << endl;
        }
    }
    cout << string(99, '=') << endl;
    if (!found) {
        cout << "No UEvents found with the department '" << filterDepartment << "'. 😔" << endl;
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
    cout << "  ║           ██║   ██║╚════██║██║   ██║            ║\n";
    cout << "  ║           ╚██████╔╝███████║╚██████╔╝            ║\n";
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
    cout << "  [6] 🚪 Exit\n";
    cout << "  " << string(45, '-') << "\n";
    cout << "  ➡️ Enter your choice: ";
}

int main() {
    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                addEvent();
                break;
            case 2:
                displayEvents();
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
                cout << "\n👋 Exiting UEvent Organizer. Have a great day! 👋\n";
                break;
            default:
                cout << "\n⚠️ Invalid choice. Please try again. ⚠️\n";
        }
    } while (choice != 6);

    return 0;
}