// Week 5 - Weekly Performance Evaluator (UPGRADE from Week 4)
// This week: abstract classes + virtual functions + polymorphism + dynamic memory
// Base class (abstract): WeeklyReport
// Derived: LevelReport, TrainingPlanReport, RecoveryReport
// Composition: SessionStats (inside derived)
// Manager/container: ReportManager (dynamic array of WeeklyReport*)
// Unit tests run in _DEBUG

#ifdef _DEBUG
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#endif

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

// --------------------
// constants (so I don't use magic numbers)
// --------------------
const double PRO_TRAIN_HRS = 6.0;
const double INT_TRAIN_HRS = 3.0;

const double SLEEP_MIN_OK = 7.0;
const double SLEEP_GOOD_MIN = 7.5;
const double SLEEP_GOOD_MAX = 9.0;

const double W_TRAIN = 10.0;
const double W_SLEEP = 5.0;

const int INPUT_FLUSH = 10000;
const int MAX_SESSIONS = 5;

const int MIN_AGE = 1;
const double MIN_TRAINING_HOURS = 0.0;
const double MIN_SLEEP_HOURS = 0.01;

// menu now includes add/print/delete
const int MENU_MIN_CHOICE = 0;
const int MENU_MAX_CHOICE = 5;

const double SHIFT_MIN = 10.0;

const double PRO_TECH_MINS = 50.0;
const double PRO_COND_MINS = 30.0;
const double INT_TECH_MINS = 45.0;
const double INT_COND_MINS = 25.0;
const double BEG_TECH_MINS = 35.0;
const double BEG_COND_MINS = 20.0;

const int REST_DAYS_HIGH = 2;
const int REST_DAYS_MODERATE = 1;
const int REST_DAYS_LOW = 0;

// player level enum (from Week 1 requirement)
enum PlayerLevel { LEVEL_AMATEUR, LEVEL_SEMI_PRO, LEVEL_PRO };

// helper functions for input/output
void setConsoleColor();
void showBanner();

int getValidInt(const string& prompt, int minValue);
double getValidDouble(const string& prompt, double minValue);

int getMenuChoice();
string levelToString(PlayerLevel level);

void printSessionsTable(const double sessions[], int sessionCount);

// --------------------
// Composition class
// --------------------
class SessionStats
{
private:
    int sessionCount;
    double totalHours;
    double avgHours;

public:
    SessionStats()
    {
        sessionCount = 0;
        totalHours = 0.0;
        avgHours = 0.0;
    }

    SessionStats(int count, double total, double avg)
    {
        sessionCount = (count < 0 ? 0 : count);
        totalHours = (total < 0.0 ? 0.0 : total);
        avgHours = (avg < 0.0 ? 0.0 : avg);
    }

    int getSessionCount() const { return sessionCount; }
    double getTotalHours() const { return totalHours; }
    double getAvgHours() const { return avgHours; }

    void setSessionCount(int count) { sessionCount = (count < 0 ? 0 : count); }
    void setTotalHours(double total) { totalHours = (total < 0.0 ? 0.0 : total); }
    void setAvgHours(double avg) { avgHours = (avg < 0.0 ? 0.0 : avg); }

    bool isEmpty() const
    {
        return sessionCount == 0;
    }
};

// --------------------
// Abstract base class
// --------------------
class WeeklyReport
{
protected:
    PlayerLevel level; // protected so child classes can access

private:
    string playerName;
    int age;

    double sleepHours;
    double readinessScore;
    string advice;

public:
    WeeklyReport()
    {
        playerName = "";
        age = 0;
        level = LEVEL_AMATEUR;
        sleepHours = 0.0;
        readinessScore = 0.0;
        advice = "";
    }

    WeeklyReport(const string& name, int playerAge, PlayerLevel lvl)
    {
        playerName = name;
        age = (playerAge < 0 ? 0 : playerAge);
        level = lvl;
        sleepHours = 0.0;
        readinessScore = 0.0;
        advice = "";
    }

    // Base is now ABSTRACT:
    // - virtual destructor (for safe delete through base*)
    // - at least 1 pure virtual function (forces derived override)
    virtual ~WeeklyReport() {}

