//
// Created by Joao Ribeiro on 19/10/2022.
//

#include "ScheduleManager.h"
#include <iostream>
#include <fstream>
#include <string>

ScheduleManager::ScheduleManager() = default;

int wdtoi(const string& weekDay) {
    vector<string> wds{"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    for (int i = 0; i < wds.size(); i++) {
        if (weekDay == wds[i]) {
            return i;
        }
    }
}

ClassSchedule ScheduleManager::getClassSchedule(const ClassUC& classUC) {
    for (ClassSchedule & schedule: classSchedules)
        if (classUC.getCodeUC() == schedule.getCodeUC() && classUC.getCodeClass() == schedule.getCodeClass())
            return schedule;
}

int ScheduleManager::classScheduleExists(const string &codeClass, const string &codeUC) {
    int index = 0;
    for (ClassSchedule c: ScheduleManager::classSchedules) {
        if (c.getCodeClass() == codeClass && c.getCodeUC() == codeUC)
            return index;
        index++;
    }
    return -1;
}

int ScheduleManager::findYear(const string& codeClass) {
    int year = codeClass[0] - '0';
    return year;
}

void ScheduleManager::generateYears() {
    Year y1 = Year(1);
    Year y2 = Year(2);
    Year y3 = Year(3);
    years.push_back(y1);
    years.push_back(y2);
    years.push_back(y3);
}


void ScheduleManager::fillYearsList(const string& codeUC, const string& codeClass) {
    int yearNumber = findYear(codeClass);
    for (auto & year : years) {
        if (year.getNumber() == yearNumber) {
            auto it = year.findUC(codeUC);
            if (it != year.end()){
                Turma t1 = Turma(codeClass);
                it->addTurma(t1);
            } else {
                Turma t1 = Turma(codeClass);
                UC uc1 = UC(codeUC);
                uc1.addTurma(t1);
                year.addUC(uc1);
            }
        }
    }
}

void ScheduleManager::placeStudentInYears(int id, const string& name, const list<ClassUC> & classes) {
    StudentCard student = StudentCard(id, name);
    for (ClassUC classUC: classes) {
        int yearNumber = findYear(classUC.getCodeClass());
        for (auto & year: years) {
            if (year.getNumber() == yearNumber) {
               auto uc = year.findUC(classUC.getCodeUC());
               auto turma = uc->findTurma(classUC.getCodeClass());
               turma->addStudent(student);
            }
        }
    }
}

void ScheduleManager::removeStudentFromYears(int studentID, const ClassUC& classUC) {
    StudentCard student = StudentCard(studentID);
    int yearNumber = findYear(classUC.getCodeClass());
    for (auto & year: years) {
        if (year.getNumber() == yearNumber) {
            auto uc = year.findUC(classUC.getCodeUC());
            auto turma = uc->findTurma(classUC.getCodeClass());
            turma->removeStudent(student);
        }
    }
}

set<Student>::iterator ScheduleManager::findStudent(const int id) {
    Student s = Student(id);
    return students.find(s);
}

set<ClassUC>::iterator ScheduleManager::findClassUC(const string& codeUC, const string& codeClass) {
    ClassUC cuc = ClassUC(codeUC, codeClass);
    return classUCs.find(cuc);
}

void ScheduleManager::addOneToClass(const string& codeUC, const string& codeClass) {
    auto it = findClassUC(codeUC, codeClass);
    ClassUC classUC = (*it);
    classUCs.erase(it);
    classUC = classUC++;
    classUCs.insert(classUC);
}

void ScheduleManager::removeOneFromClass(const string& codeUC, const string& codeClass) {
    auto it = findClassUC(codeUC, codeClass);
    ClassUC classUC = (*it);
    classUCs.erase(it);
    classUC = classUC--;
    classUCs.insert(classUC);
}

bool ScheduleManager::ucHasBalance(const std::string &codeUC) {
    ClassUC searchStart = ClassUC(codeUC);
    ClassUC searchEnd = searchStart.nextUC();
    auto itStart = findClassUC(searchStart.getCodeUC(), searchStart.getCodeClass());
    auto itStart2 = itStart;
    auto itEnd = findClassUC(searchEnd.getCodeUC(), searchEnd.getCodeClass());
    itEnd--;
    while(itStart != itEnd) {
        while (itStart2 != itEnd) {
            int difference = abs(itStart2->getNumStudents() - itEnd->getNumStudents());
            if (difference >= 4)
                return false;
            itStart2++;
        }
        itStart2 = itStart;
        itEnd--;
    }
    return true;
}

