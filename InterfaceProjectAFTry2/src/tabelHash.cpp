#include "tabelHash.h"
#include <iostream>
#include <string.h>
#include <list>
#include <fstream>

using namespace std;

int nrUtilizator = 0;

ifstream citire;
ofstream scriere;

list<tabelHash::utilizator> *tabel;

///SHA256

void tabelHash::SHA256Transform(SHA256_CTX *ctx, uchar data[])
{
    uint a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

    for (i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
    for (; i < 64; ++i)
        m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];
    f = ctx->state[5];
    g = ctx->state[6];
    h = ctx->state[7];

    for (i = 0; i < 64; ++i)
    {
        t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
        t2 = EP0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
    ctx->state[5] += f;
    ctx->state[6] += g;
    ctx->state[7] += h;
}

void tabelHash::SHA256Init(SHA256_CTX *ctx)
{
    ctx->datalen = 0;
    ctx->bitlen[0] = 0;
    ctx->bitlen[1] = 0;
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;
}

void tabelHash::SHA256Update(SHA256_CTX *ctx, uchar data[], uint len)
{
    for (uint i = 0; i < len; ++i)
    {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        if (ctx->datalen == 64)
        {
            SHA256Transform(ctx, ctx->data);
            DBL_INT_ADD(ctx->bitlen[0], ctx->bitlen[1], 512);
            ctx->datalen = 0;
        }
    }
}

void tabelHash::SHA256Final(SHA256_CTX *ctx, uchar hash[])
{
    uint i = ctx->datalen;

    if (ctx->datalen < 56)
    {
        ctx->data[i++] = 0x80;

        while (i < 56)
            ctx->data[i++] = 0x00;
    }
    else
    {
        ctx->data[i++] = 0x80;

        while (i < 64)
            ctx->data[i++] = 0x00;

        SHA256Transform(ctx, ctx->data);
        memset(ctx->data, 0, 56);
    }

    DBL_INT_ADD(ctx->bitlen[0], ctx->bitlen[1], ctx->datalen * 8);
    ctx->data[63] = ctx->bitlen[0];
    ctx->data[62] = ctx->bitlen[0] >> 8;
    ctx->data[61] = ctx->bitlen[0] >> 16;
    ctx->data[60] = ctx->bitlen[0] >> 24;
    ctx->data[59] = ctx->bitlen[1];
    ctx->data[58] = ctx->bitlen[1] >> 8;
    ctx->data[57] = ctx->bitlen[1] >> 16;
    ctx->data[56] = ctx->bitlen[1] >> 24;
    SHA256Transform(ctx, ctx->data);

    for (i = 0; i < 4; ++i)
    {
        hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
    }
}

string tabelHash::SHA256(char* data)
{
    int strLen = strlen(data);
    SHA256_CTX ctx;
    unsigned char hash[32];
    string hashStr = "";

    SHA256Init(&ctx);
    SHA256Update(&ctx, (unsigned char*)data, strLen);
    SHA256Final(&ctx, hash);

    char s[3];
    for (int i = 0; i < 32; i++)
    {
        sprintf(s, "%02x", hash[i]);
        hashStr += s;
    }

    return hashStr;
}

///SHA256 Final

tabelHash::tabelHash()
{
    tabel = new list<utilizator>[100];
}

tabelHash obj;

void tabelHash::inserareElement(char* cheie, struct tabelHash::utilizator utilizator)
{
    utilizator.parola = obj.SHA256(cheie);
    tabel[nrUtilizator].push_back(utilizator);
}

void tabelHash::inserareElementInitial(struct tabelHash::utilizator utilizator)
{
    tabel[nrUtilizator].push_back(utilizator);
}

void tabelHash::autentificare(struct tabelHash::utilizator &utilizator, int &autentificat)
{
    char numeUtilizator[100],parola[100];
    cout<<"Introduceti numele de utilizator: ";
    cin.get(numeUtilizator,99);
    cin.get();
    cout<<"Introduceti parola: ";
    cin.get(parola,99);
    cin.get();
    cout<<endl;

    for(int i=0; i<=nrUtilizator; i++)
    {
        list <struct tabelHash::utilizator> :: iterator j;
        j=tabel[i].begin();
        if(strcmp((*j).nume,numeUtilizator)==0)
        {
            if(((*j).parola).compare(obj.SHA256(parola))==0)
            {
                cout<<"V-ati autentificat cu succes!"<<endl<<endl;
                autentificat=1;
                utilizator=*j;
            }
        }
    }
}

void tabelHash::schimbareParola(struct tabelHash::utilizator utilizator)
{
    char numeUtilizator[100],confirmare[100],parola[100];
    int incercari = 3,ok=0,check;

    while(check!=1)
    {
        check = 0;
        cout<<"Introduceti parola actuala: ";
        cin.get(parola,100);
        cin.get();

        cout<<"SHAparola actuala " <<obj.SHA256(parola)<<endl;

        list<struct tabelHash::utilizator> :: iterator verificare;
        for(int i=0; i<=nrUtilizator; i++)
        {
            verificare=tabel[i].begin();
            if(strcmp((*verificare).nume,numeUtilizator)==0)
            {
                if(((*verificare).parola).compare(obj.SHA256(parola))==0)
                {
                    break;
                }
            }
        }

        if(((*verificare).parola).compare(obj.SHA256(parola))==0)
        {
            check = 1;
        }

        int optiune;

        if(check == 0 )
        {
            cout<<"Parola introdusa este gresita!"<<endl<<endl;
            cout<<"Introduceti un numar corespunzator urmatoarelor optiuni:"<<endl;
            cout<<"1.Doresc sa reincerc introducerea parolei."<<endl;
            cout<<"2.Renunt la decizia de a schimba parola."<<endl<<endl;
            cin>>optiune;
            cin.get();
            cout<<endl;
            if(optiune==2)
            {
                break;
            }
            else
            {
                if(optiune!=1)
                {
                    cout<<"NU ati introdus o valoare valida!"<<endl;

                }
            }
        }
    }

    if(check==1)
    {
        cout<<"Pentru continuarea procesului va rugam sa va confirmati parola: ";
        cin.get(confirmare,99);
        cin.get();
        while(strcmp(confirmare,parola)!=0&&incercari!=0)
        {
            cout<<endl;
            cout<<"Parolele nu se potrivesc!"<<endl;
            cout<<"Mai aveti "<<incercari<<" incercari!"<<endl;
            cout<<"Va rugam sa va confirmati parola : ";
            cin.get(confirmare,99);
            cin.get();

            incercari--;
        }
        if(incercari==0)
        {
            cout<<"Va rugam sa reveniti mai tarziu!"<<endl<<endl;
        }
        else
        {
            list <struct tabelHash::utilizator> :: iterator j;
            for(int i=0; i<=nrUtilizator; i++)
            {
                j=tabel[i].begin();
                if(strcmp((*j).nume,numeUtilizator)==0)
                {
                    if(((*j).parola).compare(obj.SHA256(parola))==0)
                    {
                        break;
                    }
                }
            }

            cout<<"Introduceti noua parola:";
            cin>>parola;
            (*j).parola = obj.SHA256(parola);

        }
    }
}

void tabelHash::schimbareNumeUtilizator(struct tabelHash::utilizator utilizatorCurent)
{
    int check;
    char parola[100];
    cout<<endl;
    while(check!=1)
    {
        check = 0;
        cout<<"Pentru a face aceasta modificare, va rugam sa reintroduceti parola:";
        cin.get(parola,99);
        cin.get();
        list<struct tabelHash::utilizator> :: iterator verificare;

        for(int i=0; i<=nrUtilizator; i++)
        {
            verificare=tabel[i].begin();
            if(((*verificare).parola).compare(obj.SHA256(parola))==0)
            {
                check=1;
                break;
            }

        }

        int optiune;

        if(check == 0 )
        {
            cout<<"Parola introdusa este gresita!"<<endl<<endl;
            cout<<"Introduceti un numar corespunzator urmatoarelor optiuni:"<<endl;
            cout<<"1.Doresc sa reincerc introducerea parolei."<<endl;
            cout<<"2.Renunt la decizia de a schimba numele de utilizator."<<endl<<endl;
            cin>>optiune;
            cin.get();
            cout<<endl;

            switch(optiune)
            {
            case 2:
                break;
            default:
                cout<<"NU ati introdus o valoare valida!"<<endl;
                break;
            }
        }
    }

    if(check==1)
    {
        char numeNou[100];
        cout<<"Introduceti noul nume de utilizator : ";
        cin.get(numeNou,99);
        cin.get();
        list<struct tabelHash::utilizator> :: iterator verificare;
        for(int i=0; i<=nrUtilizator; i++)
        {
            verificare=tabel[i].begin();

            if(((*verificare).parola).compare(obj.SHA256(parola))==0)
            {
                strcpy((*verificare).nume,numeNou);
                break;
            }

        }

    }

    cout<<endl<<"Modificarea va fi aplicata atunci cand va relogati!"<<endl;

}

void tabelHash::schimbareEmailUtilizator(struct tabelHash::utilizator utilizatorCurent)
{
    int check;
    char parola[100];
    cout<<endl;
    while(check!=1)
    {
        check = 0;
        cout<<"Pentru a face aceasta modificare, va rugam sa reintroduceti parola:";
        cin.get(parola,99);
        cin.get();
        list<struct tabelHash::utilizator> :: iterator verificare;

        for(int i=0; i<=nrUtilizator; i++)
        {
            verificare=tabel[i].begin();
            if(((*verificare).parola).compare(obj.SHA256(parola))==0)
            {
                check=1;
                break;
            }

        }

        int optiune;

        if(check == 0 )
        {
            cout<<"Parola introdusa este gresita!"<<endl<<endl;
            cout<<"Introduceti un numar corespunzator urmatoarelor optiuni:"<<endl;
            cout<<"1.Doresc sa reincerc introducerea parolei."<<endl;
            cout<<"2.Renunt la decizia de a schimba adresa de email."<<endl<<endl;
            cin>>optiune;
            cin.get();
            cout<<endl;
            if(optiune==2)
            {
                break;
            }
            else
            {
                if(optiune!=1)
                {
                    cout<<"NU ati introdus o valoare valida!"<<endl;
                    break;

                }
            }
        }
    }

    if(check==1)
    {
        char emailNou[100];
        cout<<"Introduceti noua adresa de email : ";
        cin.get(emailNou,99);
        cin.get();
        list<struct tabelHash::utilizator> :: iterator verificare;

        for(int i=0; i<=nrUtilizator; i++)
        {
            verificare=tabel[i].begin();
            if(((*verificare).parola).compare(obj.SHA256(parola))==0)
            {
                strcpy((*verificare).email,emailNou);
                break;
            }

        }

    }

    cout<<endl<<"Modificarea va fi aplicata atunci cand va relogati!"<<endl;

}

void tabelHash::rescrierefisier()
{
    list<struct tabelHash::utilizator>::iterator i;
    scriere.clear();
    scriere.open("dateUtilizatori.txt");

    for(int j = 1; j<=nrUtilizator; j++)
    {
        i=tabel[j].begin();
        //cout<<(*i).id<<" DUPA ID "<<(*i).nume<<" "<<(*i).email<<" "<<(*i).parola<<endl;
        scriere<<(*i).nume<<" "<<(*i).email<<" "<<(*i).parola<<endl;
    }
    //cout<<"Nr utilizatori dupa rescriere : "<<nrUtilizator<<endl;
    scriere.close();
}

void tabelHash::adaugareUtilizator(tabelHash th)
{
    struct tabelHash::utilizator utilizator;
    char parola[100],confirmare[100];
    cout<<"Creare cont nou:"<<endl;
    cout<<endl;
    cout<<"Numele de utilizator, email-ul, si parola nu trebuie sa contina spatii!!!"<<endl;
    cout<<endl;
    utilizator.id = nrUtilizator;
    nrUtilizator++;
    cout<<"Introduceti numele de utilizator : ";
    cin.get(utilizator.nume,99);
    cin.get();
    cout<<"Introduceti email-ul dumneavoastra : ";
    cin.get(utilizator.email,99);
    cin.get();
    cout<<"Introduceti parola : ";
    cin.get(parola,99);
    cin.get();
    cout<<"Va rugam sa va confirmati parola : ";
    cin.get(confirmare,99);
    cin.get();

    while(strcmp(confirmare,parola)!=0)
    {
        cout<<endl;
        cout<<"Parolele nu se potrivesc!"<<endl<<endl;
        cout<<"Va rugam sa va confirmati parola : ";
        cin.get(confirmare,99);
        cin.get();
    }
    th.inserareElement(parola,utilizator);
}

void tabelHash::informatiiCont(struct tabelHash::utilizator utilizatorCurent)
{
    cout<<endl;
    cout<<"Id utilizator: "<<utilizatorCurent.id<<endl;
    cout<<"Nume utilizator: "<<utilizatorCurent.nume<<endl;
    cout<<"Email utilizator: "<<utilizatorCurent.email<<endl;
    cout<<endl;
}

void tabelHash::initializareLista(tabelHash th)
{
    citire.open("dateUtilizatori.txt");

    char linie[201];
    int nrRanduri=0;
    while(!citire.eof())
    {
       citire.getline(linie,200);
       nrRanduri++;
    }

    //out<<"Numar linii: "<<nr<<endl;

    citire.close();

    citire.open("dateUtilizatori.txt");

    while(nrRanduri!=1)
    {
        struct utilizator utilizator;
        utilizator.id = nrUtilizator;
        nrUtilizator++;
        citire>>utilizator.nume;
        citire>>utilizator.email;
        citire>>utilizator.parola;
        //cout<<"Citit: "<<utilizator.nume<<" "<<utilizator.email<<" "<<utilizator.parola;

        th.inserareElementInitial(utilizator);
        nrRanduri--;
    }
    //cout<<"Nr utilizatori dupa initializare : "<<nrUtilizator<<endl;
    citire.close();
}