    virtual string getType() const = 0; // PURE VIRTUAL (required)

    // prints the base info (virtual, still has implementation)
    virtual void print() const
    {
        cout << "\n===== WEEKLY REPORT (" << getType() << ") =====\n";
        cout << "Player: " << playerName << "\n";
        cout << "Age: " << age << "\n";
        cout << "Level: " << levelToString(level) << "\n";
        cout << "Avg sleep: " << sleepHours << "\n";
        cout << "Readiness Score: " << readinessScore << "\n";
        cout << "Advice: " << advice << "\n";
    }

    // basic getters/setters
    string getPlayerName() const { return playerName; }
    int getAge() const { return age; }
    PlayerLevel getLevel() const { return level; }
    double getSleepHours() const { return sleepHours; }
    double getReadinessScore() const { return readinessScore; }
    string getAdvice() const { return advice; }

    void setPlayerName(const string& name) { playerName = name; }
    void setAge(int playerAge) { age = (playerAge < 0 ? 0 : playerAge); }
    void setLevel(PlayerLevel lvl) { level = lvl; }
    void setSleepHours(double hours) { sleepHours = (hours < 0.0 ? 0.0 : hours); }
    void setReadinessScore(double score) { readinessScore = score; }
    void setAdvice(const string& a) { advice = a; }
};

// --------------------
// LevelReport (NEW derived) - replaces old "WeeklyReport base(...)" usage
// --------------------
class LevelReport : public WeeklyReport
{
private:
    SessionStats stats;            // composition
    double sessions[MAX_SESSIONS]; // store per-session hours
    int sessionCount;
    double totalTraining;
    double avgTraining;

public:
    LevelReport()
        : WeeklyReport()
    {
        stats = SessionStats();
        sessionCount = 0;
        totalTraining = 0.0;
        avgTraining = 0.0;
        for (int i = 0; i < MAX_SESSIONS; ++i) sessions[i] = 0.0;
    }

    LevelReport(const string& name, int playerAge, PlayerLevel lvl,
        const double sess[], int count, double total, double avg)
        : WeeklyReport(name, playerAge, lvl)
    {
        sessionCount = (count < 0 ? 0 : count);
        if (sessionCount > MAX_SESSIONS) sessionCount = MAX_SESSIONS;

        totalTraining = (total < 0.0 ? 0.0 : total);
        avgTraining = (avg < 0.0 ? 0.0 : avg);

        for (int i = 0; i < MAX_SESSIONS; ++i)
            sessions[i] = (i < sessionCount ? sess[i] : 0.0);

        stats = SessionStats(sessionCount, totalTraining, avgTraining);
    }

    string getType() const override { return "Level"; }

    void print() const override
    {
        WeeklyReport::print();

        cout << "\n----- LEVEL DETAILS (DERIVED) -----\n";
        cout << "Total training this week: " << totalTraining << "\n";
        cout << "Average training per session: " << avgTraining << "\n";
        cout << "Sessions: " << stats.getSessionCount()
            << " | Total hours: " << stats.getTotalHours()
            << " | Avg hours: " << stats.getAvgHours() << "\n";

        printSessionsTable(sessions, sessionCount);
    }

    // getters for tests
    int getSessionCount() const { return sessionCount; }
    double getTotalTraining() const { return totalTraining; }
    double getAvgTraining() const { return avgTraining; }
    SessionStats getStats() const { return stats; }
};

// --------------------
// TrainingPlanReport (derived)
// --------------------
class TrainingPlanReport : public WeeklyReport
{
private:
    SessionStats stats; // composition
    string focus;
    double techMins;
    double condMins;

public:
    TrainingPlanReport()
        : WeeklyReport()
    {
        stats = SessionStats();
        focus = "";
        techMins = 0.0;
        condMins = 0.0;
    }

    TrainingPlanReport(const string& name, int playerAge, PlayerLevel lvl,
        const SessionStats& s, const string& f, double tech, double cond)
        : WeeklyReport(name, playerAge, lvl)
    {
        stats = s;
        focus = f;
        techMins = tech;
        condMins = cond;
    }

