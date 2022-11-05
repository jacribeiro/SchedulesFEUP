//
// Created by Joao Ribeiro on 19/10/2022.
//

#ifndef PROJ1_REQUEST_H
#define PROJ1_REQUEST_H

#include <string>
#include <list>

#include "ClassUC.h"

using namespace std;

class Request {

    int type;
    int studentID;
    list<ClassUC> currentClasses;
    list<ClassUC> requestedClasses;

public:
    // tipos de pedidos:
    // remover aluno de turma/UC
    // adicionar aluno a turma/UC
    // alterar turma/UC de aluno
    // alterar várias turmas/UC de aluno
    /**
     * Creates a new Request object
     * @param t Type of the request
     * @param id ID of the student making the request
     * @param cc List of the classes the student wants to change
     * @param rc List of the classes the student wants to start attending
     */
    Request(int t, int id, list<ClassUC> & cc, list<ClassUC> & rc);

    /**
     * Creates a new Request object
     * @param t Type of the request
     * @param id ID of the student making the request
     * @param cc List of the classes the student wants to change
     */
    Request(int t, int id, list<ClassUC> & cc);

    /**
     * Creates a new Request object
     * @param r1 Reference to an existing Request object
     */
    Request(const Request& r1);

    /**
     * Returns the type of the Request object
     * @return Type of the Request object
     */
    int getType() const;

    /**
     * Returns the ID of the student that made the request
     * @return ID of the student that made the request
     */
    int getStudentID() const;

    /**
     * Returns the list of classes the student wants to change
     * @return List of classes the student wants to change
     */
    list<ClassUC> getCurrentClasses() const;

    /**
     * Returns the list of classes the student wants to start attending
     * @return List of classes the student wants to start attending
     */
    list<ClassUC> getRequestedClasses() const;

    /**
     * Returns the first element in the currentClasses list
     * @return First element in currentClasses list
     */
    ClassUC getFirstClassUC() const;

};


#endif //PROJ1_REQUEST_H
