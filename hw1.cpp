#include<iostream>
#include<vector>
#include<list>
#include<queue>
using namespace std;

class graph
{
private:
    int num_vertex;
    int num_adge;
    std::vector< std::list<int> > adjlist;
    int *visited, *distance, *predecessor;
    int **destination, **next;

    void bfs(int start);
    unsigned int sent_to(int dst_idx);

public:
    graph(int num):num_vertex(num){
        adjlist.resize(num_vertex);
    }
    void add_adge(int u, int v);
    void build_table();
    void print_route(int u, int v);
};

void graph::add_adge(int u, int v)
{
    adjlist[u].push_back(v);
    adjlist[v].push_back(u);
}

void graph::bfs(int start)
{
    int i, j, next_node;
    visited = new int[num_vertex],
    distance = new int[num_vertex];

    for(i=0; i<num_vertex; i++){
        visited[i]=0;       //=0, present it is not visited;
                            //=1, present it is visited
        distance[i]=0;
    }

    std::queue<int> q;
    i=start;

    visited[i]=1;
    distance[i]=0;

    q.push(i);
    while(!q.empty()){
        next_node=q.front();
        std::list<int>::iterator child;
        for(child=adjlist[next_node].begin(); child!=adjlist[next_node].end(); child++){
            if(visited[*child]==0){
                //cout << *child << endl;
                visited[*child]=1;
                distance[*child]=distance[next_node]+1;
                q.push(*child);
            }
        }
        q.pop();
    }
}

unsigned int graph::sent_to(int dst_idx)
{
    int j=dst_idx;
    int mini=num_vertex;
    std::list<int>::iterator child;
//cout << j << " ";
    while(distance[j]!=1){
        mini=num_vertex;
        for(child=adjlist[j].begin(); child!=adjlist[j].end(); child++){
            if(*child < mini && distance[*child]==distance[j]-1)
                mini = *child;
                //cout <<"min="<< mini;

        }
        j=mini;
    }
    //cout << j << endl;
    return j;
}

void graph::build_table()
{
    int i=0, j=0, k=0;

    destination=new int*[num_vertex];
    for(i=0; i<num_vertex; i++)
        destination[i]=new int[num_vertex];
    next=new int*[num_vertex];
    for(i=0; i<num_vertex; i++)
        next[i]=new int[num_vertex];

    for(k=0; k<num_vertex; k++){
        for(i=0; i<num_vertex; i++)
            destination[k][i]=i;
    }
/*
    destination=new int[num_vertex-1];
    next=new int[num_vertex-1];
    for(i=0, j=0; i<num_vertex; i++){
        if(i!=start)
            destination[j++]=i;
    }
*/

    for(j=0; j<num_vertex; j++){
        bfs(j);
        for(i=0; i<num_vertex; i++){
            if(i==j)
                next[j][i]=i;
            else{
                next[j][i]=sent_to(destination[j][i]);
                //cout << destination[j][i] << " " << next[j][i] << endl;
            }

        }
        //cout << endl;
    }

/*
    for(i=0; i<num_vertex-1; i++){
        j=destination[i];

        while(predecessor[j]!=start)
            j=predecessor[j];

        next[i]=j;
    }

    for(i=0; i<num_vertex-1; i++)


    cout << endl;
*/
}

void graph::print_route(int u, int v)
{
    int j=u;

    cout << j << " ";

    while(next[j][v] != v){

        cout << next[j][v] << " ";

        j=next[j][v];
    }
    cout << v << endl;
}

int main()
{
    int i=0, index;
    int number, number2, num_route;
    int v1, v2;

    cin >> number >> number2;

    graph g(number);

    for(i=0; i<number2; i++){
        cin >> index;
        cin >> v1 >> v2;
        g.add_adge(v1, v2);
    }



    g.build_table();

    cin >> num_route;

    for(i=0; i<num_route; i++){
        cin >> index;
        cin >> v1 >> v2;
        cout << index << " ";
        g.print_route(v1, v2);
    }

    return 0;

}
