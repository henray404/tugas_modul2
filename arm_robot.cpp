#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <conio.h> 
#include <iomanip> 
#include <algorithm> 

using namespace std;

class robotArm {
    private:
        double L1;
        double L2;
        vector<double> theta;

    public:
        robotArm(double L1, double L2) {
            this->L1 = L1;
            this->L2 = L2;

        }

        
void calculate(double distance) {
    if (distance > (L1 + L2) && distance < 0) {
            cout << "Target unreachable." << endl;
            return;
        }
       double cosTheta2 = (distance * distance - L1 * L1 - L2 * L2) / (2 * L1 * L2);
        double theta2 = acos(cosTheta2);

        double sinTheta2 = sqrt(1 - cosTheta2 * cosTheta2);
        double theta1 = atan2(0, distance) - atan2(L2 * sinTheta2, L1 + L2 * cosTheta2);

    theta1 = -1 * (theta1 * 180.0 / M_PI);

    theta2 = -1* ((theta2 * 180.0 / M_PI)   - 180.0);

    theta.push_back(theta1);
    theta.push_back(theta2);
}



        void logArmPositions(const string& filename, double distance) const {
    ofstream outfile(filename, ios::app);

    if (!outfile) {
        cerr << "Unable to open file" << endl;
        return;
    }

    // Log the most recent pair of angles (theta1 and theta2) and the current distance
    if (theta.size() >= 2) {
        outfile << "Current distance: " << distance << "\n";
        outfile << "Joint 1: " << theta[theta.size() - 2] << " degrees\n";
        outfile << "Joint 2: " << theta[theta.size() - 1] << " degrees\n";
        outfile << "-----\n";  // Separator between logs
    }

    outfile.close();
}

};

void controlArm(robotArm& arm, double L1, double L2) {
    char input;
    double distanceChange = sqrt(L1 * L1 + L2 * L2);

    while (true) {
        if (_kbhit()) {
            input = _getch();
            switch (input) {
                case 'w':
                    distanceChange += 5.0;
                    break;
                case 's':
                    distanceChange -= 5.0;
                    break;
                case 'q':
                    return;
                default:
                    continue;
            }

            cout << "Current distance: " << distanceChange << endl;
            arm.calculate(distanceChange);
            arm.logArmPositions("arm_robot.txt", distanceChange);
        }
    }
}

int main() {
    robotArm arm(100, 100);  // Initialize arm with lengths of 100 and 100 for both joints
    controlArm(arm, 100, 100);

    return 0;
}
