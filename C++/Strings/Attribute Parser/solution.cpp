#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

struct attr{
    string name;
    string value;
    
    void clear(){ // remove all values
        name = "";
        value = "";
    }
};

struct tag{
    
    string name; // tag name
    vector<attr> attrs; 
    vector<tag*> children;
    

    void addAtrr(attr newAttr){
        attrs.push_back(newAttr);
    }
    
    void addChild(tag *child){
        children.push_back(child);
    }
    void findAttr(string &attrname){
        
        for(int i = 0 ;i < attrs.size() ; i++){
            if(attrs[i].name == attrname){
                cout << attrs[i].value << endl;
                return;
            }
        }
        
        cout << "Not Found!" << endl;
    }
};
tag* findTag(string &tagname , const vector<tag*> roots){
    
   //cout << "searching ... : " << tagname << endl;
   
    for(int i = 0 ; i < roots.size() ; i++){
     
        if(roots[i]->name == tagname)
            return roots[i];
    }
    
    return nullptr;
}
void searchAttr(const string &query ,const vector<tag*> &roots){
            
    string attrname;
    string tagspath;
    
    size_t pos = query.find('~');// find the position of ~ caractere
    if (pos != std::string::npos){
        // split the query
        tagspath = query.substr(0 , pos) + '.';
        attrname = query.substr(pos+1);

    }
    
    pos = tagspath.find('.');
    string temptagname = tagspath;
    size_t oldpos = 0;
        
    if(pos != std::string::npos){
        temptagname = tagspath.substr(oldpos, pos);
        oldpos = pos + 1;
        pos = tagspath.find('.' , oldpos);
    }
    
    tag* temptag = findTag(temptagname , roots);
    
    while(pos != std::string::npos){
        
        temptagname = tagspath.substr(oldpos, (pos - oldpos));
        
        //find in the children of the temptag
        if(temptag != nullptr)
            temptag = findTag(temptagname , temptag->children);
        
        oldpos = pos + 1;
        
        pos = tagspath.find('.' , oldpos); // find next .
    }
    
        
    if(temptag != nullptr)
            temptag->findAttr(attrname);
    else
        cout << "Not Found!" << endl;
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    
    int N,Q;
    cin >> N >> Q;
    
    vector<tag*> roots;
    vector<tag*> parentStack; // insert at the end and remove from the end
     
    for(int i = 0 ; i <  N ; i ++){
        
        // clearing the buffer
        cin >> ws;
        
        string line;
        
        getline(cin , line);
        
        if(line[0] == '<' && line[1] != '/') {// new opening tag
            tag *tempTag = new tag();
            int j = 1;
            
            while(line[j] != ' ' && line[j] != '>'){
                tempTag->name += line[j]; // append the caractere to the name
                j++;
            }
            
            // reading attributes
            attr tempAttr;
            
            while(line[j] != '>'){
                
                if(line[j] != ' ' && line[j] != '=' && line[j] != '"'){
                    tempAttr.name += line[j];
                }else if(line[j] == '"'){
                    
                    //cout << "Reading attr ... " << endl;
                    // reading value
                    
                    j++; // start from the next
                    
                    while (line[j] != '"') {
                    
                        tempAttr.value += line[j]; // apend charactere
                        j++; // next char
                    }
                    
                    tempTag->addAtrr(tempAttr); // add the new attr to the tag
                    //cout << tempTag->name << "~" << tempAttr.name << " = " << tempAttr.value << endl;
                    tempAttr.clear(); // clear old values to 
                }
                
                j++;    
            }
            if(parentStack.size() == 0){ // no parent for this 

                roots.push_back(tempTag); // adding the current tag to the roots
            }else { // if parent stack not empty 
                parentStack.back()->addChild(tempTag); // add the new tag to children list of the parent
            }
            
            parentStack.push_back(tempTag); // push the tag to stack
            
        }else{ // closing tag
        
            parentStack.pop_back(); // remove the end
        }

    }
    
    
    for(int i = 0 ; i <  Q ; i ++){
        
        // clearing the buffer
        cin >> ws;
        
        string temp;
        cin >> temp;
        
        // do searching here
        searchAttr(temp , roots);

        
    }
    return 0;
}
