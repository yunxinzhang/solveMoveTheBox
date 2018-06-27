// win10 ; vs2015
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
class Box {
public:
	int step;
	int N, M;
	char **box;
	string nextStep;
	bool **cbox;
	Box(const Box &c) {
		step = c.step;
		N = c.N;
		M = c.M;
		nextStep = c.nextStep;
		box = new char*[c.N];
		for (int i = 0; i < c.N; ++i) {
			box[i] = new char[c.M];
			for (int j = 0; j < c.M; ++j) {
				box[i][j] = c.box[i][j];
			}
		}
		cbox = new bool*[c.N];
		for (int i = 0; i < c.N; ++i) {
			cbox[i] = new bool[c.M];
			for (int j = 0; j < c.M; ++j) {
				cbox[i][j] = c.cbox[i][j];
			}
		}
		pre = NULL;
	}
	Box(string path) {
		pre = NULL;
		nextStep = "root";
		box = NULL;
		cbox = NULL;
		ifstream fin(path);
		if (!fin) {
			cout << "read file error." << endl;
			getchar();
		}
		string line;
		fin >> line;
		fin >> step >> N >> M;
		box = new char*[N];
		for (int i = 0; i < N; ++i) {
			box[i] = new char[M];
			for (int j = 0; j < M; ++j) {
				box[i][j] = ' ';
			}
		}
		cbox = new bool*[N];
		for (int i = 0; i < N; ++i) {
			cbox[i] = new bool[M];
			for (int j = 0; j < M; ++j) {
				cbox[i][j] = false;
			}
		}
		for(int i=0; i<N; ++i) {
			fin >> line;
			if (line.size() != M)cout << "file error: wrong width.";
			for (int j = 0; j < M; ++j) {
				box[i][j] = line.at(j);
			}

		}
		fin.close();
	}
	void print() {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < M; ++j) {
				cout << box[i][j];
			}
			cout << endl;
		}
	}
	bool win() {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < M; ++j) {
				if (box[i][j] != '.')return false;
			}
		}
		return true;
	}
	Box *pre;
	~Box(){
		for (int i = 0; i < N; ++i) {
			delete[] box[i];
		}
		delete box;
		for (int i = 0; i < N; ++i) {
			delete[] cbox[i];
		}
		delete cbox;
	}
	void swap(int x, int y, int newx, int newy) {
		char c = box[x][y];
		box[x][y] = box[newx][newy];
		box[newx][newy] = c;
	}
	void reset() {
		do {
			clear();
			drop();
		}while(clear())
		;
	}
	void drop() {
		for (int i = 0; i < M; ++i) {
			int mv = N - 1;
			int cur = N - 1;
			for (int j = N - 1; j >= 0; --j) {
				if (box[j][i] != '.')swap(j, i, cur--, i);
			}
		}
	}
	bool clear() {
		//cout << "clear" << endl;
		bool flag = false;
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < M; ++j) {
				if (j + 2 < M && box[i][j] == box[i][j + 1] && box[i][j] == box[i][j + 2] && box[i][j] != '.') {
					cbox[i][j] = cbox[i][j + 1] = cbox[i][j + 2] = true;
				}
				if (i + 2 < N && box[i][j] == box[i+1][j] && box[i][j] == box[i+2][j]&&box[i][j]!='.') {
					cbox[i][j] = cbox[i+1][j] = cbox[i+2][j] = true;
				}
			}
		}
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < M; ++j) {
				if (cbox[i][j]) {
					box[i][j] = '.';
					flag = true;
					cbox[i][j] = false;
				};
			}
		}
		return flag;
	}
};
pair<int, int> dir[3] = { {1,0},{0,1},{0,-1} };
bool solve(Box& b) {

	// choose two block to exchange;
	if (b.win()) {
		Box* mv = &b;
		cout << "答案由下至上看,图索引从0开始:" << endl;
		while (mv->pre != NULL) {
			cout << mv->nextStep << endl;
			mv = mv->pre;
		}
		return true;
	}
	if (b.step < 0 && !b.win())return false;
	for (int i = 0; i < b.N; ++i) {
		for (int j = 0; j < b.M; ++j) {
			if (b.box[i][j] != '.') {
				for (int k = 0; k < 3; ++k) {
					if (i + dir[k].first < b.N&&j + dir[k].second < b.M && b.step>0) {
						Box* newbox = new Box(b);
						newbox->nextStep = to_string(i) + "," + to_string(j) + "=>" + to_string(i + dir[k].first) + "," + to_string(j + dir[k].second);
						//cout << i + dir[k].first << j + dir[k].second << endl;
						newbox->pre = &b;
						newbox->swap(i, j, i + dir[k].first, j + dir[k].second);

						newbox->reset();
						newbox->step--;
						if (solve(*newbox))return true;;
					}
					
				}
			}
		}
	}
	
	// move&消除-->new box
	// 递归solve(newbox)
	return false;
}

int main() {
	Box box("box.txt");
	box.drop();
	box.clear();
	box.print();
	solve(box);
	/*for (int i = 0; i < 3; ++i) {
		cout << dir[i].first << dir[i].second << endl;
	}
	Box c = Box(box);
	c.box[0][0] = 's';
	c.print();
	solve(box);
	cout << "------" << endl;
	box.print();*/
	getchar();
}