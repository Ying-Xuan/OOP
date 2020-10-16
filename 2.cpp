#include<iostream>
#include<vector>
#include<list>
#include<queue>
using namespace std;

int num_vertex;

class node{

public:
    node(int id);
    node();
    void add_adge(int v);
    vector<int> get_neighbor();
    int get_distance(int id);
    int get_path(int id);
    void inactive();
    void IS_MIS();
    void IS_CDS();
    int get_active();
    int get_MIS();
    int get_CDS();
    void set_dist(int d[]);
    void set_path(int p[]);
    int sendTo(int dstID);
    void build_table(int dstID);
    int get_route(int dstID);

private:
    int ID;
    int active;     //=1, active; =0, inactive
    int MIS;        //=1, in MIS
    int CDS;        //=1, in CDS
    vector<int> neighbor;
    int distance[100000];
    int path[100000];
    int routing_table[100000];
};

node::node(int id):ID(id), active(1), MIS(0), CDS(0)
{

}

node::node():ID(-1)
{

}

void node::add_adge(int v)
{
    neighbor.push_back(v);
}

vector<int> node::get_neighbor()
{
    return neighbor;
}

int node::get_distance(int id)
{
    return distance[id];
}

int node::get_path(int id)
{
    return path[id];
}

void node::inactive()
{
    active=0;
}

void node::IS_MIS()
{
    MIS=1;
}

void node::IS_CDS()
{
    CDS=1;
}

int node::get_active()
{
    return active;
}

int node::get_MIS()
{
    return MIS;
}

int node::get_CDS()
{
    return CDS;
}

void node::set_dist(int d[])
{
    int i=0;
    for(i=0; i<num_vertex; i++)
        distance[i]=d[i];
}

void node::set_path(int p[])
{
    int i=0;
    for(i=0; i<num_vertex; i++)
        path[i]=p[i];
}

int node::sendTo(int dstID)
{
    int i=0;

    if(ID==dstID)
        return dstID;

    for(i=0; i<num_vertex; i++){
        if(path[i]==path[ID]+1){
            return i;
        }
    }
}

void node::build_table(int dstID)
{
    routing_table[dstID]=sendTo(dstID);
}

int node::get_route(int dstID)
{
    return routing_table[dstID];
}

void construct_MIS(node sensor[]);

void bfs(node sensor[], int start);

void deal_path(node sensor[], int start, int trail);

void construct_CDS(node sensor[]);

int find_closest_CDS(node sensor[], int idx);

int main()
{
    int i=0, j=0;
    int num_edge, num_pair;
    int idx, v1, v2, start, trail, new_trail;

    cin >> num_vertex >> num_edge;

    node *sensor;
    sensor=new node[num_vertex];

    for(i=0; i<num_vertex; i++)
        sensor[i]=node(i);

    for(i=0; i<num_edge; i++){
        cin >> idx >> v1 >> v2;
        sensor[v1].add_adge(v2);
        sensor[v2].add_adge(v1);
    }

    for(i=0; i<num_vertex; i++)     //convenience to construct CDS & path
        bfs(sensor, i);


    construct_MIS(sensor);

    construct_CDS(sensor);


    cin >> num_pair;
    for(i=0; i<num_pair; i++){

        cin >> idx >> start >> trail;

        cout << idx << ' ';
        if(sensor[start].get_CDS()==0){
                    //the node is not a proxy node, find the closest node in CDS
            cout <<start << ' ';
            start=find_closest_CDS(sensor, start);
        }


        if(sensor[trail].get_CDS()==0){
                    //the node is not a proxy node, find the closest node in CDS
            new_trail=find_closest_CDS(sensor, trail);
        }else
            new_trail=trail;


        deal_path(sensor, start, new_trail);

        cout <<start << ' ';
        while(start!=new_trail){        //output the route from start to new_trail
            j=sensor[start].get_route(new_trail);
            cout << j << ' ';
            start=j;
        }

        if(sensor[trail].get_CDS()==0)      //output the initial trail
            cout << trail;

        cout << endl;
    }

}

