#include <iostream>
#include <fstream>
#include <string>
using namespace std;


// Remove numbers from path
string formatPath(const string& path){
    string formattedPath = path;
    for (char &c : formattedPath)
    {
        if (isdigit(c))
        {
            c = ' ';
        }
    }
    return formattedPath;
}

class VehicalNode {
public:
    char start;
    char end;
    string id;
    VehicalNode* next;
    string path;

    VehicalNode(char s, char e, const string& vehicleId,const string& path = "")
        : start(s), end(e), id(vehicleId), next(nullptr),path(path) {}
    
};

class Edge {
public:
    int weight;
    char start, end;
    bool isBlocked;
    bool underConstruction;
    Edge* next;
    int NumOfVehicles;
    VehicalNode* vehicals;

    Edge(char s, char e, int w) : start(s), end(e), weight(w), isBlocked(false), underConstruction(false), next(nullptr),NumOfVehicles(0),vehicals(nullptr){}

    // function to add vehicle as an object in edge with its shortest path
    void addVehicle(const string& vehicleID , const string& path){
        VehicalNode* newVehicle = new VehicalNode(path[0],path[path.length()-1],vehicleID,path);
        if(vehicals == nullptr){
            vehicals = newVehicle;
        }
        else{
            VehicalNode *temp = vehicals;
            while (temp->next != nullptr)
            {
                temp = temp->next;
            }
            temp->next = newVehicle;
        }
        NumOfVehicles++;
    }

    VehicalNode* removeVehicle(const string& vehicleID){
        VehicalNode *temp = vehicals;
        VehicalNode *prev = nullptr;
        while (temp != nullptr)
        {
            if (temp->id == vehicleID)
            {
                if (prev == nullptr)
                {
                    vehicals = temp->next;
                }
                else
                {
                    prev->next = temp->next;
                }
                NumOfVehicles--;
                return temp;
            }
            prev = temp;
            temp = temp->next;
        }
        return nullptr;

    }


    
};

class Vertex {
public:
    char id;
    int greenTime; 
    Edge* head;    
    Vertex* next;
    Edge * tail;
    Vertex(char v) : id(v), greenTime(0), head(nullptr), next(nullptr) {}
    
    void addEdge(char end, int weight, bool isBlocked = false, bool underConstruction = false) 
    {

        Edge* newEdge = new Edge(id, end, weight);
        
        if(head==nullptr)
        {
            head=tail=newEdge;
        }
        else
        {
        newEdge->isBlocked = isBlocked;
        newEdge->underConstruction = underConstruction;
        tail->next=newEdge;
        tail=newEdge;
        // newEdge->next = head;
        // head = newEdge;
        }


    }
    void displayEdge(Edge * edge)
    {
        if(edge!=NULL){
                
            cout << edge->start << " -> " << edge->end << " (" << edge->weight << ") "<< "{ " << edge->NumOfVehicles << " } ";
            edge = edge->next;
            displayEdge(edge);
            
        }
            
    }
};


class D_Algo {
public:
    char vertex;
    char parent;
    int weight;
    bool visited;
    D_Algo* next;

    // Constructor
    D_Algo(char v, char p = '-', int w = INT_MAX, bool vis = false)
        : vertex(v), parent(p), weight(w), visited(vis), next(nullptr) {}
};

class PathNode 
{
public:
    char vertex;
    PathNode* next;

    PathNode(char v) : vertex(v), next(nullptr) {}
};

class Stack {
public:

    PathNode* top;
    Stack() : top(nullptr) {}

    ~Stack() {
        while (top != nullptr) {
            PathNode* temp = top;
            top = top->next;
            delete temp;
        }
    }

    void push(char value) {
        PathNode* newNode = new PathNode(value);
        newNode->next = top;
        top = newNode;
    }

    char pop() {
        if (top == nullptr) {
            cout << "Stack is empty! Cannot remove anything.\n";
            return '\0';
        }
        char value = top->vertex;
        PathNode* temp = top;
        top = top->next;
        delete temp;
        return value;
    }