bool ScheduleManager::isClassCompatible(const ClassUC& classUC, const Student& student) {
    list<ClassSchedule> schedule = getStudentSchedule(student.getId());
    ClassSchedule toCompare = getClassSchedule(classUC);
    for (ClassSchedule cs: schedule)
        if (cs.isCompatible(toCompare))
            return true;
    return false;
}

bool ScheduleManager::studentCanBePlaced(const Student& student, ClassUC& classUC) {
    if (isClassCompatible(classUC, student) && classUC.hasCapacity()) {
        classUC = classUC++;
        if (ucHasBalance(classUC.getCodeUC())) {
            classUC = classUC--;
            return true;
        }
        classUC = classUC--;
    }
    return false;
}

void ScheduleManager::readClassesFile(const string& fname){
    string line;
    ifstream file(fname);
    if (file.is_open()) {
        getline(file, line);
        string codeClass, codeUC, previousClass, previousUC, type, temp_string;
        int weekDay;
        float startHour, duration;
        list<Slot> slots;
        while (getline(file, line)) {
            stringstream inputString(line);
            getline(inputString, codeClass, ',');
            getline(inputString, codeUC, ',');
            getline(inputString, temp_string, ',');
            weekDay = wdtoi(temp_string);
            getline(inputString, temp_string, ',');
            startHour = stof(temp_string);
            getline(inputString, temp_string, ',');
            duration = stof(temp_string);
            getline(inputString, type, ',');
            Slot slot = Slot(weekDay, startHour, duration, type);
            int index = classScheduleExists(codeClass, codeUC);
            if (index != -1) {
                classSchedules[index].addSlot(slot);
            } else {
                slots.push_back(slot);
                ClassSchedule cs = ClassSchedule(codeUC, codeClass, slots);
                slots.clear();
                classSchedules.push_back(cs);
            }
        }
    } else
        cout << "Could not open the file" << endl;
}

void ScheduleManager::readClassesPerUC(const string& fname){
    string line;
    ifstream file(fname);
    if (file.is_open()) {
        getline(file, line);
        string codeUC, codeClass;
        while (getline(file, line)) {
            stringstream inputString(line);
            getline(inputString, codeUC, ',');
            getline(inputString, codeClass, ',');
            ClassUC c1 = ClassUC(codeUC, codeClass);
            classUCs.insert(c1);
            fillYearsList(codeUC, codeClass);
        }
    } else
        cout << "Could not open the file" << endl;
}

void ScheduleManager::readStudentsFile(const string& fname){
    string line;
    ifstream file(fname);
    if (file.is_open()) {
        getline(file, line);
        string name, temp_string, codeUC, codeClass;
        int current_id = 0;
        int previous_id = current_id;
        list<ClassUC> classes;
        classes.clear();
        while (getline(file, line)) {
            stringstream inputString(line);
            getline(inputString, temp_string, ',');
            current_id = stoi(temp_string);
            if (previous_id == 0) {
                getline(inputString, name, ',');
            } else if (previous_id == current_id) {
                getline(inputString, temp_string, ',');
            } else if (previous_id != current_id) {
                Student s1 = Student(previous_id, name, classes);
                students.insert(s1);
                placeStudentInYears(previous_id, name, classes);
                classes.clear();
                getline(inputString, name, ',');
            }
            previous_id = current_id;
            getline(inputString, codeUC, ',');
            getline(inputString, codeClass, ',');
            ClassUC c1 = ClassUC(codeUC, codeClass);
            classes.push_back(c1);
            addOneToClass(codeUC, codeClass);
        }
    } else {
        cout << "Could not open the file" << endl;
    }
}

void ScheduleManager::receiveRequest(Request& request) {
    this->requests.push(request);
}

queue<string> ScheduleManager::processRequests() {
    queue<string> replies;
    string reply;
    while (!requests.empty()) {
        reply = directRequest(requests.front());
        requests.pop();
        replies.push(reply);
    }
}

string ScheduleManager::directRequest(const Request & request) {
    string reply;
    switch (request.getType()) {
        case 1:
            reply = removeStudent(request);
            break;
        case 2:
            reply = addStudent(request);
            break;
        case 3:
            reply = changeStudentClass(request);
            break;
        case 4:
            reply = changeStudentClasses(request);
            break;
    }
    return reply;
}

string ScheduleManager::removeStudent(const Request& request) {
    // find the student
    int studentID = request.getStudentID();
    ClassUC classUC = request.getClassUC();
    auto it = findStudent(studentID);
    if (it == students.end())
        return "Error: Student not found in the system";
    else {
        // remove his entry in classes of the requested class/UC
        Student student = (*it);
        students.erase(it);
        student.removeClassUC(classUC);
        students.insert(student);
        removeOneFromClass(classUC.getCodeUC(), classUC.getCodeClass());
        removeStudentFromYears(studentID, classUC);
        return "Student successfully removed from the system";
    }
}

