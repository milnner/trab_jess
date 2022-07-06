#include <iostream>
#include <bitset>
#include <stdlib.h>
#include <vector>
#include <ctime>

using namespace std;

struct matriz_g  // 15, 11
{
    vector<bitset<11> > matriz;
};

struct matriz_h // 4, 15
{
    vector<bitset<15> > matriz; 
};

struct control // Usado para contar os erros em bits
{
    unsigned int err_to_1bit;
    unsigned int err_to_2bit ;
};


void genMG(matriz_g &G)
{
    G.matriz.push_back(0X6D5); // x
    G.matriz.push_back(0X5B3); // x
    G.matriz.push_back(0X400);
    G.matriz.push_back(0X38F); // x
    G.matriz.push_back(0X200);
    G.matriz.push_back(0X100);
    G.matriz.push_back(0X80);
    G.matriz.push_back(0XFF); // x
    G.matriz.push_back(0X40);
    G.matriz.push_back(0X20);
    G.matriz.push_back(0X10);
    G.matriz.push_back(0X8);
    G.matriz.push_back(0X4);
    G.matriz.push_back(0X2);
    G.matriz.push_back(0X1);
}

void genMH(matriz_h &H)
{
    H.matriz.push_back(0X5555);
    H.matriz.push_back(0X3333);
    H.matriz.push_back(0XF0F);
    H.matriz.push_back(0XFF);
}

bitset<15> encode_char(unsigned char charac)
{
    bitset<11> msg_bits;
    bitset<15> answ;
    matriz_g G;
    genMG(G);
    msg_bits = bitset<11>{charac};

    for (size_t i = 0; i < 15; i++)
    { 
        bitset<11> bits;
        for (int j = 10; j >= 0; j--)
            bits[j] = (int) G.matriz.at(i)[j]  && (int) msg_bits[j];
        
        answ[i] = !(bits.count() % 2 == 0);
    }
    return answ; 
}

short int valid_encode_char(bitset<15> msg_bits)
{
    matriz_h H;
    genMH (H);
    bitset<4> answ(0);

    for (int i = 0; i < 4; i++)
    {
        bitset<15> bits;
        for (int j = 14, l=0; j >= 0; j--, l++)
            bits[j] = H.matriz.at(i)[j] && msg_bits[l];

        answ[i] = !(bits.count() % 2 == 0);
    }
    return answ.to_ulong();
}

unsigned char decode_char(bitset<15> enc_msg)
{
    bitset<8> msg;
    msg[7] = enc_msg[6];
    msg[6] = enc_msg[8];
    msg[5] = enc_msg[9]; 
    msg[4] = enc_msg[10];
    msg[3] = enc_msg[11];
    msg[2] = enc_msg[12];
    msg[1] = enc_msg[13];
    msg[0] = enc_msg[14];
    return (unsigned char) msg.to_ulong();
}

void encode_str(string &str, vector<bitset<15> > &encoded_str)
{
    for (size_t i = 0; i < str.length(); i++)
        encoded_str.push_back(encode_char(str.at(i)));
}

void decode_str(vector<bitset<15> > &encoded_str, string &str)
{
    for (size_t i = 0; i < encoded_str.size(); i++)
        str+=decode_char(encoded_str.at(i));
}

bool receive(bitset<15> encoded_char, unsigned char &decoded_char)
{
    short int p = valid_encode_char(encoded_char);

    if (p) 
        encoded_char[p-1] = !encoded_char[p];

    p = valid_encode_char(encoded_char);
    decoded_char = decode_char(encoded_char);

    return p;
}

bitset<15>  dirty_msg(bitset<15> encoded_msg)
{

}

void simulator(string &msg)
{
    vector<bitset<15> > encoded_msg;
    unsigned char charac;
    encode_str(msg, encoded_msg);
    unsigned int err = 0;

    for (size_t i = 0; i < encoded_msg.size(); i++)
    {
        bitset<15> encoded_char = (encoded_msg.at(i));
        bitset<11> charac_bits;
       
        if(receive(encoded_char, charac))
        {
            i--; err++;
            continue;
        }
        charac_bits = bitset<11>{charac};
        printf("%c%10cerros: %d%5c",
                charac, '\0', err, '\0');

        cout << charac_bits << " " << encoded_char << " "
             << charac_bits.to_ulong() << endl;
            
        err = 0;
    }
}

int main(int argc, char const *argv[])
{
    time_t o; 
    string msg;
    cin >> msg;
    simulator(msg); 
    return 0;
}


// hamming
// codificação de strings para hamming
// simulador do envio das mensagens

/*

*/