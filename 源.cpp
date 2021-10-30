#include<iostream>
/*The STL stack is only used to store the backtracing path and plot to accelerate the excution.
All algorithm implementations is based on self-defined mystack.*/
#include<stack>
#include"myqueue.h"
#include"mystack.h"
#include<stdlib.h>
using namespace std;

//Location information of each nodes
struct Loc
{
public:
	int x, y;//Denote the location of nodes.
	int index;//Record the index of nodes when using queue according to their sequence of popping out.
	int origin;//Record the origin of nodes, which is, the node been poping out when this node is added into the queue.
};

//Maze map information
class Maze
{
public:
	int nroom;//Denote the number of rooms.
	int* n, * m;//n represent the rows of a single room, m represents the columns.
	Loc* start, * end;//The start and ending position of each room.
	char*** maze;//To store the whole room map.
	void DeleteMaze();
	void CreateMaze();
};

//Store the pass path of each maze room
class Realpath
{
public:
	int nroom;
	stack<Loc>* path;//store the real path of each room 
	void showpath(const Maze m);
};


//Maze escape algorithm with stack
int mazepath_stack(Maze m, int no, Realpath& p);

//Maze escape algorithm with stack
int mazepath_queue(Maze m, int no, Realpath& p);


int main(int argc, char** argv)
{
	Maze m; //Instantiate a maze
	m.CreateMaze();//Read maze from file
	Realpath p;//Store the final path
	p.nroom = m.nroom;
	p.path = new stack<Loc>[m.nroom];
	int sumindi = 0;//Record the number of rooms successfully passed

	//Determine if the number of cmd input parameters is correct
	if (argc != 2)
	{
		cout << "Arguments number does not equal to 2" << endl;
		exit(1);
	}

	//Judge whether the number of input parameters is legal
	if ((strcmp(argv[1], "-Stack") != 0) && (strcmp(argv[1], "-Queue") != 0))
	{
		cout << "arguments should be chosen from '-Stack' and '-Queue'.";
		exit(1);
	}

	if (strcmp(argv[1], "-Stack") == 0)
	{
		for (int j = 0; j < m.nroom; j++)
		{
			sumindi = sumindi + mazepath_stack(m, j, p);
		}
	}

	if (strcmp(argv[1], "-Queue") == 0)
	{
		for (int j = 0; j < m.nroom; j++)
		{
			sumindi = sumindi + mazepath_queue(m, j, p);
		}
	}

	if (sumindi == m.nroom)//If all rooms are successfully passed, output the escape route
	{
		//cout << "escape success!" << endl;
		p.showpath(m);
	}
	else
	{
		cout << "Escape Failed!" << endl;
	}
	m.DeleteMaze();//memory release
	return 0;
}

void Maze::DeleteMaze()
{
	for (int i = 0; i < nroom; i++)
	{
		for (int j = 0; j < m[i]; j++)
		{
			delete maze[i][j];
		}
		delete maze[i];
	}
	delete maze;
}

//Construct the maze map from cin
void Maze::CreateMaze()
{
	try {
		cin >> nroom;//The first line of cin is the total number.
		maze = new char** [nroom];//the fitst dimension represent the number of room.
		n = new int[nroom];//represent the number of rows for each room.
		m = new int[nroom];//represent the number of columns for each room.
		start = new Loc[nroom];//store the start location of each maze room.
		end = new Loc[nroom];//store the exit location of each maze room.

		for (int i = 0; i < nroom; i++)
		{
			cin >> m[i] >> n[i];
			maze[i] = new char* [m[i]];//The second dimension is the rows of each maze room
			for (int j = 0; j < m[i]; j++)
			{
				maze[i][j] = new char[n[i]];//The second dimension is the columns of each maze room
				for (int k = 0; k < n[i]; k++)
				{
					cin >> maze[i][j][k];
					if ((maze[i][j][k] != 'I') && maze[i][j][k] != 'O' && maze[i][j][k] != 'T' && maze[i][j][k] != '.' && maze[i][j][k] != '@')//Judge if the input is legal or not;
					{
						cout << "Illegal input, please check your maze input file." << endl;
						exit(1);
					}
					if (maze[i][j][k] == 'I')
					{
						start[i].x = j;
						start[i].y = k;
						maze[i][j][k] = '.';
					}
					else if (maze[i][j][k] == 'O' || maze[i][j][k] == 'T')//record the start and exit location of each maze room
					{
						end[i].x = j;
						end[i].y = k;
						maze[i][j][k] = '.';
					}
				}
			}
		}
	}
	catch (...)//Capture all illegal inputs.
	{
		cout << "Illegal input, please check your maze input file." << endl;
	}
}

