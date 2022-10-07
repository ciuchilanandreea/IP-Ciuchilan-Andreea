#include <iostream>
#include <cstring>
#include <graphics.h>
#include <winbgim.h>
#include <math.h>
#define H 900
#define W 1600
#define PI 3.14159265
using namespace std;

/**
        Proiect Introducere in programare
        Nr: 10
*/

int lgt = 0; /// lungime text, ne folosim de functia textwidth
int ht = 0; /// inaltime text, ne folosim de functia textheight
int culoare;
int c_bordura, c_fill, c_text; /// culoarea bordurii la nod (o folosim la coloratul arborelui ca sa nu fie la fel cu muchia)
char noduri[10]; /// pentru numarul de noduri
char rezultat[20]; /// pentru rezultatul expresiei
char alg[256];   ///expresia algebrica
int n;   ///numarul de caractere din expresie
char cuv[256][256];  ///vector de cuvinte in care se retin operatii si numere ale expresiei
int lg;   ///numarul de operatii si numere ale expresiei
int nrs,nrd;  ///inaltimea subarborelui stang si drept
int nr;  ///numarul total de noduri
int st; ///numarul de noduri in subarborele stang si subarborele drept
int LG; /// pentru a verifica incadrarea in pagina a arborelui
float rez; /// rezultatul expresiei

struct nod  ///arborele
{
    char info[256]="";
    nod* st;
    nod* dr;
};

void adauga(nod *&a,char el[256],nod *&p)   ///memorarea in arbore a informatiei
{
    if (a == 0)
    {

        a = new nod;
        strcpy(a->info, el);
        strcpy(cuv[lg],el);
        lg++;
        p=new nod;
        p=a;

        a->st = 0;
        a->dr = 0;

    }
    else if(a->st==0)
        adauga(a->st,el,p);
    else
        adauga(a->dr,el,p);

}

int cifre(char ch)   ///verifica daca un caracter dat este cifra
{
    if (ch >='0' && ch<='9')
        return 1;
    else
        return 0;
}

int paranteze(int i,int j,int poz)  ///verifica daca operatia alg[poz] se afla intre paranteze
{
    int z;
    int poz1=-1;
    int poz2=300;
    int nr0 = 0, nr1 = 0;
    for (z = i; z <= poz; z++)
    {
        if (alg[z] == '(')
        {
            nr0++;
            poz2=z;
        }
        if (alg[z] == ')')
        {
            nr1++;
            poz1=z;
        }
    }
    if (nr0 == nr1)
    {
        if(poz1<poz || poz<poz2)
            return 1;
        else
            return 2;
    }
    else
        return 0;

}

int caracter(char ch)
{
    if(ch!='(' && ch!=')' && ch!='s' && ch!='i' && ch!='n' && ch!='c' && ch!='o' && ch!='t' && ch!='a' &&ch!='m'&& ch!='l' && ch!='+' && ch!='-' && ch!='*' && ch!='/' && ch!='^' && ch!='%' && ch!=',' && !cifre(ch) && ch!='>' && ch!='<' && ch!='q' && ch!='r') return 0;
    return 1;
}


int functii(int i)///functia verifica posibila existenta a unei virgule in mod eronat in
{                ///functiile trigonometrice
    int j,z;
    int poz=i;    ///pozitia de pe care incepem cautarea
    int nrf=0,nrp=0,nrP=0,ok=0;
    nrp=1;
    i++;
    while(nrp!=nrP)   ///cautam ultima aparitie a parantezei
    {
        i++;
        if(alg[i]=='(') nrp++;
        if(alg[i]==')') nrP++;

    }

    for(z=i-2;z>=0;z--) /// incepem sa parcurgem in sens invers toti termenii si operatiile
    {                   ///din functia trigonometrica
        ok=0; nrp=0;
        if(alg[z]==',')   ///daca este virgula atunci verificam daca apartine funtiilor max si min
          {
            for(j=z; j>=poz && !ok; j--)
            {

                if(alg[j]=='m' && alg[j+1]=='i' && alg[j+2]=='n')
                    ok=1;
                    if(alg[j]=='(') nrp++;
                if(alg[j]==')')
                    nrp--;
                if(alg[j]=='m' && alg[j+1]=='a' && alg[j+2]=='x')
                    ok=1;
            }
            if(ok==1)  ///daca apar functiile max si min, dar virgula nu apare in interiorul lor returnam 0
            if(nrp!=1)
                return 0;
        }
    }
    return 1;   ///virgula apartine unui numar zecimal sau este in interiorul functiilor max si min
}

int verificare()   ///verifica daca expresia data este corecta
{
    if(!paranteze(0,n-1,n))
        return 0;
    else
    {
        int i;
        for(i=0; i<n; i++)
        {
            int ok=-1;
            if(alg[i]=='(' && !(alg[i+1]==')' || alg[i+1]==NULL))
                ok=0;
            else if(alg[i]=='+'|| alg[i]=='/' || alg[i]=='*' || alg[i]=='^' || alg[i]=='%')
            {
                if(!caracter(alg[i-1]) || !caracter(alg[i+1]) ||(alg[i-1]=='(' && alg[i+1]==')') ||alg[i-1]=='+' || alg[i-1]=='-' || alg[i-1]=='/' || alg[i-1]=='%' || alg[i-1]=='^' || alg[i-1]=='*' || alg[i+1]=='+' || alg[i+1]=='-' || alg[i+1]=='/' || alg[i+1]=='%' || alg[i+1]=='^' || alg[i+1]=='*'|| alg[i+1]==NULL)
                    return 0;
                else
                    ok=0;
            }
            else if(alg[i]=='-')
            {
                if(i>0)
                {
                    if(!caracter(alg[i-1]) || alg[i-1]=='+' || alg[i-1]=='-' || alg[i-1]=='/' || alg[i-1]=='%' || alg[i-1]=='^' || alg[i-1]=='*' || alg[i+1]=='+' || alg[i+1]=='-' || alg[i+1]=='/' || alg[i+1]=='%' || alg[i+1]=='^' || alg[i+1]=='*'|| alg[i+1]==NULL)

                        return 0;
                    else
                        ok=0;
                }
                else
                    ok=0;
            }
            else if(alg[i]=='s'&& alg[i+1]=='i' && alg[i+2]=='n' && alg[i+3]!=NULL &&alg[i+3]=='(' && functii(i+3))
                ok=2;
            else if(alg[i]=='c' && alg[i+1]=='o' && alg[i+2]=='s' && alg[i+3]!=NULL && alg[i+3]=='(' && functii(i+3))
                ok=2;
            else if(alg[i]=='t' && alg[i+1]=='a' && alg[i+2]=='n' && alg[i+3]!=NULL && alg[i+3]=='(' && functii(i+3))
                ok=2;
            else if(alg[i]=='c' && alg[i+1]=='t' && alg[i+2]=='a' && alg[i+3]=='n' && alg[i+4]!=NULL && alg[i+4]=='(' && functii(i+4))
                ok=3;
            else if(alg[i]=='m' && alg[i+1]=='i' && alg[i+2]=='n'&& alg[i+3]!=NULL && (alg[i+3]=='(' || cifre(alg[i+3])))
                ok=2;
            else if(alg[i]=='m' && alg[i+1]=='a' && alg[i+2]=='x' && alg[i+3]!=NULL && (alg[i+3]=='(' || cifre(alg[i+3])))
                ok=2;
            else if(alg[i]=='s' && alg[i+1]=='q' && alg[i+2]=='r' && alg[i+3]=='t' && alg[i+4]!=NULL && (alg[i+4]=='(' || cifre(alg[i+4])))
                ok=3;
            else if(alg[i]=='m' && alg[i+1]=='o' && alg[i+2]=='d' && alg[i+3]!=NULL && (alg[i+3]=='(' || cifre(alg[i+3])))
                ok=2;
            else if(alg[i]=='l' && alg[i+1]=='o' && alg[i+2]=='g' && alg[i+3]!=NULL && (alg[i+3]=='(' || cifre(alg[i+3])))
                ok=2;
            else if(alg[i]=='l' && alg[i+1]=='g' && alg[i+2]!=NULL && (alg[i+2]=='(' || cifre(alg[i+2])))
                ok=1;
            else if(alg[i]=='l' && alg[i+1]=='n' && alg[i+2]!=NULL && (alg[i+2]=='(' || cifre(alg[i+2])))
                ok=1;
            else if(alg[i]=='<' && alg[i+1]=='<' && alg[i+2]!=NULL && (alg[i+2]=='(' || cifre(alg[i+2])))
                ok=1;
            else if(alg[i]=='>' && alg[i+1]=='>' && alg[i+2]!=NULL && (alg[i+2]=='(' || cifre(alg[i+2])))
                ok=1;
            else if(alg[i]=='a' && alg[i+1]=='b' && alg[i+2]=='s' && alg[i+3]!=NULL && (alg[i+3]=='(' || cifre(alg[i+3])))
                ok=2;
            else if(alg[i]=='(' && caracter(alg[i+1]) && alg[i+1]!=')')
                ok=0;
            else if(alg[i]==')')
                ok=0;
            else if(alg[i]==',' && alg[i+1]!=NULL && caracter(alg[i-1]) && alg[i-1]!='(' && caracter(alg[i+1]) && alg[i+1]!=')')
                ok=0;
            else if(cifre(alg[i]))
                ok=0;
            else if(!caracter(alg[i]))
                return 0;
            if(ok==-1)
                return 0;
            else
                i=i+ok;
        }


    }
    return 1;
}

