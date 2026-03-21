// Week 9 - Weekly Performance Evaluator (UPGRADE from Week 8)
// This week: C++ Searching, Sorting, and the Vector Type (Chapter 16)
// Week 9 Requirements implemented:
// - Replace at least one existing array with std::vector
// - Add sequential (linear) search as a class member function
// - Add manual sorting algorithm (Bubble Sort) as a class member function
// - Add binary search as a class member function
// - Update doctests for vector/search/sort behavior
// - Keep prior applicable tests
// - Show diff in video
// - CRT memory leak verification output (debug)
// - Tag repo "Week09 Programming Assignment"

#ifdef _DEBUG
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#endif

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <exception>
#include <vector>

using namespace std;

// --------------------
// function template (Week 6)
// --------------------
template <typename T>
T clampMin(T value, T minValue)
{
    return (value < minValue ? minValue : value);
}

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

// player level enum
enum PlayerLevel { LEVEL_AMATEUR, LEVEL_SEMI_PRO, LEVEL_PRO };

// helper functions for input/output
void setConsoleColor();
void showBanner();

int getValidInt(const string& prompt, int minValue);
double getValidDouble(const string& prompt, double minValue);

int getMenuChoice();
string levelToString(PlayerLevel level);

void printSessionsTable(const vector<double>& sessions);

// --------------------
// Week 7: Custom exception
// --------------------
class AppException : public std::runtime_error
{
public:
    explicit AppException(const std::string& msg)
        : std::runtime_error(msg) {}
};

// --------------------
// Week 6/7: Class template
// --------------------
template <typename T>
class DynArray
{
private:
    T* data;
    int size;
    int capacity;

    void resize(int newCapacity)
    {
        if (newCapacity <= capacity) return;

        T* newData = new T[newCapacity];

        for (int i = 0; i < size; ++i)
            newData[i] = data[i];

        for (int i = size; i < newCapacity; ++i)
            newData[i] = T();

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    DynArray(int initialCapacity = 2)
    {
        if (initialCapacity < 1) initialCapacity = 1;

        capacity = initialCapacity;
        size = 0;

        data = new T[capacity];
        for (int i = 0; i < capacity; ++i)
            data[i] = T();
    }

    ~DynArray()
    {
        delete[] data;
        data = nullptr;
        size = 0;
        capacity = 0;
    }

    DynArray(const DynArray&) = delete;
    DynArray& operator=(const DynArray&) = delete;

    int getSize() const { return size; }
    int getCapacity() const { return capacity; }

    bool pushBack(const T& value)
    {
        if (this->size >= this->capacity)
            this->resize(this->capacity * 2);

        data[size] = value;
        size++;
        return true;
    }

    bool removeAt(int index)
    {
        if (index < 0 || index >= size)
            throw AppException("DynArray: invalid index in removeAt");

        for (int i = index; i < size - 1; ++i)
            data[i] = data[i + 1];

        data[size - 1] = T();
        size--;
        return true;
    }

    T at(int index) const
    {
        if (index < 0 || index >= size)
            throw AppException("DynArray: invalid index in at");
        return data[index];
    }

    T operator[](int index) const
    {
        return at(index);
    }
};

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
    PlayerLevel level;

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

    virtual ~WeeklyReport() {}

    virtual string getType() const = 0;

    virtual void toStream(ostream& os) const
    {
        os << fixed << setprecision(2);
        os << getType()
            << " | Name=" << playerName
            << " | Age=" << age
            << " | Level=" << levelToString(level)
            << " | Sleep=" << sleepHours
            << " | Ready=" << readinessScore;
    }

    friend ostream& operator<<(ostream& os, const WeeklyReport& obj)
    {
        obj.toStream(os);
        return os;
    }

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

    string getPlayerName() const { return playerName; }
    int getAge() const { return age; }
    PlayerLevel getLevel() const { return level; }
    double getSleepHours() const { return sleepHours; }
    double getReadinessScore() const { return readinessScore; }
    string getAdvice() const { return advice; }