void ScheduleManager::undoRemoveStudent(const Request& request) {
    int studentID = request.getStudentID();
    ClassUC classUC = request.getClassUC();
    auto it = findStudent(studentID);
    Student student = (*it);
    students.erase(it);
    student.addClassUC(classUC);
    students.insert(student);
    addOneToClass(classUC.getCodeUC(), classUC.getCodeClass());
    list<ClassUC> classes;
    classes.push_back(classUC);
    placeStudentInYears(studentID, student.getName(), classes);
}


string ScheduleManager::addStudent(const Request& request) {
    // find the student
    int studentID = request.getStudentID();
    ClassUC classUC = request.getClassUC();
    // check for constraints of class capacity and schedule conflicts
    auto it = findStudent(studentID);
    if (it == students.end())
        return "Error: Student not found in the system";
    // if possible add student to class
    Student student = (*it);
    if (studentCanBePlaced(student, classUC)) {
        students.erase(it);
        student.addClassUC(classUC);
        students.insert(student);
        addOneToClass(classUC.getCodeUC(), classUC.getCodeClass());
        list<ClassUC> classes;
        classes.push_back(classUC);
        placeStudentInYears(studentID, student.getName(), classes);
        return "Student successfully placed in required class";
    } else {
        return "Student can't be placed in required class";
    }
}

string ScheduleManager::changeStudentClass(const Request& request) {
    int studentID = request.getStudentID();
    auto it = findStudent(studentID);
    if (it == students.end())
        return "Error: Student not found in the system";
    Student student = (*it);
    list<ClassUC> classesToAdd = request.getCurrentClasses();
    list<ClassUC> classesToRemove = request.getRequestedClasses();
    removeStudent(Request(request.getType(), request.getStudentID(), (*classesToRemove.begin())));
    if (addStudent(Request(request.getType(), request.getStudentID(), (*classesToAdd.begin()))) !=
        "Student successfully placed in required class") {
        undoRemoveStudent(Request(request.getType(), request.getStudentID(), classesToRemove));
        return "Student can't be placed in required class";
    } else
        return "Student has successfully changed class";
}

string ScheduleManager::changeStudentClasses(const Request& request) {
    // find the student
    int studentID = request.getStudentID();
    auto it = findStudent(studentID);
    if (it == students.end())
        return "Error: Student not found in the system";
    Student student = (*it);
    list<ClassUC> classesToAdd = request.getCurrentClasses();
    list<ClassUC> classesToRemove = request.getRequestedClasses();
    auto itAdd = classesToAdd.begin();
    auto itRemove = classesToRemove.begin();
    while (itAdd != classesToAdd.end()) {
        removeStudent(Request(request.getType(), request.getStudentID(), (*itRemove)));
        if (addStudent(Request(request.getType(), request.getStudentID(), (*itAdd))) !=
            "Student successfully placed in required class") {
            undoRemoveStudent(Request(request.getType(), request.getStudentID(), (*itRemove)));
            return "At least one of the requested changes couldn't be made";
        }
    }
    return "All the changes were done successfully";

}
bool sortClass(ClassUC a, ClassUC b) {
    return(a.getCodeClass()<b.getCodeClass());
}
list<ClassUC> ScheduleManager::listClassUCbyStudent(int studentid, int sort) {
    auto it= findStudent(studentid);
    list<ClassUC> l1 = (*it).getClasses();
    if(sort==1){
        l1.sort(sortClass);
    }
    return l1;
}

list<ClassSchedule> ScheduleManager::getStudentSchedule(int studentID) {
    auto it = findStudent(studentID);
    list<ClassUC> l = it->getClasses();
    list<ClassSchedule> res;
    for (ClassUC & uc: l)
        for (ClassSchedule & cs: classSchedules)
            if (uc.getCodeClass()==cs.getCodeClass() && uc.getCodeUC()==cs.getCodeUC())
                res.push_back(cs);
    return res;
}

/*
bool ScheduleManager::sortUCCode(ClassSchedule a, ClassSchedule b) {
    return(a.getCodeUC()<b.getCodeUC());
}

void ScheduleManager::orderByUCCode(){
    sort(classSchedules.begin(), classSchedules.end(), sortUCCode);
}

bool sortName(Student a, Student b) {
    return(a.getName()<b.getName());
}

void ScheduleManager::orderByName(){
    vector<Student> stemp;
    for (Student s: students){
        stemp.push_back(s);
    }
    sort(stemp.begin(), stemp.end(), sortName);
}
*/