int functie(int i)
{
    int j;
    int nrf=0,nrp=0;
    for(j=i; j>=0; j--)
    {
        if(alg[j]=='(')
            nrp++;
        if(alg[j]=='m' && alg[j+1]=='i' && alg[j+2]=='n')
            nrf++;
        if(alg[j]==')')
            nrp--;
        if(alg[j]=='m' && alg[j+1]=='a' && alg[j+2]=='x')
            nrf++;
    }
    if(nrf>0 && nrp>0)
        return 1;
    return 0;
}

void rezolvare(int i, int j, nod *&a,nod*p)  ///functia parcurge expresia si desparte operatiile de numere si in functie
{
    ///de importanta lor se introduc in arborele a

    if (i <=j && i>=0 && j>=0)
    {
        int z, ok = 0, poz;
        for (z = j; z >= i && !ok; z--)
        {
            if (alg[z] == '+')
            {
                if (paranteze(i,j,z)==1)
                {
                    ok = 1;
                    poz = z;
                }
            }
            if (alg[z] == '-')
            {
                if (paranteze(i,z-1,z)==1 && alg[z-1]!='(' && z>0 && alg[z-1]!=',')
                {
                    ok = 1;
                    poz = z;
                }
            }
        }

        if (ok == 1)
        {
            char ch[256];
            ch[0] = alg[poz];
            ch[1] = 0;
            adauga(a, ch,p);
            rezolvare(i, poz - 1, p->st,p);
            rezolvare(poz + 1, j, p->dr,p);
        }
        if (ok == 0)
        {
            for (z = j; z >= i && !ok; z--)
                if (alg[z] == '*' || alg[z] == '/' || alg[z]=='^' || alg[z]=='%')
                    if (paranteze(i,j,z)==1)
                    {
                        ok = 1;
                        poz = z;
                    }
            if (ok == 1)
            {
                char ch[256];
                ch[0] = alg[poz];
                ch[1] = 0;
                adauga(a, ch,p);
                rezolvare(i, poz - 1, p->st,p);
                rezolvare(poz + 1, j, p->dr,p);
            }
        }
        if (ok == 0)
        {
            for (z = j; z >= i && !ok; z--)
                if ((alg[z] == '>' && alg[z+1] == '>') || (alg[z]=='<' && alg[z+1]=='<'))
                    if (paranteze(i,j,z)==1)
                    {
                        ok = 1;
                        poz = z;
                    }
            if (ok == 1)
            {
                char ch[256];
                ch[0] = alg[poz];
                ch[1]=alg[poz+1];
                ch[2] = 0;
                adauga(a, ch,p);
                rezolvare(i, poz - 1, p->st,p);
                rezolvare(poz + 2, j, p->dr,p);
            }
        }
        if (ok == 0)
        {
            for (z = j; z >= i && !ok; z--)
                if (ok == 0)
                {
                    if (alg[i] == 's' && alg[i + 1] == 'i' && alg[i + 2] == 'n')
                    {
                        char ch[256];
                        strcpy(ch, "sin");
                        adauga(a, ch,p);
                        ok = 1;
                        rezolvare(i + 3, j, p->st,p);
                    }
                    else if (alg[i] == 'c' && alg[i + 1] == 'o' && alg[i + 2] == 's')
                    {
                        char ch[256];
                        strcpy(ch, "cos");
                        adauga(a,ch,p);
                        ok = 1;
                        rezolvare(i + 3, j, p->st,p);
                    }
                    else if (alg[i] == 't' && alg[i + 1] == 'a' && alg[i + 2] == 'n')
                    {
                        char ch[256];
                        strcpy(ch, "tan");
                        adauga(a, ch,p);
                        ok = 1;
                        rezolvare(i + 3, j, p->st,p);
                    }
                    else if (alg[i] == 'c' && alg[i + 1] == 't' && alg[i + 2] == 'a' && alg[i+3]=='n')
                    {
                        char ch[256];
                        strcpy(ch, "ctan");
                        adauga(a, ch,p);
                        ok = 1;
                        rezolvare(i + 4, j, p->st,p);
                    }
                    else if (alg[i] == 'a' && alg[i + 1] == 'b' && alg[i + 2] == 's')
                    {
                        char ch[256];
                        strcpy(ch, "abs");
                        adauga(a,ch,p);
                        ok = 1;
                        rezolvare(i + 3, j, p->st,p);
                    }

                    else if (alg[i] == 'm' && alg[i + 1] == 'i' && alg[i + 2] == 'n')
                    {
                        char ch[256];
                        strcpy(ch, "min");
                        adauga(a,ch,p);
                        ok = 1;
                        int x;
                        int nrp=0;
                        for(x=i+3;x<=j;x++)
                        {
                            if(alg[x]=='(') nrp++;
                            if(alg[x]==')') nrp--;
                            if(nrp==1 && alg[x]==',') poz=x;
                        }
                        rezolvare(i+4,poz-1,p->st,p);
                        rezolvare(poz+1,j-1,p->dr,p);
                    }
                    else if (alg[i] == 'm' && alg[i + 1] == 'a' && alg[i + 2] == 'x')
                    {
                        char ch[256];
                        strcpy(ch, "max");
                        adauga(a,ch,p);
                        ok = 1;
                        int x;
                        int nrp=0;
                        for(x=i+3;x<=j;x++)
                        {
                            if(alg[x]=='(') nrp++;
                            if(alg[x]==')') nrp--;
                            if(nrp==1 && alg[x]==',') poz=x;
                        }
                        rezolvare(i+4,poz-1,p->st,p);
                        rezolvare(poz+1,j-1,p->dr,p);
                    }

                    else if (alg[i] == 'l' && alg[i + 1] == 'g')
                    {
                        char ch[256];
                        strcpy(ch, "lg");
                        adauga(a,ch,p);
                        ok = 1;
                        rezolvare(i + 2, j, p->st,p);
                    }
                    else if (alg[i] == 'l' && alg[i + 1] == 'n')
                    {
                        char ch[256];
                        strcpy(ch, "ln");
                        adauga(a,ch,p);
                        ok = 1;
                        rezolvare(i + 2, j, p->st,p);
                    }
                    else if (alg[i] == 'l' && alg[i + 1] == 'o' && alg[i+2]=='g')
                    {
                        char ch[256];
                        strcpy(ch, "log");
                        adauga(a,ch,p);
                        ok = 1;
                        rezolvare(i + 3, j, p->st,p);
                    }

                    else if (alg[i] == 's' && alg[i + 1] == 'q' && alg[i + 2] == 'r' && alg[i + 3] == 't')
                    {
                        char ch[256];
                        strcpy(ch, "sqrt");
                        adauga(a,ch,p);
                        ok = 1;
                        rezolvare(i + 4, j, p->st,p);
                    }
                }

            if (ok == 0)
                if (alg[i] == '(' && alg[j]==')')
                {
                    rezolvare(i + 1, j - 1, a,p);
                    ok=1;
                }
            if(ok==0)
            {
                if(alg[i]=='e')
                {
                    char ch[256];
                    strcpy(ch, "e");
                    adauga(a,ch,p);
                    rezolvare(i+1,j,a,p);
                    ok=1;
                }
            }
            if(ok==0)
            {
                char ch[256];
                int x = -1;
                z = i;
                while ((cifre(alg[z]) ||alg[z]==','|| alg[z]=='-') && z<=j)
                {
                    z++;
                    x++;
                    ch[x] = alg[z-1];

                }
                ch[x+1]=0;
                if(x!=-1)
                {
                    if(z==j)
                    {
                        adauga(a, ch,p);
                    }
                    else
                    {
                        adauga(a,ch,p);
                        rezolvare(i+x+1,j,p,p);
                    }
                }
            }
        }
    }
}

