#include<iostream>
using namespace std;
struct node
{
    int data;
    int count;
    struct node* adj1;
    struct node* adj2;
    struct node* adj3;
    node(int num)
    {
        data = num;
        adj1=NULL;
        adj2=NULL;
        adj3=NULL;
        count=0;
    }
}; //we know at most 3 connections are present
typedef struct root_tag
{
    node* pointer;
}root;

node* heap[8];
/*
root1->8 1 7
       |/|/|
root2->2 5 | 
       |\| |
       | |\|
       |\| |\
       4 9 10
*/
void initialize(root& root1,root& root2)
{                                                   
    node* temp = new node(8);
    heap[0]=temp;
    temp = new node(2);
    heap[1]=temp;
    temp = new node(4);
    heap[2]=temp;
    temp = new node(9);
    heap[3]=temp;
    temp = new node(10);
    heap[4]=temp;
    temp = new node(1);
    heap[5]=temp;
    temp = new node(5);
    heap[6]=temp;
    temp = new node(7);
    heap[7]=temp;
    temp = NULL;
    //now all nodes are in the 'heap',to create connections now
    //while making connections, update reference counts
    root1.pointer=heap[0];//root1->5
    heap[0]->count+=1;
    heap[0]->adj1=heap[1];//5->1
    heap[1]->count+=1;
    root2.pointer=heap[1];//root2->1
    heap[1]->count+=1;
    heap[1]->adj1=heap[2];//1->2
    heap[2]->count+=1;
    heap[1]->adj2=heap[3];//1->9
    heap[3]->count+=1;
    heap[1]->adj3=heap[4];//1->10
    heap[4]->count+=1;
    heap[5]->adj1=heap[1];//7->1
    heap[1]->count+=1;
    heap[5]->adj2=heap[6];//7->8
    heap[6]->count+=1;
    heap[6]->adj1=heap[4];//8->10
    heap[4]->count+=1;
    heap[7]->adj1=heap[6];//3->8
    heap[6]->count+=1;
    heap[7]->adj2=heap[4];//3->10
    heap[4]->count+=1;
    //connections done
}
void print_node(const node* node)
{
    if(node==NULL)
    {
        return;
    }
    cout<<" "<<node->data<<"(rfc="<<node->count<<")";
    if (node->adj1!=NULL || node->adj2!=NULL || node->adj3!=NULL)
    {
        cout<<"-[";
        print_node(node->adj1);
        print_node(node->adj2);
        print_node(node->adj3);
        cout<<" ]";
    }
}
void Display_heap(node** arr)
{
    for(int i=0;i<8;i++)
    {
        if(arr[i]!=NULL)
        {
            print_node(arr[i]);
            cout<<endl;
        }
    }
}
void print_post_heap(const root& value)
{
    cout<<"root->";
    print_node(value.pointer);
    cout<<endl;
}
void garbage_collector(node** arr)
{
    int key=0;
    for(int i=0;i<8;i++)
    {
        if(arr[i]!=NULL)
        {
            if(arr[i]->count==0)
            {
                //update reference counts of adjacent nodes as we disconnect them
                if(arr[i]->adj3!=NULL)
                {
                    arr[i]->adj3->count-=1;
                    arr[i]->adj3=NULL;
                }
                 if(arr[i]->adj2!=NULL)
                {
                    arr[i]->adj2->count-=1;
                    arr[i]->adj2=NULL;
                }
                if(arr[i]->adj1!=NULL)
                {
                    arr[i]->adj1->count-=1;
                    arr[i]->adj1=NULL;
                }
               	delete arr[i];
                arr[i]=NULL;
                key=1;
            }
        }
    }
    if(key) //if reference counts were updated, call garbage collector again
    {
        garbage_collector(arr);
    }
}
int main()
{
    root x,y;
    initialize(x,y);
    cout<<"replication for reference counting garbage collector\n";
    cout<<"Key to navigate the printed table: '-' indicates connection '[]' indicate all the elements connected to the element\n";
    cout<<"total heap: "<<endl;
    Display_heap(heap);
    cout<<"_______________________"<<endl;
    cout<<"Heap connected to the roots is: "<<endl;
    print_post_heap(x);
    print_post_heap(y);
    cout<<"________________________"<<endl;    
    garbage_collector(heap);
    cout<<"the garbage collector was called,checkout the heap:\n";
    Display_heap(heap);
    cout<<"checking out if heap connected to roots printed below\n";
    print_post_heap(x);
    print_post_heap(y);
}
