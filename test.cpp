#include <iostream>
#include <cstdlib>
#include <bitset>
#include <vector>

using namespace std;

struct matriz_g
{
    vector<bitset<11> > matriz;
};

struct matriz_h
{
    vector<bitset<15> > matriz;
};

void genMG(matriz_g &G)
{
    G.matriz.push_back(0X6D5); // x
    G.matriz.push_back(0X5B3); // x
    G.matriz.push_back(0X400);
    G.matriz.push_back(0X38F); // x
    G.matriz.push_back(0X200);

    G.matriz.push_back(0X100);
    G.matriz.push_back(0XFF); // x
    G.matriz.push_back(0X80);
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

short int valid_encode_char(bitset<15> msg_bits)
{
    matriz_h H;
    genMH (H);
    bitset<4> answ(0);

    for (int i = 0, k = 3; i < 4; i++,k--)
    {
        bitset<15> bits;
        for (int j = 14, l=0; j >= 0; j--, l++)
            bits[j] = H.matriz.at(i)[j] && msg_bits[l];
        
        answ[i] = !(bits.count() % 2 == 0);
    }
    return answ.to_ulong();
}

bitset<15> encode_char(unsigned char charac)
{
    bitset<11> msg_bits;
    bitset<15> answ(0);
    matriz_g G;
    genMG(G);
    msg_bits = bitset<11>(charac);

    for (size_t i = 0; i < 15; i++)
    {
        bitset<11> bits;
        for (int j = 10; j >= 0; j--)
            bits[j] = (int) G.matriz.at(i)[j]  && (int) msg_bits[j];
        
        answ[i] = !(bits.count() % 2 == 0);
    }
    return answ; 
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

int main(int argc, char const *argv[])
{
    matriz_g G;
    unsigned char A = 'B';
    bitset<15> enc_msg = encode_char(A);
    decode_char(enc_msg);

    cout << decode_char(enc_msg)<< endl;
    

    
    return 0;
}
