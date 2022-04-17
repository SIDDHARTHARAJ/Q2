#include<iostream>
using namespace std;
struct node
{
    int data;
    bool mark;
    struct node* adjt1;
    struct node* adjt2;
    struct node* adjt3;
    node(int num)
    {
        data = num;
        adjt1=NULL;
        adjt2=NULL;
        adjt3=NULL;
        mark=false;
    }
}; //we know at most 3 connections are present
typedef struct root_tag
{
    node* pointer;
}root;

node* heap[8];

void Display_node(const node* node)
{
    if(node==NULL)
    {
        return;
    }
    cout<<" "<<node->data;
    if (node->adjt1!=NULL || node->adjt2!=NULL || node->adjt3!=NULL)
    {
        cout<<"-[";
        Display_node(node->adjt1);
        Display_node(node->adjt2);
        Display_node(node->adjt3);
        cout<<" ]";
    }
}

/*
root1->8 2 3
       |/|/|
root2->7 8 | 
       |\| |
       | |\|
       |\| |\
       4 9 10
*/
void intialize(root& root1,root& root2)
{                                                   
    node* temp = new node(8);
    heap[0]=temp;
    temp = new node(7);
    heap[1]=temp;
    temp = new node(4);
    heap[2]=temp;
    temp = new node(9);
    heap[3]=temp;
    temp = new node(10);
    heap[4]=temp;
    temp = new node(2);
    heap[5]=temp;
    temp = new node(8);
    heap[6]=temp;
    temp = new node(3);
    heap[7]=temp;
    temp = NULL;
    //now all nodes are in the 'heap',to create connections now
    root1.pointer=heap[0];//root1->5
    heap[0]->adjt1=heap[1];//5->1
    root2.pointer=heap[1];//root2->1
    heap[1]->adjt1=heap[2];//1->2
    heap[1]->adjt2=heap[3];//1->9
    heap[1]->adjt3=heap[4];//1->10
    heap[5]->adjt1=heap[1];//7->1
    heap[5]->adjt2=heap[6];//7->8
    heap[6]->adjt1=heap[4];//8->10
    heap[7]->adjt1=heap[6];//3->8
    heap[7]->adjt2=heap[4];//3->10
    //connections done
}

void print_hp(node** arr)
{
    for(int i=0;i<8;i++)
    {
        if(arr[i]!=NULL)
        {
            Display_node(arr[i]);
            cout<<endl;
        }
    }
}


void print_post_hp(const root& value)
{
    cout<<"root->";
    Display_node(value.pointer);
    cout<<endl;
}

void mark_node(node* ptr)
{
    node* head=ptr;
    node* tail=NULL;
    node* middle=NULL;
    int flag=1;
    while(head !=NULL)
    {
        if(!head->mark) //if node we are on is unmarked, mark it
        {
            head->mark=true;
        }
        if(head->adjt3!=NULL && !head->adjt3->mark)
        {
            tail=middle;
            middle=head;
            head=head->adjt3;   
        }
         else if(head->adjt2!=NULL && !head->adjt2->mark)//only move along one adjacent path every iteration
        {
            tail=middle;
            middle=head;
            head=head->adjt2;   
        }
        else if(head->adjt1!=NULL && !head->adjt1->mark) //if adjacent node to this is unmarked, move to it
        {
            tail=middle;
            middle=head;
            head=head->adjt1;
        }
       	else
        {
            head=middle;
            middle=tail;
            tail=NULL;
        }

    }
}
void mark(const root& value)
{
    mark_node(value.pointer);
}
void sweep(node** arr)
{
    for(int i=0;i<8;i++)
    {
        if(arr[i]!=NULL)
        {
            if(!arr[i]->mark)
            {
                //disconnect the node
                arr[i]->adjt1=NULL;
                arr[i]->adjt2=NULL;
                arr[i]->adjt3=NULL;
                delete arr[i];
                arr[i]=NULL;
            }
        }
    }
}
void garbage_collector(const root& r1,const root& r2,node** hp)
{
    cout<<"Mark phase intialized"<<endl;
    mark(r1);
    mark(r2);
    cout<<"Marking done"<<endl;
    cout<<"Sweep phase intialized"<<endl;
    sweep(hp);
}
int main()
{
    root x,y;
    intialize(x,y);
    cout<<"Simulation for mark sweep garbage collector\n";
    cout<<"Key to navigate the printed table: '-'indicates connection and '[]' indicate all the elements connected to the element\n";
    cout<<"the full heap is: "<<endl;
    print_hp(heap);
    cout<<"________________________"<<endl;
    cout<<"Heap connected to the roots is: "<<endl;
    print_post_hp(x);
    print_post_hp(y);
    cout<<"_________________________"<<endl;
    garbage_collector(x,y,heap);
    cout<<"the garbage collector was called,complete heap is:\n";
    print_hp(heap);
    cout<<"checking out if it matches heap connected to roots printed below\n";
    print_post_hp(x);
    print_post_hp(y);
}
