#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

struct TestCase {
    long x1, y1; // Button A values
    long x2, y2; // Button B values
    long sumX, sumY; // Prize values
};

int main() {
    vector<TestCase> testCases;

    // Read input from cin
    while (true) {
        TestCase tc;
        if (!(cin >> tc.x1 >> tc.y1)) break; // Button A values
        if (!(cin >> tc.x2 >> tc.y2)) break; // Button B values
        if (!(cin >> tc.sumX >> tc.sumY)) break; // Prize values
        
        // Add 10000000000000 to the X and Y positions of every prize for Part 2
        tc.sumX += 10000000000000;
        tc.sumY += 10000000000000;
        testCases.push_back(tc);
        

    }
    long sum = 0;

    // Process each test case
    for (int i = 0; i < testCases.size(); i++) {
      
        Program lp(CGAL::SMALLER, true, 0, false, 0);
        const int A = 0;
        const int B = 1;

        // Constraints for exact X positioning
        lp.set_a(A, 0, testCases[i].x1); lp.set_a(B, 0, testCases[i].x2); lp.set_b(0, testCases[i].sumX);  // x1*A + x2*B <= sumX
        lp.set_a(A, 1, -testCases[i].x1); lp.set_a(B, 1, -testCases[i].x2); lp.set_b(1, -testCases[i].sumX); // x1*A + x2*B >= sumX

        // Constraints for exact Y positioning
        lp.set_a(A, 2, testCases[i].y1); lp.set_a(B, 2, testCases[i].y2); lp.set_b(2, testCases[i].sumY);  // y1*A + y2*B <= sumY
        lp.set_a(A, 3, -testCases[i].y1); lp.set_a(B, 3, -testCases[i].y2); lp.set_b(3, -testCases[i].sumY); // y1*A + y2*B >= sumY

        // Objective: Minimize 3*A + 1*B
        lp.set_c(A, 3);
        lp.set_c(B, 1);

        // Solve the linear program
        Solution s = CGAL::solve_linear_program(lp, ET());
        if (s.is_unbounded()) {
            cout << "unbounded" << "\n";
        } else if (s.is_infeasible()) {
            cout << "no" << "\n";
        } else {
            bool all_integers = true;
            for (auto it = s.variable_values_begin(); it != s.variable_values_end(); ++it) {
                if (it->denominator() != 1) {
                    all_integers = false;
                    break;
                }
            }
        
            if (all_integers) {
                auto obj_value = s.objective_value();
                if (obj_value.denominator() == 1) { // Check if the result is an integer
                    cout << obj_value.numerator() << "\n";
                    sum += obj_value.numerator().to_double();
                } else {
                    cout << "no" << "\n";
                }
            } else {
                cout << "no" << "\n";
            }
        }
    }
    cout << sum;

    return 0;
}
