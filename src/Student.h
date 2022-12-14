//
// Created by Joao Ribeiro on 19/10/2022.
//

#ifndef PROJ1_STUDENT_H
#define PROJ1_STUDENT_H

#include <string>
#include <list>

#include "ClassUC.h"

using namespace std;

/**
 * Represents a student in the system
 */
class Student {
    int id;
    string name;
    size_t numClasses = 0;
    list<ClassUC> classes;

public:
    /**
     * Creates a new student object
     * @param id Internal ID of the student
     * @param n Name of the student
     * @param c List of ClassUC the student is enrolled in
     */
    Student(int id, string n, list<ClassUC> & c);

    /**
     * Creates a new student object
     * @param id Internal ID of the student
     */
    explicit Student(int id);

    /**
     * Creates a new Student object
     * @param id Internal ID of the student
     * @param nc Number of classes the student is enrolled in
     */
    Student(int id, int nc);

    /**
     * Creates a new Student object
     * @param s Reference to an existing Student object
     */
    Student(const Student& s);

    /**
     * Returns the internal ID of the student
     * @return The internal ID of the student
     */
    int getId() const;

    /**
     * Assigns a new internal ID to the student
     * @param newId The new internal ID to assign
     */
    void setId(int newId);

    /**
     * Returns the name of the student
     * @return The name of the student
     */
    string getName() const;

    /**
     * Assigns a new name to the Student
     * @param newName The new name to assign
     */
    void setName(const string& newName);

    /**
     * Returns a list of the ClassUCs the student is enrolled in
     * @return List of the ClassUCs the student is enrolled in
     */
    list<ClassUC> getClasses() const;

    /**
     * Returns the number of classes a student is enrolled in
     * @return Number of classes a student is enrolled in
     */
    size_t getNumClasses() const;

    /**
     * Less operator for Student objects
     * @param s2 Reference to another student object
     * @return True if compared Student is less than s2, false otherwise
     */
    bool operator <(const Student& s2) const;

    /**
     * Equality operator for student objects
     * @param s2 Reference to another student object
     * @return True if compared Students are equal, false otherwise
     */
    bool operator ==(const Student& s2) const;

    /**
     * Adds a new class to the Student's list of classes
     * @param newClass The new class to be added
     */
    void addClassUC(const ClassUC& newClass);

    /**
     * Removes a class from the Student's list of classes
     * @param classToRemove Class to be removed
     */
    void removeClassUC(const ClassUC& classToRemove);
};


#endif //PROJ1_STUDENT_H
