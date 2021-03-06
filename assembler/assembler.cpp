#include<bits/stdc++.h>

using namespace std;



unordered_map<string,int> symbol_table;
unordered_map<string,string> registers;
unordered_map<string,string> memory_ref;
unordered_map<string,string> register_ref;
unordered_map<string,string> hexadecimals;
unordered_map<string,string> io_instructions;
vector<string>line;


int HexToNumber(string a){
    int num = 0;
    int p = 0;
    for(int i = a.length()-2;i>=0;i--){

        int k = a[i] - 48;
        num += k*pow(16,p);
        p++;
    }

    return num;

}

string HexToBinary(string a){
	string toreturn = "";
	for(int i = 0;i<a.length()-1;i++){
            string g = a.substr(i,1);
		toreturn += hexadecimals[g];

	}

	return toreturn;

}

string IntToBinary(int a){
	string final = "";
    while (a > 0) {

        // storing remainder in binary array
        int v= a % 2;
        a = a / 2;
		final += char(v+48);

    }
     reverse(final.begin(), final.end());




	return final;
}

string IntToBinary12(int a){
	string final = "";
    while (a > 0) {

        // storing remainder in binary array
        int v= a % 2;
        a = a / 2;
		final += char(v+48);

    }
     reverse(final.begin(), final.end());

    int l = final.length();

    string semi = "";

    for(int i = 0;i<12-l;i++){
            semi += "0";

    }

    semi += final;

    final = semi;


	return final;
}


bool first_pass(){

    int lc = -1;

    for(auto g: line){
        lc++;
        int ind = 0;
        transform(g.begin(), g.end(), g.begin(), ::toupper);
        if(g[0] =='$'){
            lc--;
            continue;
        }

        if(lc == 0){
            string prev = g.substr(0,3);
            string org = "ORG";
            if(prev!=org){
                cout<<"  ERROR : INCORRECT FIRST STATEMENT"<<endl;
                return false;
            }

            string a = "";
            for(int i = 4;i<g.length();i++)a += g[i];

            lc = HexToNumber(a)-1;

        }

        for(int i = 0;i<g.length();i++){
            if(g[i] == ':'){
                ind = i;
                break;
            }
        }

        if(ind!=0){
            symbol_table[g.substr(0,ind)]=lc;
        }

    }

    return true;
}

bool second_pass(){
	int lc = -1;

	for(auto g: line){


		lc++;



        int ind = 0;
        transform(g.begin(), g.end(), g.begin(), ::toupper);

        if(g[0] =='$'){
            lc--;
            continue;
        }

		if(lc == 0){
            string prev = g.substr(0,3);
            string org = "ORG";
            if(prev!=org){
                cout<<"  ERROR : INCORRECT FIRST STATEMENT"<<endl;
                return false;
            }

            string a = "";
            for(int i = 4;i<g.length();i++)a += g[i];

            lc = HexToNumber(a);

            lc--;
            continue;

        }


		ind = 0;

		for(int i = 0;i<g.length();i++){
			if(g[i] == ':'){
				ind = i;
				break;
			}
		}


		if(ind!=0){
			g = g.substr(ind+2,g.length()-ind-2);
		}




		vector<string> words;

		string pp = "";

		for(int i = 0;i<g.length();i++){

			if(g[i] == ' '){
				words.push_back(pp);

				pp="";
			}
			else{
				pp+=g[i];
			}
		}

		words.push_back(pp);

		if(words.size()>3){
            cout<<"  ERROR : INSTRUCTION SIZE EXCEEDS MAX SIZE"<<endl;
            return false;
		}




		string operato = words[0];


		//bool found=  false;

		if(register_ref.count(operato)>0){
            if(words.size()>1){
                cout<<"  ERROR : REGISTER REFERNCE INSTRUCTION CANT HAVE AN OPERAND"<<endl;
                return false;
            }
            cout<<IntToBinary(lc)<<" "<<HexToBinary(register_ref[operato])<<endl;
            continue;

		}

		/*for(auto g: register_ref){
			if(operato == g.first){
				cout<<IntToBinary(lc)<<" "<<HexToBinary(g.second)<<endl;
				found = true;
				break;
			}
		}


		if(found)continue;*/

            if(io_instructions.count(operato)>0){
                if(words.size()>1){
                cout<<"  ERROR : INPUT OUTPUT INSTRUCTION CANT HAVE AN OPERAND"<<endl;
                return false;
            }
            cout<<IntToBinary(lc)<<" "<<HexToBinary(io_instructions[operato])<<endl;
            continue;

		}




		if(words.size()<=1)continue;

		string operand = words[1];

		if(memory_ref.count(operato)>0){
            cout<<IntToBinary(lc)<<" ";
            if(words.size()>2){
					string lk = memory_ref[operato];
					lk[0] = '1';
					cout<<lk;
				}
				else{
					cout<<memory_ref[operato];
				}

				if(symbol_table.count(operand)>0){
            cout<<IntToBinary12(symbol_table[operand])<<endl;
            continue;


				}


                if(registers.count(operand)>0){
            cout<<"0000"<<registers[operand]<<endl;
            continue;


		}

		if(operand[0] == '['){

					string q = operand.substr(1,operand.length()-2);
					cout<<HexToBinary(q)<<endl;
					continue;

				}
				else if(operand[2] == 'H'){
					cout<<"0001";
					cout<<HexToBinary(operand)<<endl;
					continue;

				}




            cout<<"  ERROR : INVALID OPERAND FOR MEMORY REFERENCE INSTRUCTION"<<endl;
            return false;

		}




            cout<<"  ERROR : INVALID OPERATOR"<<endl;
            return false;


	}
        return true;
}


