#include "user.h"
#include "hashTable.h"
#include <iostream>
#include <string.h>
#include <list>
#include <fstream>

using namespace std;

fstream date;


 int nrUtilizator = 0, autentificat = 0;

    list<utilizator> *tabel;
    hashTable :: hashTable()
    {
        tabel = new list<utilizator>[100];
    }

    void inserareElement(char* cheie, struct utilizator utilizator)
    {
        utilizator.parola = SHA256(cheie);
        tabel[nrUtilizator].push_back(utilizator);
    }

    void inserareElementInitial(struct utilizator utilizator)
    {
        tabel[nrUtilizator].push_back(utilizator);
    }

    void autentificare(struct utilizator &utilizator)
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
            list <struct utilizator> :: iterator j;
            j=tabel[i].begin();
            if(strcmp((*j).nume,numeUtilizator)==0)
            {
                if(((*j).parola).compare(SHA256(parola))==0)
                {
                    cout<<"V-ati autentificat cu succes!"<<endl<<endl;
                    autentificat=1;
                    utilizator=*j;
                }
            }
        }
    }

    void schimbareParola(struct utilizator utilizator)
    {
        char numeUtilizator[100],confirmare[100],parola[100];
        int incercari = 3,ok=0,check;

        while(check!=1)
        {
            check = 0;
            cout<<"Introduceti parola actuala: ";
            cin.get(parola,100);
            cin.get();

            cout<<"SHAparola actuala " <<SHA256(parola)<<endl;

            list<struct utilizator> :: iterator verificare;
            for(int i=0; i<=nrUtilizator; i++)
            {
                verificare=tabel[i].begin();
                if(strcmp((*verificare).nume,numeUtilizator)==0)
                {
                    if(((*verificare).parola).compare(SHA256(parola))==0)
                    {
                        break;
                    }
                }
            }

            if(((*verificare).parola).compare(SHA256(parola))==0)
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
                list <struct utilizator> :: iterator j;
                for(int i=0; i<=nrUtilizator; i++)
                {
                    j=tabel[i].begin();
                    if(strcmp((*j).nume,numeUtilizator)==0)
                    {
                        if(((*j).parola).compare(SHA256(parola))==0)
                        {
                            break;
                        }
                    }
                }

                cout<<"Introduceti noua parola:";
                cin>>parola;
                (*j).parola = SHA256(parola);

            }
        }
    }

    void schimbareNumeUtilizator(struct utilizator utilizatorCurent)
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
            list<struct utilizator> :: iterator verificare;

            for(int i=0; i<=nrUtilizator; i++)
            {
                verificare=tabel[i].begin();
                if(((*verificare).parola).compare(SHA256(parola))==0)
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
            list<struct utilizator> :: iterator verificare;
            for(int i=0; i<=nrUtilizator; i++)
            {
                verificare=tabel[i].begin();

                if(((*verificare).parola).compare(SHA256(parola))==0)
                {
                    strcpy((*verificare).nume,numeNou);
                    break;
                }

            }

        }

        cout<<endl<<"Modificarea va fi aplicata atunci cand va relogati!"<<endl;

    }

    void schimbareEmailUtilizator(struct utilizator utilizatorCurent)
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
            list<struct utilizator> :: iterator verificare;

            for(int i=0; i<=nrUtilizator; i++)
            {
                verificare=tabel[i].begin();
                if(((*verificare).parola).compare(SHA256(parola))==0)
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
            list<struct utilizator> :: iterator verificare;

            for(int i=0; i<=nrUtilizator; i++)
            {
                verificare=tabel[i].begin();
                if(((*verificare).parola).compare(SHA256(parola))==0)
                {
                    strcpy((*verificare).email,emailNou);
                    break;
                }

            }

        }

        cout<<endl<<"Modificarea va fi aplicata atunci cand va relogati!"<<endl;

    }

    void rescrierefisier()
    {
        list<struct utilizator>::iterator i;

        date.open("dateUtilizatori.txt");

        for(int j = 1; j<nrUtilizator; j++)
        {
            i=tabel[j].begin();
            //cout<<(*i).id<<" DUPA ID "<<(*i).nume<<" "<<(*i).email<<" "<<(*i).parola<<endl;
            date<<(*i).nume<<" "<<(*i).email<<" "<<(*i).parola<<endl;
        }

        date.close();
    }