float numar(char ch[])
{
    int Lg=strlen(ch);
    int i;
    float p=1;
    float c2=0;
    for(i=Lg-1; i>=0; i--)
    {
        if(ch[i]==',')
        {
            c2=c2/p;
            p=1;
        }
        else
            if(ch[i]=='-') c2=-1*c2;
        else
        {
            c2=c2+(ch[i]-'0')*p;
            p=p*10;
        }
    }
    return c2;
}

float RezolvareExpresie(nod*a)
{
    if(a!=0)
    {
        char ch[100];
        strcpy(ch,a->info);
        if(ch[0]=='-' && !cifre(ch[1]))
            return RezolvareExpresie(a->st)-RezolvareExpresie(a->dr);
        if(ch[0]=='+')
            return RezolvareExpresie(a->st)+RezolvareExpresie(a->dr);
        if(ch[0]=='/')
            return RezolvareExpresie(a->st)/RezolvareExpresie(a->dr);
        if(ch[0]=='*')
            return RezolvareExpresie(a->st)*RezolvareExpresie(a->dr);
        if(ch[0]=='%')
        {
            int p,c;
            float c1,c2;
            c1=RezolvareExpresie(a->st);
            c2=RezolvareExpresie(a->dr);
            c=c1/c2;
            p=c2*c;
            c1=c1-p;
            return c1;
        }
        if(ch[0]=='^')
            return pow(RezolvareExpresie(a->st),RezolvareExpresie(a->dr));
        if(ch[0]=='s' && ch[1]=='i' && ch[2]=='n')
            return sin(RezolvareExpresie(a->st)*PI/180);
        if(ch[0]=='c' && ch[1]=='o' && ch[2]=='s')
            return cos(RezolvareExpresie(a->st)*PI/180);
        if(ch[0]=='m' && ch[1]=='i' && ch[2]=='n')
            return min(RezolvareExpresie(a->st),RezolvareExpresie(a->dr));
        if(ch[0]=='m' && ch[1]=='a' && ch[2]=='x')
            return max(RezolvareExpresie(a->st),RezolvareExpresie(a->dr));
        if(ch[0]=='t' && ch[1]=='a' && ch[2]=='n')
            return tan(RezolvareExpresie(a->st)*PI/180);
        if(ch[0]=='c' && ch[1]=='t' && ch[2]=='a' && ch[3]=='n')
            return 1/tan(RezolvareExpresie(a->st)*PI/180);
        if(ch[0]=='a' && ch[1]=='b' && ch[2]=='s')
            return abs(RezolvareExpresie(a->st));
        if(ch[0]=='s' && ch[1]=='q' && ch[2]=='r' && ch[3]=='t')
            return sqrt(RezolvareExpresie(a->st));
        if(ch[0]=='l' && ch[1]=='o' && ch[2]=='g')
            return log2(RezolvareExpresie(a->st));
        if(ch[0]=='l' && ch[1]=='g')
            return log10(RezolvareExpresie(a->st));
        if(ch[0]=='l' && ch[1]=='n')
        {
            if(RezolvareExpresie(a->st)==2)
            {
                 return 1;
            }
            else
            {
                return 0;
            }
        }
        if(ch[0]=='e')
            return 2;
        if(ch[0]=='<' && ch[1]=='<')
        {
            int t=1;
            float c2=RezolvareExpresie(a->dr);
            float c1=RezolvareExpresie(a->st);
            while(c2)
            {
                t=t*2;
                c2--;
            }
            c1=c1*t;
            return c1;
        }
        if(ch[0]=='>' && ch[1]=='>')
        {
            int t=1;
            float c2=RezolvareExpresie(a->dr);
            float c1=RezolvareExpresie(a->st);
            while(c2)
            {
                t=t*2;
                c2--;
            }
            c1=c1/t;
            return c1;
        }
        if(cifre(ch[0]) || cifre(ch[1]))
        {
            cout<<nr<<" ";
            return numar(ch);
        }
    }
    else
        return 0;
}

void calcul(nod *a)
{
    float c1,c2;
    c1=RezolvareExpresie(a->st);
    c2=RezolvareExpresie(a->dr);
    if(cuv[0][0]=='+')
        c1=c1+c2;
    else if(cuv[0][0]=='-')
        c1=c1-c2;
    else if(cuv[0][0]=='*')
        c1=c1*c2;
    else if(cuv[0][0]=='%')
    {
        int p,c;
        c=c1/c2;
        p=c2*c;
        c1=c1-p;
    }
    else if(cuv[0][0]=='^')
        c1=pow(c1,c2);
    else if(cuv[0][0]=='/')
        c1=c1/c2;
    else if(cuv[0][0]=='l' && cuv[0][1]=='o' && cuv[0][2]=='g')
        c1=log2(c1);
    else if(cuv[0][0]=='l' && cuv[0][1]=='g')
        c1=log10(c1);
    else if(cuv[0][0]=='l' && cuv[0][1]=='n')
        {if(c1==2) c1=1;
         else c1=0;}
    else if(cuv[0][0]=='s' && cuv[0][1]=='i' && cuv[0][2]=='n')
        c1=sin(c1*PI/180);
    else if(cuv[0][0]=='c' && cuv[0][1]=='o' && cuv[0][2]=='s')
        c1=cos(c1*PI/180);
    else if(cuv[0][0]=='t' && cuv[0][1]=='a' && cuv[0][2]=='n')
        c1=tan(c1*PI/180);
    else if(cuv[0][0]=='c' && cuv[0][1]=='t' && cuv[0][2]=='a' && cuv[0][3]=='n')
        c1=1/(tan(c1*PI/180));
    else if(cuv[0][0]=='a' && cuv[0][1]=='b' && cuv[0][2]=='s')
        c1=abs(c1);
    else if(cuv[0][0]=='s' && cuv[0][1]=='q' && cuv[0][2]=='r' && cuv[0][3]=='t')
        c1=sqrt(c1);
    else if(cuv[0][0]=='m' && cuv[0][1]=='i' && cuv[0][2]=='n')
        c1=min(c1,c2);
    else if(cuv[0][0]=='m' && cuv[0][1]=='a' && cuv[0][2]=='x')
        c1=max(c1,c2);
    else if(cuv[0][0]=='>' && cuv[0][1]=='>')
    {
        int t=1;
        while(c2)
        {
            t=t*2;
            c2--;
        }
        c1=c1/t;
    }
    else if(cuv[0][0]=='<' && cuv[0][1]=='<')
    {
        int t = 1;
        while(c2)
        {
            t = t*2;
            c2--;
        }
        c1=c1*t;
    }
    else if(cifre(cuv[0][0]) || cifre(cuv[0][1])) c1 = numar(cuv[0]);

    rez = c1;
}