//Maze escape algorithm with stack
int mazepath_stack(Maze m, int no, Realpath& p)
{
	int indicator = 0;//denote if the room is escaped successfully with 1(succeed) and 0(failed)
	mystack<Loc>start;//the stack to implement DFS
	mystack<Loc>path;//the stack to store the nodes poped out from the start for backtracking
	m.start[no].origin = -1;//mark the the former nodes of start point as -1
	m.start[no].index = 0;//the index of the start poing is 0
	start.push(m.start[no]);
	Loc curloc, nextloc;
	m.maze[no][m.start[no].x][m.start[no].y] = 'P';//matk the start point as passed

	/* In every loop, while the start stack is not empty, pop out the top of the stack, and search the all possible next step, push them into the stack until we find the exit,
	and change the indicator as 1, if we don't find exit until the start stack is empty, then escape failed.*/
	while (!start.empty())
	{
		curloc = start.top();
		int x = curloc.x;
		int y = curloc.y;
		curloc.index = path.size();
		path.push(curloc);//store the nodes that's been poped out with index and origin node index.
		start.pop();

		//Judge if we find the exit.
		if (curloc.x == m.end[no].x && curloc.y == m.end[no].y)
		{
			indicator = 1;
			break;
		}

		//find all the possible way and judge if it's the exit, if so, break the loop.
		if (((x - 1) < m.m[no] && y < m.n[no] && (x - 1) >= 0 && y >= 0) && (m.maze[no][x - 1][y] == '.'))//check if we can go "North"
		{
			nextloc.x = x - 1;
			nextloc.y = y;
			nextloc.origin = path.size() - 1;
			m.maze[no][nextloc.x][nextloc.y] = 'P';//marked as passed.
			start.push(nextloc);//push into the start stack.
			if (curloc.x == m.end[no].x && curloc.y == m.end[no].y)
			{
				indicator = 1;
				path.push(nextloc);
				break;
			}
		}

		if (((x + 1) < m.m[no] && y < m.n[no] && (x + 1) >= 0 && y >= 0) && (m.maze[no][x + 1][y] == '.'))
		{
			nextloc.x = x + 1;
			nextloc.y = y;
			nextloc.origin = path.size() - 1;
			m.maze[no][nextloc.x][nextloc.y] = 'P';
			start.push(nextloc);
			if (curloc.x == m.end[no].x && curloc.y == m.end[no].y)
			{
				indicator = 1;
				path.push(nextloc);
				break;
			}
		}

		if ((x < m.m[no] && (y + 1) < m.n[no]) && (m.maze[no][x][y + 1] == '.'))
		{
			nextloc.x = x;
			nextloc.y = y + 1;
			nextloc.origin = path.size() - 1;
			m.maze[no][nextloc.x][nextloc.y] = 'P';
			start.push(nextloc);
			if (curloc.x == m.end[no].x && curloc.y == m.end[no].y)
			{
				indicator = 1;
				path.push(nextloc);
				break;
			}
		}

		if ((x < m.m[no] && (y - 1) < m.n[no] && x >= 0 && (y - 1) >= 0) && (m.maze[no][x][y - 1] == '.'))
		{
			nextloc.x = x;
			nextloc.y = y - 1;
			nextloc.origin = path.size() - 1;
			m.maze[no][nextloc.x][nextloc.y] = 'P';
			start.push(nextloc);
			if (curloc.x == m.end[no].x && curloc.y == m.end[no].y)
			{
				indicator = 1;
				path.push(nextloc);
				break;
			}
		}
	}

	//if find the way, backtracking the real path of exit and store it into stack, if don't, return 0.
	if (indicator == 0)
	{
		return indicator;
	}
	else
	{
		Loc cur = path.top();
		stack<Loc> real_path;
		real_path.push(cur);
		while (cur.origin != -1)//if don't find the start yet.
		{
			path.pop();
			if (path.top().index == cur.origin)
			{
				cur = path.top();
				real_path.push(cur);
			}
		}
		p.path[no] = real_path;
		return indicator;
	}
}

