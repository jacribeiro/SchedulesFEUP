//
// Created by Joao Ribeiro on 18/10/2022.
//

#include "Slot.h"

Slot::Slot(int wd, float s, float e, string t): weekDay(wd), start(s), end(e), type(t) {}

Slot::Slot(const Slot& s1) {
    this->weekDay = s1.weekDay;
    this->start = s1.start;
    this->end = s1.end;
    this->type = s1.type;
}

int Slot::getWeekDay() { return this->weekDay; }

void Slot::setWeekDay(int newWeekDay) { this->weekDay = newWeekDay; }

float Slot::getStart() { return this->start; }

void Slot::setStart(float newStart) { this->start = newStart; }

float Slot::getDuration() { return this->duration; }

void Slot::setDuration(float newDuration) { this->duration = newDuration; }

string Slot::getType() { return this->type; }

void Slot::setType(string newType) { this->type = newType; }

bool Slot::operator==(const Slot &s2) const {
    return (this->weekDay == s2.weekDay &&
            this->start == s2.start &&
            this->duration == s2.duration &&
            this->type == s2.type);
}

string Slot::write() const {
    stringstream oss;
    string dayOfTheWeek = "";
    switch (weekDay) {
        case 0:
            dayOfTheWeek = "Sun";
            break;
        case 1:
            dayOfTheWeek = "Mon";
            break;
        case 2:
            dayOfTheWeek = "Tue";
            break;
        case 3:
            dayOfTheWeek = "Wed";
            break;
        case 4:
            dayOfTheWeek = "Thu";
            break;
        case 5:
            dayOfTheWeek = "Fri";
            break;
        case 6:
            dayOfTheWeek = "Sat";
            break;
    }
    oss << dayOfTheWeek << "\n" << start << "\n" << duration << "\n" << type;
    return oss.str();
}