void aranjarevec()   ///introduce caracterul $ la finalul subarborelui stang
{
    int i;
    lg++;
    for(i=lg; i>st; i--)
    {
        strcpy(cuv[i],cuv[i-1]);
    }
    strcpy(cuv[st+1],"$");
}

int nrnodurist(nod *a)
{
    if(a==0) return 0;
    else return 1+nrnodurist(a->st)+nrnodurist(a->dr);
}

void IntToChar(float n, char t[])
{
    int ascii[12];
    int cif[12];
    int i, k1, as, m;
    int cnt = 1; /// am virgula
    int neg = 0; /// nu e negativ
    int zero = 0; /// nu e zero
    if(n < 0)
    {
        n = n*(-1);
        neg = 1;
    }
    if(n == (int)n)
    {
        cnt = 0; /// nu am virgula
        m = n;
        if(n == 0)
        {
            t[0] = '0';
            t[1] = 0;
            zero = 1;
        }
    }
    k1 = 1;
    as = 48;
    for(i = 0; i <= 9; i++)
        ascii[i] = as++;
    if(cnt  == 0 && zero == 0)
    {
            while(m != 0)
            {
                cif[k1++] = m%10;
                m /= 10;
            }
            if(neg == 1)
            {
                as = 1;
                t[0] = '-';
            }
            else as = 0;
            k1--;
            for(i = k1; i >= 1; i--)
            {
                t[as++] = (char)ascii[cif[i]];
            }
            t[as] = 0;
    }
    else if(cnt == 1)
    {
        m = (int)(n*100);
        if(m < 10)
        {
            as = 0;
            if(neg == 1)
            {
                as = 1;
                t[0] = '-';
            }
            t[as++] = '0';
            t[as++] = ',';
            t[as++] = '0';
            t[as++] = (char)ascii[m];
            t[as] = 0;
        }
        else if(m >= 10 && m < 100)
        {
            as = 0;
            if(neg == 1)
            {
                as = 1;
                t[0] = '-';
            }
            t[as++] = '0';
            t[as++] = ',';
            t[as++] = (char)ascii[(m/10)%10];
            t[as++] = (char)ascii[m%10];
            t[as] = 0;
        }
        else
        {
            while(m != 0)
            {
                cif[k1++] = m%10;
                m /= 10;
            }
            if(neg == 1)
            {
                as = 1;
                t[0] = '-';
            }
            else as = 0;
            k1--;
            for(i = k1; i >= 3; i--)
            {
                t[as++] = (char)ascii[cif[i]];
            }
            t[as++] = ',';
            t[as++] = (char)ascii[cif[2]];
            t[as++] = (char)ascii[cif[1]];
        }
    }
}

int Snrnoduri(nod *a)   ///calculeaza numarul total de noduri dintr un arbore
{
    if(a==0)
        return 0;
    else
        return 1+Snrnoduri(a->st);
}
int Dnrnoduri(nod *a)   ///calculeaza numarul total de noduri dintr un arbore
{
    if(a==0)
        return 0;
    else return 1+Dnrnoduri(a->dr);
}

void DrawLine(int x1, int y1, int x2, int y2, int c) /// Functie care deseneaza o linie de culoare c
{
    setlinestyle(0, 0xFFFF, 3);
    setcolor(c);
    line(x1, y1, x2, y2);
}

void DrawSimpleBar(int x1, int y1, int x2, int y2, int c) /// Functie care deseneaza un dreptunghi de culoare c
{
    setfillstyle(SOLID_FILL, c);
    bar(x1, y1, x2, y2);
}

void DrawBorderedBar(int x1, int y1, int x2, int y2, int cfill, int cbord) /// Functie care deseneaza un dreptunghi de culoare cfill
/// si bordura culoare cbord
{
    setfillstyle(SOLID_FILL, cfill);
    bar(x1, y1, x2, y2);
    DrawSimpleBar(x1, y1, x2, y1+5, cbord);
    DrawSimpleBar(x2, y1, x2-5, y2, cbord);
    DrawSimpleBar(x2, y2, x1, y2-5, cbord);
    DrawSimpleBar(x1, y2, x1+5, y1, cbord);
}

void DrawCircle(int x, int y, int r, int cfill, int cbord, int ctext, int bc, char t[]) /// Functie care deseneaza un cerc de raza r,
/// bordura de culoare cbord si culoarea cercului cfill + text de culoare ctext si culoare background bc
{
    setcolor(cbord);
    setlinestyle(0, 0xFFF, 4);
    setfillstyle(SOLID_FILL, cfill);
    fillellipse(x, y, r, r);
    setbkcolor(bc);
    setcolor(ctext);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 5);
    outtextxy(x, y+10, t);
}

void DrawSimpleCircle(int x, int y, int r, int cfill, int cbord, int ctext, int bc) /// Functie care deseneaza un cerc de raza r,
/// bordura de culoare cbord si culoarea cercului cfill
{
    setcolor(cbord);
    setlinestyle(0, 0xFFF, 4);
    setfillstyle(SOLID_FILL, cfill);
    fillellipse(x, y, r, r);
}

void Culoare() /// Functie pentru colorarea arborelui
{
    /**
          Culorile: BLACK=0, BLUE=1, GREEN=2, CYAN=3, RED=4, MAGENTA=5, BROWN=6, LIGHTGRAY=7,
          DARKGRAY=8, LIGHTBLUE=9, LIGHTGREEN=10, LIGHTCYAN=11, LIGHTRED=12, LIGHTMAGENTA=13,
          YELLOW=14, WHITE=15
    */
    int c = rand()%16 + 0;
    if(c != 15) c_bordura = c;
    c = rand()%16 + 0;
    c_text = c;
    c = rand()%16 + 0;
    c_fill = c;
    while(c_fill == c_bordura || c_fill == c_text)
    {
        c = rand()%16 + 0;
        c_fill = c;
    }
}

void Culoare1() /// Functie pentru colorarea arborelui initiala
{
    c_bordura = c_text = 0;
    c_fill = 15;
}