int mazepath_queue(Maze m, int no, Realpath& p)
{
	int indicator = 0;//Denote if find the right way.
	myqueue<Loc>start;//the queue to implement BFS
	mystack<Loc>path;//store the nodes that has been poped out.
	m.start[no].origin = -1;//mark the former node index of start point as -1.
	m.start[no].index = 0;//mark the index of start point as 0.
	start.push(m.start[no]);
	Loc curloc, nextloc;
	m.maze[no][m.start[no].x][m.start[no].y] = 'P';

	/*In every loop, while the start queue is not empty, pop out the front of the queue, and search the all possible next step, add them into queue, until we find the exit,
	and change the indicator as 1, if we don't find exit until the start stack is empty, then escape failed.*/
	while (!start.empty())
	{
		curloc = start.front();
		int x = curloc.x;
		int y = curloc.y;
		curloc.index = path.size();
		path.push(curloc);
		start.pop();

		if (curloc.x == m.end[no].x && curloc.y == m.end[no].y)
		{
			indicator = 1;
			break;
		}

		//find all the possible nodes.
		if (((x - 1) < m.m[no] && y < m.n[no] && (x - 1) >= 0 && y >= 0) && (m.maze[no][x - 1][y] == '.'))
		{
			nextloc.x = x - 1;
			nextloc.y = y;
			nextloc.origin = path.size() - 1;
			m.maze[no][nextloc.x][nextloc.y] = 'P';
			start.push(nextloc);
		}

		if (((x + 1) < m.m[no] && y < m.n[no] && (x + 1) >= 0 && y >= 0) && (m.maze[no][x + 1][y] == '.'))
		{
			nextloc.x = x + 1;
			nextloc.y = y;
			nextloc.origin = path.size() - 1;
			m.maze[no][nextloc.x][nextloc.y] = 'P';
			start.push(nextloc);
		}

		if ((x < m.m[no] && (y + 1) < m.n[no]) && (m.maze[no][x][y + 1] == '.'))
		{
			nextloc.x = x;
			nextloc.y = y + 1;
			nextloc.origin = path.size() - 1;
			m.maze[no][nextloc.x][nextloc.y] = 'P';
			start.push(nextloc);
		}

		if ((x < m.m[no] && (y - 1) < m.n[no] && x >= 0 && (y - 1) >= 0) && (m.maze[no][x][y - 1] == '.'))
		{
			nextloc.x = x;
			nextloc.y = y - 1;
			nextloc.origin = path.size() - 1;
			m.maze[no][nextloc.x][nextloc.y] = 'P';
			start.push(nextloc);
		}

	}

	if (indicator == 0)
	{
		return indicator;
	}
	else
	{
		Loc cur = path.top();
		stack<Loc> real_path;
		real_path.push(cur);
		while (cur.origin != -1)
		{
			path.pop();
			if (path.top().index == cur.origin)
			{
				cur = path.top();
				real_path.push(cur);
			}
		}
		p.path[no] = real_path;
		return indicator;
	}
}

void Realpath::showpath(Maze m)
{
	for (int i = 0; i < nroom; i++)
	{
		stack<Loc> real_path = path[i];
		Loc temploc;
		while (!real_path.empty())
		{
			temploc = real_path.top();
			real_path.pop();
			//transform the location sequence into directions.
			if (!real_path.empty())
			{
				if (temploc.x == real_path.top().x && (temploc.y - real_path.top().y) == 1) cout << 'W';
				else if (temploc.x == real_path.top().x && (temploc.y - real_path.top().y) == (-1))cout << 'E';
				else if ((temploc.x - real_path.top().x) == 1 && temploc.y == real_path.top().y)cout << 'N';
				else if ((temploc.x - real_path.top().x) == (-1) && temploc.y == real_path.top().y)cout << 'S';
			}
		}
		//if it's O, plot another step to escape out of the room.
		if (i != (nroom - 1))
		{
			if (m.end[i].x == 0) cout << 'N';
			else if (m.end[i].x == (m.n[i] - 1)) cout << 'S';
			else if (m.end[i].y == (m.n[i] - 1)) cout << 'E';
			else if (m.end[i].y == 0) cout << 'W';
		}
		cout << endl;
	}
}