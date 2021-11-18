#include <iostream>
#include <fstream> //stream class to both read from and write to files
#include <string>
#include <vector>
#include <math.h>
#include <map>
using namespace std;



//this function reads the file and transfer it into a string vector leaving data portion and comments out
vector<string> readfile(const string& filename){
    ifstream Infile;
    Infile.open(filename);
    string line;
    vector<string> file;
    string space (" ");
    while (getline(Infile, line)){
        if (line.front() != '.')  //eliminate lines start with "."
        {
            string comment ("//");
            size_t found = line.find(comment); 
            string comment2 ("#");
            size_t found2 = line.find(comment2);            
            if (found!=string::npos){  //eliminate lines containing with "//"
                file.push_back(line.substr(0,found-1));}
            else if (found2!=string::npos){  //eliminate lines containing with "//"
                file.push_back(line.substr(0,found2-1));}
            else{
                file.push_back(line);}
        }
    }
    Infile.close();
    return file;
}


//this function seperates each line into a string vector with one command at a time
// Example add $t0, $zero, 1 ->  add  $t0  $zero  1
vector<string> sep_line(string line){

    vector<string> thisline;
    /*
    string delim1 = ",";
    string delim2 = ":";
    */

    if (line.find(":") != string::npos){   //label name
        string f = line.substr(0, line.find(":"));
        thisline.push_back(f);
    }

    else
    {
        size_t start = 0;
        size_t end = 0;
        end = line.find(" ");
        thisline.push_back(line.substr(start, end));
        start = end + 1;

        if (line.find(",") != string::npos)
        {
            end = line.find(",");
            while (end != string::npos)
            {
                string f = line.substr(start, end - start);
                thisline.push_back(f);
                start = end + 2;
                end = line.find(",", start);
            }
        }
        if (line.find("(") != string::npos){ // this for sw and lw
            end = line.find("(");
            string f = line.substr(start, end - start);
            thisline.push_back(f);
            start = end + 1;
            end = line.find(")");
            f = line.substr(start, end - start);
            thisline.push_back(f);
        }
        else {thisline.push_back(line.substr(start));}
    }
    return thisline;
}



//this function transfer register to its position in r
int topos(string s){
    int num = s[2]-48;
    if (s[1] == 'v'){ return num+2;}
    if (s[1] == 'a'){ return num+4;}
    if (s[1] == 't' && num < 8){ return num+8;}
    if (s[1] == 's'){ return num+16;}
    if (s[1] == 't' && num > 8){ return num+16;}
    if (s == "$zero"){ return 0;}

    else 
    { 
        if (s.length() == 3){ return num;}
        else {
            int num2 = s[3] - 48;
            return num*10 + num2;
         } 


    }

}

//this function convert decimal to binary
//two param: decimal n, bits allocated
string decToBinary(int n, int space) 
{ 
    //cout << n << endl;
    if (n < 0){ n = pow(2,space) + n;} //2s component
    int binaryNum[space]; 
    int i = 0; 
    while (n > 0) { 
        binaryNum[i] = n % 2; 
        n = n / 2; 
        i++; 
    } 
  
    string s = "";
    for (int j = i - 1; j >= 0; j--) {
        if ((binaryNum[j]) == 1){ s += "1";}
        else{s += "0";}
    }
    for (int j = i; j < space; j++ )
        s.insert(0, "0");
    //cout << s << endl;
    return s;
}



string BinaryToHexaDec(string s, map<string, char> hexmap){
    string str = "";
    for (int i = 0; i < s.length(); i=i+4){
        str += hexmap[s.substr(i,4)];
    }
    //cout << "|" << str << "|"<< endl;
    return str + '\n';
}

map<string, char> createMap()
{
    map<string, char> hexmap;
    hexmap["0000"] = '0';
    hexmap["0001"] = '1';
    hexmap["0010"] = '2';
    hexmap["0011"] = '3';
    hexmap["0100"] = '4';
    hexmap["0101"] = '5';
    hexmap["0110"] = '6';
    hexmap["0111"] = '7';
    hexmap["1000"] = '8';
    hexmap["1001"] = '9';
    hexmap["1010"] = 'a';
    hexmap["1011"] = 'b';
    hexmap["1100"] = 'c';
    hexmap["1101"] = 'd';
    hexmap["1110"] = 'e';
    hexmap["1111"] = 'f';
    return hexmap;
}