void DrawEllipse(int x, int y, int r1, int r2, char t[]) /// Functie care deseneaza o elipsa de raze r1 si r2,
/// + text
{
    setcolor(c_bordura);
    setlinestyle(0, 0xFFF, 3);
    setfillstyle(SOLID_FILL, c_fill);
    if(t[0] > '0' && t[0] <= '9')
    {
        if(strlen(t) == 1) r1 += 10;
        if(strlen(t) == 2) r1 -= 10;
        if(strlen(t) == 3) r1 -= 17;
        if(strlen(t) > 3) r1 -= 20;
    }
    if(t[0] == '-' && t[1] != 0)
    {
        if(strlen(t) == 2) r1 -= 15;
        if(strlen(t) == 3) r1 -= 25;
        if(strlen(t) > 3) r1 -= 30;
    }
    if(t[0] >= 'a' && t[0] <= 'z')
    {
        if(strlen(t) == 3) r1 -= 13;
        if(strlen(t) == 4) r1 -= 18;
    }
    if(t[0] == '*' || t[0] == '^' || t[0] == '/') r1 += 10;
    if(t[0] == '-' && t[1] == 0) r1 += 5;
    if(t[0] == '<' || t[0] == '>') r1 -= 15;
    fillellipse(x, y, r1, r2);
    setbkcolor(c_fill);
    setcolor(c_text);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    if(t[0] >= '0' && t[0] <= '9')
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    else if(t[0] >= 'a' && t[0] <= 'z')
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
    else settextstyle(GOTHIC_FONT, HORIZ_DIR, 4);
    if(t[0] == '%') settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    if(t[0] == '<' || t[0] == '>') settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
    if(t[0] == '*' || t[0] == '^') y += 5;
    if(t[0] == '-' && t[1] != 0)
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    outtextxy(x, y+8, t);
}

void Chenar(int x1, int y1, int x2, int y2, int c, int g) /// Functie care desenaza un chenar de culoare si grosime g
{
    DrawSimpleBar(x1, y1, x2, y1+g, c);
    DrawSimpleBar(x1, y1, x1+g, y2, c);
    DrawSimpleBar(x2, y1, x2-g, y2, c);
    DrawSimpleBar(x2, y2, x1, y2-g, c);
}

/// x1,y1,x2,y2 - coordonate pentru desenarea dreptunghiului
/// cfill = culoarea de umplere pentru dreptunghi
/// cbord = culoarea bordurii dreptunghiului
/// c = culoarea textului
/// t[] - pentru text
/// bc = culoare background text
void Button(int x1, int y1, int x2, int y2, int cfill, int cbord, int ctext, int bc, char t[])
{
    DrawBorderedBar(x1, y1, x2, y2, cfill, cbord);
    setbkcolor(bc);
    setcolor(ctext);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    outtextxy(int((x1 + x2) / 2), int((y1 + y2) / 2)+6, t);
}

void DrawNodeColorat(nod *a, int x, int y, int r, int h, int l, int dir) /// x,y-coordonate, h-inaltimea, l-lungimea, dir-directia
{
    char exp[11];
    int r1;
    strcpy(exp, a->info);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    if(exp[0] > '0' && exp[0] <= '9')
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    else if(exp[0] > 'a' && exp[0] <= 'z')
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    else settextstyle(GOTHIC_FONT, HORIZ_DIR, 4);
    r1 = textwidth(exp); /// xradius
    DrawEllipse(x, y, r1, r, exp);
    if(y > H-16-r) LG = 1;
    setlinestyle(0, 0, 3);
    setcolor(c_bordura);
    if(dir)
    {
        if(dir == 1)
            line(x, y-r, x+2*l+r, y-h);
        else
            line(x, y-r, x-2*l-r, y-h);
    }
}

void DrawTreeColorat(nod *a, int x, int y, int r, int h, int l, int dir)
{
    if(a->st != 0)
        DrawTreeColorat(a->st, x-r-l, y+r+h, r, h, l/2, 1);
    DrawNodeColorat(a, x, y, r, h, l, dir);
    if(a->dr != 0)
        DrawTreeColorat(a->dr, x+r+l, y+r+h, r, h, l/2, 2);

}

void Node(nod *a, int x, int y, int r, int h, int l, int dir)
{
    if(y > H-22-r) LG = 1;
}

void Tree(nod *a, int x, int y, int r, int h, int l, int dir)
{
    if(a->st != 0)
        Tree(a->st, x-r-l, y+r+h, r, h, l/2, 1);
    Node(a, x, y, r, h, l, dir);
    if(a->dr != 0)
        Tree(a->dr, x+r+l, y+r+h, r, h, l/2, 2);
}

void Design() /// Functie pentru a crea design-ul primei pagini
{
    setbkcolor(3);
    setcolor(0);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    outtextxy(W/2, 80, "ARBORI EXPRESII ALGEBRICE");
    lgt = textwidth("ARBORI EXPRESII ALGEBRICE");
    ht = textheight("ARBORI EXPRESII ALGEBRICE");
    Chenar(W/2-lgt/2-14, 80-ht/2-22, W/2+lgt/2+13, 80+ht/2+7, 0, 3);
    Chenar(W/2-lgt/2-6, 80-ht/2-15, W/2+lgt/2+4, 80+ht/2, 14, 2);
    setlinestyle(0, 0, 3);
    setcolor(0);
    circle(W/2+480, 485, 50);
    setcolor(14);
    circle(W/2+480, 485, 40);
    circle(W/2-630, 650, 40);
    circle(W/2+630, 650, 40);
    setcolor(0);
    circle(W/2-630, 650, 50);
    DrawLine(W/2-523, 510, W/2-600, 610, 0);
    circle(W/2-480, 485, 50);
    setcolor(14);
    circle(W/2-480, 485, 40);
    DrawLine(W/2+523, 510, W/2+600, 610, 0);
    circle(W/2+630, 650, 50);
    DrawLine(W/2-600, 355, W/2-523, 455, 0);
    DrawLine(W/2+600, 355, W/2+523, 455, 0);
    setcolor(14);
    circle(W/2-630, 315, 40);
    circle(W/2+630, 315, 40);
    setcolor(0);
    circle(W/2-630, 315, 50);
    circle(W/2+630, 315, 50);
    DrawLine(W/2-523, 175, W/2-600, 275, 0);
    DrawLine(W/2+600, 275, W/2+523, 175, 0);
    circle(W/2+480, 150, 50);
    setcolor(14);
    circle(W/2+480, 150, 40);
    setcolor(0);
    circle(W/2-480, 150, 50);
    setcolor(14);
    circle(W/2-480, 150, 40);
    DrawLine(W/2-600, 690, W/2-483, 760, 0);
    DrawLine(W/2+600, 690, W/2+483, 760, 0);
    circle(W/2+435, 770, 50);
    setcolor(14);
    circle(W/2+435, 770, 40);
    setcolor(0);
    circle(W/2-435, 770, 50);
    setcolor(14);
    circle(W/2-435, 770, 40);
}

void Menu()
{
    setlinestyle(0, 0xFFFF, 4);
    setcolor(0);
    int h = 65;
    DrawCircle(W/2, 150+h, 50, 14, 0, 0, 14, "-_-");
    line(W/2, 200+h, W/2, 241+h); /// lungime = 41
    Button(W/2-120, 241+h, W/2+120, 301+h, 14, 0, 0, 14, "Start"); /// inaltime = 60
    line(W/2, 301+h, W/2, 342+h);
    Button(W/2-120, 342+h, W/2+120, 402+h, 14, 0, 0, 14, "Instructiuni");
    line(W/2, 402+h, W/2, 443+h);
    Button(W/2-120, 443+h, W/2+120, 503+h, 14, 0, 0, 14, "Exit");
    line(W/2-32, 500+h, W/2-72, 564+h); /// lungime = 61
    line(W/2+32, 500+h, W/2+72, 564+h);
    DrawSimpleCircle(W/2-100, 604+h, 50, 14, 0, 0, 14);
    DrawSimpleCircle(W/2+100, 604+h, 50, 14, 0, 0, 14);
}

void Window1() /// Pagina de start
{
    DrawSimpleBar(0, 0, W, H, 3);
    Chenar(0, 0, W, H, 0, 6);
    Chenar(5, 5, W-5, H-5, 9, 6);
    Chenar(10, 10, W-10, H-10, 14, 7);
    Chenar(16, 16, W-16, H-16, 0, 5);
    Design();
    Menu();
}