    string getType() const override { return "Training Plan"; }

    SessionStats getStats() const { return stats; }
    string getFocus() const { return focus; }
    double getTechMins() const { return techMins; }
    double getCondMins() const { return condMins; }

    void setStats(const SessionStats& s) { stats = s; }
    void setFocus(const string& f) { focus = f; }
    void setTechMins(double mins) { techMins = mins; }
    void setCondMins(double mins) { condMins = mins; }

    void print() const override
    {
        WeeklyReport::print();

        cout << "\n----- TRAINING PLAN (DERIVED) -----\n";
        cout << "Focus: " << focus << "\n";
        cout << "Technical work: " << techMins << " min\n";
        cout << "Conditioning: " << condMins << " min\n";
        cout << "Sessions: " << stats.getSessionCount()
            << " | Total hours: " << stats.getTotalHours()
            << " | Avg hours: " << stats.getAvgHours() << "\n";
    }
};

// --------------------
// RecoveryReport (derived)
// --------------------
class RecoveryReport : public WeeklyReport
{
private:
    SessionStats stats; // composition
    string fatigue;
    int restDays;
    string tip;

public:
    RecoveryReport()
        : WeeklyReport()
    {
        stats = SessionStats();
        fatigue = "";
        restDays = 0;
        tip = "";
    }

    RecoveryReport(const string& name, int playerAge, PlayerLevel lvl,
        const SessionStats& s, const string& f, int rest, const string& t)
        : WeeklyReport(name, playerAge, lvl)
    {
        stats = s;
        fatigue = f;
        restDays = rest;
        tip = t;
    }

    string getType() const override { return "Recovery"; }

    SessionStats getStats() const { return stats; }
    string getFatigue() const { return fatigue; }
    int getRestDays() const { return restDays; }
    string getTip() const { return tip; }

    void setStats(const SessionStats& s) { stats = s; }
    void setFatigue(const string& f) { fatigue = f; }
    void setRestDays(int days) { restDays = (days < 0 ? 0 : days); }
    void setTip(const string& t) { tip = t; }

    void print() const override
    {
        WeeklyReport::print();

        cout << "\n----- RECOVERY (DERIVED) -----\n";
        cout << "Fatigue: " << fatigue << "\n";
        cout << "Rest Days: " << restDays << "\n";
        cout << "Tip: " << tip << "\n";
        cout << "Sessions: " << stats.getSessionCount()
            << " | Total hours: " << stats.getTotalHours()
            << " | Avg hours: " << stats.getAvgHours() << "\n";
    }
};

// --------------------
// ReportManager (NEW) - dynamic array of base pointers
// --------------------
class ReportManager
{
private:
    WeeklyReport** items; // dynamic array of base pointers
    int size;
    int capacity;

    void resize(int newCapacity)
    {
        if (newCapacity <= capacity) return;

        WeeklyReport** newItems = new WeeklyReport * [newCapacity];

        for (int i = 0; i < size; ++i)
            newItems[i] = items[i];

        for (int i = size; i < newCapacity; ++i)
            newItems[i] = nullptr;

        delete[] items;
        items = newItems;
        capacity = newCapacity;
    }

public:
    // stop copying (Rule of 3/5)
    ReportManager(const ReportManager&) = delete;
    ReportManager& operator=(const ReportManager&) = delete;

    ReportManager(int initialCapacity = 2)
    {
        if (initialCapacity < 1) initialCapacity = 1;
        capacity = initialCapacity;
        size = 0;

        items = new WeeklyReport * [capacity];
        for (int i = 0; i < capacity; ++i)
            items[i] = nullptr;
    }

    ~ReportManager()
    {
        clear();
        delete[] items;
        items = nullptr;
        capacity = 0;
    }

    void clear()
    {
        for (int i = 0; i < size; ++i)
        {
            delete items[i]; // safe because base has virtual destructor
            items[i] = nullptr;
        }
        size = 0;
    }

    int getSize() const { return size; }
    int getCapacity() const { return capacity; }

