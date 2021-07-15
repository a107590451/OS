#include <iostream>
#include<string>
#include<vector>
#include <bits/stdc++.h>
using namespace std;

#define CYLINDERS 5000
#define REQUESTS 1000


vector<int> C_SCAN_sort(vector<int> ran_array){
    vector<int> R_array;
    vector<int> L_array;
    vector<int> newarray;
    int start = *ran_array.begin();
    newarray.push_back(start);
    ran_array.erase(ran_array.begin());
    int pre_number = start;
    int total = 0;
    for(vector<int>::iterator it=ran_array.begin(); it!=ran_array.end();++it){
          if(*it >start){
              R_array.push_back(*it);
          }else{
              L_array.push_back(*it);
          }
    }
    sort(R_array.begin(), R_array.end());
    for(vector<int>::iterator it=R_array.begin(); it!=R_array.end();++it){
        int min = abs(pre_number - *it);
        total += min;
        newarray.push_back(*it);
        pre_number = *it;
    }

    if(newarray.at(newarray.size()-1) != 4999){
        total += (4999 - newarray.at(newarray.size()-1));
        newarray.push_back(4999);
        pre_number = 4999;
    }
    total += 4999;
    newarray.push_back(0);
    pre_number = 0;
    //反方向執行
    sort(L_array.begin(), L_array.end());
    for(vector<int>::iterator it=L_array.begin(); it!=L_array.end();++it){
        int min = abs(pre_number - *it);
        total += min;
        newarray.push_back(*it);
        pre_number = *it;
    }
    return newarray;
}
vector<int> SCAN_sort(vector<int> ran_array){
    vector<int> R_array;
    vector<int> L_array;
    vector<int> newarray;
    int start = *ran_array.begin();
    newarray.push_back(start);
    ran_array.erase(ran_array.begin());
    int pre_number = start;
    int total = 0;
    for(vector<int>::iterator it=ran_array.begin(); it!=ran_array.end();++it){
          if(*it >start){
              R_array.push_back(*it);
          }else{
              L_array.push_back(*it);
          }
    }
    sort(R_array.begin(), R_array.end());
    for(vector<int>::iterator it=R_array.begin(); it!=R_array.end();++it){
        int min = abs(pre_number - *it);
        total += min;
        newarray.push_back(*it);
        pre_number = *it;
    }

    if(newarray.at(newarray.size()-1) != 4999){
        total += (4999 - newarray.at(newarray.size()-1));
        cout<<newarray.at(newarray.size()-1)<<endl;
        newarray.push_back(4999);
        pre_number = 4999;
    }

    //反方向執行
    sort(L_array.begin(), L_array.end(), greater<int>());
    for(vector<int>::iterator it=L_array.begin(); it!=L_array.end();++it){
        int min = abs(pre_number - *it);
        total += min;
        newarray.push_back(*it);
        pre_number = *it;
    }
    return newarray;
}
vector<int> SSTF_sort(vector<int> ran_array){
    vector<int> newarray;
    int start = *ran_array.begin();
    newarray.push_back(start);
    ran_array.erase(ran_array.begin());
    int pre_number = start;
    int total = 0;
    int count = ran_array.size();
    for(int i =0; i < count; i++){

        vector<int>::iterator it;
        vector<int>::iterator next_it;
        int min = 5000;
        for(it=ran_array.begin(); it!=ran_array.end();++it){
            if(abs(pre_number - *it) <min){
                min = abs(pre_number - *it);
                next_it = it;
            }
        }
        total += min;
        newarray.push_back(*next_it);
        pre_number = *next_it;
        ran_array.erase(next_it);

    }
    return newarray;
}
void RUN(vector<int> ran_array){
    vector<int>::iterator it;
    int i = 1;
    for(it=ran_array.begin(); it!=ran_array.end();++it,++i){
        cout<<i<<":"<<*it<<endl;
    }
}
int main()
{   //設定queue
    int array[] ={2069, 1212, 2296, 2800, 544, 1618, 356, 1523, 4965, 3681} ;

    vector<int> ran_array ;
    ran_array.assign(array, array+10);
    cout<<"queue:={";
    for(vector<int>::iterator it=ran_array.begin(); it!=ran_array.end();++it){
        cout<<" "<<*it;
    }
    cout<<" }"<<endl;
    cout<<"以下為FCFS、SSTF、SCAN、C-SCAN等4種演算法的方式輸出執行順序:"<<endl;
    //FCFS輸出
    cout<<"FCFS:"<<endl;
    RUN(ran_array);
    cout<<"FCFS END"<<endl<<endl;
    //SSTF輸出
    cout<<"SSTF:"<<endl;
    RUN(SSTF_sort(ran_array));
    cout<<"SSTF END"<<endl<<endl;
    //SCAN輸出
    cout<<"SCAN:"<<endl;
    RUN(SCAN_sort(ran_array));
    cout<<"SCAN END"<<endl<<endl;
    //C-SCAN輸出
    cout<<"CSCAN:"<<endl;
    RUN(C_SCAN_sort(ran_array));
    cout<<"CSCAN END"<<endl<<endl;

    return 0;
}