    void setPlayerName(const string& name) { playerName = name; }
    void setAge(int playerAge) { age = (playerAge < 0 ? 0 : playerAge); }
    void setLevel(PlayerLevel lvl) { level = lvl; }

    void setSleepHours(double hours) { sleepHours = clampMin(hours, 0.0); }
    void setReadinessScore(double score) { readinessScore = score; }
    void setAdvice(const string& a) { advice = a; }
};

// --------------------
// LevelReport
// Week 9: sessions changed from array to vector
// --------------------
class LevelReport : public WeeklyReport
{
private:
    SessionStats stats;
    vector<double> sessions;
    double totalTraining;
    double avgTraining;

public:
    LevelReport()
        : WeeklyReport()
    {
        stats = SessionStats();
        totalTraining = 0.0;
        avgTraining = 0.0;
    }

    LevelReport(const string& name, int playerAge, PlayerLevel lvl,
        const vector<double>& sess, double total, double avg)
        : WeeklyReport(name, playerAge, lvl)
    {
        sessions = sess;
        if (static_cast<int>(sessions.size()) > MAX_SESSIONS)
            sessions.resize(MAX_SESSIONS);

        totalTraining = (total < 0.0 ? 0.0 : total);
        avgTraining = (avg < 0.0 ? 0.0 : avg);

        stats = SessionStats(static_cast<int>(sessions.size()), totalTraining, avgTraining);
    }

    string getType() const override { return "Level"; }

    bool operator==(const LevelReport& rhs) const
    {
        return (this->getPlayerName() == rhs.getPlayerName() &&
            this->getAge() == rhs.getAge() &&
            this->getLevel() == rhs.getLevel() &&
            this->sessions.size() == rhs.sessions.size() &&
            this->totalTraining == rhs.totalTraining);
    }

    void toStream(ostream& os) const override
    {
        os << fixed << setprecision(2);
        os << "Level"
            << " | Name=" << getPlayerName()
            << " | Age=" << getAge()
            << " | Level=" << levelToString(getLevel())
            << " | Sessions=" << sessions.size()
            << " | Total=" << totalTraining
            << " | Avg=" << avgTraining
            << " | Sleep=" << getSleepHours()
            << " | Ready=" << getReadinessScore();
    }

    void print() const override
    {
        WeeklyReport::print();

        cout << "\n----- LEVEL DETAILS (DERIVED) -----\n";
        cout << "Total training this week: " << totalTraining << "\n";
        cout << "Average training per session: " << avgTraining << "\n";
        cout << "Sessions: " << stats.getSessionCount()
            << " | Total hours: " << stats.getTotalHours()
            << " | Avg hours: " << stats.getAvgHours() << "\n";

        printSessionsTable(sessions);
    }

    int getSessionCount() const { return static_cast<int>(sessions.size()); }
    double getTotalTraining() const { return totalTraining; }
    double getAvgTraining() const { return avgTraining; }
    SessionStats getStats() const { return stats; }
};

// --------------------
// TrainingPlanReport
// --------------------
class TrainingPlanReport : public WeeklyReport
{
private:
    SessionStats stats;
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

    void toStream(ostream& os) const override
    {
        os << fixed << setprecision(2);
        os << "Training Plan"
            << " | Name=" << getPlayerName()
            << " | Level=" << levelToString(getLevel())
            << " | Focus=" << focus
            << " | Tech=" << techMins
            << " | Cond=" << condMins
            << " | Sleep=" << getSleepHours()
            << " | Ready=" << getReadinessScore();
    }

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
// RecoveryReport
// --------------------
class RecoveryReport : public WeeklyReport
{
private:
    SessionStats stats;
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

