#include <iostream>
#include <vector>

using namespace std;

typedef vector<vector<int> > Matrix;

/* This function is used to give you the coordinate of the most top and left grid of the
 * 3x3 region, so you can locate yourself better in that same region. It is useful to use
 * this before accessing to this region.
 * Pre: Two integers that will contain the new coordinates
 * Post: The relative (0,0) of the 3x3 region for the selected coordinate
 */
void locateMostTopLeftRegion(int& x, int& y) {
	if ((x + 1) % 3 == 0)
		x = x - 2;
	else if ((x + 2) % 3 == 0)
		x--;
	if ((y + 1) % 3 == 0)
		y = y - 2;
	else if ((y + 2) % 3 == 0)
		y--;
}

/* Just a useful function that shows the content of a vector of bool
 * Pre: any vector of bool
 * Post: Prints the contents of the vector
 */
void show_vector(vector<bool> possible) {
	for (long unsigned int i = 0; i < possible.size(); i++) {
		cout << possible[i] << " ";
	}
	cout << endl;
}

/* This function will search for the numbers that are in the row, column and region of the
 * sudoku for the selected coordinate. It'll start by assuming that all values are possible
 * and will be discarding the non possible ones by setting to false the position of
 * the flag vector.
 * Pre: The sudoku itself and the coordinates we want to inspect.
 * Post: A bool vector of size 9, where each position will hold true when it's possible for
 * the sudoku table to have that value (position + 1). For example, if (flag[3] == true),
 * then in sudoku[x][y] can contain a 4.
 */
vector<bool> find_possible(Matrix sudoku, int x, int y) {
	vector<bool>flag(9, true);
	for (int i = 0; i < 9; i++) {
		if (sudoku[x][i] != 0) {
			flag[sudoku[x][i] - 1] = false;
		}
	}
	for (int i = 0; i < 9; i++) {
		if (sudoku[i][y] != 0) {
			flag[sudoku[i][y] - 1] = false;
		}
	}
	// Look up for 3x3 grid possible values
	locateMostTopLeftRegion(x, y);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (sudoku[x + i][y + j] != 0)
				flag[sudoku[x + i][y + j] - 1] = false;
		}
	}
	return flag;
}

/* This function is used to fill a sudoku from keyboard input.
 * Pre: A Matrix object.
 * Post: A sudoku table with all the values set up from the keyboard.
 */
void fill_sudoku(Matrix& sudoku) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			int n;
			cin >> n;
			sudoku[i][j] = n;
		}
	}
}

/* show_function prints the sudoku table in a pretty way.
 * Pre: A Matrix object
 * Post: A pretty print of the sudoku.
 */
void show_sudoku(Matrix sudoku) {
	cout << "   A B C   D E F   G H I" << endl;
	for (int x = 0; x < 9; x++) {
		cout << x + 1 << " ";
		for (int y = 0; y < 9; y++) {
			if (((y) % 3 == 0) && !((y == 8) || (y == 0))) cout << " |";
			cout << " " << ((sudoku[x][y] == 0) ? "." : to_string(sudoku[x][y]));
		}
		if (((x + 1) % 3 == 0) && (x != 8))
			cout << " " << endl << "  -------+-------+-------";
		else cout << " ";
		cout << endl;
	}
}

/* It is a function that checks the row x that the value can only be placed in the position x y.
 * Pre. A matrix object, x and y int (row,column) and int value.
 * Post. Return true if value only possibility is the x y coordinate row.
 */
bool only_row(Matrix sudoku, int x, int y, int value) {
	bool result = false;
	int i = 0;
	while (result == false && i < 9) {
		vector<bool> possible(9, true);
		possible = find_possible(sudoku, x, i);
		if (i != y && (sudoku[x][i] == 0)) result = possible[value - 1];
		i++;
	}
	return !result;
}

/* It is a function that checks the column x that the value can only be placed in the position x y.
 * Pre: A matrix object, x and y int (row,column) and int value.
 * Post: Return true if value only possibility is the x y coordinate column.
 */
bool only_column(Matrix sudoku, int x, int y, int value) {
	bool result = false;
	int i = 0;
	while (result == false && i < 9) {
		vector<bool> possible(9, true);
		possible = find_possible(sudoku, i, y);
		if (i != x && (sudoku[i][y] == 0)) result = possible[value - 1];
		i++;
	}
	return !result;
}

