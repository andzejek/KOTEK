#include "huffEnc.h"
void sortowanie_babelkowe2(Code *tab,int n);
void sortowanie_babelkowe(Code *tab,int n);
float mean(Byte *data,int datasize){
    int suma=0;
    for(int i=0;i<datasize;i++)
    {
        if(data[i]>128) suma+=(256-data[i]);
        else suma+=data[i];
    }
    return (int)((float)10*suma/datasize);//(int)((float)10*suma/datasize);
}
static int cos2=0;
int Enc(BITBUFOR *BB,Byte *data,int size,Code *code){

    int suma=BB->bitIndex;
    int pos=BB->pos;
    int *iout;
    Byte cod;
    for(int i=0;i<size;i++)
    {
        for(int l=0;l<code[data[i]].dlugosc;l++){
                BB->addBit((Byte)(code[data[i]].kod[l]-0x30));
        }
    }
    return BB->pos+1;
}
int Dec(Byte *data,BITBUFOR *BB,int size,Code *code){
    sortowanie_babelkowe2(code,256);
    sortowanie_babelkowe(code,256);
    short dlugosc2[32];
    zeroMemory(dlugosc2,32*sizeof(short));
    dlugosc2[code[0].dlugosc]=0;

    int dlugoscmin=code[0].dlugosc;
    if(dlugoscmin==0) dlugoscmin=1;
    for(int i=1,j=2;i<256;i++)
    {
        if(code[i].dlugosc>code[i-1].dlugosc) {dlugosc2[code[i].dlugosc]=i; j++;}
    }
    for(int i=0;i<32;i++)
    {
        if(dlugosc2[i]<dlugosc2[dlugoscmin]) dlugosc2[i]=dlugosc2[dlugoscmin];
    }
    short lookup[9][256];
    for(int i=0;i<9;i++)
    for(int j=0;j<256;j++)
        lookup[i][j]=-1;
    if(dlugosc2[9]<=dlugosc2[8]) dlugosc2[9]=256;

    for(int i=0;i<dlugosc2[9];i++)
    {
        lookup[code[i].dlugosc][code[i].kot]=code[i].wartosc;
    }
    int temp=0;
    for(int poz=0,dlugosc=0;poz<size;)
    {
        temp*=2;
        Byte bit=BB->getBit();
        temp+=bit;
        dlugosc+=1;
        if(dlugosc>=dlugoscmin)
        if(dlugosc<9)
        {
            if(lookup[dlugosc][temp]!=-1) {data[poz]=lookup[dlugosc][temp];
            dlugosc=0;
            temp=0;
            poz++;}
        }
        else
        for(int i=dlugosc2[dlugosc];i<256;i++)
        {
            if(code[i].dlugosc==dlugosc)
            {
                if(code[i].kot==temp)
                {
                    data[poz]=code[i].wartosc;
                    poz++;
                    temp=0;
                    dlugosc=0;
                    break;
                }
            }
        }

    }
}
void killTree(Node *node){
    if(node)
    {
        killTree(node->left);
        killTree(node->right);
        delete node;
    }
}
void getFreq(Byte *data,int size,int *freq){
    zeroMemory(freq,256*sizeof(int));
    for(int i=0;i<size;i++) freq[data[i]]++;
}
int getSize(int *freq,Code *code){
    int suma=0;
    for(int i=0;i<256;i++)
        suma+=freq[code[i].wartosc]*code[i].dlugosc;
    return suma;
}
int getSize(int *freq){
    Node *tree=makeTree(freq);
    Code codes[256];
    mapCodes(codes,tree);
    killTree(tree);
    return getSize(freq,codes);
}
int getSize(Byte *data,int size){
    int freq[256];
    getFreq(data,size,freq);
    Node *tree=makeTree(freq);
    Code codes[256];
    mapCodes(codes,tree);
    killTree(tree);
    return getSize(freq,codes);
}
void showFreq(int *freq){
    for(int i=0;i<256;i++)
        if(freq[i]!=0) printf("%3d=%11d|",i,freq[i]);
    printf("\n");
}
void showCodes(Code *codes){
    for(int i=0;i<256;i++)
    {
         printf("%3d  %10d %3d=%s\n",codes[i].wartosc,codes[i].kot,codes[i].dlugosc,codes[i].kod);
    }
}
void saveTree(BITBUFOR* BB,Node *tree){
    if(tree->left==0&&tree->right==0)
    {
    BB->addBit(0);
    BB->addByte(tree->byte);
    }
    else
    {
        BB->addBit(1);
        if(tree->left!=0)
        saveTree(BB,tree->left);
        if(tree->right!=0)
        saveTree(BB,tree->right);
    }
}
void loadTree2(Node *tree,BITBUFOR* BB){
    Byte x=BB->getBit();
    if(x==0)
    {
        tree->byte=BB->getByte();
    }
    else if(x==1)
    {
        tree->left=new Node(0,0,0,0);
        tree->right=new Node(0,0,0,0);
        loadTree2(tree->left,BB);
        loadTree2(tree->right,BB);
    }
}
void loadTree(Node *tree,BITBUFOR* BB){
    tree=new Node;
    Byte x=BB->getBit();
    if(x==0)
    {
        tree->byte=BB->getByte();
        return;
    }
    else if(x==1)
    {
        tree->left=new Node(0,0,0,0);
        tree->right=new Node(0,0,0,0);
        loadTree2(tree->left,BB);
        loadTree2(tree->right,BB);
    }
}
Node* makeTree(int *freq){
    //showFreq(freq);
    Node **nodes;
    int n=0;
    for(int i=0;i<256;i++)
        if(freq[i]!=0) n++;
    nodes=new Node*[n];
    for(int i=0,j=0;i<256;i++)
        if(freq[i]!=0)
        {
            nodes[j]=new Node;
            nodes[j]->freq=freq[i];
            nodes[j]->byte=i;
            nodes[j]->left=NULL;
            nodes[j]->right=NULL;
            j++;
        }
    int x=n;

    Node *tree;
    if(x==1)//tu zmiana
    {
        tree=nodes[0];
        //tree->right=nodes[0];
        //nodes[0]->left=0;
        //nodes[0]->right=0;
    }
    while(x>1)
    {
        int min1;
        for(int i=0;i<n;i++)
        {
            if(nodes[i]!=0) min1=i;
        }

        for(int i=0;i<n;i++)
        {
            if(nodes[i]!=0)
            if(nodes[i]->freq<nodes[min1]->freq) min1=i;
        }
        int min2;
        for(int i=0;i<n;i++)
        {
            if(nodes[i]!=0&&i!=min1) min2=i;
        }

        for(int i=0;i<n;i++)
        {
            if(nodes[i]!=0&&i!=min1)
            if(nodes[i]->freq<nodes[min2]->freq) min2=i;
        }
        Node *t=new Node;
        Node *min1t=new Node;
        Node *min2t=new Node;
        min1t->freq=nodes[min1]->freq;
        min1t->byte=nodes[min1]->byte;
        min1t->left=nodes[min1]->left;
        min1t->right=nodes[min1]->right;
        min2t->freq=nodes[min2]->freq;
        min2t->byte=nodes[min2]->byte;
        min2t->left=nodes[min2]->left;
        min2t->right=nodes[min2]->right;
        delete nodes[min1];
        delete nodes[min2];
        t->freq=min1t->freq+min2t->freq;
        t->byte=0;
        t->left=min1t;
        t->right=min2t;
        nodes[min1]=t;
        nodes[min2]=0;
        x--;
        tree=t;
    }
   // printf("huff tree:\n");
    //showTree(tree);
    //printf("\n");
    delete []nodes;
    return tree;
}
void sortowanie_babelkowe(Code *tab,int n){
  for(int i=0;i<n;i++)
    for(int j=1;j<n-i;j++)
    if(tab[j-1].dlugosc>tab[j].dlugosc)
      {
          Code T;
          T.dlugosc=tab[j-1].dlugosc;
          sprintf(T.kod,"%s",tab[j-1].kod);
          T.kot=tab[j-1].kot;
          T.wartosc=tab[j-1].wartosc;
          T.wystapienia=tab[j-1].wystapienia;

          tab[j-1].dlugosc=tab[j].dlugosc;
          sprintf(tab[j-1].kod,"%s",tab[j].kod);
          tab[j-1].kot=tab[j].kot;
          tab[j-1].wartosc=tab[j].wartosc;
          tab[j-1].wystapienia=tab[j].wystapienia;

          tab[j].dlugosc=T.dlugosc;
          sprintf(tab[j].kod,"%s",T.kod);
          tab[j].kot=T.kot;
          tab[j].wartosc=T.wartosc;
          tab[j].wystapienia=T.wystapienia;
      }
}
void sortowanie_babelkowe2(Code *tab,int n){
  for(int i=0;i<n;i++)
    for(int j=1;j<n-i;j++)
    if(tab[j-1].kot>tab[j].kot)
      {
          Code T;
          T.dlugosc=tab[j-1].dlugosc;
          sprintf(T.kod,"%s",tab[j-1].kod);
          T.kot=tab[j-1].kot;
          T.wartosc=tab[j-1].wartosc;
          T.wystapienia=tab[j-1].wystapienia;

          tab[j-1].dlugosc=tab[j].dlugosc;
          sprintf(tab[j-1].kod,"%s",tab[j].kod);
          tab[j-1].kot=tab[j].kot;
          tab[j-1].wartosc=tab[j].wartosc;
          tab[j-1].wystapienia=tab[j].wystapienia;

          tab[j].dlugosc=T.dlugosc;
          sprintf(tab[j].kod,"%s",T.kod);
          tab[j].kot=T.kot;
          tab[j].wartosc=T.wartosc;
          tab[j].wystapienia=T.wystapienia;
      }
}
void mapujKody(Node *w,Code *kodes,char *kode){

    if(w->left!=0)
    {
        cos2++;
        char kod[256];
    zeroMemory(kod,256);
    sprintf(kod,"%s",kode);
        sprintf(kod,"%s%c",kod,'0');

        mapujKody(w->left,kodes,kod);
    }
    if(w->right!=0)
    {
        cos2++;
        char kod[256];
        zeroMemory(kod,256);
        sprintf(kod,"%s",kode);
        sprintf(kod,"%s%c",kod,'1');
        mapujKody(w->right,kodes,kod);

    }
    else
    {
        char kod[256];
        zeroMemory(kod,256);
        sprintf(kod,"%s",kode);
        kodes[w->byte].wartosc=w->byte;
        kodes[w->byte].wystapienia=w->freq;
        sprintf(kodes[w->byte].kod,"%s",kod);
        int value=0;
        kodes[w->byte].dlugosc=strlen(kodes[w->byte].kod);
        for(int k=0;k<kodes[w->byte].dlugosc;k++) value=value*2+(kodes[w->byte].kod[k]-0x30);
        kodes[w->byte].kot=value;
    }
}
void mapCodes(Code *codes,Node *w){
    zeroMemory(codes,256*sizeof(Code));
    if((w->left==0)&&(w->right==0))
    {
        codes[w->byte].dlugosc=1;
        codes[w->byte].kot=0;
        sprintf(codes[w->byte].kod,"%s","0");
        codes[w->byte].wartosc=w->byte;
        codes[w->byte].wystapienia=w->freq;
    }
    else
    mapujKody(w,codes,"");
}
int huffEnc(BITBUFOR *BB,Byte *data,int size){
    if(size==0) return 0;
    int freq[256];
    getFreq(data,size,freq);
    Node *tree=makeTree(freq);
    Code codes[256];
    mapCodes(codes,tree);
    saveTree(BB,tree);
    Enc(BB,data,size,codes);
    killTree(tree);
    if(BB->bitIndex==0)
        return BB->pos;
    else
        return BB->pos+1;
}
int huffDec(Byte *data,BITBUFOR *BB,int size){
    if(size==0) return 0;
    Node *tree=new Node;
    loadTree2(tree,BB);
    Code codes[256];
    mapCodes(codes,tree);
    Dec(data,BB,size,codes);
    killTree(tree);
}
void showTree(Node *tree){
    if(tree->left==0&&tree->right==0)
    {
        printf("0(%3d)",tree->byte);
    }
    else
    {
        printf("1");
        showTree(tree->left);
        showTree(tree->right);
    }
}
