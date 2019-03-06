#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>

#define _WIN32_WINNT 0x501

#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

using namespace std;

WSADATA wsaData;

int main()
{
	int iResult;

	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
	} else {
		printf("WSAStartup success: %d\n", iResult);
		return 1;
	}

	addrinfo *result = NULL,
			*ptr = NULL,
			hints;

	ZeroMemory( &hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo("127.0.0.1", "27015", &hints, &result);
	if (iResult != 0) {
		printf("GetaddrInfo failed.");
		WSACleanup();
		return 1;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;
	ptr = result;

	//create socket
	ConnectSocket = socket(ptr->ai_family,
			ptr->ai_socktype,
			ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	//connect socket
	iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		 printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	char *sendBuf = "This is a test";
	char recvbuf[512];

	iResult = send(ConnectSocket, sendBuf, strlen(sendBuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("Sent failed");
		closesocket(ConnectSocket);
		WSACleanup();

		return 1;
	}

	printf("Bytes send :%d\n", iResult);

	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();

		return 1;
	}

	do {
		iResult = recv(ConnectSocket, recvbuf, 512, 0);
		if (iResult > 0) {
			printf("Bytes received: %d.", iResult);
		} else if (iResult == 0) {
			printf("Connection closed.");
		} else {
			printf("recv failed: %d.", WSAGetLastError());
		}
	} while (iResult > 0);

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}

/*void f(int &p) {
	cout<<p;
}

class base {
  public:
	int * p;
	int Y;
    base(int X)
    {
    	Y = X;
    	int v = 1;
    	p = &v;
    	//cout<<"Constructing base \n";
    }
    int f() {
    	cout<<Y;
    	return *p;
    }
    ~base()
    { //cout<<"Destructing base \n" + Y;
    }
};

class derived: public base {
  public:
	int * p;
    derived(int Y) : base(Y)
    {
    	int v = 2;
		p = &v;
    	//cout<<"Constructing derived \n";
    }
    int f() {
    		//cout<< base::Y;
        	return *p;
        }
    virtual ~derived()
    { //cout<<"Destructing derived \n";
    }
};

Keyboard::Keyboard()
    , init()
    , def()
    , shifted()
{
}

Keyboard::Keyboard() : CFsm(){

}

Keyboard::InitState::InitState() : State(0)
{

}

Keyboard::DefaultState::DefaultState() : State(1)
{

}

Keyboard::ShiftedState::ShiftedState() : State(2)
{

}


void Keyboard::InitState::dispatch(Event& event)
{
	cout << "Init was called" << endl;
	transition(def);
}

void Keyboard::DefaultState::dispatch(Event& event)
{
	cout << "Default was called" << endl;
	transition(shifted);
}

void Keyboard::ShiftedState::dispatch(Event& event)
{
	cout << "Shifted was called" << endl;
	transition(def);
}

class Memory_t {
public:
	Memory_t(const int* _i) : i(_i) {

	}
	const int* getChunk() const;

	const int* i;
};

void* pool;

const int*  Memory_t::getChunk() const{
	return i;
}

Memory_t& getMemPool()
{
	return *(static_cast<Memory_t*>(pool));
}

const int* getChunk()
{
   return static_cast<const int*>(getMemPool().getChunk());
}

class D_Memory_t : Memory_t {
	const int* getChunk() const;
};

int&  Memory_t::getChunk() const{
	return i;
}

class PriorityNode
{
public:
        int priority;
        int value;

        PriorityNode(int _priority, int value)
        : priority    (_priority)
        , value       (value)
        {
        }

        bool operator< (PriorityNode const& other) const
        {
                return priority > other.priority;
        }

        bool operator> (PriorityNode const& other) const
        {
                return priority < other.priority;
        }
};

int graph [5][5] = {}; //adj matrix
vector<pair<int, int> > adj[5];

void init() {
	cout << "nr. of nodes = 5 " << endl;

	graph[0][1] = 5;
	graph[0][4] = 1;

	graph[1][0] = 5;
	graph[1][2] = 6;
	graph[1][3] = 2;
	graph[1][4] = 3;

	graph[2][1] = 6;
	graph[2][3] = 3;

	graph[3][1] = 2;
	graph[3][2] = 3;
	//graph[3][4] = 5;

	graph[4][0] = 1;
	graph[4][1] = 3;
	//graph[4][3] = 5;

	adj[0].push_back(make_pair(1, 5));
	adj[0].push_back(make_pair(4, 1));

	adj[1].push_back(make_pair(0, 5));
	adj[1].push_back(make_pair(2, 6));
	adj[1].push_back(make_pair(3, 2));
	adj[1].push_back(make_pair(4, 3));

	adj[2].push_back(make_pair(1, 6));
	adj[2].push_back(make_pair(3, 3));

	adj[3].push_back(make_pair(1, 2));
	adj[3].push_back(make_pair(2, 3));
	//adj[3].push_back(make_pair(4, 5));

	adj[4].push_back(make_pair(0, 1));
	adj[4].push_back(make_pair(1, 3));
	//adj[4].push_back(make_pair(3, 5));


}

int done(int unvisited[]) {
	bool b = true;
	for (int i = 0; i< 5; i++) {
		if (unvisited[i] != 0) {
			b = false;
		}
	}

	return b;
}

int min(int dist[], int sptSet[])
{
   // Initialize min value
   int min = INT_MAX, min_index;

   for (int v = 0; v < 5; v++)
     if (sptSet[v] == 1 && dist[v] <= min)
         min = dist[v], min_index = v;

   return min_index;
}

void djikstra(int source, int target) {
	int unVisited[5] = {};
	int dist[5] = {};
	int prev[5] = {};

	for (int i = 0; i < 5; i++) {
		dist[i] = 256; //max

		unVisited[i] = 1; //1 means unvisited
	}

	dist[source] = 0;

	for (int i = 0; i < 5; i++) {
		int node = min(dist, unVisited);
		cout << "Visited Node :" << node << endl;
		unVisited[node] = 0;

		for (int v = 0; v < 5; v++) {
			if (unVisited[v] == 1 && graph[node][v] > 0) {
				if (dist[node] != 256 && dist[node] + graph[node][v] < dist[v]) {
					dist[v] = dist[node] + graph[node][v];
				}
			}
		}

		for (int i = 0; i < 5; i++)
		  printf("%d ", dist[i]);
		cout << endl;
	}

	printf("Vertex   Distance from Source\n");
	   for (int i = 0; i < 5; i++)
	      printf("%d tt %d\n", i, dist[i]);
}

int heuristic(int goalX, int goalY, int currX, int currY)
{
	return abs(goalX - currX) + abs(goalY - currY);
}
int x = 0x4000;
static int* g = ((int*)x);

bool setG()
{
	//g = ((int*)0x4000);
}

bool getG()
{
	int * p_search = g;
	if (*p_search++ == 0) {
		cout << "Mesh" << endl;
	}

}
int main()
{
	setG();
	getG();
	cout << "Mesh1" << endl;
}*/

/*

int main() {
	int value = 1;
	const int * ptr = &value;
	Memory_t m = Memory_t(ptr);
	pool = static_cast<void*>(&m);
	const int * val = getChunk();
	value = 2;
	//cout <<*val<<endl;
	//State initState = State();
	//CFsm fsm = CFsm();

	base b = derived(3);
	b.f();

	Keyboard board = Keyboard();
	Event ev;
	ev.code = 'T';
	board.dispatch(ev);
	if (&board.init == 0) {
		cout << "Meh";
	}
	for(;;) {
		char ch;
		cin >> ch;
		Event ev;
		ev.code = ch;

		switch (ev.code)
		{
			case '^': ev.sig = SHIFT_DEPRESSED_SIG; break;
			case '6': ev.sig = SHIFT_RELEASED_SIG; break;
			case '.': return 0;
			default: ev.sig = ANY_KEY_SIG; break;
		}

		board.dispatch(ev);
	}

	init();

	int start = 4;
	int goal = 2;
	std::priority_queue<PriorityNode > frontier;
	frontier.push(PriorityNode(0, start));

	vector<int> came_from(5, -1);
	vector<int> cost_so_far (5, -1);
	came_from[start] = -1;
	cost_so_far[start] = 0;

	while (!frontier.empty()) {
		PriorityNode curr = frontier.top();
		frontier.pop();

		if (curr.value == goal) {
			break;
		}

		for (int i = 0; i < 5; i++) {
			if (graph[curr.value][i] != 0) { //has value
				int new_cost = cost_so_far[curr.value] + graph[curr.value][i];
				if (cost_so_far[i] == -1 || new_cost < cost_so_far[i]) {
					cost_so_far[i] = new_cost;
					frontier.push(PriorityNode(new_cost, i));
					came_from[i] = curr.value;
				}

				//with manhattan distance
				if (came_from[i] == -1) {
					int priority = heuristic(curr, i)
				}
			}
		}
	}

	int curr = goal;
	Queue* path = new Queue();
	while (start != curr && curr != -1) {
		path->enqueue(curr);
		curr = came_from[curr];
	}

	path->enqueue(start);

	cout << "Path from " << start << " to " << goal << endl;
	while (!path->isEmpty()) {
		cout<< " " << path->dequeue();
	}

	//Queue* frontier = new Queue();
	//frontier->enqueue(0); //start from 0
		vector<int> visited(5);

	visited[3] = 1;

	//visit all
	while (!frontier->isEmpty()) {
		int current = frontier->dequeue();
		for (int i = 0; i < 5; i++) {
			if (graph[current][i] != 0 && !visited[i]) {
				frontier->enqueue(i);
				visited[i] = 1;
				cout<< "Visited node : "<< i<< endl;
			}
		}
	}


	vector<int> came_from(5, -1);

	int start = 4;
	int goal = 3;

	frontier->enqueue(start);
	came_from[start] = -1;

	while (!frontier->isEmpty()) {
		int current = frontier->dequeue();

		if (current == goal) {
			break;
		}

		for (int i = 0; i < 5; i++) {
			if (graph[current][i] != 0 && came_from[i] == -1) {
				frontier->enqueue(i);
				came_from[i] = current;
			}
		}

		for (int i = 0; i < 5; i++)
		{
			cout << came_from[i] << " ";
		}


		cout << endl;

	/*for (int i = 0; i < 5; i++)
	{
		cout << came_from[i] << " ";
	}

	int curr = goal;
	Queue* path = new Queue();
	while (start != curr && curr != -1) {
		path->enqueue(curr);
		curr = came_from[curr];
	}

	path->enqueue(start);

	cout << "Path from " << start << " to " << goal << endl;
	while (!path->isEmpty()) {
		cout<< " " << path->dequeue();
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cout << graph[i][j] << " ";
		}

		cout << endl;
	}

	int v, w;
	vector<pair<int, int> >::iterator it;
	    for (int u=0; u<5; u++) {
	        cout << "Node u=" << u << " has the following neighbors:\n";

	        for (it=adj[u].begin(); it!=adj[u].end(); it++) {
	            v = it->first;
	            w = it->second;
	            cout << "\tNode v=" << v << " with edge weight w=" << w << "\n";
	        }

	        cout << "\n";
	    }

	//djikstra(0,3);

	return 0;
}
*/
