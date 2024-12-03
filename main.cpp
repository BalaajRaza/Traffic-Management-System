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

    Vertex(char v) : id(v), greenTime(0), head(nullptr), next(nullptr) {}
    
    void addEdge(char end, int weight, bool isBlocked = false, bool underConstruction = false) 
    {
        Edge* newEdge = new Edge(id, end, weight);
        newEdge->isBlocked = isBlocked;
        newEdge->underConstruction = underConstruction;
        newEdge->next = head;
        head = newEdge;
    }
    void displayEdge(Edge * edge)
    {
        if(edge!=NULL)
        {
                displayEdge(edge->next);
                cout << edge->start << " -> " << edge->end << " (" << edge->weight << ") ";
                edge = edge->next;
            
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


class Graph 
{
private:
    Vertex* head;
    Vertex * tail;
    VehicalNode* Vhead;

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
                        startVertexStr = currentToken;
                        break;
                    case 1:
                        endVertexStr = currentToken;
                        break;
                    case 2:
                        VehicleIdStr = currentToken;
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
    Vertex * temp=getVertex(startVertex);
    }

    file.close();
}

public:
    Graph() : head(nullptr), tail(nullptr),Vhead(nullptr) {}

    Vertex* getHead()
    {
        return this->head;
    }

    void addVertexHead(char id) {
        if (!getVertex(id)) {
            Vertex* newVertex = new Vertex(id);
            newVertex->next = head;
            head = newVertex;
        }
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
            displayGraph(root->next); 
            cout << "Vertex " << root->id << ": ";
            root->displayEdge(root->head);
            cout << "\n";
              
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

    void loadRoadNetwork(const string& filename,const string& filename2 ,const string& filename3) //FILE READINGS
    {
        RoadNetwork(filename);
        RoadClosure(filename2);
        GreenTime(filename3);

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
};

int main() {
    int x=0;
    Graph cityGraph;

    cityGraph.loadRoadNetwork("road_network.csv","road_closures.csv","traffic_signals.csv");

    cityGraph.displayBlockedRoads();
    cout<<"-----------------------------------------------------\n";
    cityGraph.displayGraph(cityGraph.getHead());
    cout<<"-----------------------------------------------------\n";
    cityGraph.blockRoad();
    cityGraph.displayBlockedRoads();
    cout<<"-----------------------------------------------------\n";
    cityGraph.displayGreenTime(cityGraph.getHead());

    return 0;
}