int main(){



    freopen("assembly_language_code.txt","r",stdin);
	freopen("object_code.txt","w",stdout);

    ofstream fout;
    fout.open("symbol_table.txt");

    memory_ref["STA"]="0000";
	memory_ref["LDA"]="0001";
	memory_ref["ISZ"]="0010";
	memory_ref["BSA"]="0011";
	memory_ref["BUN"]="0100";
	memory_ref["OR"]="0101";
	memory_ref["NOR"]="0110";

	hexadecimals["0"] = "0000";
	hexadecimals["1"] = "0001";
	hexadecimals["2"] = "0010";
	hexadecimals["3"] = "0011";
	hexadecimals["4"] = "0100";
	hexadecimals["5"] = "0101";
	hexadecimals["6"] = "0110";
	hexadecimals["7"] = "0111";
	hexadecimals["8"] = "1000";
	hexadecimals["9"] = "1001";
	hexadecimals["A"] = "1010";
	hexadecimals["B"] = "1011";
	hexadecimals["C"] = "1100";
	hexadecimals["D"] = "1101";
	hexadecimals["E"] = "1110";
	hexadecimals["F"] = "1111";


	register_ref["STP"]="7800H";
	register_ref["HLT"]="7400H";
	register_ref["TMP"]="7200H";
	register_ref["TMR"]="7100H";
	register_ref["LCK"]="7080H";
	register_ref["ULK"]="7040H";
	register_ref["SKZ"]="7020H";
	register_ref["CLA"]="7010H";
	register_ref["CMA"]="7008H";
	register_ref["INC"]="7004H";
	register_ref["LSA"]="7002H";



	registers["AC"]="00000001";
	registers["DR"]="00000010";
	registers["INPR"]="00000100";
	registers["AR"]="00001000";
	registers["PC"]="00010000";
	registers["SR"]="00100000";
	registers["TR"]="01000000";
	registers["TMR"]="10000000";


	io_instructions["INP"] = "F800H";
	io_instructions["OUT"] = "F400H";
	io_instructions["SKI"] = "F200H";
	io_instructions["SKO"] = "F100H";




	string tt;
	getline(cin,tt,'\n');
	while(tt!="END"){
		line.push_back(tt);
		getline(cin,tt,'\n');
	}

	bool res = first_pass();
    if(!res){
        cout<<"Code failed at the first Pass of Assembler. Please Try Again"<<endl;
        return 0;
    }

    fout<<"SYMBOL TABLE(Constructed after first pass of the assembler)"<<endl;

	fout<<"LABEL    LOCATION"<<endl;
	for(auto it: symbol_table){
        fout<<it.first<<"          "<<it.second<<endl;

	}
	fout.close();
	cout<<"Machine Code"<<endl;
	cout<<"Location"<<"    Binary Data"<<endl;

	res = second_pass();


    return 0;
}