void Window2() /// Pagina in care este desenat arborele
{
    DrawSimpleBar(0, 0, 480, H, 3);
    DrawSimpleBar(500, 0, W, H, 15);
    Chenar(0, 0, W, H, 0, 6);
    Chenar(5, 5, W-5, H-5, 9, 6);
    Chenar(10, 10, W-10, H-10, 14, 7);
    Chenar(16, 16, W-16, H-16, 0, 5);
    DrawSimpleBar(480, 16, 485, H - 16, 0);
    DrawSimpleBar(485, 10, 490, H - 10, 14);
    DrawSimpleBar(490, 10, 495, H - 5, 9);
    DrawSimpleBar(495, 10, 500, H - 10, 14);
    DrawSimpleBar(500, 16, 505, H - 16, 0);
    setbkcolor(3);
    setcolor(0);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    outtextxy(245, 72, "Introduceti expresia");
    lgt = textwidth("Introduceti expresia");
    ht = textheight("Introduceti expresia");
    setcolor(14);
    setlinestyle(0, 0, 2);
    line(245-lgt/2, 72+ht/2-4, 245+lgt/2, 72+ht/2-4);
    setlinestyle(0, 0, 3);
    setcolor(0);
    line(245-lgt/2, 72+ht/2, 245+lgt/2, 72+ht/2);
    DrawBorderedBar(35, 110, 466, 190, 14, 0);
    Button(85, 232, 405, 282, 14, 0, 0, 14, "Sterge");
    Button(85, 322, 405, 372, 14, 0, 0, 14, "Coloreaza arbore");
    setbkcolor(3);
    setcolor(0);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    outtextxy(245, 430, "Rezultat expresie");
    lgt = textwidth("Rezultat expresie");
    ht = textheight("Rezultat expresie");
    setcolor(14);
    setlinestyle(0, 0, 2);
    line(245-lgt/2, 430+ht/2-4, 245+lgt/2, 430+ht/2-4);
    setlinestyle(0, 0, 3);
    setcolor(0);
    line(245-lgt/2, 430+ht/2, 245+lgt/2, 430+ht/2);
    DrawBorderedBar(85, 452, 405, 512, 14, 0);
    setcolor(0);
    settextjustify(LEFT_TEXT, CENTER_TEXT);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    setbkcolor(3);
    outtextxy(50, 577, "Numar noduri: ");
    Chenar(textwidth("Numar noduri: ")+50, 545, 400, 594, 0, 2);
    Button(245-60, H-152, 245+60, H-102, 14, 0, 0, 14, "Back");
    setlinestyle(0, 0, 2);
    circle(245, 680, 40);
    setcolor(14);
    circle(245, 680, 30);
    setcolor(0);
    line(245-38, 690, 245-112, 730);
    line(245+38, 690, 245+112, 730);
    circle(245-123, 770, 40);
    circle(245+123, 770, 40);
    setcolor(14);
    circle(245-123, 770, 30);
    circle(245+123, 770, 30);
    setcolor(0);
}

void Window3() /// Pagina de instructiuni
{
    int h = 95;
    DrawSimpleBar(0, 0, W, H, 3);
    Chenar(0, 0, W, H, 0, 6);
    Chenar(5, 5, W-5, H-5, 9, 6);
    Chenar(10, 10, W-10, H-10, 14, 7);
    Chenar(16, 16, W-16, H-16, 0, 5);
    setbkcolor(3);
    setcolor(0);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    outtextxy(W/2, 62, "Expresii algebrice");
    setlinestyle(0, 0, 3);
    line(W/2-158, 78, W/2+158, 78);
    setcolor(14);
    setlinestyle(0,0,2);
    line(W/2-158, 74, W/2+158, 74);
    setbkcolor(14);
    setcolor(0);
    settextjustify(LEFT_TEXT, CENTER_TEXT);
    DrawBorderedBar(W/2-390, 110, W/2-15, 160, 14, 0); /// 160
    outtextxy(W/2-380, (85+135+40)/2+11, "1. Operatii disponibile");
    DrawBorderedBar(W/2+15, 110, W/2+380, 160, 14 , 0);
    outtextxy(W/2+25, (85+135+40)/2+11, "2. Reguli de scriere");
    Button(W/2-60, H-102, W/2+60, H-52, 14, 0, 0, 14, "Back");
    setbkcolor(3);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(W/2, 210, "Vizualizarea arborelui");
    setlinestyle(0, 0, 3);
    line(W/2-186, 226, W/2+186, 226);
    setcolor(14);
    setlinestyle(0, 0, 2);
    line(W/2-186, 222, W/2+186, 222);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
    outtextxy(W/2, 275, "- pentru a introduce expresia apasati CLICK pe chenarul galben din coltul stanga-sus");
    outtextxy(W/2, 315, "- pentru a vizualiza arborele apasati ENTER dupa ce ati introdus expresia");
    outtextxy(W/2, 355, "- pentru a introduce o noua expresie si pentru a crea un nou arbore apasati butonul 'Sterge'");
    outtextxy(W/2, 395, "- pentru a colora arborele apasati pe butonul 'Coloreaza arbore'");
    outtextxy(W/2, 435, "- in cazul in care ati gresit expresia puteti folosi butonul BACKSPACE de la tastatura pentru a sterge");
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
    setlinestyle(0, 0, 3);
    setcolor(0);
    circle(W/2, 500+h, 50);
    setcolor(14);
    circle(W/2, 500+h, 40);
    setcolor(0);
    line(W/2-43, 525+h, W/2-200, 625+h);
    line(W/2+43, 525+h, W/2+200, 625+h);
    circle(W/2-230, 665+h, 50);
    circle(W/2+230, 665+h, 50);
    setcolor(14);
    circle(W/2-230, 665+h, 40);
    circle(W/2+230, 665+h, 40);
    setcolor(0);
    line(W/2-260, 625+h, W/2-437, 525+h);
    line(W/2+260, 625+h, W/2+437, 525+h);
    circle(W/2-480, 500+h, 50);
    setcolor(14);
    circle(W/2-480, 500+h, 40);
    setcolor(0);
    circle(W/2+480, 500+h, 50);
    setcolor(14);
    circle(W/2+480, 500+h, 40);
    setcolor(0);
    circle(W/2-630, 665+h, 50);
    circle(W/2+630, 665+h, 50);
    setcolor(14);
    circle(W/2-630, 665+h, 40);
    circle(W/2+630, 665+h, 40);
    setcolor(0);
    DrawLine(W/2-523, 525+h, W/2-600, 625+h, 0);
    DrawLine(W/2+523, 525+h, W/2+600, 625+h, 0);

}