    bool add(WeeklyReport* p)
    {
        if (p == nullptr) return false;

        if (size >= capacity)
            resize(capacity * 2);

        items[size] = p;
        size++;
        return true;
    }

    bool removeAt(int index)
    {
        if (index < 0 || index >= size) return false;

        delete items[index];
        items[index] = nullptr;

        for (int i = index; i < size - 1; ++i)
            items[i] = items[i + 1];

        items[size - 1] = nullptr;
        size--;
        return true;
    }

    WeeklyReport* getAt(int index) const
    {
        if (index < 0 || index >= size) return nullptr;
        return items[index];
    }

    void printAll() const
    {
        if (size == 0)
        {
            cout << "\n(Manager) No reports saved yet.\n";
            return;
        }

        cout << "\n==============================\n";
        cout << " SAVED REPORTS (" << size << ")\n";
        cout << "==============================\n";

        for (int i = 0; i < size; ++i)
        {
            cout << "\n[#" << (i + 1) << "]\n";
            if (items[i] != nullptr)
                items[i]->print(); // POLYMORPHIC call
        }
    }
};

// --------------------
// TrainingLog (upgraded): uses ReportManager
// --------------------
class TrainingLog
{
private:
    string name;
    int age;
    double sleepHours;

    double sessions[MAX_SESSIONS];
    int sessionCount;

    double totalTraining;
    double avgTraining;

    PlayerLevel level;
    double readinessScore;
    string advice;

    ReportManager manager; // owns all dynamically allocated reports

    void computeTrainingStats()
    {
        totalTraining = 0.0;

        if (sessionCount < 0) sessionCount = 0;
        for (int i = 0; i < sessionCount; ++i)
            totalTraining += sessions[i];

        avgTraining = (sessionCount > 0 ? totalTraining / sessionCount : 0.0);
    }

    void evaluateLevel()
    {
        readinessScore = (avgTraining * W_TRAIN) + ((sleepHours - SLEEP_MIN_OK) * W_SLEEP);

        if (avgTraining >= PRO_TRAIN_HRS &&
            (sleepHours >= SLEEP_GOOD_MIN && sleepHours <= SLEEP_GOOD_MAX))
        {
            level = LEVEL_PRO;
        }
        else if (avgTraining >= INT_TRAIN_HRS && sleepHours >= SLEEP_MIN_OK)
        {
            level = LEVEL_SEMI_PRO;
        }
        else
        {
            level = LEVEL_AMATEUR;
        }

        if (sleepHours < SLEEP_MIN_OK || avgTraining < INT_TRAIN_HRS)
            advice = "Build base: 3 sessions + 8h sleep.";
        else if (avgTraining >= PRO_TRAIN_HRS &&
            !(sleepHours >= SLEEP_GOOD_MIN && sleepHours <= SLEEP_GOOD_MAX))
            advice = "Improve sleep routine.";
        else
            advice = "Good balance.";
    }

    SessionStats buildStats() const
    {
        return SessionStats(sessionCount, totalTraining, avgTraining);
    }

    void addLevelReport()
    {
        WeeklyReport* rpt = new LevelReport(
            name, age, level,
            sessions, sessionCount,
            totalTraining, avgTraining
        );

        rpt->setSleepHours(sleepHours);
        rpt->setReadinessScore(readinessScore);
        rpt->setAdvice(advice);

        manager.add(rpt);

        // optional file output
        ofstream out("report.txt");
        if (out)
        {
            out << "WEEKLY PERFORMANCE REPORT (Week 5)\n";
            out << "Section: LEVEL\n";
            out << "----------------------------------\n";
            out << left << setw(22) << "Player:" << right << setw(20) << name << "\n";
            out << left << setw(22) << "Age:" << right << setw(20) << age << "\n";
            out << left << setw(22) << "Total training:" << right << setw(20) << totalTraining << "\n";
            out << left << setw(22) << "Avg training:" << right << setw(20) << avgTraining << "\n";
            out << left << setw(22) << "Avg sleep:" << right << setw(20) << sleepHours << "\n";
            out << left << setw(22) << "Level:" << right << setw(20) << levelToString(level) << "\n";
            out << left << setw(22) << "Readiness:" << right << setw(20) << readinessScore << "\n";
            out << left << setw(22) << "Advice:" << right << setw(20) << advice << "\n\n";

            out << "Session details:\n";
            out << left << setw(10) << "Session" << setw(15) << "Hours" << "\n";
            for (int i = 0; i < sessionCount; ++i)
                out << left << setw(10) << (i + 1) << setw(15) << sessions[i] << "\n";
        }
        else
        {
            cout << "Could not write report.txt\n";
        }

        cout << "\n(Level report added to manager)\n";
    }