//add instruction
string add(vector<string>thisline)
{
    string rd = decToBinary(topos(thisline[1]), 5);
    string rs = decToBinary(topos(thisline[2]), 5);
    string rt = decToBinary(topos(thisline[3]), 5);

    string s = "000000 " + rs + " " + rt + " " + rd + " 00000" + " 100000";
    //cout << s << endl;
    s = "000000" + rs + rt + rd + "00000100000";
    return s;
    //return s;
}


string addi(vector<string>thisline){
    string rt = decToBinary(topos(thisline[1]), 5);
    string rs = decToBinary(topos(thisline[2]), 5);
    string im = decToBinary(stoi(thisline[3]), 16);

    string s = "001000 " + rs + " " + rt + " " + im;
    //cout << s << endl;
    s = "001000" + rt + rs + im;
    return s;

}

string sub(vector<string>thisline){
    string rd = decToBinary(topos(thisline[1]), 5);
    string rs = decToBinary(topos(thisline[2]), 5);
    string rt = decToBinary(topos(thisline[3]), 5);

    string s = "000000 " + rs + " " + rt + " " + rd + " " + "00000" + " " +  "100010";
    //cout << s << endl;
    s = "000000" + rs + rt + rd + "00000100010";
    return s;

}


string mult(vector<string>thisline){
    string rs = decToBinary(topos(thisline[1]), 5);
    string rt = decToBinary(topos(thisline[2]), 5);

    string s = "000000 " + rs + " " + rt + " " + "0000000000" + " " +  "011000";
    //cout << s << endl;
    s = "000000" + rs + rt + "0000000000011000";
    return s;

}

string mflo(vector<string>thisline){
    string rd = decToBinary(topos(thisline[1]), 5);
    string s = "0000000000000000 " + rd + " " + "00000" + " " +  "010010";
    //cout << s << endl;
    s = "0000000000000000" + rd + "00000010010";
    return s;

}

string sll(vector<string>thisline){
    string rd = decToBinary(topos(thisline[1]), 5);
    string rt = decToBinary(topos(thisline[2]), 5);
    string sa = decToBinary(topos(thisline[3]), 5);

    string s = "000000 00000 " + rt + " " + rd + " " + sa + " " + "000000";
    //cout << s << endl;
    s = "00000000000" + rt + rd + sa + "000000";
    return s;

}

string srl(vector<string>thisline){
    string rd = decToBinary(topos(thisline[1]), 5);
    string rt = decToBinary(topos(thisline[2]), 5);
    string sa = decToBinary(topos(thisline[3]), 5);

    string s = "000000 00000 " + rt + " " + rd + " " + sa + " " + "000010";
    //cout << s << endl;
    s = "00000000000" + rt + rd + sa + "000010";
    return s;

}

string lw(vector<string>thisline){
    string rt = decToBinary(topos(thisline[1]), 5);
    string rs = decToBinary(topos(thisline[3]), 5);
    string offset = decToBinary(stoi(thisline[2]), 16);

    string s = "100011 " + rs + " " + rt  + " " + offset;
    //cout << s << endl;  
    s = "100011" + rs + rt + offset;
    return s;


}

string sw(vector<string>thisline){
    string rt = decToBinary(topos(thisline[1]), 5);
    string rs = decToBinary(topos(thisline[3]), 5);
    string offset = decToBinary(stoi(thisline[2]), 16);

    string s = "100111 " + rs + " " + rt  + " " + offset;
    //cout << s << endl;  
    s = "100111" + rs + rt + offset;
    return s;

}

string slt(vector<string>thisline){
    string rd = decToBinary(topos(thisline[1]), 5);
    string rs = decToBinary(topos(thisline[2]), 5);
    string rt = decToBinary(topos(thisline[3]), 5);

    string s = "000000 " + rs + " " + rt  + " " + rd + "00000" + "101010";
    //cout << s << endl;  
    s = "000000" + rs + rt + rd + "00000101010";
    return s;

}

string beq(vector<string>thisline, map<std::string, int> mymap, string line){
    string rs = decToBinary(topos(thisline[1]), 5);
    string rt = decToBinary(topos(thisline[2]), 5);
    string label = thisline[3] + ":";
    int labelpos = mymap[label];
    int currentpos = mymap[line];
    map<string, int>::iterator it;
    int relativePos = labelpos - currentpos;
    cout << "WHYYYYYYY" << labelpos << " " << currentpos << " " << relativePos << " " << endl;
    string offset =  decToBinary(relativePos, 16);
    string s = "000100 " + rs + " " + rt + " " + offset;
    //cout << s << endl;  
    s = "000100" + rs + rt + offset;
    return s;

}

