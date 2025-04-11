#include <iostream>
#include <string.h>
#include <queue>
#include "solve_equation.h"
#include <map>
#include <fstream>
#include <vector>

#define A_student 100
#define B_student 60
#define F_student 0

using namespace std;


struct task {
    complex<double> a;
    complex<double> b;
    complex<double> c;
    solution* student_answer;
    string name;
};

vector<vector<complex<double>>> read_koef(const char* filename) {
    ifstream in(filename);
    vector<vector<complex<double>>> koefs;

    if (in.is_open())
    {
        complex<double> a, b, c;
        while (in >> a >> b >> c) {
            koefs.push_back({ a, b, c });
        }
    }
    in.close();
    return koefs;
}

map<string, string> read_student_level(const char* filename) {
    ifstream in(filename);
    map<string, string> students;

    if (in.is_open())
    {
        string name, grade;
        while (in >> name >> grade) {
            students[name] = grade;
        }
    }
    in.close();
    return students;
}

class Teacher {
private:
    queue<task> all_tasks;
public:
    void add_to_queue(task task) {
        all_tasks.push(task);
    }

    map<string, int> check_task() {

        map<string, int> student_list;

        while (!all_tasks.empty()) {
            solution* teacher_answer = solve(all_tasks.front().a, all_tasks.front().b, all_tasks.front().c);
            solution* st_answer = all_tasks.front().student_answer;

            if (student_list.find(all_tasks.front().name) == student_list.end())
                student_list[all_tasks.front().name] = 0;

            if (teacher_answer->x == st_answer->x && teacher_answer->x == st_answer->x) {
                student_list[all_tasks.front().name]++;
            }
            all_tasks.pop();
        }
        return student_list;
    }
};

class Student {
private:
    double probability;
public:
    string name;

    Student(string name, double probability) : name(name), probability(probability) {};

    void complete_task(complex<double> a, complex<double> b, complex<double> c, Teacher& teacher) {
        task* cur_task = new task();
        cur_task->student_answer = new solution();
        cur_task->a = a;
        cur_task->b = b;
        cur_task->c = c;
        cur_task->name = name;

        if ((rand() % 100) > probability) {
            cur_task->student_answer->x.push_back(C0);
        }
        else {
            cur_task->student_answer = solve(a, b, c);
        }
        teacher.add_to_queue(*cur_task);
    }
};


int main(int argc, const char* argv[]) {

    map<string, double> grades_letters = {
        {"A", A_student},
        {"B", B_student},
        {"F", F_student},
    };

    Teacher teacher;

    vector<vector<complex<double>>> A = read_koef("koef.txt");
    map<string, string> student_level = read_student_level("students.txt");

    vector<Student> all_students;

    for (auto i : student_level)
        all_students.push_back(Student(i.first, grades_letters[i.second]));

    for (auto j : all_students)
        for (int i = 0; i < A.size(); i++)
            j.complete_task(A[i][0], A[i][1], A[i][2], teacher);

    map<string, int> student_perfomance_table = teacher.check_task();

    ofstream out;
    out.open("perfomance_table.txt");
    if (out.is_open()) {
        for (auto i : student_perfomance_table) {
            out << i.first << " " << i.second << endl;
        }
    }
    out.close();

    return 0;
}