    void addTrainingPlanReport()
    {
        string planFocus;
        double techMins;
        double condMins;

        switch (level)
        {
        case LEVEL_PRO:
            techMins = PRO_TECH_MINS;
            condMins = PRO_COND_MINS;
            planFocus = "High Tempo";
            break;
        case LEVEL_SEMI_PRO:
            techMins = INT_TECH_MINS;
            condMins = INT_COND_MINS;
            planFocus = "Balanced";
            break;
        default:
            techMins = BEG_TECH_MINS;
            condMins = BEG_COND_MINS;
            planFocus = "Fundamentals";
            break;
        }

        if (sleepHours < SLEEP_MIN_OK && condMins >= SHIFT_MIN)
        {
            techMins += SHIFT_MIN;
            condMins -= SHIFT_MIN;
        }

        WeeklyReport* rpt = new TrainingPlanReport(
            name, age, level,
            buildStats(),
            planFocus, techMins, condMins
        );

        rpt->setSleepHours(sleepHours);
        rpt->setReadinessScore(readinessScore);
        rpt->setAdvice(advice);

        manager.add(rpt);
        cout << "\n(Training plan report added to manager)\n";
    }

    void addRecoveryReport()
    {
        string fatigue = "Low";
        int rest = REST_DAYS_LOW;
        string tip;

        if (sleepHours < SLEEP_MIN_OK)
            fatigue = "High";
        else if (totalTraining >= PRO_TRAIN_HRS || totalTraining < INT_TRAIN_HRS)
            fatigue = "Moderate";

        if (fatigue == "High")
            rest = REST_DAYS_HIGH;
        else if (fatigue == "Moderate")
            rest = REST_DAYS_MODERATE;

        if (sleepHours < SLEEP_MIN_OK)
            tip = "Aim for 8h sleep.";
        else if (totalTraining >= PRO_TRAIN_HRS)
            tip = "Foam roll.";
        else
            tip = "Light stretching.";

        WeeklyReport* rpt = new RecoveryReport(
            name, age, level,
            buildStats(),
            fatigue, rest, tip
        );

        rpt->setSleepHours(sleepHours);
        rpt->setReadinessScore(readinessScore);
        rpt->setAdvice(advice);

        manager.add(rpt);
        cout << "\n(Recovery report added to manager)\n";
    }

    void deleteReport()
    {
        if (manager.getSize() == 0)
        {
            cout << "\nNothing to delete.\n";
            return;
        }

        int idx = getValidInt("Enter report number to delete (1..N): ", 1);
        idx = idx - 1;

        if (manager.removeAt(idx))
            cout << "Deleted.\n";
        else
            cout << "Invalid index.\n";
    }

public:
    TrainingLog()
        : manager(2) // start small so resize can be demonstrated
    {
        name = "";
        age = 0;
        sleepHours = 0.0;

        sessionCount = 0;
        totalTraining = 0.0;
        avgTraining = 0.0;

        level = LEVEL_AMATEUR;
        readinessScore = 0.0;
        advice = "";

        for (int i = 0; i < MAX_SESSIONS; ++i)
            sessions[i] = 0.0;
    }