    void toStream(ostream& os) const override
    {
        os << fixed << setprecision(2);
        os << "Recovery"
            << " | Name=" << getPlayerName()
            << " | Level=" << levelToString(getLevel())
            << " | Fatigue=" << fatigue
            << " | RestDays=" << restDays
            << " | Tip=" << tip
            << " | Sleep=" << getSleepHours()
            << " | Ready=" << getReadinessScore();
    }

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
// ReportManager
// Week 8: recursive member function retained
// --------------------
class ReportManager
{
private:
    DynArray<WeeklyReport*> items;

    int countTypeRecursive(const std::string& type, int index) const
    {
        if (index >= items.getSize())
            return 0;

        WeeklyReport* p = items.at(index);
        int add = (p != nullptr && p->getType() == type) ? 1 : 0;

        return add + countTypeRecursive(type, index + 1);
    }

public:
    ReportManager(const ReportManager&) = delete;
    ReportManager& operator=(const ReportManager&) = delete;

    ReportManager(int initialCapacity = 2)
        : items(initialCapacity)
    {
    }

    ~ReportManager()
    {
        clear();
    }

    void clear()
    {
        for (int i = 0; i < items.getSize(); ++i)
        {
            WeeklyReport* p = items[i];
            delete p;
        }

        while (items.getSize() > 0)
            items.removeAt(items.getSize() - 1);
    }

    int getSize() const { return items.getSize(); }
    int getCapacity() const { return items.getCapacity(); }

    WeeklyReport* operator[](int index) const
    {
        return items.at(index);
    }

    bool add(WeeklyReport* p)
    {
        if (p == nullptr) return false;
        return items.pushBack(p);
    }

    bool removeAt(int index)
    {
        WeeklyReport* p = items.at(index);
        delete p;
        return items.removeAt(index);
    }

    ReportManager& operator+=(WeeklyReport* p)
    {
        this->add(p);
        return *this;
    }

    ReportManager& operator-=(int index)
    {
        this->removeAt(index);
        return *this;
    }

    int countReportsOfType(const std::string& type) const
    {
        return countTypeRecursive(type, 0);
    }

    void printAll() const
    {
        if (items.getSize() == 0)
        {
            cout << "\n(Manager) No reports saved yet.\n";
            return;
        }

        cout << "\n==============================\n";
        cout << " SAVED REPORTS (" << items.getSize() << ")\n";
        cout << "==============================\n";

        for (int i = 0; i < items.getSize(); ++i)
        {
            cout << "\n[#" << (i + 1) << "]\n";
            WeeklyReport* p = items[i];
            if (p != nullptr)
                p->print();
        }
    }
};

// --------------------
// TrainingLog
// Week 9: sessions changed from array to vector
// Added: sequential search, bubble sort, binary search
// --------------------
class TrainingLog
{
private:
    string name;
    int age;
    double sleepHours;

    vector<double> sessions;

    double totalTraining;
    double avgTraining;

    PlayerLevel level;
    double readinessScore;
    string advice;

    ReportManager manager;

    void computeTrainingStats()
    {
        totalTraining = 0.0;

        for (size_t i = 0; i < sessions.size(); ++i)
            totalTraining += sessions.at(i);

        avgTraining = (!sessions.empty() ? totalTraining / sessions.size() : 0.0);
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
        return SessionStats(static_cast<int>(sessions.size()), totalTraining, avgTraining);
    }