/* It is a function that solves the sudoku in the following way:
 * 1.For each empty sudoku box you need to get the possible numbers.
 * 2.There are two possibilities. Write the number if there is only one possibility and if there is more than one it will be looked at if any of the possible ones is obligatory to go in this box.
 * 3.The status of sudoku will be displayed at the end of all squares.
 * 4.If after changing all the sudoku boxes there has been any change, the process will be repeated.
 * 5.Sudoku resolution ends when there are no more changes.
 * Pre: A Matrix object
 * Post: A sudoku with all its available possibilities filled.
 */
void solve_sudoku(Matrix& sudoku) {
	Matrix copy_sudoku = sudoku;
	for (unsigned int x = 0; x < sudoku.size(); x++) {
		for (unsigned int y = 0; y < sudoku[x].size(); y++) {
			if (sudoku[x][y] == 0) {
				vector<bool> possible(9, true);
				possible = find_possible(sudoku, x, y);
				int counter = 0;
				vector<int> value;
				for (int i = 0; i < 9; i++) { 
					if (possible[i] == true) {
						counter++;
						value.push_back(i + 1);
					}
				}
				bool change = false;
				if (counter == 1) {
					sudoku[x][y] = value[0];
					change = true;
				}
				else {
					bool result = false;
					for (unsigned int j = 0; j < value.size(); j++) {
						result = only_row(sudoku, x, y, value[j]) || 
								 only_column(sudoku, x, y, value[j]);
						if (result == true) {
							sudoku[x][y] = value[j];
							change = true;
						}
					}
				}
				if (change) {
					string column = "A";
					if (y == 1) column = "B";
					else if (y == 2) column = "C";
					else if (y == 3) column = "D";
					else if (y == 4) column = "E";
					else if (y == 5) column = "F";
					else if (y == 6) column = "G";
					else if (y == 7) column = "H";
					else if (y == 8) column = "I";
					cout << "A la casella (" << x + 1 << "," << column << ") hi ha d'anar un " << sudoku[x][y] << endl;
				}			
			}
		}
	}
	show_sudoku(sudoku);
	cout << endl;
	if (copy_sudoku != sudoku) solve_sudoku(sudoku);
}

int main(void) {
	Matrix sudoku(9, vector<int>(9, 0));
	fill_sudoku(sudoku);
	Matrix initial = sudoku;
	char option;
	// Now wait for input for command
	while ((cin >> option) && (int(option) != 90)) {
		int x;
		int y;
		char cY;
		switch (int(option)) {
		case 65: // Option A
			// Show the possible values of the sudoku grid
			cin >> x;     // Expected input: [1, 9]
			cin >> cY;    // Expected input: [A, I]
			y = int(cY);  // Expected range: [65, 73]
			cout << x << cY << ": [";
			if ((x < 10) && (x > 0) && (y < 74) && (y > 64)) {
				y = y - 65; // Get the coordinate to 0-8 range for vector accessing.
				x--; // Range: [0, 8]
				if (sudoku[x][y] == 0) {
					vector<bool> possible(9, true);
					possible = find_possible(sudoku, x, y);
					string output;
					for (unsigned int i = 0; i < possible.size(); i++) {
						if (possible[i] == true) {
							output.append(to_string(i + 1));
							output.append(", ");
						}
					}
					if (!output.empty()) {
						output.erase(output.size() - 2, 2);
					}
					cout << output;
				}
			}
			cout << "]" << endl;
			break;
		case 66: // Option B
			cin >> x;     // Expected input: [1, 9]
			cin >> cY;    // Expected input: [A, I]
			y = int(cY);  // Expected range: [65, 73]
			int value;
			cin >> value;
			if ((x < 10) && (x > 0) && (y < 74) && (y > 64)) {
				y = y - 65; // Get the coordinate to 0-8 range for vector accessing.
				x--;        // Range: [0, 8]
				if (initial[x][y] == 0) {
					vector<bool> possible(9, true);
					possible = find_possible(sudoku, x, y);
					if (value < 10 && value > 0 && possible[value - 1]) {
						sudoku[x][y] = value;
					}
					else {
						cout << x + 1 << cY << ": " << value << " es un valor no possible" << endl;
					}
				}
				else {
					cout << x + 1 << cY << ": Casella no modificable" << endl;
				}
			}
			break;
		case 67: // Option C
			// Show the table
			show_sudoku(sudoku);
			break;
		case 82: //Option R
			solve_sudoku(sudoku);
			break;
		case 90: // Option Z
			// Finish sudoku
			break;
		}
	}
	return 0;
}
