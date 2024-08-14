#include <cstdlib>
#include <random>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;





int address = 257;



vector<string> labels;
vector<string> label_address;
vector<string> missing_label;
vector<string> error_strings;
vector<int>    missing_label_position;
vector<int>    error;


string sourcefile, hexfile, hexcode = "", line, stream;


bool skip = false, first_missing = true;



void load_address(int start){

    bool missing = true;

    if(line[start] == '$'){
        hexcode += line.substr(start+1, 4);
    }
    else{
        for(int i = 0; i < labels.size(); i++){
            if(labels[i] == line.substr(start, line.length()-1)){
                hexcode += label_address[i];

                missing = false;
            }
        }

        if(missing){

            missing_label.push_back(line.substr(start, line.length()-1));

            missing_label_position.push_back((address-1)*2);

            hexcode += "0000";
        }

    }

}






int main(){



    for(int i = 0; i < 256; i++){
        hexcode += "00";
    }




//input file names
    cout<<"\nInsert surce file name: ";
    cin>>sourcefile;
    cout<<"Insert hex file name: ";
    cin>>hexfile;

    sourcefile += ".txt";
    hexfile += ".txt";


//read the surce file
    ifstream file;
    file.open (sourcefile);

    ifstream fin(sourcefile);

    if(!fin){
        error.push_back(3);
        error_strings.push_back(sourcefile);
    }




    while(getline(file, line)){

        address++;

        if(line == "" or line[0] == '#'){
            address--;
        }
        else if(line[0] == ':'){

            address--;
            skip = false;


            for(int i = 1; i < line.size()-1; i++){

                if(line[i] == ' '){
                    skip = true;

                    cout<<endl;

                    for(int j = 0; j < labels.size(); j++){

                        if(line.substr(1, i-1) == labels[j]){
                            error.push_back(1);
                            error_strings.push_back(labels[j]);
                        }

                    }


                    labels.push_back(line.substr(1, i-1));

                    if(isdigit(line[i+1])){

                        stringstream stream;
                        stream << std::hex << std::setfill ('0')
                        << std::setw(sizeof(address))
                        << std::hex << address + stoi(line.substr(i+1, line.size()-1));

                        label_address.push_back(stream.str());

                        break;

                    }else{

                        if(line[i+1] == '$'){
                            label_address.push_back(line.substr(i+2, 4));
                        }
                        else{
                            for(int j = 0; j < labels.size(); j++){
                                if(labels[j] == line.substr(i+1, line.length()-i)){
                                    label_address.push_back(label_address[j]);
                                }
                            }

                        }

                        break;
                    }

                }


            }

            if(skip == false){

                for(int i = 0; i < labels.size(); i++){

                    if(line.substr(1, line.length()) == labels[i]){
                        error.push_back(1);
                        error_strings.push_back(labels[i]);
                    }

                }


                stringstream stream;
                stream << std::hex << std::setfill ('0') << std::setw(sizeof(address))  << std::hex << address;
                label_address.push_back(stream.str());

                labels.push_back(line.substr(1, line.length()-1));



            }


        }
        else if(line.substr(0, 3) == "nop"){
            hexcode += "00";
        }
        else if(line.substr(0, 3) == "add"){
            hexcode += "14";
        }
        else if(line.substr(0, 3) == "adc"){
            hexcode += "15";
        }
        else if(line.substr(0, 3) == "sub"){
            hexcode += "16";
        }
        else if(line.substr(0, 3) == "mul"){
            hexcode += "17";
        }
        else if(line.substr(0, 3) == "shr"){
            hexcode += "54";
        }
        else if(line.substr(0, 3) == "shl"){
            hexcode += "55";
        }
        else if(line.substr(0, 3) == "not"){
            hexcode += "57";
        }
        else if(line.substr(0, 3) == "and"){
            hexcode += "c4";
        }
        else if(line.substr(0, 3) == "or "){
            hexcode += "c5";
        }
        else if(line.substr(0, 3) == "xor"){
            hexcode += "c6";
        }


        else if(line.substr(0, 3) == "ldi"){
            if(line[4] == 'A'){
                hexcode += "28";
            }
            else if(line[4] == 'T'){
                hexcode += "30";
            }
            else if(line[4] == 'a'){
                hexcode += "04";
            }
            else if(line[4] == 'b'){
                hexcode += "05";
            }
            else if(line[4] == 'c'){
                hexcode += "06";
            }
            else if(line[4] == 'd'){
                hexcode += "07";
            }

            if(line.length() == 5){
                hexcode += "00";
            }
            else hexcode += line.substr(6, 2);


            address++;
        }
        else if(line.substr(0, 3) == "sti"){
            if(line[4] == 'A'){
                hexcode += "29";
            }
            else if(line[4] == 'T'){
                hexcode += "31";
            }
            else if(line[4] == 'a'){
                hexcode += "84";
            }
            else if(line[4] == 'b'){
                hexcode += "85";
            }
            else if(line[4] == 'c'){
                hexcode += "86";
            }
            else if(line[4] == 'd'){
                hexcode += "87";
            }

            hexcode += "00";


            address++;
        }
        else if(line.substr(0, 3) == "lda"){
            if(line[4] == 'A'){
                hexcode += "2a";
            }
            else if(line[4] == 'T'){
                hexcode += "32";
            }
            else if(line[4] == 'a'){
                hexcode += "08";
            }
            else if(line[4] == 'b'){
                hexcode += "09";
            }
            else if(line[4] == 'c'){
                hexcode += "0a";
            }
            else if(line[4] == 'd'){
                hexcode += "0b";
            }

            load_address(6);


            address += 2;

        }
        else if(line.substr(0, 3) == "sta"){
            if(line[4] == 'A'){
                hexcode += "2b";
            }
            else if(line[4] == 'T'){
                hexcode += "33";
            }
            else if(line[4] == 'a'){
                hexcode += "88";
            }
            else if(line[4] == 'b'){
                hexcode += "89";
            }
            else if(line[4] == 'c'){
                hexcode += "8a";
            }
            else if(line[4] == 'd'){
                hexcode += "8b";
            }

            load_address(6);


            address += 2;
        }
        else if(line.substr(0, 3) == "trf"){
            if(line[4] == 'A'){
                if(line[6] == 'T'){
                    hexcode += "ac";
                }
                else if(line[6] == 'a'){
                    hexcode += "2c";
                }
                else if(line[6] == 'b'){
                    hexcode += "2d";
                }
                else if(line[6] == 'c'){
                    hexcode += "2e";
                }
                else if(line[6] == 'd'){
                    hexcode += "2f";
                }
            }
            else if(line[4] == 'T'){
                if(line[6] == 'A'){
                    hexcode += "ad";
                }
            }
            else if(line[4] == 'a'){
                if(line[6] == 'A'){
                    hexcode += "0c";
                }
                else if(line[6] == 'b'){
                    hexcode += "0d";
                }
                else if(line[6] == 'c'){
                    hexcode += "0e";
                }
                else if(line[6] == 'd'){
                    hexcode += "0f";
                }
            }
            else if(line[4] == 'b'){
                if(line[6] == 'A'){
                    hexcode += "4c";
                }
                else if(line[6] == 'a'){
                    hexcode += "4d";
                }
                else if(line[6] == 'c'){
                    hexcode += "4e";
                }
                else if(line[6] == 'd'){
                    hexcode += "4f";
                }
            }
            else if(line[4] == 'c'){
                if(line[6] == 'A'){
                    hexcode += "8c";
                }
                else if(line[6] == 'a'){
                    hexcode += "8d";
                }
                else if(line[6] == 'b'){
                    hexcode += "8e";
                }
                else if(line[6] == 'd'){
                    hexcode += "8f";
                }
            }
            else if(line[4] == 'd'){
                if(line[6] == 'A'){
                    hexcode += "cc";
                }
                else if(line[6] == 'a'){
                    hexcode += "cd";
                }
                else if(line[6] == 'b'){
                    hexcode += "ce";
                }
                else if(line[6] == 'c'){
                    hexcode += "cf";
                }
            }
        }



        else if(line[0] == 'j'){
            if(line.substr(1, 2) == "mp"){
                hexcode += "10";
            }
            else if(line.substr(1, 2) == "ca"){
                hexcode += "11";
            }
            else if(line.substr(1, 2) == "zr"){
                hexcode += "12";
            }
            else if(line.substr(1, 2) == "gr"){
                hexcode += "13";
            }
            else if(line.substr(1, 2) == "sm"){
                hexcode += "50";
            }
            else if(line.substr(1, 2) == "eq"){
                hexcode += "51";
            }
            else if(line.substr(1, 2) == "po"){
                hexcode += "52";
            }
            else if(line.substr(1, 2) == "ng"){
                hexcode += "53";
            }
            else if(line.substr(1, 2) == "en"){
                hexcode += "d0";
            }
            else if(line.substr(1, 2) == "od"){
                hexcode += "d1";
            }
            else if(line.substr(1, 2) == "sh"){
                hexcode += "d2";
            }
            else if(line.substr(1, 2) == "in"){
                hexcode += "d3";
            }
            else if(line.substr(1, 2) == "ne"){
                hexcode += "d4";
            }



            load_address(4);

            address += 2;
        }



        else if(line.substr(0, 3) == "phr"){
            if(line[4] == 'A'){
                hexcode += "a8";
            }
            else if(line[4] == 'T'){
                hexcode += "34";
            }
            else if(line[4] == 'a'){
                hexcode += "18";
            }
            else if(line[4] == 'b'){
                hexcode += "19";
            }
            else if(line[4] == 'c'){
                hexcode += "1a";
            }
            else if(line[4] == 'd'){
                hexcode += "1b";
            }
        }
        else if(line.substr(0, 3) == "plr"){
            if(line[4] == 'A'){
                hexcode += "a9";
            }
            else if(line[4] == 'T'){
                hexcode += "35";
            }
            else if(line[4] == 'a'){
                hexcode += "98";
            }
            else if(line[4] == 'b'){
                hexcode += "99";
            }
            else if(line[4] == 'c'){
                hexcode += "9a";
            }
            else if(line[4] == 'd'){
                hexcode += "9b";
            }
        }
        else if(line.substr(0, 3) == "pha"){
            hexcode += "1c";
        }
        else if(line.substr(0, 3) == "pla"){
            hexcode += "1f";
        }
        else if(line.substr(0, 3) == "spi"){
            hexcode += "2000";
            address++;
        }
        else if(line.substr(0, 3) == "spa"){
            hexcode += "21";

            load_address(4);

            address += 2;
        }


        else if(line.substr(0, 3) == "laa"){
            hexcode += "26";

            load_address(4);

            address += 2;
        }
        else if(line.substr(0, 3) == "sai"){
            hexcode += "25";

            address += 2;
        }
        else if(line.substr(0, 3) == "saa"){
            hexcode += "27";

            load_address(4);

            address += 2;
        }


        else if(line.substr(0, 3) == "wri"){
            hexcode += 'f';
            hexcode += line[4];
            hexcode += "00";
            address++;
        }
        else if(line.substr(0, 3) == "wra"){
            hexcode += 'e';
            hexcode += line[4];

            load_address(6);

            address += 2;
        }
        else if(line.substr(0, 3) == "rdi"){
            hexcode += "c100";

            address++;
        }
        else if(line.substr(0, 3) == "rda"){
            hexcode += "c2";

            load_address(4);

            address += 2;
        }
        else if(line.substr(0, 3) == "rdp"){
            hexcode += 'b';
            hexcode += line[4];
            hexcode += "00";

            address++;
        }


        else if(line.substr(0, 3) == "lii"){
            hexcode += "c30000";

            address += 2;

        }
        else if(line.substr(0, 3) == "lia"){
            hexcode += "c7";

            load_address(4);

            address += 2;
        }


        else if(line.substr(0, 3) == "rti"){
            hexcode += "c8";
        }
        else{
            error.push_back(0);
            error_strings.push_back(line);
        }

    }

    file.close();


    for(int i = 0; i < missing_label.size(); i++){

        for(int j = 0; j < labels.size(); j++){

            if(missing_label[i] == labels[j]){

                if(first_missing){

                    missing_label_position[i] += 2;
                    first_missing = false;
                }


                stringstream stream;
                stream << std::hex << std::setfill ('0')
                << std::setw(4) << std::hex << (stoi(label_address[j], 0, 16));

                label_address[j] = stream.str();

                hexcode.replace(missing_label_position[i]-2, 4, label_address[j]);

                break;

            }
            else if(j == labels.size()-1){
                error.push_back(2);
                error_strings.push_back(missing_label[i]);
            }

        }

    }


// if the memory is not completly used, then fill the rest with zero
while(address < 65536){
    address++;
    hexcode += "00";
}




//write to hex file
    ofstream wfile;
    wfile.open (hexfile);
    wfile << "v3.0 hex bytes plain big-endian\n";
    wfile << hexcode;
    file.close();


if(error.size() > 0){

    cout<<"\nThere were "<<error.size()<<" errors found:";

    for(int i = 0; i < error.size(); i++){

        cout<<"\nError: "<<error[i]<<", ";

        switch (error[i]) {

            case 0:
                cout<<"instruction \""<<error_strings[i]<<"\" does not exist.";
                break;

            case 1:
                cout<<"label \""<<error_strings[i]<<"\" already exists.";
                break;

            case 2:
                cout<<"label \""<<error_strings[i]<<"\" does not exist.";
                break;

            case 3:
                cout<<"file \""<<error_strings[i]<<"\" does not exist.";
                break;

        }

    }

}else cout<<"There were 0 errors found.";





cout<<"\n\nDone\n\n";

}