string bne(vector<string>thisline, map<std::string, int> mymap, string line){
    string rs = decToBinary(topos(thisline[1]), 5);
    string rt = decToBinary(topos(thisline[2]), 5);
    string label = thisline[3] + ":";
    int labelpos = mymap[label];
    int currentpos = mymap[line];
    int relativePos = labelpos - currentpos ;
    string offset =  decToBinary(relativePos, 16);
    string s = "000101 " + rs + " " + rt + " " + offset;
    //cout << s << endl;  
    s = "000101" + rs + rt + offset;
    return s;

}

string j(vector<string>thisline, map<std::string, int> mymap){
    string label = thisline[1] + ":";
    int labelpos = mymap[label];
    string target =  decToBinary(labelpos, 26);
    string s = "000010 " + target;
    //cout << s << endl;  
    s = "000010" + target;
    return s;

}

string jal(vector<string>thisline, map<std::string, int> mymap){
    string label = thisline[1] + ":";
    int labelpos = mymap[label];
    string target =  decToBinary(labelpos, 26);
    string s = "000011 " + target;
    //cout << s << endl;  
    s = "000011" + target;
    return s;

}

string jr(vector<string>thisline){
    string rs = decToBinary(topos(thisline[1]), 5);
    string s = "000000 " + rs + " 0000000000000000" + " 01000";
    //cout << s << endl;  
    s = "000000" + rs + "000000000000000001000";
    return s;
}

map<std::string, int> mapping(vector<string> myinfile){
    map<std::string, int> mymap;
    int i = 0;
    string labels (":");
    size_t found;
    for (vector<string>::iterator it = myinfile.begin() ; it != myinfile.end(); ++it)
    {
        string line{ *it };
        mymap[line] = i;
        //cout << line << " " << i << endl;
        found = line.find(labels);
        if (found == string::npos ){ i++;} //This is for preventing labels to take place in instruction counting
 
    }
    return mymap;
}


int main(int argc, char* argv[])
{
    string infilename = argv[1];
    string outfilename = argv[2];
    vector<string> myinfile = readfile(infilename);

    map<string, int> mymap = mapping(myinfile); // mapping every instruction
    map<string, char> hexmap = createMap();
    //for (auto it = mymap.cbegin(); it != mymap.cend(); ++it){
        //cout << it ->first << endl;
    //}
    ofstream outfile;
    outfile.open(outfilename);
    outfile << "v3.0 hex words plain" << endl;
    for (vector<string>::iterator it = myinfile.begin() ; it != myinfile.end(); ++it)
    {
        string line{ *it };
        vector<string> thisline= sep_line(line);
        if (thisline.front() == "add"){ outfile<< BinaryToHexaDec(add(thisline) , hexmap);  }     
         //for (vector<string>::iterator it2 = BinaryToHexaDec(thisline.begin, hexmap() ; it2 != thisline.end(); ++it2){cout << *it2 << endl;}
        else if (thisline.front() == "addi"){ outfile<<BinaryToHexaDec(addi(thisline), hexmap); }
        else if (thisline.front() == "sub"){  outfile<<BinaryToHexaDec(sub(thisline) , hexmap); }
        else if (thisline.front() == "mult"){ outfile<<BinaryToHexaDec(mult(thisline), hexmap);}
        else if (thisline.front() == "mflo"){ outfile<<BinaryToHexaDec(mflo(thisline), hexmap);}
        else if (thisline.front() == "sll"){  outfile<<BinaryToHexaDec(sll(thisline) , hexmap);}
        else if (thisline.front() == "srl"){  outfile<<BinaryToHexaDec(srl(thisline) , hexmap);}
        else if (thisline.front() == "lw"){   outfile<<BinaryToHexaDec(lw(thisline)  , hexmap);}
        else if (thisline.front() == "sw"){   outfile<<BinaryToHexaDec(sw(thisline)  , hexmap);}
        else if (thisline.front() == "slt"){  outfile<<BinaryToHexaDec(slt(thisline) , hexmap);}
 
        // Here is the branching part
        else if (thisline.front() == "beq"){ outfile<<BinaryToHexaDec(beq(thisline, mymap, line), hexmap);}
        else if (thisline.front() == "bne"){ outfile<<BinaryToHexaDec(bne(thisline, mymap, line), hexmap);}
        else if (thisline.front() == "j"){ outfile<<BinaryToHexaDec(j(thisline, mymap), hexmap);}
        else if (thisline.front() == "jal"){ outfile<<BinaryToHexaDec(jal(thisline, mymap), hexmap);}
        else if (thisline.front() == "jr"){ outfile<<BinaryToHexaDec(jr(thisline), hexmap);}
        //else { cout << "Hi, I 'm not printed" << line << endl;}
    }
    outfile.close();

}

