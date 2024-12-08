#include <iostream>
#include <fstream>
#include <string>
using namespace std;




class Edge {
public:
    int weight;
    char start, end;
    bool isBlocked;
    bool underConstruction;
    Edge* next;
    int NumOfVehicles; //added

    Edge(char s, char e, int w) : start(s), end(e), weight(w), isBlocked(false), underConstruction(false), next(nullptr),NumOfVehicles(0){}

    void IncreamentVehicles()
    {
        NumOfVehicles+=1;
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
        if(edge!=NULL)
        {
                
                cout << edge->start << " -> " << edge->end << " (" << edge->weight << ") "<< "{ " << edge->NumOfVehicles << " } ";
                edge = edge->next;
                displayEdge(edge);
            
        }
            
    }
};
class VehicalNode {
public:
    char start;
    char end;
    string id;
    VehicalNode* next;

    VehicalNode(char s, char e, const string& vehicleId)
        : start(s), end(e), id(vehicleId), next(nullptr) {}
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

class Graph 
{
private:
    Vertex* head;
    Vertex* tail;
    VehicalNode* Vhead;
    D_Algo * PathHead;

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
    
            run_dijkstra(temp->start,temp->end); D_Algo* x=getNodeOfDAlgoPath(temp->end, PathHead);
            string str=ShortestPath(temp->end, temp->end+to_string(x->weight),x);
            Vertex * temp=getVertex(str[0]);
            Edge * e=temp->head;
            while (e!=NULL)
            {
                if(e->end==str[1])
                {
                    e->NumOfVehicles+=1;
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
            cout<<temp->start<<" "<<temp->end<<" "<<temp->id<<" "<<endl;
            temp=temp->next;
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
        cout <<  str <<  endl;
    
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
        newNode->next = Vhead;
        Vhead = newNode;
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



};
int main() {
    int x=0;
    Graph cityGraph;

    //cityGraph.loadRoadNetwork("road_network.csv","road_closures.csv","traffic_signals.csv");

    // cityGraph.displayBlockedRoads();
    // cout<<"-----------------------------------------------------\n";
    // cityGraph.displayGraph(cityGraph.getHead());
    // cout<<"-----------------------------------------------------\n";
    // cityGraph.blockRoad();
    // cityGraph.displayBlockedRoads();
    // cout<<"-----------------------------------------------------\n";
    // cityGraph.displayGreenTime(cityGraph.getHead());

    return 0;
}