    bool FoundVertex(char value) {
        PathNode* current = top;
        while (current != nullptr) {
            if (current->vertex == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool stackIsEmpty() {
        return top == nullptr;
    }

    void DisplayStck(PathNode * root) {
        if(root==nullptr)
        {
            cout << endl; 
            return;
        }
        DisplayStck(root->next);
        cout << root->vertex << " ";

    }
};

class Max_Heap {
    Edge* heap[100];
    int size;

    void swap(int i, int j) {
        Edge* temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }

    Edge* getEdge(char s, char e) {
        for (int i = 0; i < size; i++) {
            if (heap[i]->start == s && heap[i]->end == e) {
                return heap[i];
            }
        }
        return nullptr;
    }

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    void heapifyUp(int i) {
        while (i > 0 && heap[parent(i)]->NumOfVehicles < heap[i]->NumOfVehicles) {
            swap(i, parent(i));
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
        int largest = i;
        int left = leftChild(i);
        int right = rightChild(i);

        if (left < size && heap[left]->NumOfVehicles > heap[largest]->NumOfVehicles) {
            largest = left;
        }

        if (right < size && heap[right]->NumOfVehicles > heap[largest]->NumOfVehicles) {
            largest = right;
        }

        if (largest != i) {
            swap(i, largest);
            heapifyDown(largest);
        }
    }

public:
    Max_Heap() {
        for (int i = 0; i < 100; i++) {
            heap[i] = nullptr;
        }
        size = 0;
    }

    bool processed() {
        for(int i=0;i<size;i++){
            VehicalNode* vs = heap[i]->vehicals;
            while (vs){
                if(vs->path.length() > 1){
                    return false;
                }
                vs = vs->next;
            }
        }
        return true;
    }

    void addEdge(Edge* edge) {
        if (size >= 100) {
            return;
        }
        heap[size] = edge;
        heapifyUp(size);
        size++;
    }

    Edge* extractMax() {
        if (size == 0) {
            return nullptr;
        }
        Edge* maxEdge = heap[0];
        heap[0] = heap[size - 1];
        // size--;
        heapifyDown(0);
        return maxEdge;
    }

    void routeVehicles() {
        if (size <= 0) {
            
            return;
        }
        Edge* maxEdge = extractMax();
        //cout<<maxEdge->start<< " "<<maxEdge->end<<endl;
        moveAllVehicles(maxEdge);
    }

    void moveVehicle(Edge* e1, VehicalNode* vehicle) {
        Edge* edge2 = getEdge(vehicle->path[1], vehicle->path[2]);
        if (edge2 == nullptr) {
            return;
        }
        VehicalNode* v = e1->removeVehicle(vehicle->id);
        vehicle->path = vehicle->path.substr(1);
        edge2->addVehicle(vehicle->id, vehicle->path);
    }

    void moveAllVehicles(Edge* edge) {
        VehicalNode* vehicles = edge->vehicals;
        while (vehicles) {
            if (vehicles->path.length() < 2) {
                vehicles = vehicles->next;
                continue;
            }
            moveVehicle(edge, vehicles);
            vehicles = vehicles->next;
        }
    }

    void printHeap() {
        for (int i = 0; i < size; i++) {
            cout << heap[i]->start << " " << heap[i]->end << " " << heap[i]->NumOfVehicles << " Index: " << i << " Parent: " << parent(i) << endl;
        }
    }
};


class Graph 
{
private:
    Vertex* head;
    Vertex* tail;
    VehicalNode* Vhead;
    D_Algo * PathHead;
    Max_Heap heap;

    Vertex* getVertex(char id) 
    {
        Vertex* temp = head;
        while (temp) {
            if (temp->id == id) 
            {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

void RoadNetwork(const string& filename) //FILE READ 1
{
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open the file: " << filename << "\n";
        return;
    }
    string header;
    getline(file, header);

    string line;
    while (getline(file, line)) 
    {
        string currentToken = "";
        string startVertexStr = "", endVertexStr = "", edgeWeightStr = "";
        int tokenIndex = 0;

        for (int j = 0; j <= line.length(); j++) 
        {
            if (line[j] == ',' || line[j] == '\0') {
                switch (tokenIndex) {
                    case 0:
                        startVertexStr = currentToken;
                        break;
                    case 1:
                        endVertexStr = currentToken;
                        break;
                    case 2:
                        edgeWeightStr = currentToken;
                        break;
                }
                tokenIndex++;
                currentToken = ""; 
            } else {
                currentToken += line[j];
            }
        }
        char startVertex = startVertexStr[0];
        char endVertex = endVertexStr[0];
        int edgeWeight = stoi(edgeWeightStr); 

        addEdge(startVertex, endVertex, edgeWeight);
    }

    file.close();
}
void RoadClosure(const string& filename)//FILE READ 2
{
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open the file: " << filename << "\n";
        return;
    }
    string header;
    getline(file, header);

    string line;
    while (getline(file, line)) 
    {
        string currentToken = "";
        string startVertexStr = "", endVertexStr = "";
        bool closed=0,UC=0;
        int tokenIndex = 0;

        for (int j = 0; j <= line.length(); j++) 
        {
            if (line[j] == ',' || line[j] == '\0') {
                switch (tokenIndex) {
                    case 0:
                        startVertexStr = currentToken;
                        break;
                    case 1:
                        endVertexStr = currentToken;
                        break;
                    case 2:
                        if(currentToken=="Blocked")
                        {
                            closed=1;
                        }
                        else if(currentToken=="Under Repair")
                        {
                            UC=1;
                        }
                        break;
                }
                tokenIndex++;
                currentToken = ""; 
            } else {
                currentToken += line[j];
            }
        }

        updateEdgeStatus(startVertexStr[0],endVertexStr[0],closed,UC);
    }

    file.close();
}

void GreenTime(const string& filename)//FILE READ 3
{
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open the file: " << filename << "\n";
        return;
    }
    string header;
    getline(file, header);

    string line;
    while (getline(file, line)) 
    {
        string currentToken = "";
        string VertexStr = " ";
        int GT;
        int tokenIndex = 0;

        for (int j = 0; j <= line.length(); j++) 
        {
            if (line[j] == ',' || line[j] == '\0') {
                switch (tokenIndex) {
                    case 0:
                        VertexStr = currentToken;
                        break;
                    case 1:
                        GT=stoi(currentToken);
                        break;
                }
                tokenIndex++;
                currentToken = ""; 
            } else {
                currentToken += line[j];
            }
        }
        Vertex * temp=getVertex(VertexStr[0]);
        if(temp!=NULL)
        {
           temp->greenTime=GT;
        }
    }

    file.close();
}
void Vehicles(const string& filename)//FILE READ 4
{
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open the file: " << filename << "\n";
        return;
    }
    string header;
    getline(file, header);

    string line;
    while (getline(file, line)) 
    {
        string currentToken = "";
        string startVertexStr = "", endVertexStr = "", VehicleIdStr = "";
        int tokenIndex = 0;

        for (int j = 0; j <= line.length(); j++) 
        {
            if (line[j] == ',' || line[j] == '\0') {
                switch (tokenIndex) {
                    case 0:
                        VehicleIdStr = currentToken;
                        break;
                    case 1:
                        startVertexStr = currentToken;
                        break;
                    case 2:
                         endVertexStr = currentToken;
                        break;
                }
                tokenIndex++;
                currentToken = ""; 
            } else {
                currentToken += line[j];
            }
        }
        char startVertex = startVertexStr[0];
        char endVertex = endVertexStr[0];
        string Vid = VehicleIdStr; 

        insertVehicleAtHead(startVertex, endVertex, Vid);

        // Vertex * temp=getVertex(startVertex);
        //     Edge * e=temp->head;
        //     while (e!=NULL)
        //     {
        //         if(e->end==startVertex+1)
        //         {
        //             e->NumOfVehicles+=1;
        //             break;
        //         }
        //         e=e->next;
        //     }

  
    }

    file.close();
}

public:
    Graph() : head(nullptr), tail(nullptr),Vhead(nullptr) 
    {
        loadRoadNetwork("road_network.csv","road_closures.csv","traffic_signals.csv","vehicles.csv");
        PathHead=nullptr;
        Vertex * temp=head;
        while(temp!=nullptr)
        {
            insertAtHead_DAlgo(temp->id);
            temp=temp->next;
        }
    }
    void printD_AlgoPath()
    {
        D_Algo * temp=PathHead;
        while(temp!=nullptr)
        {
            if(temp->weight!=INT_MAX)
                cout<< "Vertex: "<<temp->vertex<<" Parent: "<<temp->parent << " Weight: " << temp->weight <<endl;
            temp=temp->next;
        }
    }

    void feedMaxHeap(){
        Vertex* v = head;
        while (v){
            Edge* edges = v->head;
            while (edges) {
                heap.addEdge(edges);
                edges = edges->next;
            }
            v = v->next;
        }
    }
    void displayHeap(){
        heap.printHeap();
    }

    void routeVehicles(){

        displayVehiclesOnEdges();
        cout<<"\n-------------------"<<endl;
        heap.routeVehicles();
        if(heap.processed()){
            return;
        }
        else
            routeVehicles();

        
    }

    Vertex* getHead()
    {
        return this->head;
    }

    void addVertexHead(char id) {
        if (!getVertex(id)) {
            Vertex* newVertex = new Vertex(id);
            if(head==nullptr)
            {
                head=tail=newVertex;
            }
            else
            {
                tail->next=newVertex;
                tail=newVertex;
            }
            // newVertex->next = head;
            // head = newVertex;
        }
    }
    void InsertVehicleOnEdge()
    {
        VehicalNode * temp=Vhead;
        while(temp!=nullptr)
        {
    
            run_dijkstra(temp->start,temp->end);
            
            D_Algo* x=getNodeOfDAlgoPath(temp->end, PathHead);
            string str=ShortestPath(temp->end, temp->end+to_string(x->weight),x);
            Vertex * vertex=getVertex(str[0]);
            Edge * e=vertex->head;
            while (e!=NULL)
            {
                if(e->end==str[1])
                {   
                    e->addVehicle(temp->id,formatPath(str));
                    break;
                }
                e=e->next;
            }
            temp=temp->next;

        }
    }
    void displayVehicals()
    {
        VehicalNode*temp=Vhead;
        while(temp!=NULL)
        {
            cout<<temp->start<<" "<<temp->end<<" "<<temp->id<<" "<<temp->path<<endl;
            temp=temp->next;
        }

    }
    void displayVehiclesOnEdges(){
        Vertex *temp = head;
        while (temp != nullptr)
        {
            Edge *e = temp->head;
            while (e != nullptr)
            {
                cout << temp->id << " " << e->end << " " << e->NumOfVehicles << " ";
                VehicalNode* v_on_edge = e->vehicals;
                while (v_on_edge != nullptr){
                    cout << v_on_edge->id << " " << v_on_edge->path << " ";
                    v_on_edge = v_on_edge->next;
                }
                e = e->next;
                cout << endl;
            }
            temp = temp->next;
        }
    }

    void insertAtHead_DAlgo(char v)
    {
        D_Algo* newD = new D_Algo(v);
        newD->next=PathHead;
        PathHead=newD;
    }

    D_Algo* getNodeOfDAlgoPath(char v, D_Algo* root)//helper funcuuu
    {
        if(root==nullptr)
        {
            return nullptr;
        }
        else
        {
            if(root->vertex==v)
            {
                return root;
            }
            else
            {
                return getNodeOfDAlgoPath(v,root->next);
            }
        }
    }
    void run_dijkstra(char strV,char endV)
    {
        D_Algo * temp=PathHead;
        while(temp!=nullptr)
        {
            temp->parent='-';
            temp->weight=INT_MAX;
            temp->visited=0;
            temp=temp->next;
        }
        D_Algo*source=getNodeOfDAlgoPath(strV,PathHead);
        source->weight=0;
        dijkstra_algorithm(findMinUnvisited(PathHead),endV);
        D_Algo* x=getNodeOfDAlgoPath(endV, PathHead);
        string str=ShortestPath(endV, endV+to_string(x->weight),x);
    
    }

    void dijkstra_algorithm (D_Algo *root,char destination)
    {
        if(root==nullptr)
        {
            return;
        }
    if (root->vertex == destination) {
        root->visited = true; // Mark the endpoint as visited
        return;
    }
            char v=root->vertex;
            Vertex* vertex=getVertex(v);
            Edge *temp=vertex->head;
            while(temp!=nullptr )
            {
                if(temp->isBlocked==1 )//|| temp->underConstruction==1
                {
                    temp=temp->next;
                    continue;
                }
                int w=temp->weight;
                D_Algo * AdjNode=getNodeOfDAlgoPath(temp->end,PathHead);
                if(AdjNode->weight>(root->weight)+(w))
                {
                    AdjNode->weight=(root->weight)+(w);
                    AdjNode->parent=v;    
                }
                temp=temp->next;
                // if(temp->end==destination)
                // {

                // }
            }
            root->visited=1;
            D_Algo* x=findMinUnvisited(PathHead);
            dijkstra_algorithm(x,destination);
    }

    string ShortestPath(char endV,string str,D_Algo*root)
    {
        if(root->parent=='-')
        {
            return str;    
        }
        if(root==nullptr)
        {
            return "";
        }
        else
        {
            str=root->parent+str;
            D_Algo*next=getNodeOfDAlgoPath(root->parent,PathHead);
            return ShortestPath(root->parent,str,next);
        }
    }

    D_Algo* findMinUnvisited(D_Algo* head) {
        if (head == nullptr) {
            return nullptr; 
        }

        D_Algo* current = head;
        D_Algo* minNode = nullptr;
        int minWeight = INT_MAX; 

        while (current != nullptr) {
            if (!current->visited && current->weight < minWeight) 
            {
                minWeight = current->weight;
                minNode = current;
            }
            current = current->next;
        }

        return minNode;
    }

    void addVertex(char id) {
    if (!getVertex(id)) {
        Vertex* newVertex = new Vertex(id);
        if (head == nullptr) {
            head = newVertex;
            tail = newVertex;
        } else {
            tail->next = newVertex;
            tail = newVertex;
        }
    }
    }
    void insertVehicleAtHead(char start, char end, const string& vehicleId) 
    {
        VehicalNode* newNode = new VehicalNode(start, end, vehicleId);
        if (Vhead == nullptr)
        {
            Vhead = newNode;
        }
        else{
            VehicalNode* temp = Vhead;
            while (temp->next != nullptr){
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void addEdge(char start, char end, int weight, bool isBlocked = false, bool underConstruction = false) 
    {
        addVertexHead(start);
        addVertexHead(end);
        Vertex* startVertex = getVertex(start);
        Vertex* endVertex = getVertex(end);
        if (startVertex) {
            startVertex->addEdge(end, weight, isBlocked, underConstruction);
        }
        if (endVertex) {
            endVertex->addEdge(start, weight, isBlocked, underConstruction);
        }
    }

    bool updateEdgeStatus(char start, char end, bool isBlocked, bool underConstruction) {
        Vertex* startVertex = getVertex(start);
        if (startVertex) 
        {
            Edge* temp = startVertex->head;
            while (temp) {
                if (temp->end == end) {
                    temp->isBlocked = isBlocked;
                    temp->underConstruction = underConstruction;
                    return 1;
                }
                temp = temp->next;
            }
        }
        return 0;
    }

    void displayCongestion(){
        Vertex *vertexes = head;
        
        while (vertexes){
            int n = 0;
            Edge *edge = vertexes->head;
            while (edge)
            {
                n+=edge->NumOfVehicles;
                edge = edge->next;
            }
            
            cout<<"Vertex "<<vertexes->id<<" "<<n<<endl;
            vertexes = vertexes->next;
        }
        
    }

    void displayBlockedRoads() {
        Vertex* temp = head;
        cout << "Blocked Roads:\n";
        while (temp) {
            Edge* edge = temp->head;
            while (edge) {
                if (edge->isBlocked) {
                    cout << edge->start << " -> " << edge->end << " (" << edge->weight << ")\n";
                }
                edge = edge->next;
            }
            temp = temp->next;
        }
    }
    
    void displayGraph(Vertex * root)
    {
        if(root==NULL)
        {
            return;
        }
        else
        {
            
            cout << "Vertex " << root->id << ": ";
            root->displayEdge(root->head);
            cout << "\n";

            displayGraph(root->next); 
            
              
        }
    }
     void displayGreenTime(Vertex * root)
    {
        if(root==NULL)
        {
            return;
        }
        else
        {
            displayGreenTime(root->next); 
            cout << "Intersection " << root->id << ": " << "Green Time " << root->greenTime << "s"<<endl;
        
              
        }
    }

    void loadRoadNetwork(const string& filename,const string& filename2 ,const string& filename3,const string& filename4) //FILE READINGS
    {
        RoadNetwork(filename);
        RoadClosure(filename2);
        GreenTime(filename3);
        Vehicles(filename4);


    }

    void blockRoad()
    {
        char S,E;
        do{
        cout<<"Enter the start and End vertex : ";
        cin>>S>>E;
        }
        while((S<'A' && S>'Z') && (E<'A' && E>'Z'));
        if(updateEdgeStatus(S,E,1,0))
        {
            cout<< "Blocked the road from "<<S<<" to "<<E<<endl;
        }
        else
        {
            cout<< "Road not found from "<<S<<" to "<<E<<endl;
        }
        
    
}
void DFS(Vertex * root,char end,Stack& stck,int weight)
{
    if(root==nullptr)
    {
        return;
    }
    if(!stck.FoundVertex(root->id)) 
    {
        stck.push(root->id); 
    }

    if(stck.FoundVertex(end))        
    {
        stck.DisplayStck(stck.top);
        cout<<"--->  "<< weight;
        return;
    }
    
    Edge * adj=root->head;
    while(adj!=nullptr)
    {
        if(!stck.FoundVertex(adj->end))
        {
            DFS(getVertex(adj->end),end,stck,weight+adj->weight);
            stck.pop();
        }
        adj=adj->next;   
    }
}
void DisplayAllPossiblePaths(char start, char end)
{
    Vertex * Node =  getVertex(start);
    Stack stck;
    DFS(Node,end,stck,0);
}

    void menu() {
    int choice;
    while (true) {
        cout << "Enter your choice: " << endl;
        cout << "1: Display Graph." << endl;
        cout << "2: Display Blocked Roads" << endl;
        cout << "3: Display Green Time" << endl;
        cout << "4: Block a road Manually" << endl;
        cout << "5: Print the shortest path between 2 points" << endl;
        cout << "6: Display All Possible Paths" << endl;
        cout << "7: Exit" << endl;
        cin >> choice;

        if (choice < 1 || choice > 7) {
            cout << "Invalid choice. Please select a valid option.\n";
            continue;
        }

        if (choice == 1) {
            displayGraph(head);
        } else if (choice == 2) {
            displayBlockedRoads();
        } else if (choice == 3) {
            displayGreenTime(head);
        } else if (choice == 4) {
            blockRoad();
        } else if (choice == 5) {
            char start, end;
            cout << "Enter the start vertex: ";
            cin >> start;
            cout << "Enter the end vertex: ";
            cin >> end;
            run_dijkstra(start, end);
            printD_AlgoPath();
        } else if (choice == 6) {
            char start, end;
            cout << "Enter the start vertex: ";
            cin >> start;
            cout << "Enter the end vertex: ";
            cin >> end;
            DisplayAllPossiblePaths(start, end);
        } else if (choice == 7) {
            cout << "Exiting program.\n";
            return;
        }
    }
}

};

int main() 
{
    Graph cityGraph;
    // cityGraph.displayBlockedRoads();
    cityGraph.InsertVehicleOnEdge();
    // cout<<"-----------------------------------------------------\n";
    //cityGraph.displayGraph(cityGraph.getHead());
    //cityGraph.menu();
    cout<<"-----------------------------------------------------\n";
    // cityGraph.blockRoad();
    // cityGraph.displayBlockedRoads();
    // cout<<"-----------------------------------------------------\n";
    // cityGraph.displayGreenTime(cityGraph.getHead());
    // cout<<"-----------------------------------------------------\n";
    // cityGraph.printD_AlgoPath();
    // cout<<"-----------------------------------------------------\n";
           
    // cityGraph.displayGraph(cityGraph.getHead());

    // cityGraph.run_dijkstra('A','F');
    // // cityGraph.findAllPaths('A','F');
    // cout<<"-----------------------------------------------------\n";
    // cityGraph.DisplayAllPossiblePaths('A','F');

    // cityGraph.menu();
    //cityGraph.displayVehicals();
    //cityGraph.displayVehiclesOnEdges();
    cout<<"\n-------------------"<<endl;
    //cityGraph.displayCongestion();
    //cityGraph.displayVehicals();
    cityGraph.feedMaxHeap();
    //cityGraph.displayHeap();
    //cityGraph.displayCongestion();
    cityGraph.routeVehicles();
    //cityGraph.displayVehiclesOnEdges();
    cout<<"---------------------"<<endl;
    //cityGraph.displayCongestion();
    
return 0;
}