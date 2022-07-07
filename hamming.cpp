#include <iostream>
#include <bitset>
#include <stdlib.h>
#include <vector>
#include <ctime>

using namespace std;
typedef struct matriz_g;
typedef struct matriz_h;

void genMG(matriz_g &G); // Cria a matriz geradora 15 11

void genMH(matriz_h &H); // Cria a matriz verificadora  4 15

bitset<15> encode_char(unsigned char charac);   // Codifica um seguimento de 
                                                // mensagem de tamanho 11 com o codigo de hamming.

short int valid_encode_char(bitset<15> msg_bits);   // Utiliza a matriz a matriz 
                                                    // verificadora para validar a mensagem.

unsigned char decode_char(bitset<15> enc_msg);  // Retira oito bits de inforamação de um seguimento
                                                // de mensagem de tamanho 11 codificada com o codigo
                                                // de hamming.

void encode_str(string &str, vector<bitset<15> > &encoded_str); // Utilizando codigo de hamming
                                                                // codifica um string.

void decode_str(vector<bitset<15> > &encoded_str, string &str); // Utilizando codigo de hamming
                                                                // decodifica uma string.


bitset<15>  dirty_msg(bitset<15> encoded_msg);  // Suja um seguimento de bits com tamanho 11 
                                                // gerado com o codigo de hamming de maneira 
                                                // desparametrizada em um ou dois bits ou nenhum.

void simulator(string &msg);    // Um simulador imaginário, para envio de bits

void show_ascii();  // Uma função que mostra os caracteres da tabela ascii,
                    // utilizando a função simulator.


int main(int argc, char const *argv[])
{
    string msg;

    srand(time(NULL));

    getline(cin, msg);

    simulator(msg);

    return 0;
}

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

void encode_str(string &str,vector<bitset<15> > &encoded_str)
{
    for (size_t i = 0; i < str.length(); i++)
        encoded_str.push_back(encode_char(str.at(i)));
}

void decode_str(vector<bitset<15> > &encoded_str, string &str)
{
    for (size_t i = 0; i < encoded_str.size(); i++)
        str+=decode_char(encoded_str.at(i));

}

bitset<15>  dirty_msg(bitset<15> encoded_msg)
{
    if (rand()%2 == 1)
    {
        short p1 = rand()%15;
        encoded_msg[p1] = !(encoded_msg[p1]);
    }
    
    return encoded_msg;
}

void simulator(string &msg)
{
    vector<bitset<15> > encoded_msg;
    unsigned int err = 0;
    encode_str(msg,encoded_msg);

    for (size_t i = 0; i < encoded_msg.size(); i++)
    {
        bitset<15> encoded_char = dirty_msg(encoded_msg.at(i));
        if (valid_encode_char(encoded_char))
        {
            err++; i--; continue;
        }
        printf("%c%5cerros: %2d%5c hamming:",
            decode_char(encoded_char), '\0', err, '\0');

        cout    << encoded_char << "   " << bitset<8>{decode_char(encoded_char)}
                <<"  ascii_code: " << (int)decode_char(encoded_char) << endl;  

        err = 0;
    }
}

void show_ascii()
{
    string msg;
    for (unsigned int i = 0; i < 256; i++)
    {
        msg += (unsigned char) i;
    }
    simulator(msg);
}