void Window4() /// Pagina cu operatiile disponbile
{
    int lgt;
    DrawSimpleBar(0, 0, W, H, 3);
    Chenar(0, 0, W, H, 0, 6);
    Chenar(5, 5, W-5, H-5, 9, 6);
    Chenar(10, 10, W-10, H-10, 14, 7);
    Chenar(16, 16, W-16, H-16, 0, 5);
    setcolor(0);
    setbkcolor(3);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(W/2, 80, "Operatii disponibile");
    lgt = textwidth("Operatii disponibile");
    ht = textheight("Operatii disponibile");
    DrawSimpleBar(W/2-lgt/2, 80+ht/2+1, W/2+lgt/2, 80+ht/2+5, 0);
    DrawSimpleBar(W/2-lgt/2, 80+ht/2-3, W/2+lgt/2, 80+ht/2, 14);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 4);
    settextjustify(LEFT_TEXT, CENTER_TEXT);
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(180, 190, "+"); lgt=textwidth("+"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(180+lgt+15, 190, "(adunare)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(180, 230, "-"); lgt=textwidth("-"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(180+lgt+15, 230, "(scadere)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(180, 270, "/"); lgt=textwidth("/"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(180+lgt+15, 270, "(impartire)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(180, 310, "*"); lgt=textwidth("*"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(180+lgt+15, 310, "(inmultire)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(180, 350, "%"); lgt=textwidth("%"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(180+lgt+15, 350, "(modulo - restul impartirii)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(180, 390, "^"); lgt=textwidth("^"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(180+lgt+15, 390, "(ridicare la putere)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(180, 430, "<<"); lgt=textwidth("<<"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(180+lgt+15, 430, "(shiftare pe biti la stanga)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(180, 470, ">>"); lgt=textwidth(">>"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(180+lgt+15, 470, "(shiftare pe biti la dreapta)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(180, 510, "log"); lgt=textwidth(">>"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(180+lgt+15, 510, "(logaritm in baza 2)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(180, 550, "lg"); lgt=textwidth(">>"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(180+lgt+15, 550, "(logaritm in baza 10)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(180, 590, "ln"); lgt=textwidth("ln"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(180+lgt+15, 590, "(logaritm natural)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(180, 630, "max"); lgt=textwidth("max"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(180+lgt+15, 630, "(maximul dintre 2 numere)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(180, 670, "min"); lgt=textwidth("min"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(180+lgt+15, 670, "(minimul dintre 2 numere)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(180, 710, "sqrt"); lgt=textwidth("sqrt"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(180+lgt+15, 710, "(radical)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(180, 750, "sin"); lgt=textwidth("sin"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(180+lgt+15, 750, "(sinus)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(1000, 190, "cos"); lgt=textwidth("cos"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(1000+lgt+15, 190, "(cosinus)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(1000, 230, "tan"); lgt=textwidth("tan"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(1000+lgt+15, 230, "(tangenta)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(1000, 270, "ctan"); lgt=textwidth("ctan"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(1000+lgt+15, 270, "(cotangenta)");
    setcolor(14); settextstyle(GOTHIC_FONT, HORIZ_DIR, 3); outtextxy(1000, 310, "abs"); lgt=textwidth("abs"); setcolor(0); settextstyle(COMPLEX_FONT, HORIZ_DIR, 4); outtextxy(1000+lgt+15, 310, "(valoarea absoluta)");

    Button(1300-60, H-232, 1300+60, H-182, 14, 0, 0, 14, "Back");
    setlinestyle(0, 0, 2);
    circle(1300, 600, 40);
    setcolor(14);
    circle(1300, 600, 30);
    setcolor(0);
    line(1300-38, 610, 1300-112, 650);
    line(1300+38, 610, 1300+112, 650);
    circle(1300-123, 690, 40);
    circle(1300+123, 690, 40);
    setcolor(14);
    circle(1300-123, 690, 30);
    circle(1300+123, 690, 30);
    setcolor(0);
}

void Window5() /// Pagina cu regulile de scriere
{
    DrawSimpleBar(0, 0, W, H, 3);
    Chenar(0, 0, W, H, 0, 6);
    Chenar(5, 5, W-5, H-5, 9, 6);
    Chenar(10, 10, W-10, H-10, 14, 7);
    Chenar(16, 16, W-16, H-16, 0, 5);
    setcolor(0);
    setbkcolor(3);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 4);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(W/2, 80, "Reguli de scriere");
    lgt = textwidth("Reguli de scriere");
    ht = textheight("Reguli de scriere");
    DrawSimpleBar(W/2-lgt/2, 80+ht/2+1, W/2+lgt/2, 80+ht/2+5, 0);
    DrawSimpleBar(W/2-lgt/2, 80+ht/2-3, W/2+lgt/2, 80+ht/2, 14);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
    setcolor(14);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(W/2, 180, "- expresia se scrie fara spatii intre termeni si operatii");
    outtextxy(W/2, 220, "- pentru operatiile log, lg, ln, sin, cos, tan, ctan, abs, sqrt termenii se pun intre paranteze");
    setcolor(4);
    outtextxy(W/2, 260, "ex1: sin(180), ln(2), abs(-4)");
    outtextxy(W/2, 300, "ex2: cos(40+200), ln(0+1), sqrt(2+2)");
    setcolor(14);
    outtextxy(W/2, 340, "- pentru operatiile max si min termenii se pun intre paranteze si se separa prin virgula");
    setcolor(4);
    outtextxy(W/2, 380, "ex: max(2,6), min(-1,26)");
    setcolor(14);
    outtextxy(W/2, 420, "- pentru numerele zecimale se foloseste virgula");
    setcolor(4);
    outtextxy(W/2, 460, "ex: 15,87");

    int h = 100;
    setlinestyle(0, 0, 3);
    setcolor(0);
    circle(W/2, 485+h, 50);
    setcolor(14);
    circle(W/2, 485+h, 40);
    setcolor(0);
    line(W/2-43, 510+h, W/2-200, 610+h);
    line(W/2+43, 510+h, W/2+200, 610+h);
    circle(W/2-230, 650+h, 50);
    circle(W/2+230, 650+h, 50);
    setcolor(14);
    circle(W/2-230, 650+h, 40);
    circle(W/2+230, 650+h, 40);
    setcolor(0);
    line(W/2-260, 610+h, W/2-437, 510+h);
    line(W/2+260, 610+h, W/2+437, 510+h);
    circle(W/2-480, 485+h, 50);
    setcolor(14);
    circle(W/2-480, 485+h, 40);
    setcolor(0);
    circle(W/2+480, 485+h, 50);
    setcolor(14);
    circle(W/2+480, 485+h, 40);
    setcolor(0);
    circle(W/2-630, 650+h, 50);
    circle(W/2+630, 650+h, 50);
    setcolor(14);
    circle(W/2-630, 650+h, 40);
    circle(W/2+630, 650+h, 40);
    setcolor(0);
    DrawLine(W/2-523, 510+h, W/2-600, 610+h, 0);
    DrawLine(W/2+523, 510+h, W/2+600, 610+h, 0);
    Button(W/2-60, H-102, W/2+60, H-52, 14, 0, 0, 14, "Back");
}

void Write(char t[3], char a, int &lungime) /// Functie pentru a scrie expresia in chenar
{
        t[0] = a;
        t[1] = 0;
        settextstyle(COMPLEX_FONT, HORIZ_DIR, 3);
        setbkcolor(14);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        outtextxy(58+lungime, 155 , t);
        lungime += textwidth(t)+1;
}

void ShowWarning1() /// in cazul in care expresia este gresita
{
        setcolor(4);
        setbkcolor(14);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 4);
        outtextxy(1050, H/2, "Expresia introdusa este gresita!");
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
        setcolor(0);
        outtextxy(1050, H/2+40, "Consultati operatiile disponibile si regulile de scriere din meniul de instructiuni.");
}

void ShowWarning2() /// in cazul in care nu a fost introdus nimic in chenar
{
    setcolor(4);
    setbkcolor(14);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 4);
    outtextxy(1050, H/2, "Nu ati introdus nimic in chenar!");
}

void ShowWarning3()
{
    setcolor(4);
    setbkcolor(14);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    outtextxy(1050, H/2, "Expresia este prea lunga. Arborele nu incape in pagina.");
}

int Asezare(int l) /// Functie pentru asezarea in pagina a arborelui
{
    int x, dif;
    int k = 25;
    x = 1050;
    if(nrs == nrd) x = 1050;
    else if(nrs > nrd)
    {
        dif = nrs-nrd;
        while(dif)
        {
            x += l/2-k;
            k += 25;
            dif--;
        }
    }
    else if(nrd > nrs)
    {
        dif = nrd-nrs;
        while(dif)
        {
            x -= k;
            k += 12;
            dif--;
        }
    }
    return x;
}

int main()
{
    int x, mx, my, h, i, enter, lungime, cnt, col;
    char a, t[3];
    a = 0;
    x = 1;
    i = 0;
    h = 65;
    col = 0;
    initwindow(W, H); /// fereastra grafica
    Window_1:
        Window1();
    while(x != 0)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            mx = mousex();
            my = mousey();
            if(mx >= W/2-120 && mx <= W/2+120 && my >= 241+h && my <= 301+h) /// Start
            {
                Window_2:
                    Window2();
                while(x != 0)
                {
                            if(ismouseclick(WM_LBUTTONDOWN))
                            {
                                clearmouseclick(WM_LBUTTONDOWN);
                                mx = mousex();
                                my = mousey();
                                if(mx >= 245-60 && mx <= 245+60 && my >= H-152 && my <= H-102) ///Back
                                {
                                    goto Window_1;
                                }
                                else if(mx >= 35 && mx <= 466 && my >= 110 && my <= 190) /// Chenar expresie
                                {
                                    Chenar(39, 114, 462, 186, 15, 5);
                                    enter = 0;
                                    lungime = 0;
                                    i = 0;
                                    cnt = 1; /// ne ajuta sa vedem daca s-a apasat enter fara sa fie scris nimic in chenar
                                    char nl[1];
                                    strcpy(alg, nl);
                                    while(enter == 0)
                                    {
                                        a = getch();
                                        if(int(a)== 13 && cnt == 1)
                                        {
                                            cnt = 0;
                                            ShowWarning2();
                                            delay(3000);
                                            goto Window_2;
                                        }
                                        if(int(a) != 13 && int(a) != 8 && lungime <= 390) /// Daca e apasata o tasta diferita de enter si backspace
                                        {
                                            alg[i] = a;
                                            alg[i+1] = 0;
                                            i++;
                                            Write(t, a, lungime);
                                            cnt = 0;
                                        }
                                        else if(int(a) == 13)/// daca e apasat enter
                                        {
                                            int j = 0;
                                            /// caz particualar al verificarii
                                            while(j < strlen(alg))
                                                if((alg[j] >= 'a' && alg[j] <= 'z') || (alg[j] >= 'A' && alg[j] <= 'Z') || ((strchr("+-/*^%<>", alg[j]) == 0) && strchr("0123456789", alg[j]) == 0))
                                                    j++;
                                                else j = strlen(alg)+1;
                                            if(j == strlen(alg))
                                            {
                                                ShowWarning1();
                                                delay(5000);
                                                goto Window_2;
                                            }
                                            ///
                                            enter = 1;
                                            nr = 0;
                                            lg = 0;
                                            nrs = 0;
                                            nrd = 0;
                                            st = 0;
                                            rez = 0;
                                            nod* arb = 0; /// arborele
                                            nod* p = 0;   ///p ne va ajuta sa retinem ultimul nod-radacina format in a
                                            n = strlen(alg);
                                            rezolvare(0, n-1, arb, p);
                                            calcul(arb);
                                            lg--;
                                            nr = lg+1;
                                            nrs = Snrnoduri(arb->st);
                                            nrd = Dnrnoduri(arb->dr);
                                            st = nrnodurist(arb->st);
                                            if(verificare() == 0) /// expresie gresita
                                            {
                                                ShowWarning1();
                                                delay(5000);
                                                goto Window_2;
                                            }
                                            else if(verificare() == 1) /// expresie corecta
                                            {
                                                col = 1;
                                                Culoare1();
                                                LG = 0;
                                                Tree(arb, Asezare(180), 60, 30, 100, 200, 0);
                                                if(LG == 1)
                                                {
                                                    ShowWarning3();
                                                    delay(3500);
                                                    goto Window_2;
                                                }
                                                else DrawTreeColorat(arb, Asezare(180), 60, 30, 100, 200, 0);
                                                settextjustify(CENTER_TEXT, CENTER_TEXT);
                                                setcolor(14);
                                                setbkcolor(3);
                                                settextstyle(GOTHIC_FONT, HORIZ_DIR, 4);
                                                IntToChar(nr, noduri);
                                                outtextxy(345, 579, noduri); /// Numar de noduri
                                                IntToChar(rez, rezultat);
                                                settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
                                                settextjustify(CENTER_TEXT, CENTER_TEXT);
                                                setbkcolor(14);
                                                setcolor(0);
                                                outtextxy(245, 490, rezultat); /// Rezultat expresie
                                            }
                                        }
                                        else if(int(a) == 8) /// daca e apasat backspace
                                        {
                                            if(i > 0)
                                            {
                                                enter = 0;
                                                i--;
                                                alg[i] = 0;
                                                lungime = 0;
                                                DrawSimpleBar(45, 120, 456, 180, 14);
                                                for(int j = 0; j < i; j++)
                                                    Write(t, alg[j], lungime);
                                            }
                                        }
                                    }
                                }
                                else if(mx >= 85 && mx <= 405 && my >= 232 && my <= 282 && col == 1) /// Sterge
                                {
                                    goto Window_2;
                                }
                                else if(mx >= 85 && mx <= 405 && my >= 322 && my <= 372 && col == 1) /// Coloreaza arbore
                                {
                                    nod* arb = 0; /// arborele
                                    nod* p = 0;   ///p ne va ajuta sa retinem ultimul nod-radacina format in a
                                    n = strlen(alg);
                                    rezolvare(0, n-1, arb, p);
                                    nr = 0;
                                    lg = 0;
                                    nrs = 0;
                                    nrd = 0;
                                    st = 0;
                                    rez = 0;
                                    lg--;
                                    nr = lg + 1;
                                    nrs = Snrnoduri(arb->st);
                                    nrd = Dnrnoduri(arb->dr);
                                    st = nrnodurist(arb->st);
                                    Culoare();
                                    DrawTreeColorat(arb, Asezare(180), 60, 30, 100, 200, 0);
                                }
                            }

                }
            }
            else if(mx >= W/2-120 && mx <= W/2+120 && my >= 342+h && my <= 402+h) /// Instructiuni
            {
                Window_3:
                    Window3();
                    while(x != 0)
                    {
                        if(ismouseclick(WM_LBUTTONDOWN))
                        {
                            clearmouseclick(WM_LBUTTONDOWN);
                            mx = mousex();
                            my = mousey();
                            if(mx >= W/2-60 && mx <= W/2+60 && my >= H-102 && my <= H-52) /// Back Instructiuni
                            {
                                goto Window_1;
                            }
                            else if(mx >= W/2-390 && mx <= W/2-15 && my >= 105 && my <= 155) /// Operatii disponibile
                            {
                                Window4();
                                while(x != 0)
                                {
                                    if(ismouseclick(WM_LBUTTONDOWN))
                                    {
                                        clearmouseclick(WM_LBUTTONDOWN);
                                        mx = mousex();
                                        my = mousey();
                                        if(mx >= 1300-60 && mx <= 1300+60 && my >= H-232 && my <= H-182) /// Back Operatii disponibile
                                        {
                                            goto Window_3;
                                        }
                                    }
                                }
                            }
                            else if(mx >= W/2+15 && mx <= W/2+380 && my >= 105 && my <= 155) /// Reguli de scriere
                            {
                                Window5();
                                while(x != 0)
                                {
                                    if(ismouseclick(WM_LBUTTONDOWN))
                                    {
                                        clearmouseclick(WM_LBUTTONDOWN);
                                        mx = mousex();
                                        my = mousey();
                                        if(mx >= W/2-60 && mx <= W/2+60 && my >= H-102 && my <= H-52) /// Back Reguli de scriere
                                            goto Window_3;
                                    }
                                }
                            }
                        }
                    }
            }
            else if(mx >= W/2-120 && mx <= W/2+120 && my >= 443+h && my <= 503+h) /// Exit
                return 0;
    }
}
    getch();
    closegraph();
    return 0;
}