    void setup()
    {
        cout << "Enter player's full name: ";
        getline(cin, name);

        age = getValidInt("Enter player's age: ", MIN_AGE);

        cout << fixed << showpoint << setprecision(2);

        cout << "\nHow many training sessions did you have this week (1-" << MAX_SESSIONS << ")? ";
        while (!(cin >> sessionCount) || sessionCount < 1 || sessionCount > MAX_SESSIONS)
        {
            cin.clear();
            cin.ignore(INPUT_FLUSH, '\n');
            cout << "Invalid. Enter a number between 1 and " << MAX_SESSIONS << ": ";
        }
        cin.ignore(INPUT_FLUSH, '\n');

        cout << "\nEnter training hours for each session:\n";
        for (int i = 0; i < sessionCount; ++i)
        {
            sessions[i] = getValidDouble("  Session " + to_string(i + 1) + ": ", MIN_TRAINING_HOURS);
        }
        for (int i = sessionCount; i < MAX_SESSIONS; ++i)
            sessions[i] = 0.0;

        sleepHours = getValidDouble("Avg sleep hours per night: ", MIN_SLEEP_HOURS);

        computeTrainingStats();
        evaluateLevel();
    }

    void runMenu()
    {
        int choice;
        do
        {
            choice = getMenuChoice();

            switch (choice)
            {
            case 1:
                addLevelReport();
                break;
            case 2:
                addTrainingPlanReport();
                break;
            case 3:
                addRecoveryReport();
                break;
            case 4:
                manager.printAll();
                break;
            case 5:
                deleteReport();
                break;
            case 0:
                cout << "\nExiting... (Manager destructor will clean memory)\n";
                break;
            }

        } while (choice != 0);
    }

    // functions used mostly for unit tests
    bool addSession(double hours)
    {
        if (hours < 0.0) return false;
        if (sessionCount >= MAX_SESSIONS) return false;

        sessions[sessionCount] = hours;
        sessionCount++;

        for (int i = sessionCount; i < MAX_SESSIONS; ++i)
            sessions[i] = 0.0;

        computeTrainingStats();
        return true;
    }

    void setSleep(double hours)
    {
        sleepHours = (hours < 0.0 ? 0.0 : hours);
        evaluateLevel();
    }

    // simple getters for tests
    int getSessionCount() const { return sessionCount; }
    double getTotalHours() const { return totalTraining; }
    double getAverageHours() const { return avgTraining; }
    PlayerLevel getLevel() const { return level; }
    string getAdvice() const { return advice; }
};

// --------------------
// functions (UI/input)
// --------------------
void setConsoleColor()
{
    system("color 1E");  // Windows only
}

void showBanner()
{
    cout << "===================================\n";
    cout << "    Weekly Performance Evaluator\n";
    cout << "===================================\n\n";
}

int getValidInt(const string& prompt, int minValue)
{
    int value;
    cout << prompt;

    while (!(cin >> value) || value < minValue)
    {
        cin.clear();
        cin.ignore(INPUT_FLUSH, '\n');
        cout << "Invalid input. " << prompt;
    }

    cin.ignore(INPUT_FLUSH, '\n');
    return value;
}

double getValidDouble(const string& prompt, double minValue)
{
    double value;
    cout << prompt;

    while (!(cin >> value) || value < minValue)
    {
        cin.clear();
        cin.ignore(INPUT_FLUSH, '\n');
        cout << "Invalid input. " << prompt;
    }

    cin.ignore(INPUT_FLUSH, '\n');
    return value;
}

int getMenuChoice()
{
    int choice;

    cout << "\nMenu:\n";
    cout << "  1) Add Level Report\n";
    cout << "  2) Add Training Plan Report\n";
    cout << "  3) Add Recovery Report\n";
    cout << "  4) Print All Saved Reports\n";
    cout << "  5) Delete a Report\n";
    cout << "  0) Quit\n";
    cout << "Enter choice: ";

    while (!(cin >> choice) || choice < MENU_MIN_CHOICE || choice > MENU_MAX_CHOICE)
    {
        cin.clear();
        cin.ignore(INPUT_FLUSH, '\n');
        cout << "Invalid. Enter " << MENU_MIN_CHOICE << "-" << MENU_MAX_CHOICE << ": ";
    }

    cin.ignore(INPUT_FLUSH, '\n');
    return choice;
}