    void addLevelReport()
    {
        WeeklyReport* rpt = new LevelReport(
            name, age, level,
            sessions,
            totalTraining, avgTraining
        );

        rpt->setSleepHours(sleepHours);
        rpt->setReadinessScore(readinessScore);
        rpt->setAdvice(advice);

        manager += rpt;

        ofstream out("report.txt");
        if (out)
        {
            out << "WEEKLY PERFORMANCE REPORT (Week 9)\n";
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
            for (size_t i = 0; i < sessions.size(); ++i)
                out << left << setw(10) << (i + 1) << setw(15) << sessions.at(i) << "\n";
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

        manager += rpt;
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

        manager += rpt;
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

        try
        {
            manager -= idx;
            cout << "Deleted.\n";
        }
        catch (const std::exception& ex)
        {
            cout << "Invalid index. " << ex.what() << "\n";
        }
    }

public:
    TrainingLog()
        : manager(2)
    {
        name = "";
        age = 0;
        sleepHours = 0.0;

        sessions.clear();
        totalTraining = 0.0;
        avgTraining = 0.0;

        level = LEVEL_AMATEUR;
        readinessScore = 0.0;
        advice = "";
    }

    void setup()
    {
        cout << "Enter player's full name: ";
        getline(cin, name);

        age = getValidInt("Enter player's age: ", MIN_AGE);

        cout << fixed << showpoint << setprecision(2);

        int inputSessionCount;
        cout << "\nHow many training sessions did you have this week (1-" << MAX_SESSIONS << ")? ";
        while (!(cin >> inputSessionCount) || inputSessionCount < 1 || inputSessionCount > MAX_SESSIONS)
        {
            cin.clear();
            cin.ignore(INPUT_FLUSH, '\n');
            cout << "Invalid. Enter a number between 1 and " << MAX_SESSIONS << ": ";
        }
        cin.ignore(INPUT_FLUSH, '\n');

        sessions.clear();

        cout << "\nEnter training hours for each session:\n";
        for (int i = 0; i < inputSessionCount; ++i)
        {
            double hours = getValidDouble("  Session " + to_string(i + 1) + ": ", MIN_TRAINING_HOURS);
            sessions.push_back(hours);
        }

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

    bool addSession(double hours)
    {
        if (hours < 0.0) return false;
        if (static_cast<int>(sessions.size()) >= MAX_SESSIONS) return false;

        sessions.push_back(hours);
        computeTrainingStats();
        return true;
    }

    void setSleep(double hours)
    {
        sleepHours = clampMin(hours, 0.0);
        evaluateLevel();
    }

    // Week 9: sequential (linear) search
    int sequentialSearchSession(double target) const
    {
        for (size_t i = 0; i < sessions.size(); ++i)
        {
            if (sessions.at(i) == target)
                return static_cast<int>(i);
        }
        return -1;
    }

    // Week 9: bubble sort
    void sortSessionsBubble()
    {
        if (sessions.empty()) return;

        for (size_t pass = 0; pass < sessions.size() - 1; ++pass)
        {
            for (size_t i = 0; i < sessions.size() - 1 - pass; ++i)
            {
                if (sessions.at(i) > sessions.at(i + 1))
                {
                    double temp = sessions.at(i);
                    sessions.at(i) = sessions.at(i + 1);
                    sessions.at(i + 1) = temp;
                }
            }
        }

        computeTrainingStats();
    }

    // Week 9: binary search
    int binarySearchSession(double target)
    {
        sortSessionsBubble();

        int low = 0;
        int high = static_cast<int>(sessions.size()) - 1;

        while (low <= high)
        {
            int mid = (low + high) / 2;
            double midValue = sessions.at(mid);

            if (midValue == target)
                return mid;
            else if (midValue < target)
                low = mid + 1;
            else
                high = mid - 1;
        }

        return -1;
    }

    int getSessionCount() const { return static_cast<int>(sessions.size()); }
    double getTotalHours() const { return totalTraining; }
    double getAverageHours() const { return avgTraining; }
    PlayerLevel getLevel() const { return level; }
    string getAdvice() const { return advice; }

    double getSessionAt(int index) const
    {
        if (index < 0 || index >= static_cast<int>(sessions.size()))
            throw AppException("TrainingLog: invalid index in getSessionAt");
        return sessions.at(index);
    }

    int getSavedReportCountOfType(const std::string& type) const
    {
        return manager.countReportsOfType(type);
    }
};

// --------------------
// functions (UI/input)
// --------------------
void setConsoleColor()
{
    system("color 1E");
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

void printSessionsTable(const vector<double>& sessions)
{
    cout << "\nSession Breakdown:\n";
    cout << left << setw(10) << "#" << setw(12) << "Hours" << "\n";

    for (size_t i = 0; i < sessions.size(); ++i)
    {
        cout << left << setw(10) << (i + 1)
            << setw(12) << sessions.at(i) << "\n";
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
// ---------- Week 6: template function tests ----------
TEST_CASE("Template function: clampMin works for int")
{
    CHECK(clampMin<int>(-5, 0) == 0);
    CHECK(clampMin<int>(7, 0) == 7);
}

TEST_CASE("Template function: clampMin works for double")
{
    CHECK(clampMin<double>(-2.5, 0.0) == doctest::Approx(0.0));
    CHECK(clampMin<double>(3.2, 0.0) == doctest::Approx(3.2));
}

// ---------- Week 6: template class tests that still apply ----------
TEST_CASE("Template class: DynArray<int> stores and resizes")
{
    DynArray<int> a(1);
    CHECK(a.getSize() == 0);
    CHECK(a.getCapacity() == 1);

    CHECK(a.pushBack(10) == true);
    CHECK(a.getSize() == 1);

    CHECK(a.pushBack(20) == true);
    CHECK(a.getSize() == 2);
    CHECK(a.getCapacity() >= 2);
    CHECK(a[0] == 10);
    CHECK(a[1] == 20);
}

TEST_CASE("Template class: DynArray<int> removeAt shifts")
{
    DynArray<int> a(2);
    a.pushBack(1);
    a.pushBack(2);
    a.pushBack(3);

    CHECK(a.getSize() == 3);
    CHECK(a.removeAt(1) == true);
    CHECK(a.getSize() == 2);
    CHECK(a[0] == 1);
    CHECK(a[1] == 3);
}

// ---------- Week 7: template class throws ----------
TEST_CASE("Week 7: DynArray throws on invalid index")
{
    DynArray<int> a(1);
    a.pushBack(10);

    CHECK_THROWS_AS(a.at(-1), AppException);
    CHECK_THROWS_AS(a.at(1), AppException);
    CHECK_THROWS_AS(a[-1], AppException);
}

TEST_CASE("Week 7: DynArray throws on invalid removeAt")
{
    DynArray<int> a(1);
    a.pushBack(10);

    CHECK_THROWS_AS(a.removeAt(-1), AppException);
    CHECK_THROWS_AS(a.removeAt(1), AppException);
}

// ---------- existing tests that still apply ----------
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

    delete p1;
    delete p2;
}

// ---------- Week 6: operator== tests ----------
TEST_CASE("Equality operator: LevelReport equal objects")
{
    vector<double> s1 = { 2.0, 4.0 };
    vector<double> s2 = { 2.0, 4.0 };

    LevelReport a("Alex", 20, LEVEL_SEMI_PRO, s1, 6.0, 3.0);
    LevelReport b("Alex", 20, LEVEL_SEMI_PRO, s2, 6.0, 3.0);

    CHECK(a == b);
}

TEST_CASE("Equality operator: LevelReport not equal objects")
{
    vector<double> s1 = { 2.0, 4.0 };
    vector<double> s2 = { 3.0, 4.0 };

    LevelReport a("Alex", 20, LEVEL_SEMI_PRO, s1, 6.0, 3.0);
    LevelReport b("Alex", 20, LEVEL_SEMI_PRO, s2, 7.0, 3.5);

    CHECK(!(a == b));
}

// ---------- Week 6: operator<< tests ----------
TEST_CASE("Stream output: << prints one-line summary (polymorphic)")
{
    WeeklyReport* p = new RecoveryReport("Mike", 12, LEVEL_AMATEUR, SessionStats(1, 2.0, 2.0), "High", 2, "Aim for 8h sleep.");
    p->setSleepHours(6.5);
    p->setReadinessScore(10.0);

    ostringstream oss;
    oss << *p;

    CHECK(oss.str().find("Recovery | Name=Mike") != string::npos);

    delete p;
}

TEST_CASE("Stream output: << works for LevelReport")
{
    vector<double> s1 = { 2.0, 4.0 };
    WeeklyReport* p = new LevelReport("Alex", 20, LEVEL_SEMI_PRO, s1, 6.0, 3.0);
    p->setSleepHours(8.0);
    p->setReadinessScore(25.0);

    ostringstream oss;
    oss << *p;

    CHECK(oss.str().find("Level | Name=Alex") != string::npos);
    CHECK(oss.str().find("Sessions=2") != string::npos);

    delete p;
}

// ---------- Week 6/7: manager tests ----------
TEST_CASE("Indexing: operator[] valid index returns correct pointer")
{
    ReportManager m(1);
    WeeklyReport* a = new LevelReport();
    WeeklyReport* b = new LevelReport();

    m += a;
    m += b;

    CHECK(m.getSize() == 2);
    CHECK(m[0] == a);
    CHECK(m[1] == b);
}

TEST_CASE("Week 7: ReportManager operator[] throws on invalid index")
{
    ReportManager m(1);
    CHECK(m.getSize() == 0);

    CHECK_THROWS_AS(m[0], AppException);
    CHECK_THROWS_AS(m[-1], AppException);
}

TEST_CASE("Add operator: += increases size and preserves order")
{
    ReportManager m(1);

    WeeklyReport* a = new LevelReport();
    WeeklyReport* b = new LevelReport();

    CHECK(m.getSize() == 0);

    m += a;
    CHECK(m.getSize() == 1);
    CHECK(m[0] == a);

    m += b;
    CHECK(m.getSize() == 2);
    CHECK(m[0] == a);
    CHECK(m[1] == b);
}

TEST_CASE("Add/Remove operators: += adds, -= removes and shifts")
{
    ReportManager m(1);

    WeeklyReport* a = new LevelReport();
    WeeklyReport* b = new LevelReport();
    WeeklyReport* c = new LevelReport();

    m += a;
    m += b;
    m += c;

    CHECK(m.getSize() == 3);
    CHECK(m[0] == a);
    CHECK(m[1] == b);
    CHECK(m[2] == c);

    m -= 1;

    CHECK(m.getSize() == 2);
    CHECK(m[0] == a);
    CHECK(m[1] == c);
}

TEST_CASE("Week 7: ReportManager operator-= throws on invalid removal")
{
    ReportManager m(1);
    CHECK_THROWS_AS(m -= 0, AppException);
    CHECK_THROWS_AS(m -= -1, AppException);
}

// ---------- TrainingLog tests ----------
TEST_CASE("TrainingLog: addSession updates stats (vector)")
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

TEST_CASE("TrainingLog: addSession guard cases (negative is invalid)")
{
    TrainingLog log;

    CHECK(log.addSession(-1.0) == false);
    CHECK(log.getSessionCount() == 0);

    for (int i = 0; i < MAX_SESSIONS; ++i)
        CHECK(log.addSession(1.0) == true);

    CHECK(log.getSessionCount() == MAX_SESSIONS);
    CHECK(log.addSession(1.0) == false);
}

// ---------- Week 8: recursion tests ----------
TEST_CASE("Week 8: ReportManager recursion counts types correctly (empty)")
{
    ReportManager m(1);
    CHECK(m.countReportsOfType("Level") == 0);
    CHECK(m.countReportsOfType("Recovery") == 0);
    CHECK(m.countReportsOfType("Training Plan") == 0);
}

TEST_CASE("Week 8: ReportManager recursion counts types correctly (mixed)")
{
    ReportManager m(2);

    m += new LevelReport();
    m += new RecoveryReport();
    m += new RecoveryReport();
    m += new TrainingPlanReport();

    CHECK(m.countReportsOfType("Level") == 1);
    CHECK(m.countReportsOfType("Recovery") == 2);
    CHECK(m.countReportsOfType("Training Plan") == 1);
    CHECK(m.countReportsOfType("DoesNotExist") == 0);
}

// ---------- Week 9: vector / sequential search / sort / binary search ----------
TEST_CASE("Week 9: sequential search finds correct index")
{
    TrainingLog log;
    log.addSession(3.0);
    log.addSession(1.5);
    log.addSession(4.0);

    CHECK(log.sequentialSearchSession(3.0) == 0);
    CHECK(log.sequentialSearchSession(1.5) == 1);
    CHECK(log.sequentialSearchSession(4.0) == 2);
}

TEST_CASE("Week 9: sequential search returns -1 when not found")
{
    TrainingLog log;
    log.addSession(2.0);
    log.addSession(5.0);

    CHECK(log.sequentialSearchSession(7.0) == -1);
}

TEST_CASE("Week 9: sequential search on empty vector returns -1")
{
    TrainingLog log;
    CHECK(log.sequentialSearchSession(2.0) == -1);
}

TEST_CASE("Week 9: bubble sort orders vector ascending")
{
    TrainingLog log;
    log.addSession(4.0);
    log.addSession(1.0);
    log.addSession(3.0);
    log.addSession(2.0);

    log.sortSessionsBubble();

    CHECK(log.getSessionAt(0) == doctest::Approx(1.0));
    CHECK(log.getSessionAt(1) == doctest::Approx(2.0));
    CHECK(log.getSessionAt(2) == doctest::Approx(3.0));
    CHECK(log.getSessionAt(3) == doctest::Approx(4.0));
}

TEST_CASE("Week 9: bubble sort handles empty vector")
{
    TrainingLog log;
    log.sortSessionsBubble();

    CHECK(log.getSessionCount() == 0);
}

TEST_CASE("Week 9: bubble sort handles one element")
{
    TrainingLog log;
    log.addSession(2.5);

    log.sortSessionsBubble();

    CHECK(log.getSessionCount() == 1);
    CHECK(log.getSessionAt(0) == doctest::Approx(2.5));
}

TEST_CASE("Week 9: binary search finds element after sorting")
{
    TrainingLog log;
    log.addSession(4.0);
    log.addSession(1.0);
    log.addSession(3.0);
    log.addSession(2.0);

    int index = log.binarySearchSession(3.0);

    CHECK(index != -1);
    CHECK(log.getSessionAt(index) == doctest::Approx(3.0));
}

TEST_CASE("Week 9: binary search returns -1 when not found")
{
    TrainingLog log;
    log.addSession(4.0);
    log.addSession(1.0);
    log.addSession(3.0);

    CHECK(log.binarySearchSession(9.0) == -1);
}

TEST_CASE("Week 9: binary search on empty vector returns -1")
{
    TrainingLog log;
    CHECK(log.binarySearchSession(5.0) == -1);
}

TEST_CASE("Week 9: getSessionAt throws on invalid index")
{
    TrainingLog log;
    log.addSession(1.0);

    CHECK_THROWS_AS(log.getSessionAt(-1), AppException);
    CHECK_THROWS_AS(log.getSessionAt(1), AppException);
}

int main(int argc, char** argv)
{
    int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flags |= _CRTDBG_ALLOC_MEM_DF;
    _CrtSetDbgFlag(flags);

    int res = 0;
    {
        doctest::Context ctx;
        ctx.applyCommandLine(argc, argv);
        res = ctx.run();
    }

    int leaks = _CrtDumpMemoryLeaks();
    std::cout << "\nCRT DumpMemoryLeaks result = " << leaks << "\n";

    return res;
}
#endif