void construct_MIS(node sensor[])
{
    int i=0;
    int is_MIS=1;
    int conti=1;

    while(conti){       //continue until no active node

        for(i=0; i<num_vertex; i++){
            if(sensor[i].get_active()==1){      //if it is active, judge it is MIS or not
                vector<int> n=sensor[i].get_neighbor();
                is_MIS=1;   //assume the node is in MIS
                for(vector<int>::iterator it=n.begin(); it!=n.end(); it++){

                    if(sensor[*it].get_active()==1 && *it<i){   //has a neighbor, which its ID < the node i
                        is_MIS=0;    //this node is not in MIS
                        break;
                    }
                }
                if(is_MIS==1)
                    sensor[i].IS_MIS();
            }
        }

        for(i=0; i<num_vertex; i++){
            if(sensor[i].get_MIS()==1 && sensor[i].get_active()==1){
                    //All of neighbors of nodes in MIS are marked inactive
                sensor[i].inactive();
                vector<int> n=sensor[i].get_neighbor();
                for(vector<int>::iterator it=n.begin(); it!=n.end(); it++){
                    sensor[*it].inactive();
                }
            }
        }

        conti=0;
        for(i=0; i<num_vertex; i++){
            if(sensor[i].get_active()==1){
                 //there is a node is active, so continue to find nodes in MIS
                conti=1;
                break;
            }
        }
    }

}

void bfs(node sensor[], int start)
{

    int i, j, next_node, dist, mini=num_vertex;
    int* visited = new int[num_vertex];
    int* distance = new int[num_vertex];

    for(i=0; i<num_vertex; i++){
        visited[i]=0;       //=0, present it is not visited; =1, present it is visited
        distance[i]=0;
    }

    std::queue<int> q;
    visited[start]=1;
    distance[start]=0;

    q.push(start);
    while(!q.empty()){
        next_node=q.front();
        vector<int> n=sensor[next_node].get_neighbor();
        for(vector<int>::iterator it=n.begin(); it!=n.end(); it++){
            if(visited[*it]==0){
                visited[*it]=1;
                distance[*it]=distance[next_node]+1;
                q.push(*it);
            }
        }
        q.pop();
    }
    sensor[start].set_dist(distance);
}

void deal_path(node sensor[], int start, int trail)
{
    int i=0, j=0;
    int mini, dist;
    int* path = new int[num_vertex];    //if i is not in the route between start to trail, path[i]=-1
                                        //o.w. path[i] is th distance from start to trail

    for(j=0; j<num_vertex; j++)     //initialize
        path[j]=-1;

    j=trail;
    path[j]=sensor[start].get_distance(j);
    while(j!=start){

        mini=num_vertex;
        dist=sensor[start].get_distance(j);

        vector<int> n=sensor[j].get_neighbor();
        for(vector<int>::iterator it=n.begin(); it!=n.end(); it++){     //find the node with smallest ID
                                                                        //to be its last node in the route
            if(sensor[start].get_distance(*it)==dist-1 && *it<mini)
                mini=*it;
        }
        j=mini;
        path[j]=sensor[start].get_distance(j);
    }
    path[j]=sensor[start].get_distance(j);    //j==start

    for(j=0; j<num_vertex; j++){
        if(path[j]!=-1){
            sensor[j].set_path(path);
            sensor[j].build_table(trail);
        }
    }

}

void construct_CDS(node sensor[])
{
    int i=0, j=0, k=0, idx=0;

    for(i=0; i<num_vertex; i++){    //find the node u,v in MIS s.t. d(u,v)<=3
        if(sensor[i].get_MIS()==1){
            for(j=0; j<num_vertex; j++){    //find node v
                if(j!=i && sensor[j].get_MIS()==1 && sensor[i].get_distance(j)<=3){
                    deal_path(sensor, i, j);
                    for(k=0; k<num_vertex; k++){    //let all nodes in the route from i to j be in CDS
                        if( sensor[i].get_path(k) !=-1){
                            sensor[k].IS_CDS();
                        }
                    }
                }
            }
        }
    }

}

int find_closest_CDS(node sensor[], int idx)
{
    int mini=num_vertex;

    vector<int> n=sensor[idx].get_neighbor();
    for(vector<int>::iterator it=n.begin(); it!=n.end(); it++){     //find the node in CDS
                                                                    //with the smallest ID to be proxy node
        if(sensor[*it].get_CDS()==1 && *it<mini)
            mini=*it;
    }

    return mini;
}