void printSessionsTable(const double sessions[], int sessionCount)
{
    cout << "\nSession Breakdown:\n";
    cout << left << setw(10) << "#" << setw(12) << "Hours" << "\n";

    for (int i = 0; i < sessionCount; ++i)
    {
        cout << left << setw(10) << (i + 1)
            << setw(12) << sessions[i] << "\n";
    }
}

string levelToString(PlayerLevel level)
{
    switch (level)
    {
    case LEVEL_PRO: return "Pro";
    case LEVEL_SEMI_PRO: return "Semi-Pro";
    case LEVEL_AMATEUR: return "Amateur";
    default: return "Unknown";
    }
}

#ifndef _DEBUG
// main runs only in Release (not _DEBUG)
int main()
{
    setConsoleColor();
    showBanner();

    TrainingLog tracker;
    tracker.setup();
    tracker.runMenu();

    return 0;
}
#endif

#ifdef _DEBUG
// doctest unit tests (run in _DEBUG)

TEST_CASE("Composition: SessionStats default ctor is empty")
{
    SessionStats s;
    CHECK(s.getSessionCount() == 0);
    CHECK(s.getTotalHours() == doctest::Approx(0.0));
    CHECK(s.getAvgHours() == doctest::Approx(0.0));
    CHECK(s.isEmpty() == true);
}

TEST_CASE("Abstract: WeeklyReport is polymorphic via getType()")
{
    WeeklyReport* p1 = new TrainingPlanReport("Sam", 15, LEVEL_PRO, SessionStats(2, 4.0, 2.0), "High Tempo", 50.0, 30.0);
    WeeklyReport* p2 = new RecoveryReport("Mike", 12, LEVEL_AMATEUR, SessionStats(1, 2.0, 2.0), "High", 2, "Aim for 8h sleep.");

    CHECK(p1->getType() == "Training Plan");
    CHECK(p2->getType() == "Recovery");

    delete p1; // virtual base dtor
    delete p2;
}

TEST_CASE("Manager: add() grows size and resizes capacity")
{
    ReportManager m(1);
    CHECK(m.getSize() == 0);
    CHECK(m.getCapacity() == 1);

    CHECK(m.add(new LevelReport()) == true);
    CHECK(m.getSize() == 1);

    // triggers resize
    CHECK(m.add(new LevelReport()) == true);
    CHECK(m.getSize() == 2);
    CHECK(m.getCapacity() >= 2);
}

TEST_CASE("Manager: removeAt deletes and shifts")
{
    ReportManager m(2);

    WeeklyReport* a = new LevelReport();
    WeeklyReport* b = new LevelReport();
    WeeklyReport* c = new LevelReport();

    CHECK(m.add(a) == true);
    CHECK(m.add(b) == true);
    CHECK(m.add(c) == true);
    CHECK(m.getSize() == 3);

    // remove middle
    CHECK(m.removeAt(1) == true);
    CHECK(m.getSize() == 2);

    // after shift, index 1 should now be old 'c'
    CHECK(m.getAt(1) != nullptr);
    CHECK(m.getAt(1) == c);
}

TEST_CASE("TrainingLog: addSession updates stats (no cin)")
{
    TrainingLog log;

    CHECK(log.getSessionCount() == 0);
    CHECK(log.getTotalHours() == doctest::Approx(0.0));
    CHECK(log.getAverageHours() == doctest::Approx(0.0));

    CHECK(log.addSession(2.0) == true);
    CHECK(log.addSession(4.0) == true);

    CHECK(log.getSessionCount() == 2);
    CHECK(log.getTotalHours() == doctest::Approx(6.0));
    CHECK(log.getAverageHours() == doctest::Approx(3.0));
}

TEST_CASE("TrainingLog: addSession guard cases")
{
    TrainingLog log;

    CHECK(log.addSession(-1.0) == false);
    CHECK(log.getSessionCount() == 0);

    for (int i = 0; i < MAX_SESSIONS; ++i)
        CHECK(log.addSession(1.0) == true);

    CHECK(log.getSessionCount() == MAX_SESSIONS);
    CHECK(log.addSession(1.0) == false);
}

#endif
