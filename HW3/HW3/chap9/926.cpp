#include <iostream>
#include <stdlib.h> /* 亂數相關函數 */
#include <time.h>
#include <vector>
#include <algorithm> //find lib

using namespace std;
void FIFO(int inputstring[10],int framespace){
    int PageFaultCount=0;
    int Head = 0;
    vector<int> FrameList;
    for(int i=0;i<10;i++){
        cout<<i<<".request["<<inputstring[i]<<"] :";
        vector<int>::iterator it = find(FrameList.begin(),FrameList.end(),inputstring[i]);
        if(it != FrameList.end()){//exist in frame

        }else{//no exist
            PageFaultCount++;
            if(FrameList.size() < framespace){
                FrameList.push_back(inputstring[i]);
            }else{
                FrameList.at(Head) = inputstring[i];
                Head = (Head + 1) % framespace;
            }

            
        }
        
        for(vector<int>::iterator it =FrameList.begin();it !=FrameList.end();it++){//show frame
            cout<<*it<<" ";
        }
        cout<<endl;
    }
    cout<<"page fault = "<<PageFaultCount<<endl;

}
void LRU(int inputstring[10],int framespace){
    int PageFaultCount=0;
    vector<int> FrameList;
    vector<int> FrameEntry;
    
    for(int i=0;i<10;i++){
        cout<<i<<".request["<<inputstring[i]<<"] :";
        vector<int>::iterator it = find(FrameList.begin(),FrameList.end(),inputstring[i]);
        if(it != FrameList.end()){//exist in frame
            FrameList.erase(it);
            FrameList.push_back(inputstring[i]);
        }else{//no exist
            PageFaultCount++;
            if(FrameList.size() < framespace){//not full frame 
                FrameList.push_back(inputstring[i]);
                FrameEntry.push_back(inputstring[i]);
            }else{//full frame 
                vector<int>::iterator itf = find(FrameEntry.begin(),FrameEntry.end(),FrameList.at(0));
                FrameEntry.at(itf-FrameEntry.begin()) = inputstring[i];
                FrameList.erase(FrameList.begin());
                FrameList.push_back(inputstring[i]);
            }
        }
        
        for(vector<int>::iterator it =FrameEntry.begin();it !=FrameEntry.end();it++){//show frame
            cout<<*it<<" ";
        }
        cout<<endl;
    }
    cout<<"page fault = "<<PageFaultCount<<endl;
}
void optimal(int inputstring[10],int framespace){
    int PageFaultCount=0;
    vector<int> FrameEntry;
    for(int i=0;i<10;i++){
        cout<<i<<".request["<<inputstring[i]<<"] :";
        vector<int>::iterator it = find(FrameEntry.begin(),FrameEntry.end(),inputstring[i]);
        if(it != FrameEntry.end()){//exist in frame

        }else{//no exist
            PageFaultCount++;
            if(FrameEntry.size() < framespace){//not full frame 
                FrameEntry.push_back(inputstring[i]);
            }else{//full frame
                if(framespace == 1){
                    FrameEntry.at(0) = inputstring[i];
                }else{
                    vector<int> ValueList;
                    for(int k =0;k < i;k++){
                        ValueList.push_back(INT8_MAX);
                    }
                    for(int j = i + 1; j < 10; j++){
                        vector<int>::iterator itf = find(FrameEntry.begin(),FrameEntry.end(),inputstring[j]);
                        if(itf != FrameEntry.end()){//exist
                            if(ValueList.at(itf - FrameEntry.begin()) == INT8_MAX){
                            ValueList.at(itf - FrameEntry.begin()) = j - i;
                            }
                        }
                    } 
                    FrameEntry.at(max_element(ValueList.begin(),ValueList.end()) - ValueList.begin()) = inputstring[i];
                    ValueList.clear();
                }
                    
            }
        }
        
        for(vector<int>::iterator it =FrameEntry.begin();it !=FrameEntry.end();it++){//show frame
            cout<<*it<<" ";
        }
        cout<<endl;
    }
    cout<<"page fault = "<<PageFaultCount<<endl;
}
int main(int argc, char* argv[]){
    srand( time(NULL) );
    int inputstring[10] ={7,0,1,2,0,3,0,4,2,3};
    // int framespace = rand()%7+1;
    int framespace = 3;
    // cout<<"輸入字串:";
    // for(int i=0;i<10;i++){
    //     inputstring[i] = rand()%10;
    //     cout<<inputstring[i];
    // }
    // cout<<endl;
    
    cout<<"framespace = " << framespace << endl;
    //FIFO
    cout<<"FIFO strategy" << endl;
    FIFO(inputstring,framespace);
    //LRU
    cout<<"LRU strategy" << endl;
    LRU(inputstring,framespace);
    //optimal
     cout<<"optimal strategy" << endl;
    optimal(inputstring,framespace);
    return 0;
}
